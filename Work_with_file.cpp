#include "dif_func.h"
#include "lib.h"
#include "work_with_file.h"

enum OPERATION Convert_op_to_enum(struct Node* node, char* str_op)
{
    //--------------------------------------------------------
    #define OP(name, str, num_op, def, len, op_prior, ...)   \
        if(strcasecmp(str, str_op) == 0)                \
        {                                               \
            node -> priority = op_prior;                \
            return name;                                \
        }
    //--------------------------------------------------------
    #include "operators.h"
    else
    {
        printf("\n" red(ERROR)" uncorrect operation!!!\n");
        exit(1);
    }
/*
    if(strcasecmp("+", str_op) == 0)
    {
        node -> priority = 2;
        return ADD;
    }
    if(strcasecmp("*", str_op) == 0)
    {
        node -> priority = 1;
        return MUL;
    }
    if(strcasecmp("-", str_op) == 0)
    {
        node -> priority = 2;
        return SUB;
    }
    if(strcasecmp("/", str_op) == 0)
    {
        node -> priority = 1;
        return DIV;
    }
    if(strcasecmp("√", str_op) == 0)
    {
        node -> priority = -2;
        return SQRT;
    }
    if(strcasecmp("sin", str_op) == 0)
    {
        node -> priority = 0;
        return SIN;
    }
    if(strcasecmp("cos", str_op) == 0)
    {
        node -> priority = 0;
        return COS;
    }
    if(strcasecmp("^", str_op) == 0)
    {
        node -> priority = -2;
        return POW;
    }
    if(strcasecmp("tg", str_op) == 0)
    {
        node -> priority = 0;
        return TAN;
    }
    if(strcasecmp("COT", str_op) == 0)
    {
        node -> priority = 0;
        return COT;
    }
    if(strcasecmp("log", str_op) == 0)
    {
        node -> priority = 1;
        return LOG;
    }
    else
    {
        printf("\n\033[31mERROR\033[0m uncorrect operation!!!\n");
        exit(1);
    }
*/
#undef OP
}

void Beautiful_Dump()
{
    printf( "\n\n\033[36m          /＞   フ\n"
            "         |  _  _| \n"
            "        /`ミ _x 彡 \n"
            "       /        | \n"
            "      /  ヽ     ﾉ \n"
            "  ／￣|    | | | \n"
            "  | (￣ヽ__ヽ_)_) \n"
            "  ＼二つ    \033[31mDUMP\033[0m\n\n");
}



void Print_Node_depends_of_type(struct Node* node, FILE* file_dot)
{
    if(node -> type == OP)
    {
        fprintf(file_dot, "%lld [color = \"red\", shape = record, style = \"rounded\", label = \"{", (long long int) node);
        Print_Node(node, file_dot);
        fprintf(file_dot, " | OP | pri: %d |{ <left> left | <right> right }}\"];\n\t", node -> priority);
    }
    else if(node -> type == NUM)
    {
        fprintf(file_dot, "%lld [color = \"green\", shape = record, style = \"rounded\", label = \"{%lf | NUM | pri: %d |{ <left> left | <right> right }}\"];\n\t", (long long int) node, (node -> val).num, node -> priority);
    }
    else if(node -> type == VAR)
    {
        fprintf(file_dot, "%lld [color = \"grey\", shape = record, style = \"rounded\", label = \"{%s | VAR | pri: %d |{ <left> left | <right> right }}\"];\n\t", (long long int) node, (node -> val).var, node -> priority);
    }
}

void Print_Node_to_file(struct Node* node, FILE* file_dot)
{
    if(node == NULL) return;

    Print_Node_to_file(node -> left, file_dot);

    Print_Node_depends_of_type(node, file_dot);

    Print_Node_to_file(node -> right, file_dot);
}

void Arrows_in_Graph(struct Node* node, FILE* file_dot)
{
    if(node == NULL) return;

    if (node -> left != NULL)
    {
        fprintf(file_dot, "%lld:<left> -> %lld[color = \"green\"]\n\t", (long long int) node, (long long int) node -> left);
    }

    Arrows_in_Graph(node -> left, file_dot);

    if (node -> right != NULL)
    {
        fprintf(file_dot, "%lld:<right> -> %lld[color = \"red\"]\n\t", (long long int) node, (long long int) node -> right);
    }

    Arrows_in_Graph(node -> right, file_dot);

}

