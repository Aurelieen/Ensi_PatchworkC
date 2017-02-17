#include "ast.h"

/* constantes pour l'affichage des noms */
static const char *noms_primitifs[NB_NAT_PRIMITIFS] = {
	"carre",
	"triangle"
};

static const char *noms_operations[NB_OPERATIONS] = {
	"ROT",
	"JUXT",
	"SUPER"
};



/*---------------------------------------------------------------------------*/
/*     FONCTIONS PORTEES PAR LES NOEUDS                                      */
/*  fonctions suivant les signatures des champs afficher et evaluer          */
/*  definies dans ast.h, adaptees aux differentes natures de noeud d'un ast  */
/*---------------------------------------------------------------------------*/

/*----------- Affichage */
/* Fonctions "specifiques" locales (static) d'affichage d'un noeud
   selon sa nature. Declarees ici, definies plus bas */
static void afficher_valeur(struct noeud_ast *ast);
static void afficher_unaire(struct noeud_ast *ast);
static void afficher_binaire(struct noeud_ast *ast);


/*----------- Evaluation */
/* Fonctions "specifiques" locales (static) d'evaluation d'un noeud
   selon son type. Declarees ici, definies plus bas */
static struct patchwork *evaluer_valeur(struct noeud_ast *ast);
static struct patchwork *evaluer_unaire(struct noeud_ast *ast);
static struct patchwork *evaluer_binaire(struct noeud_ast *ast);



/*----------- Creation des patchworks */

/* Types des pointeurs sur les fonctions de creation des patchworks.
 * Les signatures different selon les noeuds.
 * Les fonctions specifiques sont definies ds le module patchwork.o */
typedef struct patchwork *(*creer_patchwork_valeur_fct) 
                                                (const enum nature_primitif);
typedef struct patchwork *(*creer_patchwork_unaire_fct)
                                                (const struct patchwork *);
typedef struct patchwork *(*creer_patchwork_binaire_fct)
                                                (const struct patchwork *,
                                                 const struct patchwork *);



/*---------------------------------------------------------------------------*/
/*    STRUCTURES DES NOEUDS                                                  */
/*---------------------------------------------------------------------------*/

enum nature_noeud {
	VALEUR,         /* feuille */
	OPERATION,      /* noeud interne */
	NB_NAT_NOEUDS   /* sentinelle */
};

enum arite_operation {
	UNAIRE,
	BINAIRE,
	NB_ARITES_OPERATIONS   /* sentinelle */
};


struct operation_unaire {
	struct noeud_ast *operande;
	creer_patchwork_unaire_fct creer_patchwork;
};

struct operation_binaire {
	struct noeud_ast *operande_gauche;
	struct noeud_ast *operande_droit;
	creer_patchwork_binaire_fct creer_patchwork;
};

struct operation {
	enum arite_operation arite;
	union {
		struct operation_unaire  oper_un;
		struct operation_binaire oper_bin;
	} u;
};


struct valeur {
	enum nature_primitif nature;
	creer_patchwork_valeur_fct creer_patchwork;
};


struct noeud_ast_data {
	const char *nom;

	// nature du noeud: VALEUR ou OPERATION
	enum nature_noeud nature;
	// selon la nature, les donnees representant le noeud
	union {
		struct valeur val;       // si nature == VALEUR
		struct operation oper;   // si nature == OPERATION
	} u;
};



/*---------------------------------------------------------------------------*/
/*     AFFICHAGE                                                             */
/*---------------------------------------------------------------------------*/


/* Definitions (locale) des fonctions "specifiques" 
 * Pas de verif sur le type d'ast, ces fonctions ont ete "branchees" sur
 * les noeud de type adequat lors de leur construction */
static void afficher_valeur(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}


static void afficher_unaire(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}


static void afficher_binaire(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}


/*---------------------------------------------------------------------------*/
/*     FONCTIONS D'EVALUATION                                                */
/*---------------------------------------------------------------------------*/


/* Definitions (locale) des fonctions "specifiques" 
 * Pas de verif sur le type d'ast, ces fonctions ont ete "branchees" sur
 * les noeud de type adequat lors de leur construction */

static struct patchwork *evaluer_valeur(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}


static struct patchwork *evaluer_unaire(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}


static struct patchwork *evaluer_binaire(struct noeud_ast *ast)
{
	/*** TODO: A COMPLETER ***/
}



/*---------------------------------------------------------------------------*/
/*     CREATION DES NOEUDS                                                   */
/*---------------------------------------------------------------------------*/

// C'est a la creation des noeuds que les "branchements" vers les fonctions
// adequats sont realises

struct noeud_ast *creer_valeur(const enum nature_primitif nat_prim)
{
	/*** TODO: A COMPLETER ***/
}


struct noeud_ast *creer_unaire(const enum nature_operation nat_oper,
			       struct noeud_ast *opde)
{
	/*** TODO: A COMPLETER ***/
}


struct noeud_ast *creer_binaire(const enum nature_operation nat_oper,
				struct noeud_ast *opde_g, 
				struct noeud_ast *opde_d)
{
	/*** TODO: A COMPLETER ***/
}


/*---------------------------------------------------------------------------*/
/*     LIBERATION DES NOEUDS                                                 */
/*---------------------------------------------------------------------------*/

// Ici pas de fonction specifique portee par chaque noeud
// Comment faire? Comparer les deux modeles!

void liberer_expression(struct noeud_ast *res)
{
	/*** TODO: A COMPLETER ***/
}
