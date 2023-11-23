#ifndef USED_DF
#define USED_DF

#define tree_t "%s"

const int SIZE_STR = 100;
const int INSERT_LEFT = 1;
const int INSERT_RIGHT = -1;

enum TYPE
{
    NUM    = 0,
    OP     = 1,
    VAR    = 2
};

enum OPERATION
{
    ADD  = 0,
    SUB  = 1,
    MUL  = 2,
    DIV  = 3,
    SQRT = 4
};

//const char operations[] = "ADD SUB MUL DIV SQRT";
const char operations[] = " + - * / √ ";

struct Tree
{
    struct Node* root;
    int size;
    int len_tree_buf;
    struct Variable* var_buf;
    int num_var;
};

struct Value
{
    enum OPERATION op;
    double num;
    char* var;
};

struct Variable
{
    char* var;
    double val;
};

typedef Value Tree_t;

struct Node
{
    enum TYPE type;
    Tree_t val;
    int priority;
    struct Node* left;
    struct Node* right;
    struct Node* prev;
};

struct Node* Insert_to_Pointer(struct Tree* tree, struct Value* value, struct Node* now_node, int indicator, enum TYPE type);
struct Node* Create_Node(enum TYPE type, struct Value* value, struct Node* left_node, struct Node* right_node);
double Eval(struct Tree* tree, struct Node* node);
void Input_variable(struct Tree* tree);

#endif


