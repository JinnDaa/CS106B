// This is a .cpp program that accepts user input searching keywords
// and outputs the correspoding websites.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// This function accepts a string and remove its punctuation
// at the beginning and the end. It also transfers the string
// to lower case, makes sure there is at least one character
// in the string.
string cleanToken(string s)
{
    // Check if s is a pure non-alpha string
    int alphaChecker = 0;
    for(int i = 0; i < s.size(); i++){
        if(isalpha(s.at(i)))
            alphaChecker = 1;
    }
    if(alphaChecker == 0){
        return "";
    }

    // Remove the front and back punctuations
    while(ispunct(s.at(0))){
        s = s.substr(1);
    }
    while(ispunct(s.at(s.size() - 1))){
        s = s.substr(0, s.size() - 1);
    }

    // Convert s to lower case.
    s = toLowerCase(s);

    return s;
}

// This function accepts a website text input and
// returns the contained distinct strings in the
// form of Set.
Set<string> gatherTokens(string text)
{
    Set<string> tokens;

    // Split the text
    Vector<string> splitText;
    splitText = stringSplit(text, " ");

    // Add the cleaned string to set.
    for(string elem : splitText){
        if(cleanToken(elem) != ""){
            tokens.add(cleanToken(elem));
        }

    }

    return tokens;
}

// This function reads contents from a database file.
// builds a reverse index, and returns the number of
// websites processed.
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    // Read the database file in to a Vector of strings
    // representing the lines of the file..
    ifstream in;
    if(!openFile(in, dbfile)){
        error("Cannot open file named " + dbfile);
    }
    Vector<string> lines;
    readEntireFile(in, lines);
    // process website's contents and its url
    for(int i = 0; i < lines.size() - 1; i += 2){
        // transfer the sentences into tokens
        Set<string> stringSet = gatherTokens(lines[i+1]);
        // for every tokens, if there already exists a key in the Map,
        // then the url would be added the the value associated with the key;
        // otherwise create a new key and store its url.
        for(string elem : stringSet){
            if(!index.containsKey(elem)){
                Set<string> urlSet;
                urlSet.add(lines[i]);
                index.put(elem, urlSet);
            }
            else{
                index[elem].add(lines[i]);
            }
        }
    }
    return lines.size()/2;
}

// This function accepts a index and a query, returns a Set of urls contaning the
// keywords of query.
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // split the query keywords
    Vector<string> splitQuery = stringSplit(query, " ");
    // check if the index contains any of the keywords
    int indexContainChecker = 0;
    for(string elem : splitQuery){
        if(index.containsKey(elem)){
            indexContainChecker = 1;
            break;
        }
    }
    if(indexContainChecker == 0){
        cout << "No matching websites found in database!";
        return {};
    }
    // for a single keyword input, return all the urls containing the keyword
    if(splitQuery.size() == 1){
        return index.get(splitQuery[0]);
    }

    // for compound keywords
    for(string elem : splitQuery){
        if(elem.at(0) == '+'){
            elem = elem.substr(1);
            elem = cleanToken(elem);
            result.intersect(index.get(elem));
        }
        else if(elem.at(0) == '-'){
            elem = elem.substr(1);
            elem = cleanToken(elem);
            result.difference(index.get(elem));
        }
        else{
            elem = cleanToken(elem);
            result.unionWith(index.get(elem));
        }

    }

    return result;
}

// This function accepts a database file and construct a reverse index based on the
// content, then it  prints the number of websites used in this process and distinct keywords found.
// Then it enters a loop and accepts a user input of a query and outputs all the assocaited urls.
// Program ends when it receives an empty input.
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." << "\n";
    // build the reverse index
    int nPages = buildIndex(dbfile, index);
    cout << "Indexed " << nPages << " pages " <<" containing "
         <<  index.size() << " unique terms" << "\n";
    string query;
    cout << "Enter query sentence (RETURN/ENTER to quit): ";
    getline(cin, query);
    while(query != ""){
        Set<string> result;
        // match the query with the index
        result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages"
             << "\n" << result << "\n\n";
        cout << "Enter query sentence (RETURN/ENTER to quit): ";
        getline(cin, query);
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


STUDENT_TEST("cleanToken on strings with more than 1 punctuations at beginning and end") {
    EXPECT_EQUAL(cleanToken("*/Jinda"), "jinda");
    EXPECT_EQUAL(cleanToken("StickMInd!?"), "stickmind");
}

STUDENT_TEST("cleanToken on strings with  punctuations at beginning, middle and end") {
    EXPECT_EQUAL(cleanToken("*/-Ha/p/py~~~"), "ha/p/py");
    EXPECT_EQUAL(cleanToken("--E!M!O!!!"), "e!m!o");
}

STUDENT_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> expected = {"apple", "banana", "cat", "peach", "pear"};
    Set<string> tokens = gatherTokens("Apple BanaNa CAT !Cat!!! APPLE .... ///PEACH// PeaR");
    EXPECT_EQUAL(tokens, expected);
}

STUDENT_TEST("buildIndex from tiny.txt, the value of key 'fish' is a Set containing 3 urls") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expected = {"www.shoppinglist.com", "www.dr.seuss.net", "www.bigbadwolf.com"};

    EXPECT_EQUAL(index.get("fish"), expected);
    EXPECT(index.containsKey("red"));
    EXPECT(index.containsKey("one"));
    EXPECT(index.containsKey("eat"));
}

STUDENT_TEST("Time operation of buildIndex on tinyinput and large input"){
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    TIME_OPERATION(index.size(), buildIndex("res/tiny.txt", index));
    index.clear();
    buildIndex("res/website.txt", index);
    TIME_OPERATION(index.size(), buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expected = {"www.rainbow.org", "www.dr.seuss.net", "www.bigbadwolf.com"};
    Set<string> matchesFIshOrGreenWithoutMilk = findQueryMatches(index, "fish green -milk");
}

