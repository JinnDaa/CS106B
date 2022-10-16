#pragma once
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
int sumOfVector(Vector<int>& vec, int low, int high);
bool checkKeyVoteb(int iTargetBlock, int& sofarSum, Vector<int>& blocks, int total);
void computePowerIndexesRec(Vector<int>& blocks, int index, int iTargetBlock, int sofarSum, Vector<int>& result, int& total);
