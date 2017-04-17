#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "result.h"

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

void _print_tree(Tree t, unsigned int n) {
    unsigned int i;
    if (!t || t->type == OP_UNKNOWN) return;
    printf("\n");
    for (i = 0; i < n; i++) printf("\t");

    if (is_operator(t->type)) {
        printf("Opérateur %s\n", operator_reverse(t->type));
    } else if (is_number(t->type)) {
        printf("VALEUR = %.4f\n", t->value);
    }

    for (i = 0; i < n; i++) printf("\t");
    printf(" - GAUCHE :\n");
    _print_tree(t->left_child, n+1);
    for (i = 0; i < n; i++) printf("\t");
    printf(" - DROIT :\n");
    _print_tree(t->right_child, n+1);

}

// affiche un arbre binaire (de manière indentée, idéal pour débugger)
void print_tree(Tree t) {
    _print_tree(t, 0);
}

unsigned int is_number_left_tree(Tree t) {
    if (!t) return 0;
    else if (t->left_child && is_number(t->left_child->type)) {
        return 1;
    } else return 0;
}

unsigned int is_number_right_tree(Tree t) {
    if (!t) return 0;
    else if (t->right_child && is_number(t->right_child->type)) {
        return 1;
    } else return 0;
}

void calc_node_tree(Tree t) {
    if (!t) return;
    else if (operator_size(t->type) == 2) {
        if (is_number_left_tree(t) && is_number_right_tree(t)) {
            switch (t->type) {
                case OP_ADDITION:
                    t->value = t->left_child->value + t->right_child->value;
                    break;
                case OP_SUBSTRACTION:
                    t->value = t->left_child->value - t->right_child->value;
                    break;
                case OP_MULTIPLICATION:
                    t->value = t->left_child->value * t->right_child->value;
                    break;
                case OP_DIVISION:
                    t->value = t->left_child->value / t->right_child->value;
                    break;
                case OP_MINIMUM:
                    if (t->left_child->value > t->right_child->value) {
                        t->value = t->right_child->value;
                    } else t->value = t->left_child->value;
                    break;
                case OP_MAXIMUM:
                    if (t->left_child->value > t->right_child->value) {
                        t->value = t->left_child->value;
                    } else t->value = t->right_child->value;
                    break;
                default:
                    break;
            }
            t->type = OP_NUMBER;
            free_tree(t->left_child);
            free_tree(t->right_child);
            t->left_child = NULL;
            t->right_child = NULL;
        }
    }
}

void reduce_tree(Tree t) {
    if (!t) return;
    reduce_tree(t->left_child);
    reduce_tree(t->right_child);
    calc_node_tree(t);
}

Cursor cursor_tree(Tree t) {
    Cursor c;
    c.t = NULL;
    c.ok = 0;

    if (!t) return c;
    else if (!is_operator(t->type)) return c;
    else if (is_number_left_tree(t) && is_number_right_tree(t)) {
        return c;
    } else {
        if (!t->left_child) {
            c.t = t;
            c.ok = 1;
            return c;
        } else {
            c = cursor_tree(t->left_child);
            if (c.ok) {
                return c;
            } else if (!t->right_child) {
                c.t = t;
                c.ok = 1;
                return c;
            } else {
                c = cursor_tree(t->right_child);
            }
        }
    }

    return c;
}

void add_operator_to_tree(Tree t, Type op) {
    if (!t) return;
    else if (t->type == OP_UNKNOWN) {
        t->type = op;
    } else if (is_operator(t->type)) {

        Cursor c = cursor_tree(t);
        if (!c.ok) {
            printf("PAS DE PLACE DISPO !\n");
            return;
        }

        if (!c.t->left_child) {
            c.t->left_child = construct_tree();
            c.t->left_child->type = op;
        } else if (!c.t->right_child) {
            c.t->right_child = construct_tree();
            c.t->right_child->type = op;
        } else {
            printf("PAS DE PLACE DISPO !\n");
            return;
        }


    }
}

void add_number_to_tree(Tree t, double n) {
    if (!t) return;
    else if (t->type == OP_UNKNOWN) {
        t->value = n;
        t->type = OP_NUMBER;
    } else if (is_operator(t->type)) {

        Cursor c = cursor_tree(t);
        if (!c.ok) {
            printf("PAS DE PLACE DISPO !\n");
            return;
        }

        if (!c.t->left_child) {
            c.t->left_child = construct_tree();
            c.t->left_child->value = n;
            c.t->left_child->type = OP_NUMBER;
        } else if (!c.t->right_child) {
            c.t->right_child = construct_tree();
            c.t->right_child->value = n;
            c.t->right_child->type = OP_NUMBER;
        } else {
            printf("PAS DE PLACE DISPO !\n");
            return;
        }

    }
}
