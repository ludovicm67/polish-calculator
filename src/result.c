#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "result.h"

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

Result compute_result(char * line) {
    unsigned int line_length;

    if (!line) return error_result();
    else if ((line_length = strlen(line)) == 0) return empty_result();

    char * tok = strdup(line);
    char * start_tok = tok;
    unsigned int is_first = 1;

    memcpy(tok, line, line_length);

    while ((tok = strtok(tok, "  \t")) != NULL) {
        if (*tok == '#') break;
        if (is_first) {
            // Quitte le programme si l'instruction commence par "quit"
            if (!memcmp(tok, "quit", 4)) {
                free(start_tok);
                exit(EXIT_SUCCESS);
            }
            is_first = 0;
        }
        // printf(" == WORD == : %s\n", tok);
        tok = NULL;
    }

    if (start_tok == tok) return empty_result();

    free(start_tok);

    return value_result(42);
}
