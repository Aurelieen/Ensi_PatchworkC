#ifndef ANASYNT_H
#define ANASYNT_H

#include "ast.h"

/* analyser(f, &expr) : analyse une expression de motif (cf. description
 * du langage dans enonce du TP) contenue dans le fichier de nom f et
 * construit l'arbre syntaxique abstrait correspondant ; si f = NULL,
 * l'expression est lue sur l'entree standard */
extern void analyser(unsigned char *, struct noeud_ast **);

#endif /* ANASYNT_H */
