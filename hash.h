
#ifndef HEADER_HASH
#define HEADER_HASH

#define HASH_SIZE 997

#define SYMBOL_LITINT 1
#define SYMBOL_LITFLOAT 2
#define SYMBOL_LITCHAR 3
#define SYMBOL_LITSTRING 4

#define SYMBOL_IDENTIFIER 101
#define SYMBOL_SCALAR 202
#define SYMBOL_ARRAY 303
#define SYMBOL_FUNCTION 404

#define DATATYPE_INT 100
#define DATATYPE_BYTE 200
#define DATATYPE_FLOAT 300
#define DATATYPE_STRING 400


typedef struct hash_node
{
	int type;
	int datatype;
	char *text;
	struct hash_node *next;
} NODE;


NODE *Table[HASH_SIZE];
void hashInit(void);
void hashPrint(void);
NODE* hashInsert(int type, char *text);
NODE* hashFind(char* text);
int hashAddress(char *text);
void hashCheckUndeclared(void);

NODE* makeTemp();
NODE* makeLabel();


#endif