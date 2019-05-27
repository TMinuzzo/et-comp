
#ifndef HEADER_HASH
#define HEADER_HASH

#define HASH_SIZE 997
#define FUNCTION	0
#define SCALAR		1
#define VECTOR		2


typedef struct hash_node
{
	int type;
	int nature;
	char *text;
	struct hash_node *next;
} NODE;


NODE *Table[HASH_SIZE];
void hashInit(void);
void hashPrint(void);
NODE* hashInsert(int type, char *text);
NODE* hashFind(char* text);
int hashAddress(char *text);

#endif
