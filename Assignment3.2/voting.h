#pragma once
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
bool checkKeyVoteb(int iTargetBlock, int& sofarSum, Vector<int>& blocks, int total);
bool checkKeyVoteb(int iTargetBlock, int& sofarSum, Vector<int>& blocks, int total, int inludeChecker);
void computePowerIndexesRec(Vector<int>& blocks, int index, int iTargetBlock, int sofarSum, int judge, Vector<int>& result, int& total);
void computePowerIndexesRec(Vector<int>& blocks, int index, int iTargetBlock, int sofarSum, int inludeChecker, Vector<int>& result, int& total);
