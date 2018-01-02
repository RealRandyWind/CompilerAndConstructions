#ifndef FNODE_H
#define FNODE_H

#include "types.h"

typedef enum ENode
{
	NUnkonwn = 0,
	NNumber,
	NOpperator,
	ENode_EnumSize
} ENode;

typedef union UValue
{
	char opp;
	double num;
	void *_ptr;
} UValue;

typedef struct FNode
{
	int associative;
	int priority;
	ENode type;
	UValue Value;
	struct FNode *left, *right, **_parent;
} FNode;

/* default functions */
FNode* newNode(int priority, int associative, FNode **ptrinto);
FNode* insertNode(FNode *node, FNode **ptrinto);
void freeNode(FNode *node);
void printNode(FNode *node);

/* specialized functions */
FNode* newOperatorNode(char value, int priority, int associative, FNode **ptrinto);
FNode* newNumberNode(double value, int priority, FNode **ptrinto);

#endif