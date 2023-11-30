#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"


//                                                    Delete
//------------------------------------------------------------------------------------------------------------------------------
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

void Delete_Node(struct Node** node)
{
    free(*node);
    *node = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                                  Create_Tree
//------------------------------------------------------------------------------------------------------------------------------
int Set_Priority(enum OPERATION op)
{
    switch(op)
    {
    //----------------------------------------------------------
    #define OP(name, str, num_op, def, len, op_prior, ...)   \
        case name:                                           \
            return op_prior;
    //----------------------------------------------------------

    #include "operators.h"

    default:
        fprintf(stderr, "\n" red(NO_OPERATION)" in Print_in_Operation");
    }

    #undef OP
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
    if(type == NUM || type == VAR)
    {
         new_node -> priority = NUM_PRIORITY;
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
        printf("\nPrint value of " green(%s)" !\n", (tree -> var_buf)[i].var);
        scanf("%lf", &value);

        (tree -> var_buf)[i].val = value;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                                  Differinstor
//------------------------------------------------------------------------------------------------------------------------------
struct Node* Copy_Subtree(struct Node* node)
{
    if(node == NULL) return NULL;

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
    new_node -> left = Copy_Subtree(node -> left);
    new_node -> right = Copy_Subtree(node -> right);
    new_node -> prev = node -> prev;
    new_node -> priority = node -> priority;

    return new_node;
}

struct Node* Der(struct Node* node)
{
    assert(node);

    FILE* file_tex = fopen("Expression.tex", "a");

    switch (node -> type)
    {
    case NUM:
        return CONST(0);
    case VAR:
        if (strcmp((node -> val).var, "e") == 0) return E;
        else return CONST(1);
    default:
/*
    Print_to_TEX(node, file_tex);        \
            fprintf(file_tex,"\n");             \
            fclose(file_tex);                    \
*/
    //--------------------------------------------------------
    #define OP(name, str_symbol, enum, dif, ...) \
        case name:                               \
            return dif;                          \
            break;
    //--------------------------------------------------------

    switch((node -> val).op)
    {
    #include "operators.h"
    }

    #undef OP
    return node;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                              Reduce_Tree_functions
//------------------------------------------------------------------------------------------------------------------------------

struct Node* Copy_Node(struct Node* sourse_node, struct Node* dest_node)
{
    switch(sourse_node -> type)
    {
    case OP:
        (dest_node -> val).op = (sourse_node -> val).op;
        break;
    case NUM:
        (dest_node -> val).num = (sourse_node -> val).num;
        break;
    case VAR:
        (dest_node -> val).var = (sourse_node -> val).var;
        break;
    default:
        printf(red(ERROR) "no type node in C\"" green(Copy_Node) "\"");
        exit(1);
    }

    dest_node -> left = sourse_node -> left;
    dest_node -> right = sourse_node -> right;
    dest_node -> type = sourse_node -> type;
    dest_node -> priority = sourse_node -> priority;
    //(sourse_node -> left) -> prev = dest_node;
    //(sourse_node -> right) -> prev = dest_node;

    return sourse_node;
}

double Calculate_val(struct Node* node)
{
    assert(node);
    assert(R);

    double left = 0;

    if(L)
    {
        left = ((node -> left) -> val).num;
    }

    double right = ((node -> right) -> val).num;

    //---------------------------------------------------------------
    #define OP(name, str_op, num_op, def, len, priority, calculate, ...) \
    case name:                                                           \
        return calculate;
    //---------------------------------------------------------------

    switch((node -> val).op)
    {

    #include "operators.h"

    default:
        fprintf(stderr, "\n" red(ERROR) " no operation!!!");
        exit(1);
    }
    #undef OP
}

/*
int Reduce_MUL_Left(struct Node* node)
{
    assert(node);
    assert(L);
    assert((L) -> right);
    assert((L) -> left);

    struct Node* tmp_node = L;

    if((((L) -> right) -> val).num == 1)
    {
        L = (L) -> left;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 1)
    {
        L = (L) -> right;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 0 || (((L) -> right) -> val).num == 0)
    {
        L = CONST(0);
        Delete_Node(&tmp_node);
        return 1;
    }

    return 0;
}

int Reduce_ADD_Left(struct Node* node)
{
    assert(node);
    assert(L);
    assert((L) -> right);
    assert((L) -> left);

    struct Node* tmp_node = L;

    if((((L) -> left) -> val).num == 0)
    {
        L = (L) -> right;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 0)
    {
        L = (L) -> left;
        Delete_Node(&tmp_node);
        return 1;
    }

    return 0;
}

int Reduce_POW_Left(struct Node* node)
{
    assert(node);
    assert(L);
    assert((L) -> right);
    assert((L) -> left);

    struct Node* tmp_node = L;

    if((((L) -> left) -> val).num == 1)
    {
        L = CONST(1);
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 1)
    {
        L = (L) -> left;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> left) -> val).num == 1)
    {
        L = CONST(0);
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((L) -> right) -> val).num == 1)
    {
        L = CONST(1);
        Delete_Node(&tmp_node);
        return 1;
    }

    return 0;
}

int Join_Const_Left(struct Node* node)
{
    assert(node);
    assert(L);
    assert((L) -> right);

    if((L) -> left)
    {
        if(((L) -> left) -> type == NUM)
        {
            printf(green(operation)" = %d\n", ((L) -> val).op);
            double new_val = Calculate_val(L);
            L = CONST(new_val);
            return 1;
        }

        return 0;
    }
    else
    {
        printf(green(operation)" = %d\n", ((L) -> val).op);
        double new_val = Calculate_val(L);
        L = CONST(new_val);
        return 1;
    }
}


int Reduce_MUL_Right(struct Node* node)
{
    assert(node);
    assert(L);
    assert(R);
    assert((R) -> right);
    assert((R) -> left);

    struct Node* tmp_node = R;

    if((((R) -> right) -> val).num == 1)
    {
        R = (R) -> left;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> left) -> val).num == 1)
    {
        R = (R) -> right;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> left) -> val).num == 0 || (((R) -> right) -> val).num == 0)
    {
        R = CONST(0);
        Delete_Node(&(tmp_node));
        return 1;
    }

    return 0;
}

int Reduce_ADD_Right(struct Node* node)
{
    struct Node* tmp_node = R;

    if((((R) -> left) -> val).num == 0)
    {
        R = (R) -> right;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 0)
    {
        R = (R) -> left;
        Delete_Node(&tmp_node);
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
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 1)
    {
        R = (R) -> left;
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> left) -> val).num == 1)
    {
        R = CONST(0);
        Delete_Node(&tmp_node);
        return 1;
    }
    if((((R) -> right) -> val).num == 1)
    {
        R = CONST(1);
        Delete_Node(&tmp_node);
        return 1;
    }

    return 0;
}

int Join_Const_Right(struct Node* node)
{
    if((R) -> left)
    {
        if(((R) -> left) -> type == NUM)
        {
            printf(red(operation)" = %d\n", ((R) -> val).op);
            double new_val = Calculate_val(R);
            R = CONST(new_val);
            return 1;
        }

        return 0;
    }
    else
    {
        printf(green(operation)" = %d\n", ((R) -> val).op);
        double new_val = Calculate_val(R);
        R = CONST(new_val);
        return 1;
    }
}


int Join_Left_Op(struct Node* node)
{   /*
    if(((L) -> right) -> type == NUM)
    {
        printf("\nsosi huy_L\n");
        if(Join_Const_Left(node)) return 1;
    }

    switch(((node -> left) -> val).op)
    {
    case MUL:
        printf("\nMUL_LEFT");
        if (Reduce_MUL_Left(node)) return 1;
        break;
    case ADD:
        printf("\nADD_LEFT");
        if (Reduce_ADD_Left(node)) return 1;
        break;
    case POW:
        printf("\nPOW_LEFT");
        if (Reduce_POW_Left(node)) return 1;
        break;
    default:
        break;
    }

    return 0;
}

int Join_Right_Op(struct Node* node)
{   /*
    if(((R) -> right) -> type == NUM)
    {
        printf("\nsosi huy_R\n");
        if(Join_Const_Right(node)) return 1;
    }

    switch(((node -> right) -> val).op)
    {
    case MUL:
        printf("\nMUL_RIGHT");
        if (Reduce_MUL_Right(node)) return 1;
        break;
    case ADD:
        printf("\nADD_RIGHT");
        if (Reduce_ADD_Right(node)) return 1;
        break;
    case POW:
        printf("\nPOW_RIGHT");
        if (Reduce_POW_Right(node)) return 1;
        break;
    default:
        break;
    }

    return 0;
}
*/

int Reduce_MUL(struct Node* node)
{
    assert(node);
    assert(node -> right);
    assert(node -> left);
    assert(&((node -> right) -> val));

    if(((node -> right) -> val).num == 1)
    {
        Node* tmp_node = L;
        Delete_Node(&(R));
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> left) -> val).num == 1)
    {
        Node* tmp_node = R;
        Delete_Node(&(L));
        Copy_Node(R, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> left) -> val).num == 0 || ((node -> right) -> val).num == 0)
    {

        Delete_Node(&(R));
        Copy_Node(CONST(0), node);

        return 1;
    }

    return 0;
}

int Reduce_ADD(struct Node* node)
{

    if(((node -> left) -> val).num == 0)
    {
        Node* tmp_node = R;
        Delete_Node(&(L));
        Copy_Node(R, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> right) -> val).num == 0)
    {
        Node* tmp_node = L;
        //Delete_Node(&(R));
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }

    return 0;
}

int Reduce_POW(struct Node* node)
{
    if(((node -> left) -> val).num == 1)
    {
        Node* tmp_node = L;
        Delete_Node(&(R));
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> right) -> val).num == 1)
    {
        Node* tmp_node = L;
        Delete_Node(&(R));
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> left) -> val).num == 0)
    {
        Node* tmp_node = L;
        Delete_Node(&(R));
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> right) -> val).num == 0)
    {
        Delete_Node(&(R));
        Delete_Node(&(L));
        Copy_Node(CONST(1), node);

        return 1;
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END

int Reduce_Const(struct Node* node)
{
    //assert(node);
    /*
    if(node -> left)
    {
        if((node -> left) -> type == OP)
        {
            if(Join_Left_Op(node)) return 1;
        }
    }
    if(node -> right)
    {
        if((node -> right) -> type == OP)
        {
            if(Join_Right_Op(node)) return 1;
        }
    }
    */
    if(node)
    {
        if(L && R)
        {
            if((L) -> type == NUM && (R) -> type == NUM)
            {
                printf("\n" green(Red_const) "\n");
                double new_val = Calculate_val(node);
                Copy_Node(CONST(new_val), node);
                return 1;
            }
        }

        if(node -> type == OP)
        {
            switch((node -> val).op)
            {
            case MUL:
                fprintf(stderr, "\nMUL");
                if (Reduce_MUL(node)) return 1;
                break;
            case ADD:
                printf("\nADD");
                if (Reduce_ADD(node)) return 1;
                break;
            case POW:
                printf("\nPOW");
                if (Reduce_POW(node)) return 1;
                break;
            default:
                return 0;
                break;
            }
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

    printf("\nversion = %d\n", tree -> version);
    Tree_Dump(tree);
    Draw_Graph(tree);
    //system("make draw");
    //getchar();

    if(tree -> version != 0)
    {
        tree -> version = 0;
        Reduce_Tree(tree, tree -> root);
    }
    //return;
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

