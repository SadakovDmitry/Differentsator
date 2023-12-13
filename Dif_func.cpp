#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "const_massivs.h"


void Fill_Labels(struct Labels* labels)
{
    labels[0].var = "e";
    labels[0].type = VAR;
    labels[0].val = 2.7;
}

//                                                    Delete
//------------------------------------------------------------------------------------------------------------------------------
int Delete_SubTree(struct Node* node)
{
    //assert(node);
    if(!node) return 0;

    if(!L)
    {
        Delete_SubTree(L);
        L = NULL;
        free(node -> left);
    }
    if(!R)
    {
        Delete_SubTree(R);
        R = NULL;
        free(node -> right);
    }

    //free(node);
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
    default:
        fprintf(stderr, red(ERROR)" unkniwn type in\"" green(Check_Operation)"\"");
        exit(1);
    }
}

struct Node* Create_Node(enum TYPE type, Tree_t value, struct Node* left_node, struct Node* right_node, struct Node* prev_node, struct Tree* tree)
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
    if(type == NUM)
    {
        new_node -> priority = NUM_PRIORITY;
    }
     if(type == VAR)
    {
        new_node -> priority = NUM_PRIORITY;
        //printf( green(name) " : %s\n", value.name);
        (new_node -> val).var_id = Add_Variable(tree, tree -> var_buf, value.name);
    }

    return new_node;
}

struct Node* Insert_to_Pointer(struct Tree* tree, Tree_t* value, struct Node* now_node, int indicator, enum TYPE type)
{
    struct Node* new_node = Create_Node(type, *value, NULL, NULL, now_node, tree);
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

double Find_Var_Val(struct Tree* tree, int ind)
{
    //for (int i = 0; i < tree -> num_var; i++)
    //{
       // if(strcmp((tree -> var_buf)[i].var, variable) == 0)
        //{
            //return (tree -> var_buf)[i].val;
        //}
    //}
    return (tree -> var_buf)[ind].val;

    printf("\n" red(ERROR) " no such variable!!! in \"" green(Find_Var_Val) "\"\n");
    exit(1);
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
        double var_val = Find_Var_Val(tree, (node -> val).var_id);
        return var_val;
    }

    double left  = Eval(tree, node -> left );
    double right = Eval(tree, node -> right);

    //Check_Node(node);

    //--------------------------------------------------------
    #define OP(name, str_symbol, enum, diff, size, priotity, math_f, ...) \
        case name:                                                        \
            return math_f;
           // break;
    //--------------------------------------------------------

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
        (new_node -> val).var_id = (node -> val).var_id;
    }

    new_node -> type  = node -> type;
    new_node -> left  = Copy_Subtree(node -> left);
    new_node -> right = Copy_Subtree(node -> right);
    new_node -> prev  = node -> prev;
    new_node -> priority = node -> priority;

    return new_node;
}

struct Node* Der(struct Tree* tree, struct Node* node, struct Remove* rems, FILE* file_tex)
{
    assert(node);

    struct Node* last_node = node;

    switch (node -> type)
    {
    case NUM:
        node =  CONST(0);
        break;
    case VAR:
        if ((node -> val).var_id == 0)
        {
            node = E;
        }
        else
        {
            node = CONST(1);
        }
        break;
    default:

    //--------------------------------------------------------
    #define OP(name, str_symbol, enum, diff, ...) \
        case name:                                \
            node = diff;                          \
            break;
    //--------------------------------------------------------

    switch((node -> val).op)
    {
        #include "operators.h"
        default:
            fprintf(stderr, red(ERROR)" no %d operation in \"" green(Der)"\"", (node -> val).op);
            exit(1);
    }

    #ifndef TEYLOR
        Print_One_Diff(tree, node, last_node, rems, file_tex);
    #endif

    #undef OP
    }
    return node;
}

void   Print_One_Diff(struct Tree* tree, struct Node* node, struct Node* last_node, struct Remove* rems, FILE* file_tex)
{
    fprintf(file_tex, "%s", PHRASE_TEX[rand() % SIZE_PHRASE_BANK]);
    fprintf(file_tex, "$$( ");
    Print_Tex(tree, last_node, file_tex, rems);
    fprintf(file_tex, ")' =  ");
    Print_Tex(tree, node, file_tex, rems);
    fprintf(file_tex, " $$\\newline\n");
}



int Factorial(int n)
{
    int ans = 1;
    for(int i = 1; i <= n; i++)
    {
        ans = ans * i;
    }

    return ans;
}

void Teylor(struct Tree* tree, FILE* file_tex, struct Remove* rems)
{
    struct Tree* Teylor_tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    struct Node* node = CONST(0);

    Teylor_tree -> root = node;
    Teylor_tree -> var_buf = tree -> var_buf;
    Teylor_tree -> num_var = tree -> num_var;
    Teylor_tree -> version = 0;


    int n = 0;
    printf("\nPrint n: ");
    scanf("%d", &n);

    fprintf(file_tex, "$$ f(x) = ");
    Print_Tex(tree, tree -> root, file_tex, rems);
    fprintf(file_tex, " $$\\newline\n");

    fprintf(file_tex, "$$ f(x) = ");

    Teylor_tree -> root = ADD( Teylor_tree -> root, MUL(DIV(CONST(Eval(tree, tree -> root)), CONST(1)), POW(VAR("x"),CONST(0))));

    for(int i = 1; i <= n; i++)
    {
        tree -> root = Der(tree, tree -> root, rems, file_tex);
        Teylor_tree -> root = ADD( Teylor_tree -> root, MUL(DIV(CONST(Eval(tree, tree -> root)), CONST((double) Factorial(i))), POW(VAR("x"), CONST( (double) i))));
    }

    Reduce_Tree(Teylor_tree, Teylor_tree -> root);
    Print_Tex(Teylor_tree, Teylor_tree -> root, file_tex, rems);

    fprintf(file_tex, " $$\\newline\n");

}



