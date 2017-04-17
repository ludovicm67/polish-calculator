#ifndef __TREE_H__
#define __TREE_H__

typedef struct s_tree {
    char * op; // operator (NULL if not)
    double val; // ignored if op != NULL
    struct s_tree *left_child, *right_child;
}  * Tree;

Tree construct_tree();
void free_tree(Tree t);
Tree last_op_tree(Tree t);

#endif
