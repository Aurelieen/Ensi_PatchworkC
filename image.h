#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <string.h>
#include "patchwork.h"

/* Cree une image du patchwork patch, a partir des deux images ppm
 * representant les images primitives carre et triangle.
 * Le resultat est enregistre dans ficher_sortie au format ppm P6.
 * Precondition: les deux images primitives sont carrees et de meme taille. */
extern void creer_image(const struct patchwork *patch,
                        const char *fichier_ppm_carre,
                        const char *fichier_ppm_triangle,
                        FILE *fichier_sortie,
                        const char *fichier_nom);

#endif /* IMAGE_H */