void   Dif_n(struct Tree* tree, FILE* file_tex, struct Remove* rems)
{
    int n = 0;
    printf("Print n: ");
    scanf("%d", &n);

    fprintf(file_tex, "$$ f(x) = ");
    Print_Tex(tree, tree -> root, file_tex, rems);
    fprintf(file_tex, " $$\\newline\n");

    for(int i = 1; i <= n; i++)
    {
        tree -> root = Der(tree, tree -> root, rems, file_tex);
        Reduce_Tree(tree, tree -> root);
    }

    Calculate_Size(tree -> root);
    Join_Long_Tree(tree, tree -> root, rems);

    fprintf(file_tex, "$$ f(x)^{(%d)} = ", n);
    Print_Tex(tree, tree -> root, file_tex, rems);
    fprintf(file_tex, " $$\\newline\n");
    Print_Replaces(tree, rems, file_tex);
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
        (dest_node -> val).var_id = (sourse_node -> val).var_id;
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

    return dest_node;
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

int Is_Zero(struct Node* node)
{
    if(node -> type == NUM && (node -> val).num == 0) return 1;

    return 0;
}

int Is_One(struct Node* node)
{
    if(node -> type == NUM && (node -> val).num == 1) return 1;

    return 0;
}


int Reduce_MUL(struct Tree* tree, struct Node* node)
{
    assert(node);
    assert(node -> right);
    assert(node -> left);
    assert(&((node -> right) -> val));

    if(Is_One(R))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_One(L))
    {
        Node* tmp_node = R;
        Delete_SubTree(L);
        Copy_Node(R, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(L) || Is_Zero(R))
    {
        Delete_SubTree(R);
        Copy_Node(CONST(0), node);

        return 1;
    }

    return 0;
}

int Reduce_ADD(struct Tree* tree, struct Node* node)
{

    if(Is_Zero(L))
    {
        Node* tmp_node = R;
        //Delete_SubTree(L);
        Copy_Node(R, node);
        //Delete_Node(&(tmp_node));

        return 1;
    }
    if(Is_Zero(R))
    {
        Node* tmp_node = L;
        //Delete_SubTree(R);
        Copy_Node(L, node);
        //Delete_Node(&(tmp_node));

        return 1;
    }

    return 0;
}

int Reduce_POW(struct Tree* tree, struct Node* node)
{
    if(Is_One(L))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_One(R))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(L))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(R))
    {
        Delete_SubTree(R);
        Delete_SubTree(L);
        Copy_Node(CONST(1), node);

        return 1;
    }

    return 0;
}


int Reduce_Node(struct Tree* tree, struct Node* node)
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
                if (Reduce_MUL(tree, node)) return 1;
                break;
            case ADD:
                if (Reduce_ADD(tree, node)) return 1;
                break;
            case POW:
                if (Reduce_POW(tree, node)) return 1;
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

    tree -> version += Reduce_Node(tree, node);

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





//                                                   Cut_Tree
//------------------------------------------------------------------------------------------------------------------------------
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

    return node -> size;
}

int Join_Long_Tree(struct Tree* tree, struct Node* node, struct Remove* rems)
{
    if(node == NULL) return 0;

    Join_Long_Tree(tree, node -> left, rems);
    Join_Long_Tree(tree, node -> right, rems);

    if(rems -> num_rems >= MAX_NUM_REPLASES) return 0;

    if(node -> size > MAX_LEN_EXP)
    {

        if((L) -> size > (R) -> size)
        {
            rems[rems -> num_rems].name = replaces[rems -> num_rems];
            rems[rems -> num_rems].rem  = Copy_Node(L , CONST(0));
            Copy_Node(VAR(replaces[rems -> num_rems]), L);
            rems -> num_rems += 1;

            Calculate_Size(tree -> root);
        }
        else
        {
            rems[rems -> num_rems].name = replaces[rems -> num_rems];
            rems[rems -> num_rems].rem  = Copy_Node(R , CONST(0));
            Copy_Node(VAR(replaces[rems -> num_rems]), R);
            rems -> num_rems += 1;

            Calculate_Size(tree -> root);
        }

        return 1;
    }

    return 0;
}

void Print_Replaces(struct Tree* tree, struct Remove* rems, FILE* file_tex)
{
    for(int i = 0; i < rems -> num_rems; i++)
    {
        fprintf(file_tex, "$$ %s = ", rems[i].name);
        Print_Tex(tree, rems[i].rem, file_tex, rems);
        fprintf(file_tex, " $$\\newline\n");
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END
