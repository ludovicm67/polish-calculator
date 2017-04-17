#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct s_tree {
    char * op; // operator (NULL if not)
    double val; // ignored if op != NULL
    struct s_tree *left_child, *right_child;
}  * Tree;

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



typedef struct s_result {
    unsigned int is_error;
    unsigned int is_empty;
    double value;
    char * err_msg;
} * Result;

Result construct_result() {
    Result r = malloc(sizeof(struct s_result));
    if (!r) {
        fprintf(stderr, "Le malloc a échoué !\n");
        exit(EXIT_FAILURE);
    }
    r->is_error = 0;
    r->is_empty = 1;
    r->err_msg = NULL;
    return r;
}

Result empty_result() {
    Result r = construct_result();
    r->is_empty = 1;
    return r;
}

Result error_result() {
    Result r = construct_result();
    r->is_error = 1;
    return r;
}

Result error_msg_result(char * msg) {
    Result r = error_result();
    r->err_msg = msg;
    return r;
}

Result value_result(double d) {
    Result r = construct_result();
    r->is_empty = 0;
    r->is_error = 0;
    r->value = d;
    return r;
}

void free_result(Result r) {
    if (!r) return;
    free(r);
}

void print_result(Result r) {
    if (!r) return;
    else if (r->is_error) {
        printf("ERROR");
        if (r->err_msg) printf(": %s", r->err_msg);
        printf("\n");
    }
    else if (!r->is_empty) printf("%.4f\n", r->value);
    free_result(r);
}

// Retourne la taille d'un opérateur (-1 en cas d'erreur)
int operator_size(char * op) {

    if (!op) return -1;

    // opérateurs binaires
    if (!strcmp(op, "+")
        || !strcmp(op, "-")
        || !strcmp(op, "*")
        || !strcmp(op, "/")
        || !strcmp(op, "min")
        || !strcmp(op, "max")) {
        return 2;
    }

    // opérateurs unaires
    if (!strcmp(op, "sqrt")
        || !strcmp(op, "exp")
        || !strcmp(op, "ln")
        || !strcmp(op, "abs")
        || !strcmp(op, "floor")
        || !strcmp(op, "ceil")) {
        return 1;
    }

    return -1;
}

// Indique si un opérateur existe ou non
unsigned int operator_exists(char * op) {
    return operator_size(op) != -1;
}

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

Result compute(char * line) {
    unsigned int line_length;

    if (!line) return error_result();
    else if ((line_length = strlen(line)) == 0) return empty_result();

    char * tok = strdup(line);
    char * start_tok = tok;

    memcpy(tok, line, line_length);

    while ((tok = strtok(tok, "  \t")) != NULL) {
        if (*tok == '#') break;
        // printf(" == WORD == : %s\n", tok);
        tok = NULL;
    }

    if (start_tok == tok) return empty_result();

    free(start_tok);

    return value_result(42);
}


int main() {

    struct stat sb;
    unsigned int is_pipe = check_if_pipe();
    int read;

    char * lineptr = NULL;
    size_t n = 0;

    if (!is_pipe) printf(">>> ");
    while (read = getline(&lineptr, &n, stdin) != -1) {
        print_result(compute(lineptr));
        if (!is_pipe) printf(">>> ");
    }
    free(lineptr);


    return EXIT_SUCCESS;
}