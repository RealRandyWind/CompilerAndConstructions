#include "node.h"

#include <stdlib.h>
#include <stdio.h>

FNode* insertNode(FNode* node, FNode **ptrinto)
{
	FNode *into;

	if(!ptrinto || !node) { exit(EXIT_FAILURE); }
	into = (*ptrinto);
	if(!into) { (*ptrinto) = node; return node; }
	if(!node->associative)
	{
		if(!into->left)
		{
			into->left = node;
			node->_parent = &into->left;
		}
		else if(!into->right)
		{
			into->right = node;
			node->_parent = &into->right;
		}
		return into;
	}
	if(node->associative < 0)
	{
		node->left = into;
		(*into->_parent) = node;
		node->_parent = &into->left;
		return node;
	}
	if(node->associative > 0)
	{
		node->right = into;
		(*into->_parent) = node;
		node->_parent = &into->right;
		return node;
	}
	
	(*ptrinto) = node; return node;
}

FNode* newNode(int priority, int associative, FNode **ptrinto)
{
	FNode *node;

	node = (FNode*) malloc(sizeof(FNode));
	if(!node) { exit(EXIT_FAILURE); }
	node->priority = priority;
	node->associative = associative;
	node->type = NUnkonwn; node->Value._ptr = nullptr;
	node->left = nullptr; node->right = nullptr;
	node->_parent = nullptr;
	insertNode(node, ptrinto);
	return node;
}

void freeNode(FNode *node)
{	
	if(!node) { return; }
	freeNode(node->left); freeNode(node->right); free(node);
}

void printNode(FNode *node)
{
	if(!node) { return; }
	printNode(node->left); printNode(node->right);
	if(node->type == NNumber) { printf("%f", node->Value.num); }
	if(node->type == NOpperator) { printf("%c", node->Value.opp); }
}

FNode* newOperatorNode(char value, int priority, int associative, FNode **ptrinto)
{
	FNode* node;

	node = newNode(priority, associative, ptrinto);
	node->Value.opp = value;
	node->type = NOpperator;
	return node;
}

FNode* newNumberNode(double value, int priority, FNode **ptrinto)
{
	FNode* node;

	node = newNode(priority, 0, ptrinto);
	node->Value.num = value;
	node->type = NNumber;
	return node;
}