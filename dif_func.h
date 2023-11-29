#ifndef USED_DF
#define USED_DF

#define tree_t "%s"
#define red(str) "\033[31m"#str"\033[0m"
#define green(str) "\033[32m"#str"\033[0m"

#define L               node->left
#define R               node->right
#define CONST(val)      Create_Node(NUM, {.num = val}, NULL, NULL, node)
#define D(node)         Der(node)
#define C(node)         Copy_Node(node)
#define E               Create_Node(VAR,{.var = "e"}, NULL, NULL, node)
#define ADD(left,right) Create_Node(OP, {.op = ADD},  left, right, node)
#define SUB(left,right) Create_Node(OP, {.op = SUB},  left, right, node)
#define MUL(left,right) Create_Node(OP, {.op = MUL},  left, right, node)
#define DIV(left,right) Create_Node(OP, {.op = DIV},  left, right, node)
#define TAN(left,right) Create_Node(OP, {.op = TAN},  left, right, node)
#define COT(left,right) Create_Node(OP, {.op = COT},  left, right, node)
#define SIN(right)      Create_Node(OP, {.op = SIN},  NULL, right, node)
#define COS(right)      Create_Node(OP, {.op = COS},  NULL, right, node)
#define SQRT(right)     Create_Node(OP, {.op = SQRT}, NULL, right, node)
#define POW(left,right) Create_Node(OP, {.op = POW},  left, right, node)
#define LOG(left,right) Create_Node(OP, {.op = LOG},  left, right, node)

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
    SQRT = 4,
    SIN  = 5,
    COS  = 6,
    TAN  = 7,
    COT  = 8,
    POW  = 9,
    LOG  = 10

};

//const char operations[] = "ADD SUB MUL DIV SQRT";
const char operations[] = " + - * / √ sin cos log ^ tg COT";   //POW

struct Tree
{
    struct Node* root;
    int size;
    int len_tree_buf;
    struct Variable* var_buf;
    int num_var;
    int version;
};

union Value
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

struct Node* Insert_to_Pointer(struct Tree* tree, Tree_t* value, struct Node* now_node, int indicator, enum TYPE type);
struct Node* Create_Node(enum TYPE type, Tree_t value, struct Node* left_node, struct Node* right_node, struct Node* prev_node);
double Eval(struct Tree* tree, struct Node* node);
void Input_variable(struct Tree* tree);
struct Node* Der(struct Node* node);
struct Node* Copy_Node(struct Node* node);
struct Node* Create_OP_Node(enum TYPE type, enum OPERATION val, struct Node* left_node, struct Node* right_node);
struct Node* Create_VAR_Node(enum TYPE type, char* val, struct Node* left_node, struct Node* right_node);
struct Node* Create_NUM_Node(enum TYPE type, double val, struct Node* left_node, struct Node* right_node);
void Reduce_Tree(struct Tree* tree, struct Node* node);
int Reduce_Const(struct Node* node);
double Calculate_val(struct Node* node);
int Dtor_SubTree(struct Node* node);

#endif


