#include "start.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <time.h>

start::start(MainWindow *mw)
{
    w = mw;
}

double start::eqm(OCTET*ImgIn, size_t idx, int i, int j, int larg, int resolution){
    double err_quadra = 0.;

    OCTET* image_db;
    int nH, nW;
    char name_image_db[250];
    std::string nom = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/" + std::to_string(idx) + ".pgm";
    strlcpy(name_image_db, nom.c_str(), sizeof(name_image_db));
    lire_nb_lignes_colonnes_image_pgm(name_image_db, &nH, &nW);
    allocation_tableau(image_db, OCTET, nH*nW);
    lire_image_pgm(name_image_db, image_db, nH*nW);

    for(size_t x = 0; x<resolution; x++){
        for(size_t y = 0; y<resolution; y++){
            float id = ((float)x*(float)nH/(float)resolution)*(float)nW + ((float)y*(float)nW/(float)resolution);
            err_quadra += pow(ImgIn[(i+x)*larg + j+y] - image_db[(int)id], 2);
        }
    }

    free(image_db);
    return err_quadra / (resolution*resolution);
}

//Calcule la moyenne d'un rectangle débutant en (ideb,j_deb) et de taille larg_traitee x haut_traitee
OCTET start::moy(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, int pas){
    int taille_img = 0;//larg_traitee * haut_traitee;
    double moyenne = 0.;

    for(size_t i=i_deb; i<(i_deb+haut_traitee); i+=pas){
        for(size_t j = j_deb; j<(j_deb+larg_traitee); j+=pas){
            moyenne += img[i*larg + j];
            taille_img++;
        }
    }

    return moyenne/(double)taille_img;
}

double start::var(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, OCTET moyenne, int pas){
    int taille_img=0;// = larg_traitee * haut_traitee;
    double variance = 0.;

    for(size_t i=i_deb; i<(i_deb+haut_traitee); i+=pas){
        for(size_t j = j_deb; j<(j_deb+larg_traitee); j+=pas){
            variance += abs(img[i*larg + j] - moyenne);
            taille_img++;
        }
    }

    return variance/(double)taille_img;

}

//Trouve l'une des moyennes les plus proches de la valeur donnée dans le tableau des moyennes
size_t start::moyenne_plus_proche(OCTET moy, OCTET* M, size_t taille_tab){
    int ecart = 255;
    size_t idx_plus_proche;
    int offset = rand()%taille_tab;

    for(size_t i=0; i<taille_tab; i++){
        int idx = (i+offset)%taille_tab;
        if(abs(moy - M[idx])<ecart){
            ecart = abs(moy - M[idx]);
            idx_plus_proche = idx;
        }
    }

    return idx_plus_proche;
}

size_t start::image_plus_proche(OCTET moy, OCTET* M, double var, double* V, size_t taille_tab){
    int ecart = 512;
    size_t idx_plus_proche;
    int offset = rand()%taille_tab;

    for(size_t i=0; i<taille_tab; i++){
        int idx = (i+offset)%taille_tab;
        if(abs(moy-M[idx])+abs((int)(var-V[idx]))<ecart){
            ecart = abs(moy-M[idx])+abs((int)(var-V[idx]));
            idx_plus_proche = idx;
        }
    }
    return idx_plus_proche;
}

/*Remplace le bloc débutant en (i,j) et de taille resolution
* par l'image de la base de données de numéro idx, redimensionnée à la bonne taille */
void start::remplacer_bloc(OCTET*ImgOut, size_t idx, int i, int j, int larg, int resolution){
    OCTET* image_db;
    int nH, nW;
    char name_image_db[250];
    std::string nom = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/" + std::to_string(idx) + ".pgm";
    strcpy(name_image_db, nom.c_str());
    lire_nb_lignes_colonnes_image_pgm(name_image_db, &nH, &nW);
    allocation_tableau(image_db, OCTET, nH*nW);
    lire_image_pgm(name_image_db, image_db, nH*nW);

    for(size_t x = 0; x<resolution; x++){
        for(size_t y = 0; y<resolution; y++){
            float id = ((float)x*(float)nH/(float)resolution)*(float)nW + ((float)y*(float)nW/(float)resolution);
            //float dx =(float)nH/(float)resolution;
            //float dy =(float)nW/(float)resolution;
            ImgOut[(i+x)*larg + j+y] = image_db[(int)id];//moy(image_db, nW, nH, (int)((float)x*dx), (int)((float)y*dy) , (int)dx, (int)dy);
        }
    }

    free(image_db);
}

