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

/*----------- Fonctions de vérification */
static void erreur(const char *msg);

/*---------------------------------------------------------------------------*/
/*     AFFICHAGE                                                             */
/*---------------------------------------------------------------------------*/


/* Definitions (locale) des fonctions "specifiques"
 * Pas de verif sur le type d'ast, ces fonctions ont ete "branchees" sur
 * les noeud de type adequat lors de leur construction */
static void afficher_valeur(struct noeud_ast *ast)
{
	if (ast != NULL && ast->data != NULL) {
		printf("%s", ast->data->nom);
	} else {
		printf("%s", "null");
	}
}


static void afficher_unaire(struct noeud_ast *ast)
{
	if (ast != NULL) {
		struct noeud_ast *op = ast->data->u.oper.u.oper_un.operande;

		printf("%s[", ast->data->nom);
		(*(op->afficher))(op);
		printf("]");
	} else {
		printf("%s", "null");
	}
}


static void afficher_binaire(struct noeud_ast *ast)
{
	if (ast != NULL) {
		struct noeud_ast *opg = ast->data->u.oper.u.oper_bin.operande_gauche;
		struct noeud_ast *opd = ast->data->u.oper.u.oper_bin.operande_droit;

		printf("%s[", ast->data->nom);
		(*(opg->afficher))(opg);
		printf(", ");
		(*(opd->afficher))(opd);
		printf("]");
	} else {
		printf("%s", "null");
	}
}


/*---------------------------------------------------------------------------*/
/*     FONCTIONS D'EVALUATION                                                */
/*---------------------------------------------------------------------------*/


/* Definitions (locale) des fonctions "specifiques"
 * Pas de verif sur le type d'ast, ces fonctions ont ete "branchees" sur
 * les noeud de type adequat lors de leur construction */

static struct patchwork *evaluer_valeur(struct noeud_ast *ast)
{
	if (ast != NULL && ast->data != NULL) {
		return ast->data->u.val.creer_patchwork(ast->data->u.val.nature);
	} else {
		erreur("ERREUR. Problème d'évaluation de valeur.");
		return NULL;
	}
}


static struct patchwork *evaluer_unaire(struct noeud_ast *ast)
{
	if (ast != NULL && ast->data != NULL) {
		struct noeud_ast *op = ast->data->u.oper.u.oper_un.operande;
		struct patchwork *base = (*(op->evaluer))(op);

		struct patchwork *res = ast->data->u.oper.u.oper_un.creer_patchwork(base);
		liberer_patchwork(base);

		return res;
	} else {
		erreur("ERREUR. Problème d'évaluation de valeur.");
		return NULL;
	}
}


static struct patchwork *evaluer_binaire(struct noeud_ast *ast)
{
	if (ast != NULL && ast->data != NULL) {
		struct noeud_ast *op_g = ast->data->u.oper.u.oper_bin.operande_gauche;
		struct noeud_ast *op_d = ast->data->u.oper.u.oper_bin.operande_droit;

		struct patchwork *base_g = (*(op_g->evaluer))(op_g);
		struct patchwork *base_d = (*(op_d->evaluer))(op_d);

		struct patchwork *res = ast->data->u.oper.u.oper_bin.creer_patchwork(base_g, base_d);
		liberer_patchwork(base_g);
		liberer_patchwork(base_d);

		return res;
	} else {
		erreur("ERREUR. Problème d'évaluation de valeur.");
		return NULL;
	}
}



/*---------------------------------------------------------------------------*/
/*     CREATION DES NOEUDS                                                   */
/*---------------------------------------------------------------------------*/

// C'est a la creation des noeuds que les "branchements" vers les fonctions
// adequats sont realises

struct noeud_ast *creer_valeur(const enum nature_primitif nat_prim)
{
	if ((int) nat_prim < 0 || (int) nat_prim >= NB_NAT_PRIMITIFS)
		erreur("ERREUR. Valeur du primitif inexistante.");

	struct noeud_ast *noeud = malloc(sizeof(struct noeud_ast));
	struct noeud_ast_data *data = malloc(sizeof(struct noeud_ast_data));

