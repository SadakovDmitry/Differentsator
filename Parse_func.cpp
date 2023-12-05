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
    double val = 0;
    int old_p = inf -> pos;
    struct Node* node = NULL;

    if('0' <= (inf -> str)[inf -> pos] && (inf -> str)[inf -> pos] <= '9')   //isdigit()
    {
        while('0' <= (inf -> str)[inf -> pos] && (inf -> str)[inf -> pos] <= '9')
        {
            val = val * 10 + (inf -> str)[inf -> pos] - '0';
            (inf -> pos)++;
            assert((inf -> pos) > old_p);
        }

        return CONST(val);
    }
    else
    {
        char* str = &(inf -> str)[inf -> pos];

        while('a' <= (inf -> str)[inf -> pos] && (inf -> str)[inf -> pos] <= 'z')
        {
            (inf -> pos)++;
            assert((inf -> pos) > old_p);
        }
        char* new_str = (char*) calloc((1 + &(inf -> str)[inf -> pos] - str), sizeof(char));
        memcpy(new_str, str, (&(inf -> str)[inf -> pos] - str));

        return VAR(new_str);
    }
}

struct Node* Get_MD(struct Parse_inf* inf)
{
    struct Node* left = Get_PL(inf);
    struct Node* node = left;

    while ((inf -> str)[inf -> pos] == '/' || (inf -> str)[inf -> pos] == '*')
    {
        char op = (inf -> str)[inf -> pos];
        (inf -> pos)++;

        struct Node* right = Get_PL(inf);

        switch (op)
        {
        case '/':
            return DIV(left, right);
            break;
        case '*':
            return MUL(left, right);
            break;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }
    return node;
}

struct Node* Get_PM(struct Parse_inf* inf)
{
    struct Node* left = Get_MD(inf);
    struct Node* node = left;

    //printf("%c\n", (inf -> str)[inf -> pos]);
    while((inf -> str)[inf -> pos] == '+' || (inf -> str)[inf -> pos] == '-')
    {
        char op = (inf -> str)[inf -> pos];
        (inf -> pos)++;

        struct Node* right = Get_MD(inf);

        switch(op)
        {
        case '-':
            node =  SUB(left, right);
            break;
        case '+':
            node =  ADD(left, right);
            break;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }
    return node;
}

struct Node* Get_B(struct Parse_inf* inf)
{
    struct Node* node = NULL;

    if((inf -> str)[inf -> pos] == '(')
    {
        (inf -> pos)++;
        node = Get_PM(inf);
        //printf(" I waiting ')' but got \'%c\' ...\n", (inf -> str)[inf -> pos]);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;
        return node;
    }
    else
    {
        return Get_N(inf);
    }
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
    struct Node* node = NULL;

    //fprintf(stderr, "now pos is %s\n", &(inf -> str)[inf -> pos]);
    if (strncmp(&(inf -> str)[inf -> pos], "sin(", 4) == 0)
    {
        (inf -> pos) += 4;
        struct Node* right = Get_PM(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;

        return SIN(right);
    }
    if (strncmp(&(inf -> str)[inf -> pos], "cos(", 4) == 0)
    {
        (inf -> pos) += 4;
        struct Node* right = Get_PM(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;

        return COS(right);
    }
    if (strncmp(&(inf -> str)[inf -> pos], "sqrt(", 5) == 0)
    {
        (inf -> pos) += 5;
        struct Node* right = Get_PM(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;

        return SQRT(right);
    }
    if (strncmp(&(inf -> str)[inf -> pos], "tg(", 3) == 0)
    {
        (inf -> pos) += 3;
        struct Node* right = Get_PM(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;

        return TAN(right);
    }
    if (strncmp(&(inf -> str)[inf -> pos], "ctg(", 4) == 0)
    {
        (inf -> pos) += 4;
        struct Node* right = Get_PM(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;

        return COT(right);
    }

    //printf("Call Get_B from Get_U!!!\n");
    return Get_B(inf);
}

struct Node* Get_PL(struct Parse_inf* inf)
{
    struct Node* left = Get_U(inf);
    struct Node* node = left;

    if((inf -> str)[inf -> pos] == '^')
    {
        (inf -> pos) += 1;
        struct Node* right = Get_U(inf);
        return POW(left, right);
    }
    if(strncmp(&(inf -> str)[inf -> pos], "ln(", 3) == 0)
    {
        (inf -> pos) += 3;
        struct Node* right = Get_U(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;
        return LOG(left, right);
    }

    return node;
}




struct Node* Sintactic_Pars(char* buf)
{
    struct Node* node_buf = (struct Node*) calloc(100, sizeof(Node*));
    int node_buf_pos = 0;
    int buf_pos = 0;

    while(buf[buf_pos] != '\n')
    {
        //printf("now on:%s\n", &buf[buf_pos]);
        buf_pos = Set_Lex_Val(&node_buf[node_buf_pos], buf, buf_pos);
        //buf_pos = Set_Node_Value(tree, &node_buf[node_buf_pos], buf, buf_pos);
        //buf_pos = Skip_Num(buf, buf_pos);
        //node_buf[pos_node_buf] -> left = node_buf[pos_node_buf - 1];
        //node_buf[pos_node_buf] -> left = node_buf[pos_node_buf];
        node_buf_pos++;
    }

    node_buf = (struct Node*) realloc(node_buf, sizeof(Node*) * (node_buf_pos + 1));

    return node_buf;
}
/*
int Skip_Num(char* buf, int buf_pos)
{
    while(isalnum(buf[buf_pos]) || buf[buf_pos] == '.')
    {
        buf_pos++;
    }
    return buf_pos;
}
*/
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
        printf("read number :%lg\n", val);
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
            //assert(buf[pos_buf] == '(');
            //pos_buf++;
            Copy_Node(VAR(new_str), node);
            printf("read operation : %s\n", new_str);
            //assert(buf[pos_buf] == ')');
            //pos_buf++;
            #warning delete node
        }
        else
        {
            printf("read variable : %s\n", new_str);
        }
    }

    return pos_buf - 1;
}

int Set_Lex_Val(struct Node* node, char* buf, int pos_buf)
{
    switch(buf[pos_buf])
    {
    case ' ':
    case '\n':
        printf("read op : <%c>\n", buf[pos_buf]);
        break;
    case '+':
        node -> type = OP;
        (node -> val).op = ADD;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '-':
        node -> type = OP;
        (node -> val).op = SUB;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '*':
        node -> type = OP;
        (node -> val).op = MUL;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '^':
        node -> type = OP;
        (node -> val).op = POW;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '/':
        node -> type = OP;
        (node -> val).op = DIV;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '(':
        node -> type = OP;
        (node -> val).op = L_BRACKET;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case ')':
        node -> type = OP;
        (node -> val).op = R_BRACKET;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '\0':
        node -> type = OP;
        (node -> val).op = ZERO;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    default:
        pos_buf = Det_Lex_Val(node, buf, pos_buf);

    }

    pos_buf++;

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

