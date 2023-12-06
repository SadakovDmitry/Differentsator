#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "lib_pars.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    struct Variable* var_buf = (struct Variable*) calloc(NUM_VAR, sizeof(struct Variable));
    struct Remove* rems = (struct Remove*) calloc(MAX_NUM_REPLASES, sizeof(struct Remove));
    rems -> num_rems = 0;
    tree -> var_buf = var_buf;
    tree -> num_var = 0;
    tree -> version = 0;

    Read_tree_file(tree);
    //Draw_Graph(tree);
    system("make draw");
    /*
    FILE* file_tex = fopen("Expression.tex", "w");
    Print_Title_TEX(file_tex);
    //fprintf(file_tex, "$$ ");
    //Print_Tex(tree -> root, file_tex);
    //fprintf(file_tex, " = ");

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
    */
    //system("make draw");

}
