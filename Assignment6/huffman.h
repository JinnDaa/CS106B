#pragma once

#include "bits.h"
#include "treenode.h"
#include "queue.h"
#include <string>
#include "stack.h"
#include "map.h"
#include "priorityqueue.h"

// Required prototypes
// Your function implementations must match these without changes

void deallocateTree(EncodingTreeNode* t);
EncodingTreeNode* buildHuffmanTree(std::string messageText);

std::string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits);
Queue<Bit> encodeText(EncodingTreeNode* tree, std::string messageText);

void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves);
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves);

EncodedData compress(std::string messageText);
std::string decompress(EncodedData& data);

void  traverseTree(EncodingTreeNode* tree, Map<char, Stack<Bit>>& charBitSequence, Stack<Bit>& curSequence);
void auxBuildHuffmanTree(PriorityQueue<char>& charPQ, Map<char, int>& charMap, Map<char, EncodingTreeNode*>& charPtr);
