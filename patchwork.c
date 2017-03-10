#include "patchwork.h"


// precond: nat ok, verifiee a la construction
struct patchwork *creer_primitif(const enum nature_primitif nat)
{
	struct patchwork *pw = malloc(sizeof (struct patchwork));
	pw->hauteur = 1;
	pw->largeur = 1;

	// TODO. Réfléchir s'il est plus avantageux de créer tout de suite un
	// gros tableau (type 10 * 10) pour éviter les réallocations systématiques
	pw->primitifs = calloc(1, sizeof (struct primitif *));
	pw->primitifs[0] = calloc(1, sizeof (struct primitif));

	pw->primitifs[0][0].nature = nat;
	pw->primitifs[0][0].orientation = EST;

	printf("Primitif %u.\n", nat);
	return pw;
}


// precond: p valide
struct patchwork *creer_rotation(const struct patchwork *p)
{
	if (p == NULL)
		return NULL;

	// Une rotation dans le sens direct inverse les dimensions (hauteur, largeur)
	struct patchwork *nouv_p = malloc(sizeof (struct patchwork));
	nouv_p->hauteur = p->largeur;
	nouv_p->largeur = p->hauteur;
	nouv_p->primitifs = calloc(nouv_p->hauteur, sizeof (struct primitif *));

	for (uint16_t i = 0; i < nouv_p->hauteur; ++i) {
		nouv_p->primitifs[i] = calloc(nouv_p->largeur, sizeof (struct primitif));
	}

	// Mise à jour de la position des sous-patchworks
	uint16_t h = nouv_p->hauteur;
	uint16_t l = nouv_p->largeur;

	for (uint16_t i = 0; i < h; ++i) {
		for (uint16_t j = 0; j < l; ++j) {
			nouv_p->primitifs[i][j].nature = p->primitifs[j][h - i - 1].nature;
			nouv_p->primitifs[i][j].orientation = (p->primitifs[j][h - i - 1].orientation + 1) % NB_ORIENTATIONS;
		}
	}

	printf("Rotation.\n");
	return nouv_p;
}


// precond: p_g et p_d valides
struct patchwork *creer_juxtaposition(const struct patchwork *p_g,
				      const struct patchwork *p_d)
{
	if (p_g == NULL
		|| p_d == NULL
		|| p_g->hauteur != p_d->hauteur)	// Dimensions incompatibles !
		return NULL;

	struct patchwork *nouv_p = malloc(sizeof (struct patchwork));
	nouv_p->hauteur = p_g->hauteur;
	nouv_p->largeur = p_g->largeur + p_d->largeur;
	nouv_p->primitifs = calloc(nouv_p->hauteur, sizeof (struct primitif *));

	for (uint16_t i = 0; i < nouv_p->hauteur; ++i) {
		nouv_p->primitifs[i] = calloc(nouv_p->largeur, sizeof (struct primitif));
	}

	// Mise à jour de la position des sous-patchworks
	for (uint16_t i = 0; i < nouv_p->hauteur; ++i) {
		for (uint16_t j = 0; j < nouv_p->largeur; ++j) {
			if (j < p_g->largeur) {
				nouv_p->primitifs[i][j].nature = p_g->primitifs[i][j].nature;
				nouv_p->primitifs[i][j].orientation = p_g->primitifs[i][j].orientation;
			} else {
				nouv_p->primitifs[i][j].nature = p_d->primitifs[i][j - p_g->largeur].nature;
				nouv_p->primitifs[i][j].orientation = p_d->primitifs[i][j - p_g->largeur].orientation;
			}
		}
	}

	printf("Juxtaposition.\n");
	return nouv_p;
}


// precond: p_h et p_b valides
struct patchwork *creer_superposition(const struct patchwork *p_h,
                                      const struct patchwork *p_b)
{
	if (p_h == NULL
		|| p_b == NULL
		|| p_h->largeur != p_b->largeur)	// Dimensions incompatibles !
		return NULL;

	struct patchwork *nouv_p = malloc(sizeof (struct patchwork));
	nouv_p->hauteur = p_h->hauteur + p_b->hauteur;
	nouv_p->largeur = p_h->largeur;
	nouv_p->primitifs = calloc(nouv_p->hauteur, sizeof (struct primitif *));

	for (uint16_t i = 0; i < nouv_p->hauteur; ++i) {
		nouv_p->primitifs[i] = calloc(nouv_p->largeur, sizeof (struct primitif));
	}

	// Mise à jour de la position des sous-patchworks
	for (uint16_t i = 0; i < nouv_p->hauteur; ++i) {
		for (uint16_t j = 0; j < nouv_p->largeur; ++j) {
			if (i < p_h->hauteur) {
				nouv_p->primitifs[i][j].nature = p_h->primitifs[i][j].nature;
				nouv_p->primitifs[i][j].orientation = p_h->primitifs[i][j].orientation;
			} else {
				nouv_p->primitifs[i][j].nature = p_b->primitifs[i - p_h->hauteur][j].nature;
				nouv_p->primitifs[i][j].orientation = p_b->primitifs[i - p_h->hauteur][j].orientation;
			}
		}
	}

	printf("Superposition.\n");
	return nouv_p;
}


void liberer_patchwork(struct patchwork *patch)
{
	if (patch != NULL) {
		for (uint16_t i = 0; i < patch->hauteur; i++) {
			free(patch->primitifs[i]);
		}

		free(patch->primitifs);
		free(patch);
	}
}
