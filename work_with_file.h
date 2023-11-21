#ifndef USED_WWF
#define USED_WWF

#include "lib.h"

void Beautiful_Dump();
void Print_Operation(enum OPERATION op, FILE* file);
void Print_Node(struct Node* node, FILE* file);
void Print_In_Order(struct Node* node);
void Tree_Dump(struct Tree* tree);
enum OPERATION Convert_op_to_enum(char* str_op);
char* Read_file(FILE* file);
struct Node* Read_and_Insert_Node(struct Tree* tree, struct Node* now_node, char* buf, int indicator, int i);
void Convert_file_to_tree_with_pointers(struct Tree* tree);
void Print_Node_depends_of_type(struct Node* node, FILE* file_dot);
void Print_Node_to_file(struct Node* node, FILE* file_dot);
void Arrows_in_Graph(struct Node* node, FILE* file_dot);
void Draw_Graph(struct Tree* tree);

#endif
