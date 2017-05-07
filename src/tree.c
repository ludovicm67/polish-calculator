#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "tree.h"
#include "result.h"

// Concstructeur qui permet de construire un nouvel arbre
Tree construct_tree() {
    Tree t = malloc(sizeof(struct s_tree));
    if (!t) {
        fprintf(stderr, "Le malloc a échoué !\n");
        exit(EXIT_FAILURE);
    }
    t->type = OP_UNKNOWN; // par défaut le type est inconnu
    t->left_child = NULL;
    t->right_child = NULL;
    return t;
}

// On libère l'espace occupé par l'arbre t en mémoire
void free_tree(Tree t) {
    if (!t) return;
    free_tree(t->left_child);
    free_tree(t->right_child);
    free(t);
}

// FOnction auxilière pour print_tree(t)
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

// Affiche un arbre binaire (de manière indentée, idéal pour débugger)
void print_tree(Tree t) {
    _print_tree(t, 0);
}

// Vaut 1 si le fils gauche est un nombre, 0 sinon
unsigned int is_number_left_tree(Tree t) {
    if (!t) return 0;
    else if (t->left_child && is_number(t->left_child->type)) {
        return 1;
    } else return 0;
}

// Vaut 1 si le fils de droit est un nombre, 0 sinon
unsigned int is_number_right_tree(Tree t) {
    if (!t) return 0;
    else if (t->right_child && is_number(t->right_child->type)) {
        return 1;
    } else return 0;
}

// Permet de cacluler un noeud de l'arbre
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
    } else if (operator_size(t->type) == 1) {
        if (is_number_left_tree(t) && !t->right_child) {
            switch (t->type) {
                case OP_SQRT:
                    t->value = sqrt(t->left_child->value);
                    break;
                case OP_EXP:
                    t->value = exp(t->left_child->value);
                    break;
                case OP_LN:
                    t->value = log(t->left_child->value);
                    break;
                case OP_ABS:
                    t->value = fabs(t->left_child->value);
                    break;
                case OP_FLOOR:
                    t->value = floor(t->left_child->value);
                    break;
                case OP_CEIL:
                    t->value = ceil(t->left_child->value);
                    break;
                default:
                    break;
            }
            t->type = OP_NUMBER;
            free_tree(t->left_child);
            t->left_child = NULL;
        }
    }
}

// Réduit un arbre au maximum en effectueant les différentes opérations
void reduce_tree(Tree t) {
    if (!t) return;
    reduce_tree(t->left_child);
    reduce_tree(t->right_child);
    calc_node_tree(t);
}

// Retourne un curseur au prochain emplacement disponible
Cursor cursor_tree(Tree t) {
    Cursor c;
    c.t = NULL;
    c.ok = 0;

    if (!t || !is_operator(t->type)) return c;
    else if (is_number_left_tree(t) && is_number_right_tree(t)) return c;
    else if (!t->left_child) {
        c.t = t;
        c.ok = 1;
        return c;
    } else {
        c = cursor_tree(t->left_child);
        if (c.ok) return c;
        else if (operator_size(t->type) == 2) {
            if (!t->right_child) {
                c.t = t;
                c.ok = 1;
                return c;
            } else c = cursor_tree(t->right_child);
        }
    }

    return c;
}

// Ajoute un opérateur dans l'arbre (retourne 1 en cas de succès, 0 sinon)
unsigned int add_operator_to_tree(Tree t, Type op) {
    if (!t) return 0;
    else if (t->type == OP_UNKNOWN) {
        t->type = op;
        return 1;
    } else if (is_operator(t->type)) {
        Cursor c = cursor_tree(t);
        if (!c.ok) return 0;
        else if (!c.t->left_child) {
            c.t->left_child = construct_tree();
            c.t->left_child->type = op;
            return 1;
        } else if (!c.t->right_child) {
            c.t->right_child = construct_tree();
            c.t->right_child->type = op;
            return 1;
        } else return 0;
    }
    return 0;
}

// Ajoute un nombre dans l'arbre (retourne 1 en cas de succès, 0 sinon)
unsigned int add_number_to_tree(Tree t, double n) {
    if (!t) return 0;
    else if (t->type == OP_UNKNOWN) {
        t->value = n;
        t->type = OP_NUMBER;
        return 1;
    } else if (is_operator(t->type)) {
        Cursor c = cursor_tree(t);
        if (!c.ok) return 0;
        else if (!c.t->left_child) {
            c.t->left_child = construct_tree();
            c.t->left_child->value = n;
            c.t->left_child->type = OP_NUMBER;
            return 1;
        } else if (!c.t->right_child) {
            c.t->right_child = construct_tree();
            c.t->right_child->value = n;
            c.t->right_child->type = OP_NUMBER;
            return 1;
        } else return 0;
    }
    return 0;
}
