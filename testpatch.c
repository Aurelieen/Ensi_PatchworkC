#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "image.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

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
	struct noeud_ast *noeud_valeur = valeur(TRIANGLE);
	struct noeud_ast *noeud_rotation = rotation(noeud_valeur);

	struct noeud_ast *noeud_feuille1 = valeur(CARRE);
	struct noeud_ast *noeud_feuille2 = valeur(CARRE);
	struct noeud_ast *noeud_juxtapo = juxtaposition(noeud_feuille1, noeud_feuille2);

	liberer_expression(noeud_rotation);
	liberer_expression(noeud_juxtapo);

	// 2. TEMPORAIRE. Test de l'AST et de l'évaluation via le parser
	struct noeud_ast *noeud_analyseur;
	analyser("exemples_expressions/exemple_sujet", &noeud_analyseur);

	noeud_analyseur->afficher(noeud_analyseur);
	struct patchwork *patch = noeud_analyseur->evaluer(noeud_analyseur);
	printf("\n");

	// 3. TEMPORAIRE. Test de la traduction en image.
	creer_image(patch, "motifs/carre_4.ppm", "motifs/triangle_4.ppm", fopen("test.ppm", "wb"));

	liberer_expression(noeud_analyseur);
	liberer_patchwork(patch);
}
