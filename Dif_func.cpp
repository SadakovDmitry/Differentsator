#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"

int Dtor_SubTree(struct Node* node)
{
    assert(node);

    if(!L)
    {
        Dtor_SubTree(L);
        node = NULL;
    }
    if(!R)
    {
        Dtor_SubTree(R);
        node = NULL;
    }

    free(node);
    node = NULL;

    return 1;
}

void Delete_Node(struct Node* node)
{
    free(node);
    node = NULL;


}




int Set_Priority(enum OPERATION op)
{
    switch(op)
    {
    #define OP(name, str, num_op, def, len, op_prior, ...)   \
        case name:                                      \
            return op_prior;

    #include "operators.h"
    default:
        fprintf(stderr, "\n" red(NO_OPERATION)" in Print_in_Operation");
    }
    #undef OP
    /*
switch(op)
    {
    case ADD:
        return 0;
    case SUB:
        return 0;
    case MUL:
        return 1;
    case DIV:
        return 1;
    case SQRT:
        return 20;
    case COS:
        return 0;
    case SIN:
        return 0;
    case TAN:
        return 0;
    case COT:
        return 0;
    case POW:
        return -100;
    case LOG:
        return 1;
    default:
        fprintf(stderr, "\n\033[31mNO_OPERATION\033[0m in Print_in_Operation");
    }
    */
}

struct Node* Create_Node(enum TYPE type, Tree_t value, struct Node* left_node, struct Node* right_node, struct Node* prev_node)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    new_node -> val   = value;
    new_node -> type  = type;
    new_node -> left  = left_node;
    new_node -> right = right_node;
    new_node -> prev  = prev_node;

    if(type == OP)
    {
        new_node -> priority = Set_Priority(value.op);
    }

    return new_node;
}

struct Node* Insert_to_Pointer(struct Tree* tree, Tree_t* value, struct Node* now_node, int indicator, enum TYPE type)
{
    struct Node* new_node = Create_Node(type, *value, NULL, NULL, now_node);
    tree -> size++;

    if(!(now_node))
    {
        tree -> root = new_node;
        return tree -> root;
    }

    //new_node -> prev = now_node;

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
    if(node == NULL) return NAN;

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
    case POW:
        return pow(left, right);
    case SIN:
        return sin(right);
    case COS:
        return cos(right);
    case TAN:
        return tan(right);
    case COT:
        return 1 / tan(right);
    case LOG:
        return log(right);
    default:
        fprintf(stderr, "\n\033[31mERROR\033[0m no operation!!!");
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
struct Node* Create_NUM_Node(enum TYPE type, double val, struct Node* left_node, struct Node* right_node)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    (new_node -> val).num = val;
    new_node -> type = type;
    new_node -> left = left_node;
    new_node -> right = right_node;
    new_node -> priority = -1;

    return new_node;
}

struct Node* Create_VAR_Node(enum TYPE type, char* val, struct Node* left_node, struct Node* right_node)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    (new_node -> val).var = val;
    new_node -> type = type;
    new_node -> left = left_node;
    new_node -> right = right_node;
    new_node -> priority = -1;

    return new_node;
}

struct Node* Create_OP_Node(enum TYPE type, enum OPERATION val, struct Node* left_node, struct Node* right_node)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    (new_node -> val).op = val;
    new_node -> type = type;
    new_node -> left = left_node;
    new_node -> right = right_node;
    new_node -> priority = Set_Priority(val);

    return new_node;
}
*/


struct Node* Copy_Node(struct Node* node)
{
    assert(node);

    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    if (node -> type == OP)
    {
        (new_node -> val).op = (node -> val).op;
    }
    else if (node -> type == NUM)
    {
        (new_node -> val).num = (node -> val).num;
    }
    else if (node -> type == VAR)
    {
        (new_node -> val).var = (node -> val).var;
    }

    new_node -> type  = node -> type;
    new_node -> left  = node -> left;
    new_node -> right = node -> right;
    new_node -> prev  = node -> prev;
    new_node -> priority = node -> priority;

    return new_node;
}

struct Node* Der(struct Node* node)
{
    assert(node);

    switch (node -> type)
    {
    case NUM:
        return CONST(0);
    case VAR:
        if (strcmp((node -> val).var, "e") == 0) return E;
        else return CONST(1);
    default:

    //--------------------------------------------------------
    #define OP(name, str_symbol, enum, dif, ...) \
        case name:                               \
            return dif;
    //--------------------------------------------------------

    switch((node -> val).op)
    {
    #include "operators.h"
    /*
        case ADD:
            return ADD(D(L), D(R));
        case SUB:
            return SUB(D(L), D(R));
        case MUL:
            return ADD(MUL(C(L), Der(R)) , MUL(D(L), C(L)));
        case DIV:
            return DIV(SUB(MUL(C(L), D(R)) , MUL(Der(L), C(L))), POW(C(R), CONST(2)));
        case SQRT:
            return MUL(D(R), DIV(CONST(1), MUL(CONST(2), C(node))));
        case SIN:
            return MUL(D(R), COS(C(R)));
        case COS:
            return MUL(D(R), MUL(CONST(-1), SIN(C(R))));
        case TAN:
            return MUL(D(R), DIV(CONST(1), POW(COS(C(R)), CONST(2))));
        case COT:
            return MUL(D(R), MUL(CONST(-1), SIN(DIV(CONST(1), POW(SIN(C(R)), CONST(2))))));
        case POW:
            return MUL(C(node), ADD(MUL(D(R), LOG(E, C(L))), MUL(DIV(Der(L), C(L)), C(R))));
        case LOG:
            return MUL(D(R), DIV(CONST(1), C(R)));
        default:
            return C(node);
        */
    }

    #undef OP
    return node;
    }
}

