#ifndef START_H
#define START_H

#include "mainwindow.h"

#ifndef IMGPPM
#define IMGPPM
#include "image_ppm.h"
#endif

class start
{
public:
    start(MainWindow *mw);
    void letsgo();
    void remplacer_bloc(OCTET*ImgOut, size_t idx, int i, int j, int larg, int resolution);
    size_t image_plus_proche(OCTET moy, OCTET* M, double var, double* V, size_t taille_tab);
    size_t moyenne_plus_proche(OCTET moy, OCTET* M, size_t taille_tab);
    double var(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, OCTET moyenne, int pas);
    OCTET moy(OCTET* img, int larg, int haut, int i_deb, int j_deb, int larg_traitee, int haut_traitee, int pas);
    double eqm(OCTET*ImgIn, size_t idx, int i, int j, int larg, int resolution);

    MainWindow *w;
    int resolution;
    const int nb_img_db = 10001; //nombre d'images utilisées de la base de données, max 10001
};

#endif // START_H
