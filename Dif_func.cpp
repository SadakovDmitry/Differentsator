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

int Check_Operation(struct Node* node)
{
    assert(node);

    switch(node -> type)
    {
    case OP:
        if(node -> right == NULL)
        {
            fprintf(stderr, red(ERROR)"right == NULL after OP node");
            return 0;
        }
        break;
    case NUM:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after NUM node");
            return 0;
        }
        break;
    case VAR:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after VAR node");
            return 0;
        }
        break;
    }
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

    //FILE* file_tex = fopen("Expression.tex", "a");
    //Print_Tex(node, file_tex);
    //fprintf(file_tex,">\n");
    //fclose(file_tex);

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
    dest_node -> size = sourse_node -> size;
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

int Reduce_MUL(struct Node* node)
{
    assert(node);
    assert(node -> right);
    assert(node -> left);
    assert(&((node -> right) -> val));

    if(((node -> right) -> val).num == 1)
    {
        Node* tmp_node = L;
        //Delete_Node(&(R));
        Copy_Node(L, node);
        //Delete_Node(&(tmp_node));

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

        //Delete_Node(&(R));
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
        //Delete_Node(&(L));
        Copy_Node(R, node);
        //Delete_Node(&(tmp_node));

        return 1;
    }
    if(((node -> right) -> val).num == 0)
    {
        Node* tmp_node = L;
        //Delete_Node(&(R));
        Copy_Node(L, node);
        //Delete_Node(&(tmp_node));

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


int Reduce_Node(struct Node* node)
{
    if(node)
    {
        if(L && R)
        {
            if((L) -> type == NUM && (R) -> type == NUM)
            {
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
                if (Reduce_MUL(node)) return 1;
                break;
            case ADD:
                if (Reduce_ADD(node)) return 1;
                break;
            case POW:
                if (Reduce_POW(node)) return 1;
                break;
            default:
                break;
            }
        }
    }

    return 0;
}

void Reduce_Tree(struct Tree* tree, struct Node* node)
{
    if(node == NULL) return;

    Check_Operation(node);
    Reduce_Tree(tree, node -> left);

    tree -> version += Reduce_Node(node);

    Check_Operation(node);
    Reduce_Tree(tree, node -> right);

    if(tree -> version != 0)
    {
        tree -> version = 0;
        Reduce_Tree(tree, tree -> root);
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END


void Dif_n(struct Tree* tree, FILE* file_tex, struct Remove* rems)
{
    int n = 0;
    printf("Print n: ");
    scanf("%d", &n);

    fprintf(file_tex, "$$ f(x) = ");
    Print_Tex(tree -> root, file_tex, rems);
    fprintf(file_tex, " $$\\newline\n");

    for(int i = 1; i <= n; i++)
    {
        tree -> root = Der(tree -> root);
        Reduce_Tree(tree, tree -> root);

        fprintf(file_tex, "$$ f(x)^{(%d)} = ", i);
        Print_Tex(tree -> root, file_tex, rems);
        fprintf(file_tex, " $$\\newline\n");
    }

}

int Calculate_Size(struct Node* node)
{
    //----------------------------------------------------
    #define OP(name, str_op, num_op, def, len, ...)  \
        case name:                                    \
            node -> size = len;                       \
            break;
    //----------------------------------------------------

    if(node -> type == OP)
    {
        switch((node -> val).op)
        {
            #include "operators.h"
        }
        return node -> size;
    }
    if(node -> type == VAR || node -> type == NUM)
    {
        node -> size = 1;
        return node -> size;
    }
}

int Join_Long_Tree(struct Node* node)
{
    if(node == NULL) return 0;

    //Join_Long_Tree(node -> left);
    if(node -> size > MAX_LEN_EXP)
    {
        if(MAX((node -> left) -> size, (node -> right) -> size) == (node -> left) -> size)
        {
            struct Node* A = node -> left;
        }
        else
        {
            struct Node* A = node -> right;
        }
        return 1;
    }
    //Join_Long_Tree(node -> right);

    return 0;
}
