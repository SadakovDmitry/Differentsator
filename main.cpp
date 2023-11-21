#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    Convert_file_to_tree_with_pointers(tree);

    Tree_Dump(tree);

    Draw_Graph(tree);
}
