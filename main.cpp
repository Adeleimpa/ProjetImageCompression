#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <time.h>

const int resolution = 16;
const int nb_img_db = 10001; //nombre d'images utilisées de la base de données, max 10001

double PSNR(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW){
  double psnr;

  double sum;
  for(int i = 0; i < nH; i++) {
      for (int j = 0; j < nW; j++) {
          sum += pow(ImgOut[i * nW + j] - ImgIn[i * nW + j],2);
      }
  }
  double eqm;
  eqm = sum / (nH * nW);

  psnr = 10 * log10(pow(255,2)/eqm);

  return psnr;
}

//Calcule la moyenne d'un rectangle débutant en (ideb,j_deb) et de taille larg_traitee x haut_traitee
OCTET moy(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, int pas){
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

double var(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, OCTET moyenne, int pas){
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
size_t moyenne_plus_proche(OCTET moy, OCTET* M, size_t taille_tab){
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

size_t image_plus_proche(OCTET moy, OCTET* M, double var, double* V, size_t taille_tab){
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
    srand(time(NULL));

	char name_main_image[250], name_image_out[250], name_image_db[250];
	int nH, nW, nTaille;
    OCTET Moyennes[nb_img_db];
    double Variances[nb_img_db];

	if (argc != 3) {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", name_main_image);
    sscanf(argv[2], "%s", name_image_out);

    sscanf("img/0.pgm", "%s", name_image_db);

    OCTET* image_db;
    std::string nom;
    lire_nb_lignes_colonnes_image_pgm(name_image_db, &nH, &nW);
    allocation_tableau(image_db, OCTET, nH*nW);

    int taille_img_db = nH*nW;

    //Pour chaque image de la base de données : copie dans le tableau d'octets, puis calcul de la moyenne
    for (int i = 0; i < nb_img_db; ++i)
    {
        //Base de données : dossier "img" avec des images de "0.pgm" à "10000.pgm"
        nom = "img/" + std::to_string(i) + ".pgm";
        strcpy(name_image_db, nom.c_str());
        lire_image_pgm(name_image_db, image_db, taille_img_db);

        Moyennes[i] = moy(image_db, nW, nH, 0, 0, nW, nH, nW/resolution);
        Variances[i] = var(image_db, nW, nH, 0, 0, nW, nH, Moyennes[i], nW/resolution);
    }

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(name_main_image, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(name_main_image, ImgIn, nTaille);

    //Pour chaque bloc de côté resolution :
    //calcul de la moyenne puis remplacement par l'une des images dont la moyenne est la plus proche
    for(size_t i = 0; i<nH; i+=resolution){
        for(size_t j = 0; j<nW; j+=resolution){
            int moy_bloc = moy(ImgIn, nW, nH, i, j, resolution, resolution, 1);            
            double var_bloc = var(ImgIn, nW, nH, i, j, resolution, resolution, moy_bloc, 1);

            remplacer_bloc(ImgOut, image_plus_proche(moy_bloc, Moyennes, var_bloc, Variances, nb_img_db), i, j, nW, resolution);
        }
    }

    ecrire_image_pgm(name_image_out, ImgOut, nH, nW);

    free(ImgIn); free(ImgOut); free(image_db);

	return 1;
}