	// Initialisation du contenu du "noeud_ast" et branchements
	noeud->data = data;
	noeud->evaluer = &evaluer_valeur;
	noeud->afficher = &afficher_valeur;

	// Initialisation du contenu du "noeud_ast_data"
	data->nom = noms_primitifs[nat_prim];
	data->nature = VALEUR;
	data->u.val.nature = nat_prim;
	data->u.val.creer_patchwork = &creer_primitif;

	return noeud;
}


struct noeud_ast *creer_unaire(const enum nature_operation nat_oper,
			       struct noeud_ast *opde)
{
	if ((int) nat_oper < 0 || (int) nat_oper >= NB_OPERATIONS)
		erreur("ERREUR. Opération unaire inexistante.");

	struct noeud_ast *noeud = malloc(sizeof(struct noeud_ast));
	struct noeud_ast_data *data = malloc(sizeof(struct noeud_ast_data));

	// Initialisation du contenu du "noeud_ast" et branchements
	noeud->data = data;
	noeud->evaluer = &evaluer_unaire;
	noeud->afficher = &afficher_unaire;

	// Initialisation du contenu du "noeud_ast_data"
	data->nom = noms_operations[nat_oper];
	data->nature = OPERATION;
	data->u.oper.arite = UNAIRE;
	data->u.oper.u.oper_un.operande = opde;

	// INFO. Fonctionne tant que la seule opération unaire est "ROTATION".
	// Si cela change, il faudra différencier les cas (cf. binaire).
	data->u.oper.u.oper_un.creer_patchwork = &creer_rotation;

	return noeud;
}


struct noeud_ast *creer_binaire(const enum nature_operation nat_oper,
				struct noeud_ast *opde_g,
				struct noeud_ast *opde_d)
{
	if ((int) nat_oper < 0 || (int) nat_oper >= NB_OPERATIONS)
		erreur("ERREUR. Opération binaire inexistante.");

	struct noeud_ast *noeud = malloc(sizeof(struct noeud_ast));
	struct noeud_ast_data *data = malloc(sizeof(struct noeud_ast_data));

	// Initialisation du contenu du "noeud_ast" et branchements
	noeud->data = data;
	noeud->evaluer = &evaluer_binaire;
	noeud->afficher = &afficher_binaire;

	// Initialisation du contenu du "noeud_ast_data"
	data->nom = noms_operations[nat_oper];
	data->nature = OPERATION;
	data->u.oper.arite = BINAIRE;
	data->u.oper.u.oper_bin.operande_gauche = opde_g;
	data->u.oper.u.oper_bin.operande_droit = opde_d;

	switch (nat_oper) {
		case JUXTAPOSITION:
			data->u.oper.u.oper_bin.creer_patchwork = &creer_juxtaposition;
			break;
		case SUPERPOSITION:
			data->u.oper.u.oper_bin.creer_patchwork = &creer_superposition;
			break;
		default:
			exit(EXIT_FAILURE);
	}

	return noeud;
}


/*---------------------------------------------------------------------------*/
/*     LIBERATION DES NOEUDS                                                 */
/*---------------------------------------------------------------------------*/

// Ici pas de fonction specifique portee par chaque noeud
// Comment faire? Comparer les deux modeles!

// On libère l'arbre syntaxique abstrait selon un parcours postfixe.
void liberer_expression(struct noeud_ast *res)
{
	if (res != NULL) {
		if (res->data->nature == OPERATION) {
			switch (res->data->u.oper.arite) {
				case UNAIRE:
					liberer_expression(res->data->u.oper.u.oper_un.operande);
					break;
				case BINAIRE:
					liberer_expression(res->data->u.oper.u.oper_bin.operande_gauche);
					liberer_expression(res->data->u.oper.u.oper_bin.operande_droit);
					break;
				default:
					break;
			}
		}

		free(res->data);
		free(res);
	}
}

void erreur(const char *msg) {
	printf("%s", msg);
	exit(EXIT_FAILURE);
}
