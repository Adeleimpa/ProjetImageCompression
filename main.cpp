#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <time.h>

const int resolution = 32;
const int nb_img_db = 10001; //nombre d'images utilisées de la base de données, max 10001

double eqm(OCTET*ImgIn, size_t idx, int i, int j, int larg, int resolution){
    double err_quadra = 0.;

    OCTET* image_db;
    int nH, nW;
    char name_image_db[250];
    std::string nom = "img/" + std::to_string(idx) + ".pgm";
    strcpy(name_image_db, nom.c_str());
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

/*Remplace le bloc débutant en (i,j) et de taille resolution
* par l'image de la base de données de numéro idx, redimensionnée à la bonne taille */
void remplacer_bloc(OCTET*ImgOut, size_t idx, int i, int j, int larg, int resolution){
    OCTET* image_db;
    int nH, nW;
    char name_image_db[250];
    std::string nom = "img/" + std::to_string(idx) + ".pgm";
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

int main(int argc, char* argv[]){
    //srand(time(NULL));

	char name_main_image[250], name_image_out[250];
	int nH, nW, nTaille;

	if (argc != 3) {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", name_main_image);
    sscanf(argv[2], "%s", name_image_out);


    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(name_main_image, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(name_main_image, ImgIn, nTaille);

    double err;
    double err_min;
    size_t idx_err_min;

    for(size_t i = 0; i<nH; i+=resolution){
        for(size_t j = 0; j<nW; j+=resolution){
            err_min = FLT_MAX;
            for (size_t idx = 0; idx < nb_img_db; idx++)
            {
                err = eqm(ImgIn, idx, i, j, nW, resolution);
                if(err<err_min){
                    idx_err_min = idx;
                    err_min = err;
                }
                //std::cout<<"Erreur bloc "<<i<<" "<<j<<" et img "<<idx<<"= "<<err<<"\n";
            }

            remplacer_bloc(ImgOut, idx_err_min, i, j, nW, resolution);
            std::cout<<"Bloc "<<i<<" "<<j<<" traite : err_min ="<<err_min<<" avec "<<idx_err_min<<"\n";
        }
    }

    //Pour chaque bloc de côté resolution :
    //calcul de la moyenne puis remplacement par l'une des images dont la moyenne est la plus proche
    /*
    for(size_t i = 0; i<nH; i+=resolution){
        for(size_t j = 0; j<nW; j+=resolution){
            int moy_bloc = moy(ImgIn, nW, nH, i, j, resolution, resolution, 1);            
            double var_bloc = var(ImgIn, nW, nH, i, j, resolution, resolution, moy_bloc, 1);

            remplacer_bloc(ImgOut, image_plus_proche(moy_bloc, Moyennes, var_bloc, Variances, nb_img_db), i, j, nW, resolution);
        }
    }
*/
    ecrire_image_pgm(name_image_out, ImgOut, nH, nW);

    free(ImgIn); free(ImgOut);

	return 1;
}