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

Tree construct_tree();
void free_tree(Tree t);

#endif
