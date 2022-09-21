/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in soundex.cpp that 
 * will be called from main.cpp
 */
#pragma once
#include <string>

void soundexSearch(std::string filepath);
std::string soundex(std::string s);
std::string removeNonLetters(std::string s);
std::string soundexEncode(std::string s);
std::string soundexMerge(std::string s);
std::string soundexReplace(std::string s, std::string name);
std::string soundexRemoveZero(std::string s);
std::string soundexKeepLength(std::string s);
