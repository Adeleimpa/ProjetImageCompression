#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>

int main(int argc, char* argv[]){

	char name_main_image[250];
	int nH, nW, nTaille;

	if (argc != 2) {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", name_main_image);

    // TODO read data base of images

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = 3*nTaille;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // TODO make image out

	return 1;
}