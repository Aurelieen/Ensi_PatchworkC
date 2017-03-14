#include <stdlib.h>
#include <argp.h>
#include <stdio.h>
#include <inttypes.h>
#include "ast.h"
#include "parser.h"
#include "image.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
// Implémentées mais non utilisées
// Commentées pour éviter les avertissements de -Wununsed-function

/* fonctions de "confort" pour alleger (?) la syntaxe de creation d'un noeud */
// static struct noeud_ast *valeur(const enum nature_primitif nat_prim)
// {
// 	return creer_valeur(nat_prim);
// }
//
// static struct noeud_ast *rotation(struct noeud_ast *opde)
// {
// 	return creer_unaire(ROTATION, opde);
// }
//
// static struct noeud_ast *juxtaposition(struct noeud_ast *opde_gauche,
// 				       struct noeud_ast *opde_droit)
// {
// 	return creer_binaire(JUXTAPOSITION, opde_gauche, opde_droit);
// }
//
// static struct noeud_ast *superposition(struct noeud_ast *opde_gauche,
// 				       struct noeud_ast *opde_droit)
// {
// 	return creer_binaire(SUPERPOSITION, opde_gauche, opde_droit);
// }


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
// Lecture de la ligne de commande

const char *argp_program_version = "Patchwork / v1.0";
const char *argp_program_bug_address = "<aurelien.pepin@ensimag.fr>";
static char doc[] = "testpatch -- Construction de patchworks via des images primitives";
static char args_doc[] = "";

static struct argp_option options[] = {
	{ "file", 'f', "exemples_expressions/exemple_sujet", 0, "Chemin vers le fichier d'entrée", 0 },
	{ "size", 's', "32", 0, "Taille (de côté) d'un motif : 4, 15, 32, 64", 0 },
	{ "output", 'o', "resultat.ppm", 0, "Chemin vers le patchwork final", 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

struct arguments {
  char *output;
  char *input;
  uintmax_t size;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
		case 'o':
			arguments->output = arg;
			break;
		case 'f':
			arguments->input = arg;
			break;
		case 's':
			arguments->size = strtoumax(arg, NULL, 10);
			if (arguments->size == UINTMAX_MAX && errno == ERANGE)
				argp_usage (state);

			if (arguments->size != 4
				&& arguments->size != 15
				&& arguments->size != 32
				&& arguments->size != 64)
				argp_usage (state);
			break;
		case ARGP_KEY_END:
			if (state->arg_num > 0) {
				argp_usage (state);
			}
			break;
		default:
	      return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp arg_p = { options, parse_opt, args_doc, doc, 0, 0, 0 };

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.input = NULL;
	arguments.output = "resultat.ppm";
	arguments.size = 32;

	/* Valeurs par défaut des arguments. */

	argp_parse (&arg_p, argc, argv, 0, 0, &arguments);
	struct noeud_ast *noeud_analyseur;

	// Si pas de -f, on prend le flux clavier
	if (arguments.input == NULL) {
		printf(":: Patchwork :: CTRL+D pour lancer la création du patchwork.\n");
		analyser(NULL, &noeud_analyseur);
	} else {
		printf(":: Patchwork :: Génération depuis %s.\n", arguments.input);
		analyser((unsigned char *) arguments.input, &noeud_analyseur);
	}

	// Génération du patchwork à partir de l'arbre syntaxique abstrait de l'expression
	struct patchwork *patch = noeud_analyseur->evaluer(noeud_analyseur);

	// Création de l'image. L'argument de sortie par défaut est <resultat.ppm>
	char chaine_carre[100];
	char chaine_triangle[100];

	sprintf(chaine_carre, "motifs/carre_%u.ppm", (unsigned int) arguments.size);
	sprintf(chaine_triangle, "motifs/triangle_%u.ppm", (unsigned int) arguments.size);

	// char chaine_triangle[] = "motifs/carre_32.ppm";

	creer_image(patch, chaine_carre, chaine_triangle,
				fopen(arguments.output, "wb"), arguments.output);

	// Libération de la mémoire
	liberer_expression(noeud_analyseur);
	liberer_patchwork(patch);

	// printf ("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
	//           "SILENT = %s\n",
	//           arguments.args[0], arguments.args[1],
	//           arguments.output_file,
	//           arguments.silent ? "yes" : "no");

	// 1. TEMPORAIRE. Test de l'AST
	// struct noeud_ast *noeud_valeur = valeur(TRIANGLE);
	// struct noeud_ast *noeud_rotation = rotation(noeud_valeur);
	//
	// struct noeud_ast *noeud_feuille1 = valeur(CARRE);
	// struct noeud_ast *noeud_feuille2 = valeur(CARRE);
	// struct noeud_ast *noeud_juxtapo = juxtaposition(noeud_feuille1, noeud_feuille2);
	//
	// liberer_expression(noeud_rotation);
	// liberer_expression(noeud_juxtapo);
	//
	// // 2. TEMPORAIRE. Test de l'AST et de l'évaluation via le parser
	// struct noeud_ast *noeud_analyseur;
	// analyser("exemples_expressions/exemple_sujet", &noeud_analyseur);
	//
	// noeud_analyseur->afficher(noeud_analyseur);
	// struct patchwork *patch = noeud_analyseur->evaluer(noeud_analyseur);
	// printf("\n");
	//
	// // 3. TEMPORAIRE. Test de la traduction en image.
	// creer_image(patch, "motifs/carre_32.ppm", "motifs/triangle_32.ppm", fopen("test.ppm", "wb"));
	//
	// liberer_expression(noeud_analyseur);
	// liberer_patchwork(patch);
	return EXIT_SUCCESS;
}
