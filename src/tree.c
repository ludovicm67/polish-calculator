#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

Tree construct_tree() {
    Tree t = malloc(sizeof(struct s_tree));
    if (!t) {
        fprintf(stderr, "Le malloc a échoué !\n");
        exit(EXIT_FAILURE);
    }
    t->op = NULL;
    t->left_child = NULL;
    t->right_child = NULL;
    return t;
}

void free_tree(Tree t) {
    if (!t) return;
    free_tree(t->left_child);
    free_tree(t->right_child);
    free(t);
}

// Dernier opérateur de l'arbre
Tree last_op_tree(Tree t) {
    if (!t) return t;
    else if (!t->op) return NULL;
    else if (!t->left_child) return t;
    while (t->left_child->op) t = t->left_child;
    return t;
}
