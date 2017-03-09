#include <stdio.h>
#include "ast.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* INFO. Dernière MàJ : 09/03, 19:39. Ensemble de fonctions implémenté. 	 */

/* fonctions de "confort" pour alleger (?) la syntaxe de creation d'un noeud */
static struct noeud_ast *valeur(const enum nature_primitif nat_prim)
{
	return creer_valeur(nat_prim);
}

static struct noeud_ast *rotation(struct noeud_ast *opde)
{
	return creer_unaire(ROTATION, opde);
}

static struct noeud_ast *juxtaposition(struct noeud_ast *opde_gauche,
				       struct noeud_ast *opde_droit)
{
	return creer_binaire(JUXTAPOSITION, opde_gauche, opde_droit);
}

static struct noeud_ast *superposition(struct noeud_ast *opde_gauche,
				       struct noeud_ast *opde_droit)
{
	return creer_binaire(SUPERPOSITION, opde_gauche, opde_droit);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	// 1. TEMPORAIRE. Test de l'AST
	struct noeud_ast *noeud_racine = valeur(TRIANGLE);
	struct noeud_ast *noeud_rotation = rotation(noeud_racine);

	liberer_expression(noeud_rotation);
	liberer_expression(noeud_racine);
}
