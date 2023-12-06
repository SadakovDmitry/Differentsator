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
    //double val = 0;
    //int old_p = inf -> pos;
    struct Node* node = NULL;

    if((inf -> str_lex)[inf -> pos].type == NUM)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;
        printf(green(READ)"NUM\n");
        printf("Get_N return NUM 43");
        printf("\n" red(pos)"%d\n", inf -> pos);
        Print_Node(node);

        return node;
    }
    if(((inf -> str_lex)[inf -> pos]).type == VAR)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;
        printf(green(READ)"VAR\n");
        printf("Get_N return VAR 51");
        Print_Node(node);
        return node;
    }

    printf(red(somthing wrong read unknown type)"\n");
    return NULL;
    /*
    if(isdigit((inf -> str)[inf -> pos]))
    {
        while(isdigit((inf -> str)[inf -> pos]))
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

        while(isalpha((inf -> str)[inf -> pos]))
        {
            (inf -> pos)++;
            assert((inf -> pos) > old_p);
        }
        char* new_str = (char*) calloc((1 + &(inf -> str)[inf -> pos] - str), sizeof(char));
        memcpy(new_str, str, (&(inf -> str)[inf -> pos] - str));

        return VAR(new_str);
    }
    */
}

struct Node* Get_MD(struct Parse_inf* inf)
{
    struct Node* left = Get_PL(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];
    printf("Get_MD start node from Get_PL 91");
    Print_Node(node);

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;

    printf(" I waiting DIV, MUL but got (99 line) ...\n");
    Print_Node(&(inf -> str_lex)[inf -> pos]);
    while(node_type == OP && (node_val.op == DIV || node_val.op == MUL))
    {
        (inf -> pos)++;

        struct Node* right = Get_PL(inf);
        printf("Get_MD right from Get_PL 102");
        Print_Node(right);

        switch (node_val.op)
        {
        case DIV:
            //return DIV(left, right);
            printf(green(READ)"DIV\n");
            node -> left = left;
            node -> right = right;
            return node;
            break;
        case MUL:
            printf(green(READ)"MUL\n");
            node -> left = left;
            node -> right = right;
            //return MUL(left, right);
            return node;
            break;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }
    printf("Get_MD return node 122");
    Print_Node(node);
    return left;
    /*
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
    */
}

struct Node* Get_PM(struct Parse_inf* inf)
{
    struct Node* left = Get_MD(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];
    printf("Get_PM return Get_MD 154\n");
    Print_Node(node);

    //printf("%c\n", (inf -> str)[inf -> pos]);
    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;

    printf(" I waiting ADD, SUB but got (169 line) ...\n");
    Print_Node(&(inf -> str_lex)[inf -> pos]);
    while(node_type == OP && (node_val.op == ADD || node_val.op == SUB))
    //while((inf -> str)[inf -> pos] == '+' || (inf -> str)[inf -> pos] == '-')
    {
        //char op = (inf -> str)[inf -> pos];
        enum OPERATION op = node_val.op;
        (inf -> pos)++;

        struct Node* right = Get_MD(inf);
        printf("Get_PM return right from Get_MD 179");
        Print_Node(right);

        switch(op)
        //switch(node_val.op)
        {
        case SUB:
            //node =  SUB(left, right);
            printf(green(READ)" SUB\n");
            node -> left = left;
            node -> right = right;
            return node;
            break;
        case ADD:
            //node =  ADD(left, right);
            printf(green(READ)" ADD\n");
            assert(node);
            //assert(node -> left);
            //assert(node -> right);
            assert(left);
            assert(right);
            node -> left = left;
            node -> right = right;
            //node = ADD(left, right);
            printf("NODE\n");
            Print_Node(node);
            return node;
            break;
        default:
            printf(red(ERROR)"syntax error!!!");
            exit(1);
        }
    }
    printf("Get_PM return node 200");
    Print_Node(node);
    return left;
}

struct Node* Get_B(struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;

    printf(" I waiting '(' but got (211 line) ...\n");
    Print_Node(&(inf -> str_lex)[inf -> pos]);
    if(node_type == OP && node_val.op == L_BRACKET)
    //if((inf -> str)[inf -> pos] == '(')
    {
        printf(green(READ)" (\n");
        (inf -> pos)++;
        node = Get_PM(inf);
        printf(" I waiting ')' but got (219 line) ...\n");
        Print_Node(&(inf -> str_lex)[inf -> pos]);
        //assert((inf -> str)[inf -> pos] == ')');
        assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == R_BRACKET);
        (inf -> pos)++;
        printf(green(READ)" )\n");
        printf("Get_B return from Get_PM 225");
        Print_Node(node);
        return node;
    }
    else
    {
        node = Get_N(inf);
        printf("Get_B return from Get_N 226");
        Print_Node(node);
        return node;
    }
}

