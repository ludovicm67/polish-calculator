#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

Tree construct_tree() {
    Tree t = malloc(sizeof(struct s_tree));
    if (!t) {
        fprintf(stderr, "Le malloc a échoué !\n");
        exit(EXIT_FAILURE);
    }
    t->type = OP_UNKNOWN;
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

// int add_number_to_tree(Tree t, double d) {
//     if (t->type == OP_UNKNOWN && !left_child && !right_child) {
//         t->type = OP_NUMBER;
//         t->value = d;
//     } else if (is_operator(t->type)) {

//     }

//     return -1; // @TODO : add error check
// }
