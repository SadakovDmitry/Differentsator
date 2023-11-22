#include "dif_func.h"
#include "lib.h"
#include "work_with_file.h"

enum OPERATION Convert_op_to_enum(char* str_op)
{
    /*
    if(strcasecmp("ADD", str_op) == 0)
    {
        return ADD;
    }
    if(strcasecmp("MUL", str_op) == 0)
    {
        return MUL;
    }
    if(strcasecmp("SUB", str_op) == 0)
    {
        return SUB;
    }
    if(strcasecmp("DIV", str_op) == 0)
    {
        return DIV;
    }
    if(strcasecmp("SQRT", str_op) == 0)
    {
        return SQRT;
    }
    else
    {
        printf("\n\033[33mERROR\033[0m uncorrect operation!!!\n");
        exit(1);
    }
    */
    if(strcasecmp("+", str_op) == 0)
    {
        return ADD;
    }
    if(strcasecmp("*", str_op) == 0)
    {
        return MUL;
    }
    if(strcasecmp("-", str_op) == 0)
    {
        return SUB;
    }
    if(strcasecmp("/", str_op) == 0)
    {
        return DIV;
    }
    if(strcasecmp("√", str_op) == 0)
    {
        return SQRT;
    }
    else
    {
        //if(strcasecmp(")", str_op) != 0)
        //{
            printf("\n\033[31mERROR\033[0m uncorrect operation!!!\n");
            exit(1);
        //}
    }

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
        fprintf(file_dot, "%lld [shape = record, style = \"rounded\", label = \"{", (long long int) node);
        Print_Node(node, file_dot);
        fprintf(file_dot, " |{ <left> left | <right> right }}\"];\n\t");
    }
    else if(node -> type == NUM)
    {
        fprintf(file_dot, "%lld [shape = record, style = \"rounded\", label = \"{%lf |{ <left> left | <right> right }}\"];\n\t", (long long int) node, (node -> val).num);
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

    fprintf(file_dot, "digraph G\n{\n\trankdir = \"TB\";\n\n\tnode[color = \"red\", fontsize = 14];\n\tedge[color = \"black\", fontcolor = \"blue\", fontsize = 12, weight = 0];\n\n\t");

    fprintf(file_dot, "Tree [shape = record, style = \"rounded\", label = \"root: %p | size: %d\"];\n\t", tree -> root, tree -> size);

    Print_Node_to_file(tree -> root, file_dot);

    Arrows_in_Graph(tree -> root, file_dot);

    fprintf(file_dot, "\n}\n");

    fclose(file_dot);
}

/*

enum TYPE Check_Type(char* buf, int pos)
{
    double new_val = 0;
    char str_val[SIZE_STR] = "";

    if(sscanf(&buf[pos], "%lf", &new_val) == 1)
    {
        return NUM;
    }

    sscanf(&buf[pos], "%s", str_val);

    if(strcasecmp("ADD", str_val) == 0)
    {
        return OP;
    }
    if(strcasecmp("MUL", str_val) == 0)
    {
        return OP;
    }
    if(strcasecmp("SUB", str_val) == 0)
    {
        return OP;
    }
    if(strcasecmp("DIV", str_val) == 0)
    {
        return OP;
    }
    if(strcasecmp("SQRT", str_val) == 0)
    {
        return OP;
    }
    else
    {
        printf("\n\033[33mERROR\033[0m uncorrect operation!!!\n");
        exit(1);
    }
}
*/



void Print_Operation(enum OPERATION op, FILE* file)
{
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
        fprintf(file, "sqrt ");
        break;
    default:
        printf("\033[31mNO_OPERATION\033[0m");
    }
}

void Print_Node(struct Node* node, FILE* file)
{
    if (node -> type == OP)
    {
        Print_Operation((node -> val).op, file);
    }
    else if(node -> type == NUM)
    {
        fprintf(file, "%6lf ", (node -> val).num);
    }
}

void Print_In_Order(struct Node* node)
{
    if(node == NULL){
        //printf("_ ");
        return;
    }

    printf("( ");
    Print_In_Order(node -> left);
    Print_Node(node, stdout);
    Print_In_Order(node -> right);
    printf(") ");
}

void Tree_Dump(struct Tree* tree)
{
    assert(tree);

    Beautiful_Dump();
    printf("root = %p\nsize = %d\n", tree -> root, tree -> size);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    Print_In_Order(tree -> root);
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
    struct Value value = {};
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
        value.op = Convert_op_to_enum(str_op);

        now_node = Insert_to_Pointer(tree, &value, now_node, indicator, OP);
    }

    return now_node;
}

void Set_Node_Value(struct Node* node, char* buf, int i)
{
    char str_op[SIZE_STR] = "";
    double new_val = 0;

    if(sscanf(&buf[i], "%lf", &new_val) == 1)
    {
        //printf("\nnew_val = %f\n", new_val);
        node -> type = NUM;
        (node -> val).num = new_val;
    }
    else
    {
        sscanf(&buf[i], "%s", str_op);

        //printf("\nstr_op = %s\n", str_op);
        enum OPERATION op = Convert_op_to_enum(str_op);

        node -> type = OP;
        (node -> val).op = op;
    }
}


void Convert_In_Order_Tree(struct Tree* tree)
{
    struct Value value = {};
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
                Set_Node_Value(now_node, buf, i + 1);
            }

            indicator = INSERT_RIGHT;
        }
        else if(buf[i] == '_')
        {
            if (indicator == INSERT_LEFT)
            {
                Set_Node_Value(now_node, buf, i + 1);
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

void Read_Next_Node(struct Tree* tree, struct Node* node, struct Value value, char* buf, int* i)
{
    Skip_Space(buf, i);

    Insert_Node_from_file(tree, node, value, buf, i, INSERT_LEFT);

    if(*i >= tree -> len_tree_buf) return;

    Set_Node_Value(node, buf, *i);
    Skip_Value(buf, i);

    Insert_Node_from_file(tree, node, value, buf, i, INSERT_RIGHT);

    return;
}

void Insert_Node_from_file(struct Tree* tree, struct Node* node, struct Value value, char* buf, int* i, int L_or_R_insert)
{

    if(buf[*i] == '(')
    {
        *i = *i + 1;
        Skip_Space(buf, i);

        node = Insert_to_Pointer(tree, &value, node, L_or_R_insert, NUM);
        Read_Next_Node(tree, node, value, buf, i);
    }
    if(buf[*i] == '_' || buf[*i] == ')')
    {
        *i = *i + 1;
        Skip_Space(buf, i);
    }
}

void Read_tree_file(struct Tree* tree)
{
    struct Value value = {};
    int i = 0;

    FILE* file = fopen("Tree.txt", "r");

    char* buf = Read_file(tree, file);

    fclose(file);

    Read_Next_Node(tree, tree -> root, value, buf, &i);
}
