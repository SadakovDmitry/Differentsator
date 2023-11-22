#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    //Convert_Pre_Order_Tree(tree);
    //Convert_In_Order_Tree(tree);
    Read_tree_file(tree);

    Tree_Dump(tree);

    double answer = Eval(tree -> root);
    printf("\n\033[32mEnd = %lf\033[0m\n", answer);

    Draw_Graph(tree);
}
