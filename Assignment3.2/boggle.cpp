/*
 * This is a cpp file that searches words in a given
 * alpha board and calculates the maximum socres.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This fucntion calculates the score of given word.
 */
int points(string str) {
    return str.size() > 3 ? str.size() - 3 : 0;
}

/*
 * This function generates a set of neighbor locations of a given location in the given board.
 */
Set<GridLocation> generateValidNeighbors(Grid<char>& board, GridLocation location){
    Set<GridLocation> candidates, neighbors;
    // List all the possible neighbors
    candidates.add({location.row, location.col + 1});
    candidates.add({location.row, location.col - 1});
    candidates.add({location.row + 1, location.col});
    candidates.add({location.row - 1, location.col});
    candidates.add({location.row - 1, location.col + 1});
    candidates.add({location.row - 1, location.col - 1});
    candidates.add({location.row + 1, location.col + 1});
    candidates.add({location.row + 1, location.col - 1});
    // Judge if the neighbors are within the board
    for(GridLocation elem : candidates){
        if(board.inBounds(elem)){
            neighbors.add(elem);
        }
    }
    return neighbors;
}

/*
 * This fucntion receives a alpha board and searches
 * all the words included in the given wordlist.
 */
 void scoreBoardRec(Grid<char>& board, Lexicon& lex, string& thisWord, Set<string>& wordsFound,
                    GridLocation& location, Set<GridLocation>& vistedLocations, int& score) { 
     //1 base case: left out
     Set<GridLocation> validNeighbors = generateValidNeighbors(board, location);
     // remove the visited locations
     validNeighbors = validNeighbors.difference(vistedLocations);
     for(GridLocation elem : validNeighbors){
         //2.1 choose
         thisWord += board.get(elem);
         vistedLocations.add(elem);
         //2.2 explore
         if(lex.containsPrefix(thisWord)){
             //note: ADT Lexion ignores the case of words
             if(!wordsFound.contains(toLowerCase(thisWord)) && lex.contains(thisWord)){
                 wordsFound.add(toLowerCase(thisWord));
                 score += points(thisWord);
             }
             scoreBoardRec(board, lex, thisWord, wordsFound, elem, vistedLocations, score);
         }
         //2.3 unchoose
         thisWord = thisWord.substr(0, thisWord.size() - 1);
         vistedLocations.remove(elem);
     }
 }


int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int score = 0;
    Set<string> wordsFound = {};
    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            GridLocation location(i, j);
            Set<GridLocation> vistedLocations;
            vistedLocations.add(location);
            string prefix = charToString(board.get(location));
            scoreBoardRec(board, lex, prefix, wordsFound, location, vistedLocations, score);
        }
    }
    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}


PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("generateValidNeighbors on the edge of 4x4 grid") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};
    GridLocation location0 = {0, 0};
    Set<GridLocation> expected0 = {{0, 1}, {1, 0}, {1, 1}};

    GridLocation location1 = {2, 1};
    Set<GridLocation> expected1 = {{2, 2}, {2, 0}, {3,1}, {1,1},
                                  {1, 2}, {1, 0}, {3, 2}, {3, 0}};

    EXPECT_EQUAL(generateValidNeighbors(board, location0), expected0);
    EXPECT_EQUAL(generateValidNeighbors(board, location1), expected1);
}


STUDENT_TEST("Test scoreBoard, board contains A word, score of one") {
    Grid<char> board = {{'W','E','L','L'},
                        {'_','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}
