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

double Find_Var_Val(struct Tree* tree, const char* variable)
{
    for (int i = 0; i < tree -> num_var; i++)
    {
        if(strcmp((tree -> var_buf)[i].var, variable) == 0)
        {
            return (tree -> var_buf)[i].val;
        }
    }
    printf("\n\033[31mERROR\033[0m no such variable!!! in \"\033[33mFind_Var_Val\033[0m\"\n");
    return 0;
}

double Eval(struct Tree* tree, struct Node* node)
{
    if(node -> type == NUM)
    {
        return (node -> val).num;
    }
    else if(node -> type == VAR)
    {
        double var_val = Find_Var_Val(tree, (node -> val).var);
        return var_val;
    }

    double left  = Eval(tree, node -> left );
    double right = Eval(tree, node -> right);

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
        return sqrt(right);
    default:
        printf("\n\033[31mERROR[0m no operation!!!");
        exit(1);
    }
}

void Input_variable(struct Tree* tree)
{
    double value = 0;

    for (int i = 0; i < tree -> num_var; i++)
    {
        printf("\nPrint value of \033[32m%s\033[0m !\n", (tree -> var_buf)[i].var);
        scanf("%lf", &value);

        (tree -> var_buf)[i].val = value;
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
