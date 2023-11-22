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
void Convert_Pre_Order_Tree(struct Tree* tree);
void Convert_In_Order_Tree(struct Tree* tree);
void Print_Node_depends_of_type(struct Node* node, FILE* file_dot);
void Print_Node_to_file(struct Node* node, FILE* file_dot);
void Arrows_in_Graph(struct Node* node, FILE* file_dot);
void Draw_Graph(struct Tree* tree);
void Read_tree_file(struct Tree* tree);
void Insert_Node_from_file(struct Tree* tree, struct Node* node, struct Value value, char* buf, int* i, int L_or_R_insert);
void Skip_Value(char* buf, int* i);

#endif
