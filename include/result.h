#ifndef __RESULT_H__
#define __RESULT_H__

typedef struct s_result {
    unsigned int is_error;
    unsigned int is_empty;
    unsigned int need_exit;
    double value;
    char * err_msg;
} * Result;

Result construct_result();
Result empty_result();
Result error_result();
Result error_msg_result(char * msg);
Result value_result(double d);
Result exit_result();
unsigned int need_exit(Result r);
void free_result(Result r);
void print_result(Result r);
Result compute_result(char * line);


Type operator_type(char * op);
int operator_size(Type op);
char * operator_reverse(Type op);
unsigned int is_operator(Type op);
unsigned int is_number(Type op);

#endif