struct Node* Get_Start(struct Parse_inf* inf)
{
    inf -> pos = 0;

    struct Node* node = Get_PM(inf);
    //assert((inf -> str)[inf -> pos] == '\0');
    printf("Get_Start return from Get_PM 239");
    Print_Node(node);
    return node;
}

struct Node* Get_U(struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];
    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;

    printf(" I waiting SIN, COS, TAN, COT but got (249 line) ...\n");
    Print_Node(&(inf -> str_lex)[inf -> pos]);
    if(node_type == OP)
    {
        switch(node_val.op)
        {
        case SIN:
        case COS:
        case TAN:
        case COT:
        case SQRT:
            printf(green(READ)"SIN, COS\n");
            (inf -> pos)++;

            assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == L_BRACKET);
            (inf -> pos)++;
            printf(green(READ)"  <(\n");

            node -> right = Get_PM(inf);
            assert((inf -> str_lex)[inf -> pos].type == OP && (inf -> str_lex)[inf -> pos].val.op == R_BRACKET);
            (inf -> pos)++;                              //Get_PM()!!!!
            printf(green(READ)" )>\n");
            printf("Get_U return from Get_PM 269");
            Print_Node(node);
            return node;
        default:
            //printf("\nloh\n");
            //node = Get_B(inf);
            //printf("Get_U return from Get_B 275");
            //Print_Node(node);
            //return node;
            break;
        }
    }
    //else
    //{
        //printf("\nloh\n");
        node = Get_B(inf);
        printf("Get_U return from Get_B 263");
        Print_Node(node);
        return node;
    //}
    //printf("Call Get_B from Get_U!!!\n");
    //return Get_B(inf);

    /*
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
    */
}

struct Node* Get_PL(struct Parse_inf* inf)
{
    struct Node* left = Get_U(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t node_val = (inf -> str_lex)[inf -> pos].val;
    /*
    if((inf -> str)[inf -> pos] == '^')
    {
        (inf -> pos) += 1;
        struct Node* right = Get_U(inf);
        return POW(left, right);
    }
    if(strncmp(&(inf -> str)[inf -> pos], "log(", 3) == 0)
    {
        (inf -> pos) += 3;
        struct Node* right = Get_U(inf);
        assert((inf -> str)[inf -> pos] == ')');
        (inf -> pos)++;
        return LOG(left, right);
    }
    */
    printf( green(ERROR)" I waiting MUL, LOG but type :%d, and pos = %d\n", node -> type, inf -> pos );

    if(node_type == OP && node_val.op == POW)
    {
        printf(green(READ)" POW\n");
        (inf -> pos)++;
        node -> right = Get_U(inf);
        printf("Get_PL return right from Get_U 350");
        Print_Node(node -> right);

        node -> left = left;
        return node;
    }
    if(node_type == OP && node_val.op == LOG)
    {
        printf(green(READ)" LOG\n");
        (inf -> pos)++;
        node -> right = Get_U(inf);
        printf("Get_PL return right from Get_U 360");
        Print_Node(node -> right);
        node -> left = left;
        return node;
    }
    printf("Get_PL return node 363");
    Print_Node(node);
    return left;
}




struct Node* Sintactic_Pars(char* buf)
{
    struct Node* node_buf = (struct Node*) calloc(100, sizeof(Node*));
    int node_buf_pos = 0;
    int buf_pos = 0;

    while(buf[buf_pos] != '\n')
    {
        //printf("now on:%s\n", &buf[buf_pos]);
        buf_pos = Set_Lex_Val(&node_buf[node_buf_pos], buf, buf_pos, &node_buf_pos);
        //buf_pos = Set_Node_Value(tree, &node_buf[node_buf_pos], buf, buf_pos);
        //buf_pos = Skip_Num(buf, buf_pos);
        //node_buf[pos_node_buf] -> left = node_buf[pos_node_buf - 1];
        //node_buf[pos_node_buf] -> left = node_buf[pos_node_buf];
        //node_buf_pos++;
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
            enum OPERATION op = Convert_op_to_enum(node, new_str);
            node -> type = OP;
            (node -> val).op = op;
            printf("read operation : %s\n", new_str);
            //assert(buf[pos_buf] == ')');
            //pos_buf++;
        }
        else
        {
            //Copy_Node(VAR(new_str), node);
            node -> type = VAR;
            (node -> val).var = new_str;
            node -> right = NULL;
            node -> left = NULL;
            //#warning delete node
            printf("read variable : %s\n", new_str);
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
        printf("read op : <%c>\n", buf[pos_buf]);
        (*node_buf_pos)--;
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
