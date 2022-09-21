/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * Empty inputs and pure nonletter inputs to this function
 * are incorrectly handled.
 * s[] does not check the length of the input string.
 * To fix this we use s.at() function instead.
 */
string removeNonLetters(string s) {
/*
 * buggy version
 */
//    string result = charToString(s[0]);
//    for (int i = 1; i < s.length(); i++) {
//        if (isalpha(s[i])) {
//            result += s[i];
//        }
//    }

/*
 * corrected version
 */
    string result = "";
    if(s.length() == 0){
        return result;
    }
    for(int i = 0; i<= s.length() - 1; i++){
        if (isalpha(s.at(i))){
            result += s.at(i);
        }
    }
    return result;
}


/*
 * This function encodes the name letters to digits according to the Soundex Table.
 */

string soundexEncode(string s){
    if(s.length() == 0){
        return "";
    }
    string result = toUpperCase(s);
    for(int i = 0; i<= result.length()- 1; i++)
        if(result.at(i) == 'A' || result.at(i) == 'E' || result.at(i) == 'I' ||
           result.at(i) == 'O' || result.at(i) == 'U' || result.at(i) == 'H' ||
           result.at(i) == 'W' || result.at(i) == 'Y'){
            result.at(i) = '0';
        }
        else if(result.at(i) == 'B' || result.at(i) == 'F' || result.at(i) == 'P' ||
                result.at(i) == 'V'){
            result.at(i) = '1';
        }
        else if(result.at(i) == 'C' || result.at(i) == 'G' || result.at(i) == 'J' ||
                result.at(i) == 'K' || result.at(i) == 'Q' || result.at(i) == 'S' ||
                result.at(i) == 'X' || result.at(i) == 'Z'){
            result.at(i) = '2';
        }
        else if(result.at(i) == 'D' || result.at(i) == 'T'){
            result.at(i) = '3';
        }
        else if(result.at(i) == 'L'){
            result.at(i) = '4';
        }
        else if(result.at(i) == 'M' || result.at(i) == 'N'){
            result.at(i) = '5';
        }
        else if(result.at(i) == 'R'){
            result.at(i) = '6';
        }
    return result;
}


/*
 * This function merges adjacent equal numbers yielded by encoding process.
 */
string soundexMerge(string s){
    if(s.length() == 0){
        return "";
    }
    string result = charToString(s.at(0));
    for(int i = 1; i<= s.length()- 1; i++){
       if(s.at(i) != s.at(i - 1)){
           result += s.at(i);
       }
    }
    return result;
}

/*
 * This function replaces the first digit with the first letter of surname in upper form.
 */
string soundexReplace(string s, string name){
    if(s.length() == 0){
        return "";
    }
    string result = s;
    char firstLetter = toUpperCase(name).at(0);
    result.at(0) = firstLetter;

    return result;
}

/*
 * This function removes all the '0' digits.
 */
string soundexRemoveZero(string s){
    if(s.length() == 0){
        return "";
    }
    string result = "";
    for(int i = 0; i<= s.length()- 1; i++){
        if(s.at(i) != '0'){
            result += s.at(i);
        }
    }

    return result;
}

/*
 * This function Keeps the final length to 4 by adding '0' or cuuting the extraneous digits;
 */
string soundexKeepLength(string s){
    if(s.length() == 0){
        return "";
    }
    string result = s;
    if(s.length() == 4){
        result = s;
    }
    else if(s.length() > 4){
        result = s.substr(0, 4);
    }
    else{
        int addNum = 4-s.length();
        for(int i = 1; i <= addNum; i++){
            result += integerToString(0);
        }
    }

    return result;
}

/*
 * This function accepts a name input and gets its Soundex code
 * by calling the auxiliary fucntions.
 */
string soundex(string s) {
    if(s.length() == 0){
        return "";
    }
    string result = removeNonLetters(s);
    result =soundexEncode(result);
    result =soundexMerge(result);
    result =soundexReplace(result, s);
    result =soundexRemoveZero(result);
    result =soundexKeepLength(result);
    return result;
}


/* This functions gets name inputs from the user
 * and calculate the corresponding Soundex code
 * and finally find all the names with the same code
 * in the database.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    string name, soundexCode;
    Vector<string> matchNames;
    cout << "Enter a surname (RETURN to quit): ";
    getline(cin, name);
    while(name != ""){
        soundexCode = soundex(name);
        cout << "Soundex code is " << soundexCode << endl;
        for(int i = 0; i <= databaseNames.size() - 1; i++){
            if(soundex(databaseNames[i]) == soundexCode){
                matchNames.add(databaseNames[i]);
            }
        }
        matchNames.sort();
        cout << matchNames << endl;

        cout << "Enter a surname (RETURN to quit): ";
        getline(cin, name);
        }
        cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("Test removeNonLetters with empty and pure nonletter inputs") {
    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "9'''''";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test Encoding fucntion"){
    string s = "Aria";
    string result = soundexEncode(s);
    EXPECT_EQUAL(result, "0600");
    s = "bolzano";
    result = soundexEncode(s);
    EXPECT_EQUAL(result, "1042050");
}

STUDENT_TEST("Test Merging fucntion"){
    string s = "562551";
    string result = soundexMerge(s);
    EXPECT_EQUAL(result, "56251");
    s = "00122366";
    result = soundexMerge(s);
    EXPECT_EQUAL(result, "01236");
}

STUDENT_TEST("Test Replacing fucntion"){
    string s = "0621", name = "abby";
    string result = soundexReplace(s, name);
    EXPECT_EQUAL(result, "A621");
    s = "5134", name = "Mary";
    result = soundexReplace(s, name);
    EXPECT_EQUAL(result, "M134");
}

STUDENT_TEST("Test Removing Zero fucntion"){
    string s = "M6010";
    string result = soundexRemoveZero(s);
    EXPECT_EQUAL(result, "M61");
    s = "Y134";
    result = soundexRemoveZero(s);
    EXPECT_EQUAL(result, "Y134");
}

STUDENT_TEST("Test Keeping length fucntion"){
    string s = "M601";
    string result = soundexKeepLength(s);
    EXPECT_EQUAL(result, "M601");
    s = "Y134675";
    result = soundexKeepLength(s);
    EXPECT_EQUAL(result, "Y134");
    s = "T1";
    result = soundexKeepLength(s);
    EXPECT_EQUAL(result, "T100");
}

STUDENT_TEST("Test 'soundex' with empty and pure nonletter inputs"){
    string s = "";
    string result = soundex(s);
    EXPECT_EQUAL(result, "");
    s = "91498'''";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}