void Draw_Graph(struct Tree* tree)
{
    FILE* file_dot = fopen("Tree_Graph.dot", "w");

    assert(file_dot);

    fprintf(file_dot, "digraph G\n{\n\trankdir = \"TB\";\n\n\tnode[color = \"red\", style=filled, fontsize = 14];\n\tedge[color = \"black\", fontcolor = \"blue\", fontsize = 12, weight = 0];\n\n\t");

    fprintf(file_dot, "Tree [shape = record, style = \"rounded\", label = \"root: %p | size: %d\"];\n\t", tree -> root, tree -> size);

    Print_Node_to_file(tree -> root, file_dot);

    Arrows_in_Graph(tree -> root, file_dot);

    fprintf(file_dot, "\n}\n");

    fclose(file_dot);
}




enum TYPE Check_Type(char* str_val)
{
    if(strstr(operations, str_val) != NULL)
    {
        return OP;
    }
    else
    {
        return VAR;
    }

}




void Print_Operation(enum OPERATION op, FILE* file)
{
    switch(op)
    {
    #define OP(name, str_op, ...)      \
    case name:                         \
        fprintf(file, str_op" ");      \
        break;

    #include "operators.h"
    }
    /*
    switch(op)
    {
    case ADD:
        fprintf(file, "+ ");
        break;
    case SUB:
        fprintf(file, "- ");
        break;
    case MUL:
        fprintf(file, "* ");
        break;
    case DIV:
        fprintf(file, "/ ");
        break;
    case SQRT:
        fprintf(file, "√ ");
        break;
    case COS:
        fprintf(file, "cos ");
        break;
    case SIN:
        fprintf(file, "sin ");
        break;
    case TAN:
        fprintf(file, "tg ");
        break;
    case COT:
        fprintf(file, "COT ");
        break;
    case POW:
        fprintf(file, "^ ");
        break;
    case LOG:
        fprintf(file, "log ");
        break;
    default:
        printf("\n\033[31mNO_OPERATION\033[0m in Print_in_Operation");
    }
    */
}

void Print_Node(struct Node* node, FILE* file)
{
    switch (node -> type)
    {
    case OP:
        Print_Operation((node -> val).op, file);
        break;
    case NUM:
        fprintf(file, "%lg ", (node -> val).num);
        break;
    case VAR:
        fprintf(file, "%s ", (node -> val).var);              //var
        break;
    default:
        printf("\033[31mERROR\033[0m uncorrect type in \"\033[32mPrint_Node\033[0m\"!!!");
        exit(1);
    }
}

void Print_In_Order(struct Node* node, FILE* file)
{
    if(node == NULL){
        return;
    }

    if(node -> left != NULL)
    {
        if((node -> left) -> priority < node -> priority)
        {
            fprintf(file, "( ");
        }
    }

    Print_In_Order(node -> left, file);
    Print_Node(node, file);
    Print_In_Order(node -> right, file);

    if(node -> right != NULL)
    {
        if((node -> right) -> priority < node -> priority)
        {
            fprintf(file, ") ");
        }
    }

}




int Need_figur_bracket(enum OPERATION op)
{
    switch(op)
    {
    case DIV:
        return 1;
    case POW:
        return 1;
    case SQRT:
        return 1;
    default:
        return 0;
    }
}

int Need_round_bracket(enum OPERATION op)
{
    switch(op)
    {
    case SIN:
        return 1;
    case COS:
        return 1;
    case TAN:
        return 1;
    case COT:
        return 1;
    case LOG:
        return 1;
    default:
        return 0;
    }
}

void Print_Node_to_TEX(struct Node* node, FILE* file_tex)
{
    switch (node -> type)
    {
    case OP:
        Print_Operation_to_TEX((node -> val).op, file_tex);
        break;
    case NUM:
        fprintf(file_tex, "%lg", (node -> val).num);
        break;
    case VAR:
        fprintf(file_tex, "%s", (node -> val).var);
        break;
    default:
        printf(red(ERROR)" uncorrect type in \"\033[32mPrint_Node_to_TEX\033[0m\"!!!");
        exit(1);
    }
}

void Print_Operation_to_TEX(enum OPERATION op, FILE* file_tex)
{
    switch(op)
    {
    case ADD:
        fprintf(file_tex, " + ");
        break;
    case SUB:
        fprintf(file_tex, " - ");
        break;
    case MUL:
        fprintf(file_tex, " * ");
        break;
    case DIV:
        fprintf(file_tex, "}");
        break;
    case SQRT:
        fprintf(file_tex, "\\sqrt{ ");
        break;
    case COS:
        fprintf(file_tex, "\\cos(");
        break;
    case SIN:
        fprintf(file_tex, "\\sin(");
        break;
    case TAN:
        fprintf(file_tex, "\\tg(");
        break;
    case COT:
        fprintf(file_tex, "\\ctg(");
        break;
    case POW:
        fprintf(file_tex, "^{");
        break;
    case LOG:
        fprintf(file_tex, "}");
        break;
    default:
        printf("\n" red(NO_OPERATION)" in Print_in_Operation_to_TEX");
    }
}

