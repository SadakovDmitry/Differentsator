#include "lib.h"
#include "dif_func.h"



struct Node* Create_Node(enum TYPE type, struct Value* value, struct Node* left_node, struct Node* right_node)
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

struct Node* Insert_to_Pointer(struct Tree* tree, struct Value* value, struct Node* now_node, int indicator, enum TYPE type)
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

    return new_node;                //new_node
}

double Eval(struct Node* node)
{
    if(node -> type == NUM)
    {
        return (node -> val).num;
    }

    double left  = Eval(node -> left );
    double right = Eval(node -> right);

    //Check_Node(node);

    switch((node -> val).op)
    {
    case ADD:
        return left + right;
    case SUB:
        return left - right;
    case MUL:
        return left * right;
    case DIV:
        return left / right;
    case SQRT:
        return sqrt(left);
    default:
        printf("\n\033[31mERROR[0m no operation!!!");
        exit(1);
    }
}

/*
void Check_Operation(struct Node* node)
{
    if (type == OP)
    {

    }
    else if (type == NUM)
    {

    }
}
*/
