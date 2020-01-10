#include "analyse.h"

/* Fonction AUXILIAIRE pour la construction d'arbre : renvoie un squelette
 * d'arbre pour 'nbChildren' fils et d'etiquette 'op' donnee. L'appelant
 * doit lui-même stocker ses fils dans la strucutre que MakeNode renvoie
 */
TreeP makeNode(int nbChildren, Etiquette op) {
  TreeP tree = NEW(1, Tree);
  tree->op = op; tree->nbChildren = nbChildren;
  tree->u.children = nbChildren > 0 ? NEW(nbChildren, TreeP) : NIL(TreeP);
  return(tree);
}


/* Construction d'un arbre a nbChildren branches, passees en parametres
 * 'op' est une etiquette symbolique qui permet de memoriser la construction
 * dans le programme source qui est representee par cet arbre.
 * Une liste preliminaire d'etiquettes est dans tp.h; il faut l'enrichir selon
 * vos besoins.
 * Cette fonction prend un nombre variable d'arguments: au moins deux.
 * Les eventuels arguments supplementaires doivent etre de type TreeP
 * (defini dans tp.h)
 */
TreeP makeTree(Etiquette op, int nbChildren, ...) {
  va_list args;
  int i;
  TreeP tree = makeNode(nbChildren, op); 
  va_start(args, nbChildren);
  for (i = 0; i < nbChildren; i++) { 
    tree->u.children[i] = va_arg(args, TreeP);
  }
  va_end(args);
  return(tree);
}


/* Retourne le i-ieme fils d'un arbre (de 0 a n-1) */
TreeP getChild(TreeP tree, int i) {
  return tree->u.children[i];
}


/* Constructeur de feuille dont la valeur est un entier */
TreeP makeLeafInt(Etiquette op, int val) {
  TreeP tree = makeNode(0, op); 
  tree->u.val = val;
  return(tree);
}


/* Constructeur de feuille dont la valeur est une chaine de caracteres.
 * Construit un doublet pour la future variable et stocke son nom dedans.
 */
TreeP makeLeafStr(Etiquette op, char *str) {
  TreeP tree = makeNode(0, op); 
  tree->u.str = str;
  return(tree);
}


/* Verifie que nouv n'apparait pas deja dans list. l'ajoute en tete et
 * renvoie la nouvelle liste. 
 */
VarDeclP addToScope(VarDeclP list, VarDeclP nouv) {
  VarDeclP p;
  for(p=list; p != NIL(VarDecl); p = p->next) {
    if (! strcmp(p->name, nouv->name)) {
      fprintf(stderr, "Error: Multiple declaration in the same scope of %s\n",
	      p->name);
      setError(CONTEXT_ERROR);
      break;
    }
  }
  /* On continue meme en cas de double declaration, pour pouvoir eventuellement
   * detecter plus d'une erreur
   */
  nouv->next=list;
  return nouv;
}


/**
 * 	A partir d'ici les fonctions ont besoin d'etre modifiees/completees
 **/

void printOpBinaire(char op) {
  switch(op) {
  case Eeq:    printf("="); break;
  case Eneq:    printf("<>"); break;
  case Egt:    printf("<"); break;
  case Ege:    printf(">="); break;
  case Elt:    printf("<"); break;
  case Ele:    printf("<="); break;
  case Eadd:  printf("+"); break;
  case Eminus:printf("-"); break;
  case Emult: printf("*"); break;
  case Ediv:  printf("/"); break;
  default:
    fprintf(stderr, "Unexpected binary operator of code: %d\n", op);
    exit(UNEXPECTED);
  }
}

void printExpr(TreeP tree) {
  switch (tree->op) {
  case Eidvar :
    printf("%s", tree->u.str); break;
  case Econst:
    printf("%d", tree->u.val); break;
  case Eite:
    printf("[ITE "); printExpr(getChild(tree, 0)); /* la condition */
    printf(", "); printExpr(getChild(tree, 1)); /* la partie 'then' */
    printf(", "); printExpr(getChild(tree, 2)); /* la partie 'else' */
    printf("]");
    break;
  case Eeq:
  case Eneq:
  case Egt:
  case Ege:
  case Elt:
  case Ele:
  case Eadd:
  case Eminus:
  case Emult:
  case Ediv:
    printf("(");
    printOpBinaire(tree->op);
    printf(" "); printExpr(getChild(tree, 0));
    printf(" "); printExpr(getChild(tree, 1));
    printf(")"); break;
  default:
    fprintf(stderr, "Erreur! etiquette indefinie: %d\n", tree->op);
    exit(UNEXPECTED);
  }
}

/* decls est l'AST représentant lapartie declarations */
void printDecls(TreeP decls) {
  if (decls == NIL(Tree)) return;
}


/* decls est l'AST représentant lapartie declarations 
 * main represente l'expression finale du programme
 */

void printAST(TreeP decls, TreeP main) {
  printDecls(decls);
  printExpr(main); printf("\n");
}


/* Avant evaluation, verifie si tout identificateur qui apparait dans tree a
 * bien ete declare (dans ce cas il doit apparaitre dans lvar).
 */
bool checkScope(TreeP tree, VarDeclP lvar) {
  fprintf(stderr, "Erreur! etiquette indefinie: %d\n", tree->op);
  exit(UNEXPECTED);
}


/* Associe une variable a l'expression qui definit sa valeur, et procede a
 * l'evaluation de cette expression, sauf si on est en mode noEval
 */
VarDeclP declVar(char *name, TreeP tree, VarDeclP decls) {
  return NIL(VarDecl);
}


/* Traite une declaration representee par l'AST 'tree'.
 * 'decls' represente la liste des couples (variable, valeur) definis par les
 * declarations qui precedaient la declaration courante dans le programme,
 * donc les variables qui peuvent apparaitre aux feuilles de 'tree'
*/
VarDeclP evalAff (TreeP tree, VarDeclP decls) {
  return NIL(VarDecl);
}

/* Ici 'tree' correspond a l'AST pour une liste de declarations. */
VarDeclP evalDecls (TreeP tree) {
  return NIL(VarDecl);
}


/* Evaluation d'un if then else
 * le premier fils represente la condition,
 * les deux autres fils correspondent respectivement aux parties then et else.
 * Attention a n'evaluer qu'un seul de ces deux sous-arbres !
 */
int evalIf(TreeP tree, VarDeclP decls) {
  fprintf(stderr, "evalIf: a definir\n");
  exit(UNEXPECTED);
}


/* retourne la valeur d'une variable: 'tree' correspond a une feuille qui
 * represente un identificateur. decls est la liste courante des couples
 * (variable, valeur). On est suppose avoir deja verifie que l'identificateur
 * etait bien declare, donc on doit trouver sa valeur.
 */
int getValue(TreeP tree, VarDeclP decls) {
  fprintf(stderr, "evalIf: a definir\n");
  exit(UNEXPECTED);
}


/* eval: parcours recursif de l'AST d'une expression en cherchant dans
 * l'environnement la valeur des variables referencee
 * tree: l'AST d'une expression
 * decls: la liste des variables deja declarées avec leur valeur.
 */
int eval(TreeP tree, VarDeclP decls) {
  fprintf(stderr, "eval: a definir\n");
  exit(UNEXPECTED);
}

/* evalMain: evaluation de l'expression finale.
 * tree: AST de l'expression comprise entre le BEGIN et le END
 * decls: l'environnement forme par les variables declarees
 */
int evalMain(TreeP tree, VarDeclP decls) {
  fprintf(stderr, "evalMain: a definir\n");
  exit(UNEXPECTED);
}