void start::letsgo(){
    srand(time(NULL));

    char method_key[250];

    char name_main_image[250], name_image_out[250];
    int nH, nW, nTaille;

    OCTET Moyennes[nb_img_db];
    double Variances[nb_img_db];

    // récupérer paramètres de l'interface
    strlcpy(name_main_image, w->input_path.toStdString().c_str(), sizeof(name_main_image));
    strlcpy(name_image_out, w->output_path.toStdString().c_str(), sizeof(name_image_out));
    resolution = std::stoi(w->resolution.toStdString());
    std::string methode_str = w->methode.toStdString();
    if(methode_str == "PSNR"){
        strlcpy(method_key, "P", sizeof(method_key));
    }else if(methode_str == "Moyenne"){
        strlcpy(method_key, "M", sizeof(method_key));
    }else if(methode_str == "Moyenne+Variance"){
        strlcpy(method_key, "MV", sizeof(method_key));
    }else if(methode_str == "Hybride"){
        strlcpy(method_key, "H", sizeof(method_key));
    }

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(name_main_image, &nH, &nW);
    nTaille = nH * nW;

    //std::cout << "nH " << nH << std::endl;
    //std::cout << "nW " << nW << std::endl;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(name_main_image, ImgIn, nTaille);

    // --------------------------------------------------------------------------------------------
    if(method_key[0] == 'M' || (method_key[0] == 'M' and method_key[1] == 'V')){ // moyenne ou moyenne+variance

        std::cout << "M or MV" << std::endl;

        char name_image_db[250];
        sscanf("/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/0.pgm", "%s", name_image_db);

        OCTET* image_db;
        std::string nom;

        int nH_db, nW_db;
        lire_nb_lignes_colonnes_image_pgm(name_image_db, &nH_db, &nW_db);
        allocation_tableau(image_db, OCTET, nH_db*nW_db);

        int taille_img_db = nH_db*nW_db;

        //Pour chaque image de la base de données : copie dans le tableau d'octets, puis calcul de la moyenne
        for (int i = 0; i < nb_img_db; ++i){

            //Base de données : dossier "img" avec des images de "0.pgm" à "10000.pgm"
            nom = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/" + std::to_string(i) + ".pgm";
            strcpy(name_image_db, nom.c_str());
            lire_image_pgm(name_image_db, image_db, taille_img_db);

            Moyennes[i] = moy(image_db, nW_db, nH_db, 0, 0, nW_db, nH_db, nW_db/resolution);
            Variances[i] = var(image_db, nW_db, nH_db, 0, 0, nW_db, nH_db, Moyennes[i], nW_db/resolution);
        }

        // ---------------------------------------------------
        if(method_key[0] == 'M' and method_key[1] != 'V'){ // moyenne

            std::cout << "M" << std::endl;

            //Pour chaque bloc de côté resolution :
            //calcul de la moyenne puis remplacement par l'une des images dont la moyenne est la plus proche
            for(size_t i = 0; i<nH; i+=resolution){
                for(size_t j = 0; j<nW; j+=resolution){
                    int moy_bloc = moy(ImgIn, nW, nH, i, j, resolution, resolution, 1);
                    double var_bloc = var(ImgIn, nW, nH, i, j, resolution, resolution, moy_bloc, 1);

                    remplacer_bloc(ImgOut, moyenne_plus_proche(moy_bloc, Moyennes, nb_img_db), i, j, nW, resolution);
                }
            }

        }
        // ---------------------------------------------------
        else if(method_key[0] == 'M' and method_key[1] == 'V'){ // moyenne+variance

            std::cout << "MV" << std::endl;

            //Pour chaque bloc de côté resolution :
            //calcul de la moyenne et de la variance
            //puis remplacement par l'une des images dont l'écart est la plus proche
            for(size_t i = 0; i<nH; i+=resolution){
                for(size_t j = 0; j<nW; j+=resolution){
                    int moy_bloc = moy(ImgIn, nW, nH, i, j, resolution, resolution, 1);
                    double var_bloc = var(ImgIn, nW, nH, i, j, resolution, resolution, moy_bloc, 1);

                    remplacer_bloc(ImgOut, image_plus_proche(moy_bloc, Moyennes, var_bloc, Variances, nb_img_db), i, j, nW, resolution);
                }
            }

        }

        free(image_db);
    }

    // --------------------------------------------------------------------------------------------
    else if(method_key[0] == 'P'){ // PSNR

        std::cout << "P" << std::endl;

        double seuil_erreur = 25;
        double err;
        double err_min;
        size_t idx_err_min;

        /*std::cout<<"Entrez le seuil du PSNR : ";
        std::cin>>seuil_erreur;
        std::cout<<std::endl;*/

        for(size_t i = 0; i<nH; i+=resolution){
            for(size_t j = 0; j<nW; j+=resolution){
                err_min = FLT_MAX;
                size_t offset = rand()%nb_img_db;

                for (size_t idx = 0; idx < nb_img_db; idx++)
                {
                    size_t idx_offset = (idx + offset)%nb_img_db;
                    err = eqm(ImgIn, idx_offset, i, j, nW, resolution);
                    if(err<seuil_erreur){
                        idx_err_min = idx_offset;
                        err_min = err;
                        break;
                    }
                    else if(err<err_min){
                        idx_err_min = idx_offset;
                        err_min = err;
                    }
                    //std::cout<<"Erreur bloc "<<i<<" "<<j<<" et img "<<idx<<"= "<<err<<"\n";
                }

                remplacer_bloc(ImgOut, idx_err_min, i, j, nW, resolution);
                std::cout<<"Bloc "<<i<<" "<<j<<" traite : err_min ="<<err_min<<" avec "<<idx_err_min<<"\n";
            }
        }

    }
    // --------------------------------------------------------------------------------------------
    else if(method_key[0] == 'H'){ // Hybride

        std::cout << "H" << std::endl;

        char name_image_db[250];
        sscanf("/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/0.pgm", "%s", name_image_db);

        OCTET* image_db;
        std::string nom;

        int nH_db, nW_db;
        lire_nb_lignes_colonnes_image_pgm(name_image_db, &nH_db, &nW_db);
        allocation_tableau(image_db, OCTET, nH_db*nW_db);

        int taille_img_db = nH_db*nW_db;

        for (int i = 0; i < nb_img_db; ++i){
            nom = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Projet-Codage-Image/official/ProjetImageCompression/img/" + std::to_string(i) + ".pgm";
            strcpy(name_image_db, nom.c_str());
            lire_image_pgm(name_image_db, image_db, taille_img_db);

            Moyennes[i] = moy(image_db, nW_db, nH_db, 0, 0, nW_db, nH_db, nW_db/resolution);
            Variances[i] = var(image_db, nW_db, nH_db, 0, 0, nW_db, nH_db, Moyennes[i], nW_db/resolution);
        }

        size_t nb_candidats = 10;
        int Candidats[nb_candidats][2];
        double CandidatsPSNR[nb_candidats];

        for(size_t i = 0; i<nH; i+=resolution){
            for(size_t j = 0; j<nW; j+=resolution){
                int moy_bloc = moy(ImgIn, nW, nH, i, j, resolution, resolution, 1);            
                double var_bloc = var(ImgIn, nW, nH, i, j, resolution, resolution, moy_bloc, 1);

                for(int idx = 0; idx<nb_candidats; idx++){
                    Candidats[idx][0] = idx;
                    Candidats[idx][1] = abs(moy_bloc-Moyennes[idx])+abs((int)(var_bloc-Variances[idx]));
                }

                for(int idx = 0; idx<nb_img_db; idx++){
                    bool b = 0;
                    int k = 0;
                    int ecart = abs(moy_bloc-Moyennes[idx])+abs((int)(var_bloc-Variances[idx]));

                    while((!b)&&(k<nb_candidats)){
                        if(ecart < Candidats[k][1]){
                            Candidats[k][0] = idx;
                            Candidats[k][1] = ecart;
                            CandidatsPSNR[k] = eqm(ImgIn, idx, i, j, nW, resolution);
                            b=1;
                        }
                        k++;
                    }
                }            

                size_t img_psnr_max = 0;
                for(int idx = 1; idx<nb_candidats; idx++){
                    if(CandidatsPSNR[idx] < CandidatsPSNR[img_psnr_max])
                        img_psnr_max = idx;
                }

                remplacer_bloc(ImgOut, Candidats[img_psnr_max][0], i, j, nW, resolution);
            }
        }
    free(image_db);
    }
    // --------------------------------------------------------------------------------------------
    else{
        printf("first argument is incorrect \n");
    }


    ecrire_image_pgm(name_image_out, ImgOut, nH, nW);
    free(ImgIn); free(ImgOut);

}
