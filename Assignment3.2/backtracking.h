#pragma once

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"
#include <string>
using namespace std;
int scoreBoard(Grid<char>& board, Lexicon& lex);
void scoreBoardRec(Grid<char>& board, Lexicon& lex, string& thisWord, Set<string>& wordsFound,
                   GridLocation& location, Set<GridLocation>& vistedLocations, int& score);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
