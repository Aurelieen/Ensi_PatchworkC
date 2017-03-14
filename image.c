#include "image.h"
#define PPMREADBUFLEN 256


/* Sauvegarde d'une image PPM
 * sous la forme d'un tableau à deux dimensions de pixels. */
struct motif_pixel {
    unsigned char R;
    unsigned char V;
    unsigned char B;
};

struct motif {
    unsigned int hauteur;
    unsigned int largeur;
    struct motif_pixel **pixels;    // Tableau de dimensions hauteur x largeur
};


/* Vérification des motifs.
 * Renvoie : taille si correcte, -1 si problème. */
int ppm_verifications(FILE *, FILE *, struct motif *, struct motif *);

/* Lecture de la taille d'un PPM/P6.
 * Renvoie : taille si carré, -1 si non carré ou problème. */
int ppm_caracteristiques(FILE *, struct motif *);

/* Génération de l'en-tête d'un fichier PPM. */
void ppm_entete(FILE *, uint16_t, uint16_t);

/* Génération d'un PPM à partir des directives d'un patchwork. */
void ppm_from_patchwork(FILE *, const struct patchwork *, struct motif *, struct motif *);

/* Ajout d'un pixel dans le fichier de sortie */
void ppm_pixel(FILE *, uint16_t, uint16_t, const struct primitif *, struct motif *);

/* Remplissage d'un tableau avec les données d'un fichier PPM/P6. */
void ppm_remplir(FILE *, struct motif *);

/* Libère la mémoire prise par un motif et ses pixels. */
void ppm_liberer(struct motif);

/* ============================================================ */

/* Cree une image du patchwork patch, a partir des deux images ppm
 * representant les images primitives carre et triangle.
 * Le resultat est enregistre dans ficher_sortie au format ppm P6.
 * Precondition: les deux images primitives sont carrees et de meme taille. */
void creer_image(const struct patchwork *patch,
                 const char *fichier_ppm_carre,
                 const char *fichier_ppm_triangle,
                 FILE *fichier_sortie,
                 const char *fichier_nom) {

    if (patch == NULL || fichier_sortie == NULL) {
        fprintf(stderr, "ERREUR. L'expression en entrée est incorrecte.\n");
        return;
    }

    // ETAPE 0. Vérifications des fichiers source.
    FILE *file_carre, *file_triangle;
    struct motif motif_ppm1, motif_ppm2;

    // unsigned char *data_ppm1 = NULL, *data_ppm2 = NULL;

    if ((file_carre = fopen(fichier_ppm_carre, "rb")) == NULL) {
        fprintf(stderr, "ERREUR. Impossible d'ouvrir : %s.\n", fichier_ppm_carre);
        return;
    }

    if ((file_triangle = fopen(fichier_ppm_triangle, "rb")) == NULL) {
        fprintf(stderr, "ERREUR. Impossible d'ouvrir : %s.\n", fichier_ppm_carre);
        return;
    }

    int cote = ppm_verifications(file_carre, file_triangle, &motif_ppm1, &motif_ppm2);
    if (cote < 1) {
        fclose(file_carre);
        fclose(file_triangle);
        fprintf(stderr, "ERREUR. Dimensions incohérentes des PPM.");
        return;
    }

    // ETAPE 1. Ecriture de l'en-tête du fichier PPM/P6.
    uint16_t nb_pixels_hauteur = cote * patch->hauteur;
    uint16_t nb_pixels_largeur = cote * patch->largeur;
    ppm_entete(fichier_sortie, nb_pixels_hauteur, nb_pixels_largeur);

    // ETAPE 2. Traduction du patchwork.
    ppm_from_patchwork(fichier_sortie, patch, &motif_ppm1, &motif_ppm2);

    // Libération des ressources en mémoire
    printf(":: Patchwork :: Résultat : %s.\n", fichier_nom);
    ppm_liberer(motif_ppm1);
    ppm_liberer(motif_ppm2);

    fclose(file_carre);
    fclose(file_triangle);
    fclose(fichier_sortie);
}

/* ============================================================ */

/* Vérification des motifs.
 * Renvoie : taille si correcte, NULL si problème. */
int ppm_verifications(FILE *ppm1, FILE *ppm2, struct motif *motif1, struct motif *motif2) {
    int taille_ppm1 = ppm_caracteristiques(ppm1, motif1);
    int taille_ppm2 = ppm_caracteristiques(ppm2, motif2);

    int res = ((taille_ppm1 > 0) && (taille_ppm2 > 0) && (taille_ppm1 == taille_ppm2));
    return res ? taille_ppm1 : -1;
}


