#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "result.h"

// Constructeur pour transmettre des résultats
Result construct_result() {
    Result r = malloc(sizeof(struct s_result));
    if (!r) {
        fprintf(stderr, "Le malloc a échoué !\n");
        exit(EXIT_FAILURE);
    }
    r->is_error = 0;
    r->is_empty = 1;
    r->need_exit = 0;
    r->err_msg = NULL;
    return r;
}

// Retourne un résultat vide
Result empty_result() {
    Result r = construct_result();
    r->is_empty = 1;
    return r;
}

// Retourne un résultat avec une erreur
Result error_result() {
    Result r = construct_result();
    r->is_error = 1;
    return r;
}

// Retourne un résultat avec un message d'erreur
Result error_msg_result(char * msg) {
    Result r = error_result();
    r->err_msg = msg;
    return r;
}

// Retourne un résultat contenant une valeur
Result value_result(double d) {
    Result r = construct_result();
    r->is_empty = 0;
    r->is_error = 0;
    r->value = d;
    return r;
}

// Pour dire que l'on souhaite faire quitter le programme
Result exit_result() {
    Result r = empty_result();
    r->need_exit = 1;
    return r;
}

// Permet de vérifier s'il y a besoin de faire quitter le programme ou non
unsigned int need_exit(Result r) {
    if (!r) return 0;
    else return r->need_exit;
}

// Permet de libérer la structure utilisée pour la transmission de résultat
void free_result(Result r) {
    if (!r) return;
    free(r);
}

// Affiche un résultat
void print_result(Result r) {
    if (!r) return;
    else if (r->is_error) {
        if (r->err_msg) fprintf(stderr, "Error: %s\n", r->err_msg);
        printf("ERROR\n");
    }
    else if (!r->is_empty) printf("%.4f\n", r->value);
}

// Vérifie si deux chaînes de caractères sont égales ou non
unsigned int string_equals(char * str1, char * str2) {
    if (strlen(str1) != strlen(str2)) return 0;
    return !memcmp(str1, str2, strlen(str2));
}

Type operator_type(char * op) {
    if (!op) return OP_UNKNOWN;

    // opérateurs binaires
    switch (*op) {
        case '+':
            return OP_ADDITION;
        case '-':
            return OP_SUBSTRACTION;
        case '*':
            return OP_MULTIPLICATION;
        case '/':
            return OP_DIVISION;
    }
    if (string_equals(op, "min")) return OP_MINIMUM;
    else if (string_equals(op, "max")) return OP_MAXIMUM;

    // opérateurs unaires
    else if (string_equals(op, "sqrt")) return OP_SQRT;
    else if (string_equals(op, "exp")) return OP_EXP;
    else if (string_equals(op, "ln")) return OP_LN;
    else if (string_equals(op, "abs")) return OP_ABS;
    else if (string_equals(op, "floor")) return OP_FLOOR;
    else if (string_equals(op, "ceil")) return OP_CEIL;

    return OP_UNKNOWN;
}

// Retourne la taille d'un opérateur (-1 en cas d'erreur)
int operator_size(Type op) {

    switch (op) {
        case OP_UNKNOWN:
            return -1;

        case OP_NUMBER:
            return 0;

        case OP_ADDITION:
        case OP_SUBSTRACTION:
        case OP_MULTIPLICATION:
        case OP_DIVISION:
        case OP_MINIMUM:
        case OP_MAXIMUM:
            return 2;

        case OP_SQRT:
        case OP_EXP:
        case OP_LN:
        case OP_ABS:
        case OP_FLOOR:
        case OP_CEIL:
            return 1;

        default:
            return -1;
    }

}

char * operator_reverse(Type op) {

    switch (op) {
        case OP_UNKNOWN:
            return "?";
        case OP_NUMBER:
            return "isNumber";
        case OP_ADDITION:
            return "+";
        case OP_SUBSTRACTION:
            return "-";
        case OP_MULTIPLICATION:
            return "*";
        case OP_DIVISION:
            return "/";
        case OP_MINIMUM:
            return "min";
        case OP_MAXIMUM:
            return "max";
        case OP_SQRT:
            return "sqrt";
        case OP_EXP:
            return "exp";
        case OP_LN:
            return "ln";
        case OP_ABS:
            return "abs";
        case OP_FLOOR:
            return "floor";
        case OP_CEIL:
            return "";
        default:
            return "?";
    }

}

unsigned int is_operator(Type op) {
    int size = operator_size(op);
    return size == 1 || size == 2;
}

unsigned int is_number(Type op) {
    return operator_size(op) == 0;
}


Result compute_result(char * line) {
    unsigned int line_length;

    if (!line) return error_result();
    else if ((line_length = strlen(line)) == 0 || *line == '\n') {
        return empty_result();
    }

    char * tok = strdup(line);
    char * start_tok = tok;
    char *lastptr, *endptr;
    double number;
    unsigned int is_first = 1, is_empty = 0, is_err = 0;
    Tree t = construct_tree();
    Type op_t;

    memcpy(tok, line, line_length);

    while ((tok = strtok(tok, "  \t\r\n")) != NULL) {
        if (*tok == '#') break;
        if (is_first) {
            // Quitte le programme si l'instruction commence par "quit"
            if (!memcmp(tok, "quit", 4)) {
                free(start_tok);
                free_tree(t);
                return exit_result();
            }
            is_first = 0;
        }

        lastptr = tok;
        number = strtod(lastptr, &endptr);

        // S'il a réussi  lire un nombre
        if (lastptr != endptr && *endptr == '\0') {
            add_number_to_tree(t, number);
        } else {
            op_t = operator_type(endptr);
            if (!is_operator(op_t)) {
                free(start_tok);
                free_tree(t);
                return error_msg_result("unknown operator");
            } else add_operator_to_tree(t, op_t);
        }

        tok = NULL;
    }

    // print_tree(t);
    reduce_tree(t);
    // print_tree(t);

    if (start_tok == tok) is_empty = 1;
    if (is_number(t->type)) number = t->value;
    else is_err = 1;

    free(start_tok);
    free_tree(t);

    if (is_empty) return empty_result();
    else if (is_err) return error_msg_result("missing operand");
    else return value_result(number);
}
