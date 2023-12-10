#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "lib_pars.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    struct Labels* var_buf = (struct Labels*) calloc(NUM_VAR, sizeof(struct Labels));
    struct Remove* rems = (struct Remove*) calloc(MAX_NUM_REPLASES, sizeof(struct Remove));
    var_buf[0].var = (char*) calloc(5, sizeof(char));
    strcpy(var_buf[0].var, "e");
    var_buf[0].var = "e";
    var_buf[0].type = VAR;
    var_buf[0].val = 2.7;

    rems -> num_rems = 0;
    tree -> var_buf = var_buf;
    tree -> num_var = 1;
    tree -> version = 0;

    printf("var_buf[0] = %s = %lg\n", var_buf[0].var, var_buf[0].val);
    Read_tree_file(tree);

    FILE* file_tex = fopen("Expression.tex", "w");
    Print_Title_TEX(file_tex);

    Dif_n(tree, file_tex, rems);
    Calculate_Size(tree -> root);


    Tree_Dump(tree);

    //Print_to_TEX(tree -> root, file_tex);
    //fprintf(file_tex, "\n\t\t");
    //Print_Tex(tree -> root, file_tex);
    //fprintf(file_tex, "$$\\newline\n");
    //fprintf(file_tex, "\n\\end{document}\n");
    fprintf(file_tex, "\n\\end{document}\n");
    fclose(file_tex);

    Draw_Graph(tree);

    system("make draw");

}
