#ifndef USED_PARSE_FUNC
#define USED_PARSE_FUNC


#define red(str) "\033[31m"#str"\033[0m"
#define green(str) "\033[32m"#str"\033[0m"

#include "lib.h"

struct Parse_inf
{
    char* str;
    int pos;
};


char* Read_file(FILE* file);
struct Node* Get_B(struct Parse_inf* inf);
struct Node* Get_N(struct Parse_inf* inf);
struct Node* Get_MD(struct Parse_inf* inf);
struct Node* Get_Start(struct Parse_inf* inf);
struct Node* Get_PM(struct Parse_inf* inf);
struct Node* Get_PL(struct Parse_inf* inf);
struct Node* Get_U(struct Parse_inf* inf);

struct Node* Sintactic_Pars(char* buf);
int Set_Lex_Val(struct Node* node, char* buf, int pos_buf);
int Det_Lex_Val(struct Node* node, char* buf, int pos_buf);

#endif
