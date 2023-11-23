#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    struct Variable* var_buf = (struct Variable*) calloc(10, sizeof(struct Variable));
    tree -> var_buf = var_buf;
    tree -> num_var = 0;

    //Convert_Pre_Order_Tree(tree);
    //Convert_In_Order_Tree(tree);
    Read_tree_file(tree);

    Input_variable(tree);

    Tree_Dump(tree);

    FILE* file_tex = fopen("Expression.tex", "w");
    fprintf(file_tex, "$$ ");
    Print_to_TEX(tree -> root, file_tex);
    fprintf(file_tex, "$$");
    fclose(file_tex);


    double answer = Eval(tree, tree -> root);
    printf("\n\033[32mEnd = %lf\033[0m\n", answer);

    Draw_Graph(tree);
}
