#include "patchwork.h"

// precond: nat ok, verifiee a la construction
struct patchwork *creer_primitif(const enum nature_primitif nat)
{	
	/*** TODO: A COMPLETER ***/
}


// precond: p valide
struct patchwork *creer_rotation(const struct patchwork *p)
{	
	/*** TODO: A COMPLETER ***/
}


// precond: p_g et p_d valides
struct patchwork *creer_juxtaposition(const struct patchwork *p_g,
				      const struct patchwork *p_d)
{
	/*** TODO: A COMPLETER ***/
}


// precond: p_h et p_b valides
struct patchwork *creer_superposition(const struct patchwork *p_h,
                                      const struct patchwork *p_b)
{
	/*** TODO: A COMPLETER ***/
}


void liberer_patchwork(struct patchwork *patch)
{
	/*** TODO: A COMPLETER ***/
}
