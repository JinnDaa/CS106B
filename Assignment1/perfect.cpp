/*
 * This program implement a exhaustive method and its improved version,
 * as well as a Euclid method to find the perfect number in some specific range.
 * Simpletest used the test the correctness and effectiveness of these functions are provided.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
//    long total = 1;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This fucntion simplify the search process.
 * It takes a loop ranging from 1 to sqrt(n).
 */
long smarterSum(long n) {
    long total = 0;
    for(long divisor = 1; divisor <= sqrt(n); divisor++){
        if(n % divisor == 0 && divisor != n / divisor && divisor != 1){
            total += (divisor + n/divisor);
        }
        else if((n % divisor == 0 && divisor == n/divisor) || divisor == 1){
            total += divisor;
        }
    }
    if(n == 1){
        return 0;
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function tests if 2 to the kth power is a Mersenne number.
 * If true, then examine if this number is prime.
 * If also true, then this very number is confirmed to be a perfect number.
 */
long findNthPerfectEuclid(long n) {
    long nthNum;
    for(long k = 1, counter = 0; counter < n; k++){
        long m = pow(2, k) - 1;
        if(smarterSum(m) == 1){
            cout << "Found perfect number: " << pow(2, k-1)*(pow(2, k)-1) << endl;
            counter++;
            if(counter == n){
                nthNum = pow(2, k-1)*(pow(2, k)-1);
            }
        }
         if (k % 15 == 0) cout << "." << flush; // progress bar
    }
    return nthNum;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// My own student tests

STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(26250, findPerfects(26250));
    TIME_OPERATION(52500, findPerfects(52500));
    TIME_OPERATION(105000, findPerfects(105000));
    TIME_OPERATION(210000, findPerfects(210000));
}

STUDENT_TEST("Confirm negative numbers are not perfect"){
    EXPECT_EQUAL(isPerfect(-1), false);
    EXPECT_EQUAL(isPerfect(-200), false);
    EXPECT_EQUAL(isPerfect(-100000), false);
}

STUDENT_TEST("Confirm smarterSum of edge cases: 24, 25 and 26"){
    EXPECT_EQUAL(divisorSum(24), smarterSum(24));
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
    EXPECT_EQUAL(divisorSum(26), smarterSum(26));
}

STUDENT_TEST("Test oddballs 0 and 1 are not perfect with isPerfectSmarter"){
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
}

STUDENT_TEST("Confirm 33550336 is perfect with isPerfectSmarter") {
    EXPECT(isPerfectSmarter(33550336));
}

STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(375000, findPerfectsSmarter(375000));
    TIME_OPERATION(750000, findPerfectsSmarter(750000));
    TIME_OPERATION(1500000, findPerfectsSmarter(1500000));
    TIME_OPERATION(3000000, findPerfectsSmarter(3000000));
}

STUDENT_TEST("Confirm the first 5 perfect numbers are successfully found by findNthPerfectEuclid") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(1)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(3)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(4)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(5)));
}

STUDENT_TEST("Time trials of findNthPerfectEuclid on finding the fifth perfect number") {
    TIME_OPERATION(5, findNthPerfectEuclid(5));
}