/* Lecture de la taille d'un PPM/P6.
 * Inspiré de <https://rosettacode.org/wiki/Bitmap/Read_a_PPM_file#C>
 * Renvoie : taille si carré, -1 si non carré ou problème. */
int ppm_caracteristiques(FILE *ppm, struct motif *m) {
    char buf[PPMREADBUFLEN], *t;
    int r, d, nb_dimensions;
    unsigned int nb_col, nb_lignes;

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

    /* Ici, on enregistre les données PPM dans une structure motif. */
    m->hauteur = nb_lignes;
    m->largeur = nb_col;

    ppm_remplir(ppm, m);

    rewind(ppm); /* Pour repartir du début ensuite. */
    return ((nb_dimensions == 2) && (nb_col == nb_lignes)) ? nb_col : -1;
}


/* Génération de l'en-tête d'un fichier PPM. */
/* Précondition vérifiée dans "creer_image" : le descripteur existe. */
void ppm_entete(FILE *fichier_sortie, uint16_t hauteur, uint16_t largeur) {
    fputs("P6\n", fichier_sortie);
    fprintf(fichier_sortie, "%u %u\n", largeur, hauteur);
    fprintf(fichier_sortie, "255\n");
}


/* Génération d'un PPM à partir des directives d'un patchwork. */
void ppm_from_patchwork(FILE *f_sortie, const struct patchwork *patch,
                        struct motif *carre, struct motif *triangle) {

    // Chaque primitif du patch est divisé en "carre->hauteur" pixels
    for (uint16_t i = 0; i < patch->hauteur * carre->hauteur; ++i) {
        for (uint16_t j = 0; j < patch->largeur * carre->hauteur; ++j) {

            // Coordonnées du primitif dans le patchwork
            uint16_t primitif_i = i / carre->hauteur;
            uint16_t primitif_j = j / carre->largeur;
            struct primitif prim = patch->primitifs[primitif_i][primitif_j];

            switch (prim.nature) {
                case CARRE:
                    ppm_pixel(f_sortie, i % carre->largeur, j % carre->largeur, &prim, carre);
                    break;
                case TRIANGLE:
                    ppm_pixel(f_sortie, i % carre->largeur, j % carre->largeur, &prim, triangle);
                    break;
                default:
                    fclose(f_sortie);
                    return;
            }
        }
    }
}


void ppm_pixel(FILE *f_sortie, uint16_t i, uint16_t j, const struct primitif *prim, struct motif *m) {
    // Les pixels à dessiner sont affectés par l'orientation
    uint16_t draw_i, draw_j;

    switch (prim->orientation) {
        case EST:
            draw_i = i;
            draw_j = j;
            break;
        case NORD:
            draw_i = j;
            draw_j = m->largeur - i - 1;
            break;
        case OUEST:
            draw_i = j;
            draw_j = i;
            break;
        case SUD:
            draw_i = m->largeur - j - 1;
            draw_j = i;
            break;
        default:
            fclose(f_sortie);
            return;
    }

    // Ajout des composantes R, V et B
    unsigned char R = m->pixels[draw_i][draw_j].R;
    unsigned char V = m->pixels[draw_i][draw_j].V;
    unsigned char B = m->pixels[draw_i][draw_j].B;
    unsigned char couleurs[] = { R, V, B };

    fwrite(couleurs, sizeof (couleurs), 1, f_sortie);
}


void ppm_remplir(FILE *f_ppm, struct motif *m) {
    // Un motif est de taille hauteur * largeur.
    // Chaque case allouée est une structure pixel (R, V, B)
    m->pixels = calloc(m->hauteur, sizeof (struct motif_pixel *));
    for (uint16_t i = 0; i < m->hauteur; ++i) {
        m->pixels[i] = calloc(m->largeur, sizeof (struct motif_pixel));
    }

    for (uint16_t i = 0; i < m->hauteur; ++i) {
        for (uint16_t j = 0; j < m->largeur; ++j) {
            char R, V, B;
            fread(&R, 1, 1, f_ppm);
            fread(&V, 1, 1, f_ppm);
            fread(&B, 1, 1, f_ppm);

            m->pixels[i][j].R = (unsigned char) R;
            m->pixels[i][j].V = (unsigned char) V;
            m->pixels[i][j].B = (unsigned char) B;
        }
    }
}


/* Libère la mémoire prise par les pixels d'un motif. */
void ppm_liberer(struct motif m) {
    for (uint16_t i = 0; i < m.hauteur; ++i) {
        free(m.pixels[i]);
    }

    free(m.pixels);
}