void Print_to_TEX(struct Node* node, FILE* file_tex)
{
    if(node == NULL){
        return;
    }

    if(node -> type == OP)
    {
        if((node -> val).op == DIV)
        {
            fprintf(file_tex, "\\frac{");
        }
        if((node -> val).op == LOG)
        {
            fprintf(file_tex, "\\log_{");
        }
    }
    if(node -> left != NULL)
    {
        if((node -> left) -> priority > node -> priority)
        {
            fprintf(file_tex, "( ");
        }
    }

    Print_to_TEX(node -> left, file_tex);
    Print_Node_to_TEX(node, file_tex);

    if(node -> type == OP)
    {
        switch((node -> val).op)
        {
        case DIV:
            fprintf(file_tex, "{");
            break;
        case LOG:
            fprintf(file_tex, "(");
        default:
            break;
        }
    }

    Print_to_TEX(node -> right, file_tex);

    if(node -> type == OP)
    {
        if(Need_round_bracket((node -> val).op) == 1)
        {
            fprintf(file_tex, ") ");
        }
        if(Need_figur_bracket((node -> val).op) == 1)
        {
            fprintf(file_tex, "} ");
        }
    }
    if(node -> right != NULL)
    {
        if((node -> right) -> priority > node -> priority)
        {
            fprintf(file_tex, ") ");
        }
    }
}


/*
void Print_to_TEX(struct Node* node)
{
    FILE* file_tex = fopen("Expression.tex", "w");

    Print_In_Order(node, file_tex);

    fclose(file_tex);
}
*/


void Tree_Dump(struct Tree* tree)
{
    assert(tree);

    Beautiful_Dump();
    printf("root = %p\nsize = %d\n", tree -> root, tree -> size);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    Print_In_Order(tree -> root, stdout);
}

char* Read_file(struct Tree* tree, FILE* file)
{
    struct stat st = {};

    stat("Tree.txt", &st);
    tree -> len_tree_buf = st.st_size - 1;

    char* buf = (char*) calloc(st.st_size + 1, sizeof(char));
    fread(buf, sizeof (char), st.st_size, file);

    printf("\nbuf = %s\n", buf);


    return buf;
}

struct Node* Read_and_Insert_Node(struct Tree* tree, struct Node* now_node, char* buf, int indicator, int i)
{
    Tree_t value = {};
    char str_op[SIZE_STR] = "";
    double new_val = 0;


    if(sscanf(&buf[i], "%lf", &new_val) == 1)
    {
        value.num = new_val;
        now_node = Insert_to_Pointer(tree, &value, now_node, indicator, NUM);
    }
    else
    {
        sscanf(&buf[i], "%s", str_op);
        value.op = Convert_op_to_enum(now_node, str_op);

        now_node = Insert_to_Pointer(tree, &value, now_node, indicator, OP);
    }

    return now_node;
}




