#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tree.h"
#include "result.h"

#define PRINT_PROMPT() printf(">>> ")

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

int main() {
    unsigned int is_pipe = check_if_pipe();
    int read;

    char * lineptr = NULL;
    size_t n = 0;

    if (!is_pipe) PRINT_PROMPT();
    while ((read = getline(&lineptr, &n, stdin)) != -1) {
        print_result(compute_result(lineptr));
        if (!is_pipe) PRINT_PROMPT();
    }
    free(lineptr);


    return EXIT_SUCCESS;
}
