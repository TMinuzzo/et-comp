#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hashInit(void)
{
	int i;
	for(i=0; i < HASH_SIZE; ++i)
	{
		Table[i] = 0;
	}
}

int hashAddress(char *text)
{
	int address = 1;
	int i;
	for (i=0; i<strlen(text); ++i)
	{
		address = (address*text[i]) % HASH_SIZE + 1;
	}
	return address -1;
}

NODE* hashInsert(int type, char *text)
{
	NODE *newNode;
	int address = hashAddress(text);
	newNode = (NODE*)calloc(1, sizeof(NODE));
	newNode->type = type;
	newNode->text = calloc(strlen(text) + 1, sizeof(char));
	strcpy(newNode->text, text);
	newNode->next = Table[address];
	Table[address] = newNode;
}

void hashPrint(void)
{
	int i;
	NODE* node;
	for (i=0; i<HASH_SIZE; ++i)
	{
		for (node=Table[i]; node; node=node->next)
			fprintf(stderr, "Table[%d] has %s\n", i, node->text);
	}
}
