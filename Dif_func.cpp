#include "lib.h"
#include "dif_func.h"



struct Node* Create_Node(enum TYPE type, union Value* value, struct Node* left_node, struct Node* right_node)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    if (type == OP)
    {
        (new_node -> val).op = value -> op;
    }
    else if (type == NUM)
    {
        (new_node -> val).num = value -> num;
    }

    new_node -> type = type;
    new_node -> left = left_node;
    new_node -> right = right_node;

    return new_node;
}

struct Node* Insert_to_Pointer(struct Tree* tree, union Value* value, struct Node* now_node, int indicator, enum TYPE type)
{
    struct Node* new_node = Create_Node(type, value, NULL, NULL);
    tree -> size++;

    if(!(now_node))
    {
        tree -> root = new_node;
        return tree -> root;
    }

    new_node -> prev = now_node;

    if(indicator == INSERT_LEFT)
    {
        now_node -> left  = new_node;
    }
    else if(indicator == INSERT_RIGHT)
    {
        now_node -> right = new_node;
    }

    return new_node;
}
