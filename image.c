#include "image.h"
#define PPMREADBUFLEN 256


/* Vérification des motifs.
 * Renvoie : taille si correcte, -1 si problème. */
int ppm_verifications(FILE *, FILE *, unsigned char *, unsigned char *);

/* Lecture de la taille d'un PPM/P6.
 * Renvoie : taille si carré, -1 si non carré ou problème. */
int ppm_caracteristiques(FILE *, unsigned char *);

/* Génération de l'en-tête d'un fichier PPM. */
void ppm_entete(FILE *, int);

/* Génération d'un PPM à partir des directives d'un patchwork. */
void ppm_from_patchwork(FILE *, const struct patchwork *, unsigned char *, unsigned char *, int);

/* Génération d'une ligne de pixels (de plusieurs primitifs) dans un PPM. */
void ppm_ligne_pixel(FILE *, const struct primitif *, unsigned char *, unsigned char *, int, int);

/* Remplissage d'un tableau avec les données d'un fichier PPM/P6. */
void ppm_remplir(FILE *f_ppm, unsigned char *data, uint16_t taille, int cote_motif);

/* ============================================================ */

/* Cree une image du patchwork patch, a partir des deux images ppm
 * representant les images primitives carre et triangle.
 * Le resultat est enregistre dans ficher_sortie au format ppm P6.
 * Precondition: les deux images primitives sont carrees et de meme taille. */
void creer_image(const struct patchwork *patch,
                 const char *fichier_ppm_carre,
                 const char *fichier_ppm_triangle,
                 FILE *fichier_sortie) {

    if (patch == NULL || fichier_sortie == NULL) return;

    // ETAPE 0. Vérifications des fichiers source.
    FILE *file_carre, *file_triangle;
    unsigned char *data_ppm1 = NULL, *data_ppm2 = NULL;

    if ((file_carre = fopen(fichier_ppm_carre, "rb")) == NULL) {
        fprintf(stderr, "ERREUR. Impossible d'ouvrir : %s.\n", fichier_ppm_carre);
        exit(1);
    }

    if ((file_triangle = fopen(fichier_ppm_triangle, "rb")) == NULL) {
        fprintf(stderr, "ERREUR. Impossible d'ouvrir : %s.\n", fichier_ppm_carre);
        exit(1);
    }

    int cote = ppm_verifications(file_carre, file_triangle, data_ppm1, data_ppm2);
    if (cote < 1) {
        fclose(file_carre);
        fclose(file_triangle);
        fprintf(stderr, "ERREUR. Dimensions incohérentes des PPM.");
        return;
    }

    // ETAPE 1. Ecriture de l'en-tête du fichier PPM/P6.
    ppm_entete(fichier_sortie, cote);

    // ETAPE 2. Traduction du patchwork.
    ppm_from_patchwork(fichier_sortie, patch, data_ppm1, data_ppm2, cote);

    // TODO. Faire un vrai message de fin de création.
    printf("----------------- TERMINE");
    fclose(file_carre);
    fclose(file_triangle);
    fclose(fichier_sortie);
}

/* ============================================================ */

/* Vérification des motifs.
 * Renvoie : taille si correcte, NULL si problème. */
int ppm_verifications(FILE *ppm1, FILE *ppm2, unsigned char *data_ppm1, unsigned char *data_ppm2) {
    int taille_ppm1 = ppm_caracteristiques(ppm1, data_ppm1);
    int taille_ppm2 = ppm_caracteristiques(ppm2, data_ppm2);

    int res = ((taille_ppm1 > 0) && (taille_ppm2 > 0) && (taille_ppm1 == taille_ppm2));
    return res ? taille_ppm1 : -1;
}


/* Lecture de la taille d'un PPM/P6.
 * Inspiré de <https://rosettacode.org/wiki/Bitmap/Read_a_PPM_file#C>
 * Renvoie : taille si carré, -1 si non carré ou problème. */
int ppm_caracteristiques(FILE *ppm, unsigned char *data) {
    char buf[PPMREADBUFLEN], *t;
    int r, d, nb_dimensions, nb_col, nb_lignes;

    t = fgets(buf, PPMREADBUFLEN, ppm);

    if ((t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ))
        return -1;

    do
    {  /* Elimination des commentaires (lignes commençant par un croisillon). */
        t = fgets(buf, PPMREADBUFLEN, ppm);
        if (t == NULL)
            return -1;
    } while (strncmp(buf, "#", 1) == 0);

    nb_dimensions = sscanf(buf, "%u %u", &nb_col, &nb_lignes);
    if (nb_dimensions < 2)
        return -1;

    r = fscanf(ppm, "%u", &d);
    if ((r < 1) || (d != 255 ))
        return -1;
    fseek(ppm, 1, SEEK_CUR);

    /* Ici, on enregistre les données PPM dans un tableau à 1 dimension. */
    uint16_t taille = nb_col * nb_col * 3;
    data = calloc(taille, sizeof (unsigned char));

    ppm_remplir(ppm, data, taille, nb_col);

    // for (uint16_t i = 0; i < taille; ++i) {
    //     printf("%u ", data_carre[i]);
    // }
    // printf("\n");

    rewind(ppm); /* Pour repartir du début ensuite. */
    return ((nb_dimensions == 2) && (nb_col == nb_lignes)) ? nb_col : -1;
}

/* Génération de l'en-tête d'un fichier PPM. */
/* Précondition vérifiée dans "creer_image" : le descripteur existe. */
void ppm_entete(FILE *fichier_sortie, int cote) {
    fputs("P6\n", fichier_sortie);
    fprintf(fichier_sortie, "%u %u\n", cote, cote);
    fprintf(fichier_sortie, "255\n");
}


/* Génération d'un PPM à partir des directives d'un patchwork. */
void ppm_from_patchwork(FILE *f_sortie, const struct patchwork *patch,
                        unsigned char *carre, unsigned char *triangle, int cote_motif) {
    uint16_t largeur = patch->largeur;
    uint16_t hauteur = patch->hauteur;

    for (uint16_t i = 0; i < hauteur; ++i) {
        ppm_ligne_pixel(f_sortie, patch->primitifs[i],
                        carre, triangle, cote_motif, largeur);
    }
}


void ppm_remplir(FILE *f_ppm, unsigned char *data, uint16_t taille, int cote_motif) {
    for (uint16_t i = 0; i < taille; ++i) {
        char value;
        fread(&value, 1, 1, f_ppm);
        data[i] = (unsigned char) value;
    }
}


/* Génération d'une ligne de pixels (de plusieurs primitifs) dans un PPM. */
void ppm_ligne_pixel(FILE *f_sortie, const struct primitif *primitifs,
                     unsigned char *carre, unsigned char *triangle,
                     int cote_motif, int largeur) {

    for (uint16_t c = 0; c < cote_motif; ++c) {
        for (uint16_t j = 0; j < largeur; ++j) {
            for (uint16_t pix = 0; pix < cote_motif; ++pix) {
                ; // TODO. WRITE IN BINARY FILE.
            }
        }
    }
}
