#include "parse_func.h"
#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
//#include "/Users/dima/MIPT/Differintsator/work_with_file.h"


char* Read_file(FILE* file)
{
    struct stat st = {};

    stat("Expression.txt", &st);
    //tree -> len_tree_buf = st.st_size - 1;

    char* buf = (char*) calloc(st.st_size + 1, sizeof(char));
    fread(buf, sizeof (char), st.st_size, file);

    printf("\nbuf = %s\n", buf);

    return buf;
}

/*
void syn_assert(struct Parse_inf* inf)
{
    if(true)
    {
        print(inf -> pos < (inf -> str)[inf -> pos]);
    }
}
*/

struct Node* Get_N(struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;

    if(node_type == NUM || node_type == VAR)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        return node;
    }

    printf(red(somthing wrong read unknown type)"\n");
    return NULL;
}

struct Node* Get_MD(struct Parse_inf* inf)
{
    struct Node* left = Get_PL(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    while(node_type == OP && (node_val.op == DIV || node_val.op == MUL))
    {
        (inf -> pos)++;

        struct Node* right = Get_PL(inf);

        switch (node_val.op)
        {
        case DIV:
        case MUL:
            node -> left = left;
            node -> right = right;

            return node;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }

    return left;
}

struct Node* Get_PM(struct Parse_inf* inf)
{
    struct Node* left = Get_MD(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    while(node_type == OP && (node_val.op == ADD || node_val.op == SUB))
    {
        (inf -> pos)++;

        struct Node* right = Get_MD(inf);

        switch(node_val.op)
        {
        case SUB:
        case ADD:
            node -> left = left;
            node -> right = right;

            return node;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }

    return left;
}

struct Node* Get_B(struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;

    if(node_type == OP && node_val.op == L_BRACKET)
    {
        (inf -> pos)++;
        node = Get_PM(inf);

        assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == R_BRACKET);
        (inf -> pos)++;

        return node;
    }

    return Get_N(inf);
}

struct Node* Get_Start(struct Parse_inf* inf)
{
    inf -> pos = 0;

    struct Node* node = Get_PM(inf);
    //assert((inf -> str)[inf -> pos] == '\0');

    return node;
}

struct Node* Get_U(struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    if(node_type == OP)
    {
        switch(node_val.op)
        {
        case SIN:
        case COS:
        case TAN:
        case COT:
        case SQRT:
            (inf -> pos)++;

            assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == L_BRACKET);
            (inf -> pos)++;

            node -> right = Get_PM(inf);

            assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == R_BRACKET);
            (inf -> pos)++;

            return node;
        default:
            break;
        }
    }

    return Get_B(inf);;
}

struct Node* Get_PL(struct Parse_inf* inf)
{
    struct Node* left = Get_U(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    if(node_type == OP)
    {
        switch(node_val.op)
        {
        case POW:
        case LOG:
            (inf -> pos)++;

            node -> right = Get_U(inf);
            node -> left = left;

            return node;
        default:
            break;
        }
    }

    return left;
}




struct Node* Sintactic_Pars(char* buf)
{
    struct Node* node_buf = (struct Node*) calloc(100, sizeof(Node*));
    int node_buf_pos = 0;
    int buf_pos = 0;

    while(buf[buf_pos] != '\n')
    {
        buf_pos = Set_Lex_Val(&node_buf[node_buf_pos], buf, buf_pos, &node_buf_pos);
    }

    node_buf = (struct Node*) realloc(node_buf, sizeof(Node) * (node_buf_pos + 1));

