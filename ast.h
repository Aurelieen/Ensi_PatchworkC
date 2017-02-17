#ifndef AST_H
#define AST_H

#include "patchwork.h"

/* Natures des operations sur les motifs */
enum nature_operation {
	ROTATION,
	JUXTAPOSITION,
	SUPERPOSITION,
	NB_OPERATIONS	/* sentinelle */
};


/* Structure de noeud de l'arbre (AST: Abstrat Syntax Tree), representant une
 * expression de type operation unaire ou binaire (noeud interne) ou une
 * valeur (image primitive, sur une feuille).
 * La partie data est "abstraite" et privee, simplement declaree ici et 
 * definie dans le fichier .c.
 * Pour la partie "methodes" (afficher et evaluer), on donne simplement le profil 
 * generique des fonctions. Les fonctions specifiques a utiliser, qui dependront
 * du type de noeud, seront definies de maniere statique dans le .c. Les champs
 * afficher et evaluer des noeuds devront etre initialises pour pointer vers la 
 * bonne fonction lors de la creation de chaque noeud.
 */
struct noeud_ast_data;

struct noeud_ast {
	// donnees privees du noeud
	struct noeud_ast_data *data;

	/* pointeur vers la fonction d'affichage du noeud (specifique du type de noeud):
	 * affiche l'expression portee par l'arbre de racine ast, en notation infixee.
	 * Pas de verification de la syntaxe: si un noeud est NULL, il n'est
	 * simplement pas affiche (ou "null"). */
	void (*afficher) (struct noeud_ast *);
	
	/* pointeur vers la fonction d'evaluation du noeud (specifique du type de noeud):
	 * applique la procedure d'evaluation a l'arbre de racine ast,
	 * et retourne le patchwork cree correspondant.
	 * En cas d'erreur syntaxique, un message approprie est affiche,
	 * toute la memoire liberee et l'execution interrompue. */
	struct patchwork *(*evaluer) (struct noeud_ast *);

	/* on pourrait utiliser le meme mecanisme pour liberer un noeud;
	 * ce n'est pas fait ici pour voir les differences entre les deux modeles
	 * (regarder liberer_expression par exemple) */
};


/*---------------------------------------------------------------------------*/

/* Libere la memoire associee a l'arbre ast passe en parametre. */
extern void liberer_expression(struct noeud_ast *ast);

/* Cree et retourne une valeur (feuille d'un AST) correspondant 
 * a une image primitive de nature nat_prim. */
extern struct noeud_ast *creer_valeur(const enum nature_primitif nat_prim);

/* Cree et retourne un noeud correspondant a une operation unaire 
 * de nature nat_oper, avec opde comme operande. */
extern struct noeud_ast *creer_unaire(const enum nature_operation nat_oper,
				      struct noeud_ast *opde);

/* Cree et retourne un noeud correspondant a une operation binaire
 * de nature nat_oper, avec opde_g et opde_d comme operandes. */
extern struct noeud_ast *creer_binaire(const enum nature_operation nat_oper,
				       struct noeud_ast *opde_g, 
				       struct noeud_ast *opde_d);

#endif /* AST_H */
