#pragma once
#include "vector.h"
#include "listnode.h"

void runSort(ListNode*& front);
void quickSort(ListNode*& front);
void partitionList(ListNode*& front, ListNode*& subFront1, ListNode*& subFront2, ListNode*& subFront3);
void concatenateList(ListNode*& front, ListNode*& subFront1, ListNode*& subFront2, ListNode*& subFront3);
void printList(ListNode* front);
void deallocateList(ListNode* front);
void deallocateListRec(ListNode* front);
ListNode* createList(Vector<int> values);
bool areEquivalent(ListNode* front, Vector<int> v);