double Calculate_val(struct Node* node)
{
    assert(node);

    double left = ((node -> left) -> val).num;
    double right = ((node -> right) -> val).num;

    //---------------------------------------------------------------
    #define OP(name, str_op, num_op, def, len, priority, calculate, ...) \
    case name:                                                      \
        return calculate;
    //---------------------------------------------------------------

    switch((node -> val).op)
    {
    #include "operators.h"
    /*
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
    case POW:
        return pow(left, right);
    case SIN:
        return sin(right);
    case COS:
        return cos(right);
    case TAN:
        return tan(right);
    case COT:
        return 1 / tan(right);
    case LOG:
        return log(right);
    */
    default:
        fprintf(stderr, "\n" red(ERROR) " no operation!!!");
        exit(1);
    }
    #undef OP
}



int Reduce_MUL_Left(struct Node* node)
{
    struct Node* tmp_node = L;

    if((((L) -> right) -> val).num == 1)
    {
        L = (L) -> left;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 1)
    {
        L = (L) -> right;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 0 || (((L) -> right) -> val).num == 0)
    {
        L = CONST(0);
        Delete_Node(tmp_node);
        return 1;
    }

    return 0;
}

int Reduce_ADD_Left(struct Node* node)
{
    struct Node* tmp_node = L;

    if((((L) -> left) -> val).num == 0)
    {
        L = (L) -> right;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 0)
    {
        L = (L) -> left;
        Delete_Node(tmp_node);
        return 1;
    }

    return 0;
}

int Reduce_POW_Left(struct Node* node)
{
    struct Node* tmp_node = L;

    if((((L) -> left) -> val).num == 1)
    {
        L = CONST(1);
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 1)
    {
        L = (L) -> left;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 1)
    {
        L = CONST(0);
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 1)
    {
        L = CONST(1);
        Delete_Node(tmp_node);
        return 1;
    }

    return 0;
}



int Reduce_MUL_Right(struct Node* node)
{
    struct Node* tmp_node = R;

    if((((R) -> right) -> val).num == 1)
    {
        R = (R) -> left;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 1)
    {
        R = (R) -> right;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((R) -> left) -> val).num == 0 || (((R) -> right) -> val).num == 0)
    {
        R = CONST(0);
        Delete_Node(tmp_node);
        return 1;
    }

    //Delete_Node(tmp_node);
    return 0;
}

int Reduce_ADD_Right(struct Node* node)
{
    struct Node* tmp_node = R;

    if((((R) -> left) -> val).num == 0)
    {
        R = (R) -> right;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 0)
    {
        R = (R) -> left;
        Delete_Node(tmp_node);
        return 1;
    }

    return 0;
}

int Reduce_POW_Right(struct Node* node)
{
    struct Node* tmp_node = R;

    if((((R) -> left) -> val).num == 1)
    {
        R = CONST(1);
        Delete_Node(tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 1)
    {
        R = (R) -> left;
        Delete_Node(tmp_node);
        return 1;
    }
    if((((R) -> left) -> val).num == 1)
    {
        R = CONST(0);
        Delete_Node(tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 1)
    {
        R = CONST(1);
        Delete_Node(tmp_node);
        return 1;
    }

    return 0;
}

void Join_Const(struct Node* node)
{
    double new_val = Calculate_val(node);
    node = CONST(new_val);
}



int Reduce_Const(struct Node* node)
{
    assert(node);

    if(node -> left != NULL)
    {
        if((node -> left) -> type == OP)
        {
            switch(((node -> left) -> val).op)
            {
            case MUL:
                if (Reduce_MUL_Left(node)) return 1;
            case ADD:
                if (Reduce_ADD_Left(node)) return 1;
            case POW:
                if (Reduce_POW_Left(node)) return 1;
            default:
                break;
            }
        }
    }
    if(node -> right != NULL)
    {
        if((node -> right) -> type == OP)
        {
            switch(((node -> right) -> val).op)
            {
            case MUL:
                if (Reduce_MUL_Right(node)) return 1;
            case ADD:
                if (Reduce_ADD_Right(node)) return 1;
            case POW:
                if (Reduce_POW_Right(node)) return 1;
            default:
                break;
            }
        }

        if((node -> right) -> type == NUM)
        {
            Join_Const(node);
            return 1;
        }
    }

    return 0;

}

void Reduce_Tree(struct Tree* tree, struct Node* node)
{
    if(node == NULL) return;

    Reduce_Tree(tree, node -> left);
    tree -> version += Reduce_Const(node);
    Reduce_Tree(tree, node -> right);

    if(tree -> version != 0)
    {
        tree -> version = 0;
        Reduce_Tree(tree, tree -> root);
    }

    return;
}



void Check_Operation(struct Node* node)
{
    switch(node -> type)
    {
    case OP:
        if(node -> right == NULL)
        {
            fprintf(stderr, red(ERROR)"right == NULL after OP node");
        }
    case NUM:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after NUM node");
        }
    case VAR:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after VAR node");
        }
    }
}

