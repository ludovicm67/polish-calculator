#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h> // for tolower()
#include "tree.h"
#include "result.h"

#define PRINT_PROMPT() printf(">>> ")


// Indique si l'entrée provient d'un pipe ou non
unsigned int check_if_pipe() {
    struct stat buf;
    if (fstat(STDIN_FILENO, &buf) == 0) {
        if (S_ISFIFO(buf.st_mode)) return 1;
        else return 0;
    } else {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    return 0;
}

char * strtolower(char * str) {
    char * tmp = str;
    for (; *tmp; tmp++) {
        *tmp = tolower((unsigned char) *tmp);
    }
    return str;
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
    if (is_operator(t->type)) {
        if (operator_size(t->type) == 2) {
            if (is_number_left_tree(t) && is_number_right_tree(t)) {
                switch (t->type) {
                    case OP_ADDITION:
                        t->value = t->left_child->value + t->left_child->value;
                        break;
                    case OP_SUBSTRACTION:
                        t->value = t->left_child->value - t->left_child->value;
                        break;
                    case OP_MULTIPLICATION:
                        t->value = t->left_child->value * t->left_child->value;
                        break;
                    case OP_DIVISION:
                        t->value = t->left_child->value / t->left_child->value;
                        break;
                    case OP_MINIMUM:
                        if (t->left_child->value > t->left_child->value) {
                            t->value = t->right_child->value;
                        } else t->value = t->left_child->value;
                        break;
                    case OP_MAXIMUM:
                        if (t->left_child->value > t->left_child->value) {
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
}


typedef struct s_cursor {
    Tree t;
    unsigned int ok;
} Cursor;

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

int main() {
    // char * lineptr = NULL;
    // int read;
    // Result r;
    // size_t n = 0;
    // unsigned int is_pipe = check_if_pipe();

    // if (!is_pipe) PRINT_PROMPT();
    // while ((read = getline(&lineptr, &n, stdin)) != -1) {
    //     r = compute_result(strtolower(lineptr));
    //     print_result(r);
    //     if (need_exit(r)) {
    //         free_result(r);
    //         break;
    //     }
    //     free_result(r);
    //     if (!is_pipe) PRINT_PROMPT();
    // }
    // free(lineptr);

    Tree t = construct_tree();

    // add_operator_to_tree(t, OP_ADDITION);
    // add_number_to_tree(t, 1);
    // add_operator_to_tree(t, OP_ADDITION);
    // add_number_to_tree(t, 2);
    // add_number_to_tree(t, 3);
    // calc_node_tree(t);

    add_operator_to_tree(t, OP_SUBSTRACTION);
    add_operator_to_tree(t, OP_MULTIPLICATION);
    add_operator_to_tree(t, OP_DIVISION);
    add_number_to_tree(t, 15);
    add_operator_to_tree(t, OP_SUBSTRACTION);
    add_number_to_tree(t, 7);
    add_operator_to_tree(t, OP_ADDITION);
    add_number_to_tree(t, 1);
    add_number_to_tree(t, 1);
    add_number_to_tree(t, 3);
    add_operator_to_tree(t, OP_ADDITION);
    add_number_to_tree(t, 2);
    add_operator_to_tree(t, OP_ADDITION);
    add_number_to_tree(t, 1);
    add_number_to_tree(t, 1);


    print_tree(t);

    free_tree(t);


    return EXIT_SUCCESS;
}
