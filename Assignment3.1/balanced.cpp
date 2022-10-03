/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * This function accepts a string str and returns a string
 * retaining only the parentheses in str.
 */
string operatorsFrom(string str) {
    if(str.empty()){
        // base case
        return "";
    }else{
        // take out the first character and check if it is a parenthesis
        if(str.at(0) == '(' || str.at(0) == ')'
                || str.at(0) == '[' || str.at(0) == ']'
                || str.at(0) == '{' || str.at(0) ==  '}'){
            // If yes, add str.at(0) to result
            return str.at(0) + operatorsFrom(str.substr(1));
        }
        else{
            // If no, drop str.at(0) and pass the remaining to this function
            return operatorsFrom(str.substr(1));
        }
    }

}

/*
 * This function receives a processed pure parenthesis string
 * and check if it is balanced.
 * If yes, return true, otherwise return false.
 */
bool operatorsAreMatched(string ops) {
    if(ops.empty()){
        // base case
        return true;
    }
    // if the string contains "()" or "[]" or "{}", remove it
    // and pass the remaining to this fucntion.
    else if(stringContains(ops, "()")){
        return operatorsAreMatched(ops.erase(stringIndexOf(ops, "()"), 2));
    }
    else if(stringContains(ops, "[]")){
        return operatorsAreMatched(ops.erase(stringIndexOf(ops, "[]"), 2));
    }
    else if(stringContains(ops, "{}")){
        return operatorsAreMatched(ops.erase(stringIndexOf(ops, "{}"), 2));
    }
    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom on stirng without parenthesis") {
    EXPECT_EQUAL(operatorsFrom("stickmind"), "");
}

STUDENT_TEST("operatorsFrom on stirng with parenthesis in middle, front and end") {
    EXPECT_EQUAL(operatorsFrom("{([stic[k]min])d}"), "{([[]])}");
}

STUDENT_TEST("isBalanced on balanced example ") {
    string example ="{i (am) ba}lan[ce]d; ";
    EXPECT(isBalanced(example));
}

STUDENT_TEST("isBalanced on non-balanced examples") {
    EXPECT(!isBalanced("( ( [ STICK] ) ) }"));
    EXPECT(!isBalanced("} [ ] {"));
    EXPECT(!isBalanced("[ { ] ( } )"));
}
