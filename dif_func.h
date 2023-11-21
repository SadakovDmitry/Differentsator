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

struct Tree
{
    struct Node* root;
    int size;
};

struct Value
{
    enum OPERATION op;
    double num;
};

typedef Value Tree_t;

struct Node
{
    enum TYPE type;
    Tree_t val;
    struct Node* left;
    struct Node* right;
    struct Node* prev;
};

struct Node* Insert_to_Pointer(struct Tree* tree, struct Value* value, struct Node* now_node, int indicator, enum TYPE type);
struct Node* Create_Node(enum TYPE type, struct Value* value, struct Node* left_node, struct Node* right_node);
double Eval(struct Node* node);

#endif


