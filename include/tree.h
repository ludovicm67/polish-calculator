#ifndef __TREE_H__
#define __TREE_H__

enum op_type {
    OP_UNKNOWN,
    OP_NUMBER,

    OP_ADDITION,
    OP_SUBSTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_MINIMUM,
    OP_MAXIMUM,

    OP_SQRT,
    OP_EXP,
    OP_LN,
    OP_ABS,
    OP_FLOOR,
    OP_CEIL
};
typedef enum op_type Type;

typedef struct s_tree {
    enum op_type type;
    double value; // ignored if (type != OP_UNKNOWN && type != OP_NUMBER)
    struct s_tree *left_child, *right_child;
}  * Tree;

typedef struct s_cursor {
    Tree t;
    unsigned int ok;
} Cursor;

Tree construct_tree();
void free_tree(Tree t);
void _print_tree(Tree t, unsigned int n);
void print_tree(Tree t);
unsigned int is_number_left_tree(Tree t);
unsigned int is_number_right_tree(Tree t);
void calc_node_tree(Tree t);
void reduce_tree(Tree t);
Cursor cursor_tree(Tree t);
unsigned int add_operator_to_tree(Tree t, Type op);
unsigned int add_number_to_tree(Tree t, double n);

#endif