    return node_buf;
}

int Det_Lex_Val(struct Node* node, char* buf, int pos_buf)
{
    assert(node);
    assert(isalnum(buf[pos_buf]));

    if(isdigit(buf[pos_buf]))
    {
        double val = 0;

        while(isdigit(buf[pos_buf]))
        {
            val = val * 10 + buf[pos_buf] - '0';
            pos_buf++;
        }

        node -> type = NUM;
        (node -> val).num = val;
        node -> left = NULL;
        node -> right = NULL;

        //printf("read number :%lg\n", val);
    }
    else
    {
        char* str = &buf[pos_buf];

        while(isalpha(buf[pos_buf]))
        {
            pos_buf++;
        }

        char* new_str = (char*) calloc((&buf[pos_buf] - str + 1), sizeof(char));
        memcpy(new_str, str, (&buf[pos_buf] - str));

        if(Check_Type(new_str) == OP)
        {
            enum OPERATION op = Convert_op_to_enum(node, new_str);
            node -> type = OP;
            (node -> val).op = op;
            //printf("read operation : %s\n", new_str);
        }
        else
        {
            node -> type = VAR;
            (node -> val).var = new_str;
            node -> right = NULL;
            node -> left = NULL;

            //printf("read variable : %s\n", new_str);
        }
    }

    return pos_buf - 1;
}

int Set_Lex_Val(struct Node* node, char* buf, int pos_buf, int* node_buf_pos)
{
    switch(buf[pos_buf])
    {
    case ' ':
    case '\n':
        //printf("read op : <%c>\n", buf[pos_buf]);
        (*node_buf_pos)--;
        break;
    case '+':
        node -> type = OP;
        (node -> val).op = ADD;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '-':
        node -> type = OP;
        (node -> val).op = SUB;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '*':
        node -> type = OP;
        (node -> val).op = MUL;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '^':
        node -> type = OP;
        (node -> val).op = POW;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '/':
        node -> type = OP;
        (node -> val).op = DIV;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '(':
        node -> type = OP;
        (node -> val).op = L_BRACKET;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case ')':
        node -> type = OP;
        (node -> val).op = R_BRACKET;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    case '\0':
        node -> type = OP;
        (node -> val).op = ZERO;
        //printf("read op : %c\n", buf[pos_buf]);
        break;
    default:
        pos_buf = Det_Lex_Val(node, buf, pos_buf);

    }

    pos_buf++;
    (*node_buf_pos)++;

    return pos_buf;
    /*
    char str[SIZE_STR] = "";
    double new_val = 0;

    if(sscanf(&buf[i], "%lf", &new_val) == 1)
    {
        node -> type = NUM;
        node -> priority = NUM_PRIORITY;
        (node -> val).num = new_val;

        return i;
    }
    else
    {
        sscanf(&buf[i], "%s", str);

        if(Check_Type(str) == OP)
        {
            enum OPERATION op = Convert_op_to_enum(node, str);
            node -> type = OP;
            (node -> val).op = op;
        }
        else
        {
            node -> type = VAR;
            node -> priority = NUM_PRIORITY;
            (node -> val).var = Add_Variable(tree, tree -> var_buf, str);
        }

        return i + strlen(str);
    }
    */
}




void Print_Lex_Str(struct Node* node)
{
    printf("\n\n");
    for(int i = 0; i < 9; i++)
    {
        if(node[i].type == OP)
        {
            printf("OP:%d\n", node[i].val.op);
        }
        if(node[i].type == VAR)
        {
            printf("VAR:%s\n", node[i].val.var);
        }
        if(node[i].type == NUM)
        {
            printf("NUM:%lf\n", node[i].val.num);
        }
    }
}

void Print_Node(struct Node* node)
{
    printf("\n");
    if(node == NULL)
    {
        printf("NULL\n");
        return;
    }
    if(node -> type == OP)
    {
        printf("OP:%d\n", (node -> val).op);
        printf("R:%p\n", node -> right);
        printf("L:%p\n", node -> left);
    }
    if(node -> type == VAR)
    {
        printf("VAR:%s\n", node -> val.var);
        printf("R:%p\n", node -> right);
        printf("L:%p\n", node -> left);
    }
    if(node -> type == NUM)
    {
        printf("NUM:%lf\n", node -> val.num);
        printf("R:%p\n", node -> right);
        printf("L:%p\n", node -> left);
    }
    printf("\n\n");
}
