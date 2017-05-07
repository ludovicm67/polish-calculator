#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h> // for tolower()
#include "tree.h"
#include "result.h"

#define PRINT_PROMPT() printf(">>> ")

char * strtolower(char * str) {
    char * tmp = str;
    for (; *tmp; tmp++) {
        *tmp = tolower((unsigned char) *tmp);
    }
    return str;
}

int main() {
    char * lineptr = NULL;
    int is_tty = isatty(0), read;
    Result r;
    size_t n = 0;
    unsigned int stop = 0;

    if (is_tty) PRINT_PROMPT();
    while ((read = getline(&lineptr, &n, stdin)) != -1) {
        r = compute_result(strtolower(lineptr));
        print_result(r);
        stop = need_exit(r);
        if (stop) {
            free_result(r);
            break;
        }
        free_result(r);
        if (is_tty) PRINT_PROMPT();
    }
    free(lineptr);

    if (!stop) printf("\n");

    return EXIT_SUCCESS;
}
