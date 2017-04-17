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


int main() {
    char * lineptr = NULL;
    int read;
    Result r;
    size_t n = 0;
    unsigned int is_pipe = check_if_pipe();

    if (!is_pipe) PRINT_PROMPT();
    while ((read = getline(&lineptr, &n, stdin)) != -1) {
        r = compute_result(strtolower(lineptr));
        print_result(r);
        if (need_exit(r)) {
            free_result(r);
            break;
        }
        free_result(r);
        if (!is_pipe) PRINT_PROMPT();
    }
    free(lineptr);

    // Tree t = construct_tree();

    // add_operator_to_tree(t, OP_SUBSTRACTION);
    // add_operator_to_tree(t, OP_MULTIPLICATION);
    // add_operator_to_tree(t, OP_DIVISION);
    // add_number_to_tree(t, 15);
    // add_operator_to_tree(t, OP_SUBSTRACTION);
    // add_number_to_tree(t, 7);
    // add_operator_to_tree(t, OP_ADDITION);
    // add_number_to_tree(t, 1);
    // add_number_to_tree(t, 1);
    // add_number_to_tree(t, 3);
    // add_operator_to_tree(t, OP_ADDITION);
    // add_number_to_tree(t, 2);
    // add_operator_to_tree(t, OP_ADDITION);
    // add_number_to_tree(t, 1);
    // add_number_to_tree(t, 1);

    // reduce_tree(t);


    // print_tree(t);

    // free_tree(t);


    return EXIT_SUCCESS;
}
