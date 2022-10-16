// This is a cpp file that calculate the Banzaf index of given blocks.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;


// This function recursively calculates the sum of an int Vector
int sumOfVector(Vector<int>& vec, int low, int high){
    //base case
    if(vec.isEmpty()){
        return 0;
    }
    else if(low == high){
        return vec[low];
    }
    else{
        // divide and conquer
        int mid = (low + high) / 2;
        return sumOfVector(vec, low, mid) + sumOfVector(vec, mid + 1, high);
    }
}


// This function judges if a given block is a key vote of a given coalition.
bool checkKeyVoteb(int iTargetBlock, int& sofarSum, Vector<int>& blocks, int total){
    if(float(sofarSum - blocks[iTargetBlock]) <= total * 1.0 / 2.0
            && float(sofarSum) > total * 1.0 / 2.0){
        return true;
    }
    return false;
}

// This version avoids stroring the sofar Vector.
void computePowerIndexesRec(Vector<int>& blocks, int index, int iTargetBlock, int sofarSum, Vector<int>& result, int& total){
    //1 base case: a coalition is formed, calculate the total votes
    if(index == blocks.size()){
        if(checkKeyVoteb(iTargetBlock, sofarSum, blocks, total)){
            result[iTargetBlock]++;
            return;
        }
    }
    else{
        // If the coalition is going to win regardless of the target block, stop recursion.
        if(index == iTargetBlock){
            if(float(sofarSum) > total * 1.0 / 2.0)
                return;
        }
        //2.1 include
        sofarSum += blocks.get(index);
        computePowerIndexesRec(blocks, index + 1, iTargetBlock, sofarSum, result, total);
        //2.2 exclude
        sofarSum -= blocks.get(index);
        // If the coalition does not include the target block, stop recursion.
         if(index == iTargetBlock){
             return;
         }
        computePowerIndexesRec(blocks, index + 1, iTargetBlock, sofarSum, result, total);
    }
}

// This function receives a int Vector representing blocks and returns
// a Vector containing the power index of each block.
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result(blocks.size(), 0);
    int total = sumOfVector(blocks, 0, blocks.size() - 1);
    for(int iTargetBlock = 0; iTargetBlock < blocks.size(); iTargetBlock++){
        int sofarSum = 0;
        int index = 0;
        // Use iCopy to check if there is any previous blocks[i] with the same count as target block.
        // If Yes, iCopy=i, and copy result[i] to result[iTargetBlock]
        int iCopy = blocks.size();
        for(int i = 0; i < iTargetBlock; i++){
            if(blocks[iTargetBlock] == blocks[i])
            {
                iCopy = i;
                break;
            }
        }
        // Avoid repeated calculation
        if(iCopy != blocks.size()){
            result[iTargetBlock] = result[iCopy];
        }
        else{
            computePowerIndexesRec(blocks, index, iTargetBlock, sofarSum, result, total);
        }
    }
    // convert the index to percent form
    int sumResult = sumOfVector(result, 0, result.size() - 1 );
    if(sumResult == 0)
        return result;
    for(int i = 0; i < result.size(); i++){
        result[i] = result[i] * 100 / sumResult ;
    }
    return result;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


STUDENT_TEST("TIME TEST power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int n = 10 ; n <= 28; n += 2){
        blocks.clear();
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}