int Check_var_in_var_buf(struct Tree* tree, char* name)
{
    for (int i = 0; i < tree -> num_var; i++)
    {
        if(strcmp((tree -> var_buf)[i].var, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

char* Add_Variable(struct Tree* tree, struct Variable* var_buf, char* name)
{
    int len = strlen(name);
    char* spot_for_cpy = (char*) calloc(len + 1, sizeof(char));
    strcpy(spot_for_cpy, name);

    if(Check_var_in_var_buf(tree, name) == 0)
    {
        var_buf[tree -> num_var].var = spot_for_cpy;
        var_buf[tree -> num_var].val = 0;
        (tree -> num_var)++;
    }

    return spot_for_cpy;
}

void Set_Node_Value(struct Tree* tree, struct Node* node, char* buf, int i)
{
    char str[SIZE_STR] = "";
    double new_val = 0;

    if(sscanf(&buf[i], "%lf", &new_val) == 1)
    {
        //printf("\nnew_val = %f\n", new_val);
        node -> type = NUM;
        node -> priority = -1;
        (node -> val).num = new_val;

    }
    else
    {
        sscanf(&buf[i], "%s", str);
        //printf("\nstr_op = %s\n", str);

        if(Check_Type(str) == OP)
        {
            enum OPERATION op = Convert_op_to_enum(node, str);
            node -> type = OP;
            (node -> val).op = op;
        }
        else
        {
            node -> type = VAR;
            node -> priority = -1;
            (node -> val).var = Add_Variable(tree, tree -> var_buf, str);
        }
    }
}



void Convert_In_Order_Tree(struct Tree* tree)
{
    Tree_t value = {};
    int open_brackets = 0;
    int close_brackets = 0;
    struct Node* temp = NULL;
    int i = 0;
    int indicator = INSERT_LEFT;

    FILE* file = fopen("Tree.txt", "r");

    char* buf = Read_file(tree, file);

    struct Node* now_node = tree -> root;

    while (true)
    {
        if(buf[i] == '(')
        {
            open_brackets++;
            i++;

            temp = now_node;

            now_node = Insert_to_Pointer(tree, &value, now_node, indicator, NUM);
            now_node -> prev = temp;

            indicator = INSERT_LEFT;
        }
        else if(buf[i] == ')')
        {

            while(buf[i] == ')')
            {
                i = i + 2;
                close_brackets++;
                now_node = now_node -> prev;
            }

            if(close_brackets == open_brackets)                      //polnoe govno no i ebal etu recursiu becouse 10 hours and i do not know how i can write it pisdets 8 am on clock...
            {
                break;
            }

            i = i - 2;

            if (indicator == INSERT_LEFT)
            {
                Set_Node_Value(tree, now_node, buf, i + 1);
            }

            indicator = INSERT_RIGHT;
        }
        else if(buf[i] == '_')
        {
            if (indicator == INSERT_LEFT)
            {
                Set_Node_Value(tree, now_node, buf, i + 1);
            }

            indicator = indicator * INSERT_RIGHT;
            i++;
        }

        if(close_brackets == open_brackets)
        {
            break;
        }
        i++;

        #ifdef DUMP_ON_TREE
            Tree_Dump(tree);
        #endif
    }
    fclose(file);
}

void Convert_Pre_Order_Tree(struct Tree* tree)
{
    int open_brackets = 0;
    int close_brackets = 0;
    struct Node* temp = NULL;
    int i = 0;
    int indicator = INSERT_LEFT;

    FILE* file = fopen("Tree.txt", "r");

    char* buf = Read_file(tree, file);

    struct Node* now_node = tree -> root;

    while (true)
    {
        if(buf[i] == '(')
        {
            open_brackets++;
            temp = now_node;
            i++;

            now_node = Read_and_Insert_Node(tree, now_node, buf, indicator, i);

            indicator = INSERT_LEFT;
            now_node -> prev = temp;

        }
        else if(buf[i] == ')')
        {
            close_brackets++;
            now_node = now_node -> prev;
            indicator = INSERT_RIGHT;
        }
        else if(buf[i] == '_')
        {
            indicator = indicator * INSERT_RIGHT;
            i++;
        }

        if(close_brackets == open_brackets)
        {
            break;
        }
        i++;

        #ifdef DUMP_ON_TREE
            Tree_Dump(tree);
        #endif
    }
    fclose(file);
}




void Skip_Space(char* buf, int* i)
{
    while(buf[*i] == ' ')
    {
        *i = *i + 1;
    }
}

void Skip_Value(char* buf, int* i)
{
    while(buf[*i] != '_' && buf[*i] != '(')
    {
        *i = *i + 1;
    }
}

void Read_Next_Node(struct Tree* tree, struct Node* node, Tree_t value, char* buf, int* i)
{
    Skip_Space(buf, i);

    Insert_Node_from_file(tree, node, value, buf, i, INSERT_LEFT);

    if(*i >= tree -> len_tree_buf) return;

    Set_Node_Value(tree, node, buf, *i);
    Skip_Value(buf, i);

    Insert_Node_from_file(tree, node, value, buf, i, INSERT_RIGHT);

    return;
}

void Insert_Node_from_file(struct Tree* tree, struct Node* node, Tree_t value, char* buf, int* i, int L_or_R_insert)
{

    if(buf[*i] == '(')
    {
        *i = *i + 1;
        Skip_Space(buf, i);

        node = Insert_to_Pointer(tree, &value, node, L_or_R_insert, NUM);
        Read_Next_Node(tree, node, value, buf, i);
    }
    if(buf[*i] == ')')
    {
        *i = *i + 1;
        Skip_Space(buf, i);
    }
    if(buf[*i] == '_')
    {
        *i = *i + 1;
        Skip_Space(buf, i);
        node = NULL;
    }
}

void Read_tree_file(struct Tree* tree)
{
    Tree_t value = {};
    int i = 0;

    FILE* file = fopen("Tree.txt", "r");

    char* buf = Read_file(tree, file);

    fclose(file);

    Read_Next_Node(tree, tree -> root, value, buf, &i);
}
