#ifndef PATCHWORK_H
#define PATCHWORK_H

#include <stdint.h>

enum nature_primitif {
	CARRE,
	TRIANGLE,
	NB_NAT_PRIMITIFS    /* sentinelle */
};

enum orientation_primitif {
	EST,
	NORD,
	OUEST,
	SUD,
	NB_ORIENTATIONS /* sentinelle */
};

struct primitif {
	enum nature_primitif nature;
	enum orientation_primitif orientation;
};

struct patchwork {
	uint16_t hauteur, largeur;
	struct primitif **primitifs;	/* tableau de hauteur pointeurs 
					   vers des tableaux de largeur primitifs */
};

/* Cree et retourne un patchwork compose d'une image primitive,
 * de taille 1x1, de nature nat et d'orientation EST. */
extern struct patchwork *creer_primitif(const enum nature_primitif nat);

/* Cree et retourne un nouveau patchwork en appliquant a p une rotation 
 * de 90 degres dans le sens direct. */
extern struct patchwork *creer_rotation(const struct patchwork *p);

/* Cree et retourne un nouveau patchwork par juxtaposition de p_g et
 * p_d (p_g a gauche de p_d).
 * Si les tailles ne sont par concordantes, retourne NULL. */
extern struct patchwork *creer_juxtaposition(const struct patchwork *p_g,
                                             const struct patchwork *p_d);

/* Cree et retourne un nouveau patchwork par superposition de p_h et
 * p_b (p_h au dessus de p_b).
 * Si les tailles ne sont par concordantes, retourne NULL. */
extern struct patchwork *creer_superposition(const struct patchwork *p_h,
                                             const struct patchwork *p_b);

/* Libere toute la memoire allouee pour le patchwork p. */
extern void liberer_patchwork(struct patchwork *p);

#endif /* PATCHWORK_H */
