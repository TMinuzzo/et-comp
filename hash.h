#define HASH_SIZE 997

typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node *next;
} NODE;


NODE *Table[HASH_SIZE];
void hashInit(void);
void hashPrint(void);
NODE *hashInsert(int type, char *text);
int hashAddress(char *text);
