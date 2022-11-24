#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "set.h"
#include "stack.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    EncodingTreeNode* cur = tree;
    string result;

    // for a single node tree
    if(tree->isLeaf()){
        while(!messageBits.isEmpty()){
            messageBits.dequeue();
            result += tree->getChar();
        }
        return result;
    }

    while(!messageBits.isEmpty()){
         if(messageBits.dequeue() == 1){
             cur = cur->one;
         }
         else{
             cur = cur->zero;
         }
         if(cur->isLeaf()){
             result += cur->getChar();
             cur = tree;
         }
    }

    return result;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* root = nullptr;
    // base case
    if(treeShape.dequeue() == 0){
        root = new EncodingTreeNode(treeLeaves.dequeue());
    }
    else{
        EncodingTreeNode* left = unflattenTree(treeShape, treeLeaves);
        EncodingTreeNode* right = unflattenTree(treeShape, treeLeaves);
        root = new EncodingTreeNode(left, right);
    }

    return root;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 */
string decompress(EncodedData& data) {
    string result;
    EncodingTreeNode* tree;
    tree = unflattenTree(data.treeShape, data.treeLeaves);
    result = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return result;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    void auxBuildHuffmanTree(PriorityQueue<char>& charPQ, Map<char, int>& charMap, Map<char, EncodingTreeNode*>& charPtr);
    Map<char, int> charWeightMap;
    // calculate the weight of each character
    for(int i = 0; i < text.size(); i++){
        if(!charWeightMap.containsKey(text.at(i))){
            charWeightMap.put(text.at(i), 1);
        }else{
            charWeightMap[text.at(i)]++;
        }
    }

    cout<< endl<< charWeightMap << endl;

    //store the character in a PriorityQueue
    PriorityQueue<char> charPQ;
    for(char key : charWeightMap){
        charPQ.enqueue(key, charWeightMap[key]);
    }

    // Used to store the pointer of memory-allocated  node
    Map<char, EncodingTreeNode*> charPtrMap;

    auxBuildHuffmanTree(charPQ, charWeightMap, charPtrMap);

    return charPtrMap.get(charPQ.dequeue());
}


// This is a auxiliary function that recursively builds a tree with a given PQ and WeightMap;
void auxBuildHuffmanTree(PriorityQueue<char>& charPQ, Map<char, int>& charWeightMap, Map<char, EncodingTreeNode*>& charPtrMap){
    EncodingTreeNode* root = nullptr;
    //base case
    if(charPQ.size() == 1){
        return;
    }
    else{
        // take out the chars with the smallest weight
        char lastChar1 = charPQ.dequeue();
        char lastChar2 = charPQ.dequeue();

        EncodingTreeNode* left = nullptr;
        EncodingTreeNode* right = nullptr;

        // allocate meomory for a node if not before
        if(!charPtrMap.containsKey(lastChar1)){
            left = new EncodingTreeNode(lastChar1);
            charPtrMap.put(lastChar1, left);
        }
        else{
            left = charPtrMap.get(lastChar1);
        }

        if(!charPtrMap.containsKey(lastChar2)){
            right = new EncodingTreeNode(lastChar2);
            charPtrMap.put(lastChar2, right);
        }
        else{
            right = charPtrMap.get(lastChar2);
        }

        // Use the lastChar1 to identify the combined interior node
        //  and add the two weights to that interior node.
        // Note : when using the tree, the char in interior node is meaningless.

        charWeightMap[lastChar1] = charWeightMap[lastChar1] + charWeightMap[lastChar2];
        charPQ.enqueue(lastChar1, charWeightMap[lastChar1]);

        root = new EncodingTreeNode(left, right);
        // two nodes are dequeued above and a new node is enqueued below
        charPtrMap.put(lastChar1, root);

        // Note: from now on, the weight of lastChar1
        // in charWeightMap DOES NOT represent the single weight of lastChar1 any more.
        // Also, the pointer associated with lastChar1 DOES NOT point to the
        // char(leaf node), but to its uppermost parent(interior node) instead.

        auxBuildHuffmanTree(charPQ, charWeightMap, charPtrMap);

    }
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 */

#define vector_ver

#ifdef stack_ver
// This Version uses stack to store the Sequence
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    void  traverseTree(EncodingTreeNode* tree, Map<char, Stack<Bit>>& charBitSequence, Stack<Bit>& curSequence);
    Queue<Bit> result;
    // get the map between char and sequence
    Map<char, Stack<Bit>> bitSequenceMap;
    Stack<Bit> curSequence;

    traverseTree(tree, bitSequenceMap, curSequence);


    //add the sequence to the final queue
    for(int i = 0; i < text.size(); i++){
        int sequenceSize = bitSequenceMap[text.at(i)].size();

        // Because the storing orders of a Stack and a Queue are reverse,
        // here we use an array to store the sequence temporarily and reverse it finally
        Bit temp[sequenceSize];
        Stack<Bit> copySequecne = bitSequenceMap[text.at(i)];
        for(int j = 0; j < sequenceSize; j++){
            temp[j] = copySequecne.pop();
        }
        // reverse the sequence
        for(int j = 0; j < sequenceSize; j++){
            result.enqueue(temp[sequenceSize - 1 - j]);
        }
    }
    return result;

}

// This function traverses a tree and builds a map
// that associates each character with its encoded bit sequence.
void  traverseTree(EncodingTreeNode* tree, Map<char, Stack<Bit>>& bitSequenceMap, Stack<Bit>& curSequence){
    EncodingTreeNode* cur = tree;
    //base case
    if(cur->isLeaf()){
        bitSequenceMap[cur->getChar()] = curSequence;
    }
    else{
        //choose right
        curSequence.push(1);
        traverseTree(tree->one, bitSequenceMap, curSequence);
        //unchoose
        curSequence.pop();

        //choose left
        curSequence.push(0);
        traverseTree(tree->zero, bitSequenceMap, curSequence);
        //unchoose
        curSequence.pop();

    }
}
#endif

#ifdef vector_ver
// This Version uses vector to store the Sequence
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    void  traverseTree(EncodingTreeNode* tree, Map<char, Vector<Bit>>& bitSequenceMap, Vector<Bit>& curSequence);
    Queue<Bit> result;

    // get the map between char and sequence
    Map<char, Vector<Bit>> bitSequenceMap;
    Vector<Bit> curSequence;
    traverseTree(tree, bitSequenceMap, curSequence);

    //add the sequence to the final queue
    for(int i = 0; i < text.size(); i++){
        int sequenceSize = bitSequenceMap[text.at(i)].size();

        for(int j = 0; j < sequenceSize; j++){
            result.enqueue(bitSequenceMap[text.at(i)].get(j));
        }
    }
    return result;

}

// This function traverses a tree and builds a map
// that associates each character with its encoded bit sequence.
void  traverseTree(EncodingTreeNode* tree, Map<char, Vector<Bit>>& bitSequenceMap, Vector<Bit>& curSequence){
    EncodingTreeNode* cur = tree;
    //base case
    if(cur->isLeaf()){
        bitSequenceMap[cur->getChar()] = curSequence;
    }
    else{
        //choose right
        curSequence.add(1);
        traverseTree(tree->one, bitSequenceMap, curSequence);
        //unchoose
        curSequence.remove(curSequence.size() - 1);

        //choose left
        curSequence.add(0);
        traverseTree(tree->zero, bitSequenceMap, curSequence);
        //unchoose
        curSequence.remove(curSequence.size() - 1);

    }
}


#endif
/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* cur = tree;
    //base case
    if(cur->isLeaf()){
        treeShape.enqueue(0);
        treeLeaves.enqueue(cur->getChar());
    }
    else{
        treeShape.enqueue(1);
        flattenTree(cur->zero, treeShape, treeLeaves);
        flattenTree(cur->one, treeShape, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 */
EncodedData compress(string messageText) {
    //check if the text contains at least two distinct chars.
    if(messageText.size() <= 1){
        error("The text HAS TO contain at least two distinct chars.");
    }
    char firstChar = messageText.at(0);
    for(int i = 0; i < messageText.size(); i++){
        if(messageText.at(i) != firstChar) break;
        if(i == messageText.size() - 1){
            error("The text HAS TO contain at least two distinct chars.");
        }
    }

    EncodedData result;
    // Build a tree
    EncodingTreeNode* tree = buildHuffmanTree(messageText);

    // Flatten the tree structure
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(tree, treeShape, treeLeaves);

    // Encode the given data
    Queue<Bit>  messageBits = encodeText(tree, messageText);

    result.treeShape = treeShape;
    result.treeLeaves = treeLeaves;
    result.messageBits = messageBits;

    deallocateTree(tree);
    return result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* e = new EncodingTreeNode('E');
    EncodingTreeNode* rs = new EncodingTreeNode(r, s);
    EncodingTreeNode* rse = new EncodingTreeNode(rs, e);
    EncodingTreeNode* root = new EncodingTreeNode(t, rse);

    return root;
}

void deallocateTree(EncodingTreeNode* t) {
    //base case
    if(t->isLeaf()){
        delete t;
    }
    else{
        deallocateTree(t->one);
        t->one = nullptr;
        deallocateTree(t->zero);
        t->zero = nullptr;
        deallocateTree(t);
    }
}



bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    // handle empty tree inputs
    if((a == nullptr && b != nullptr)|| (a != nullptr && b == nullptr)){
        return false;
    }
    if(a == nullptr && b == nullptr){
        return true;
    }

    //base case
    if(a->isLeaf() && b->isLeaf()){
        if(a->getChar() != b->getChar()){
            return false;
        }
    }
    else if(!a->isLeaf() && !b->isLeaf()){
        return areEqual(a->one, b->one) && areEqual(a->zero, b->zero);
    }
    //return false if the shapes are different
    else{
        return false;
    }

    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */

STUDENT_TEST("deallocateTree, check for memory leak"){
    EncodingTreeNode* tree = createExampleTree();
    deallocateTree(tree);
}

STUDENT_TEST("areEqual, compare empty tree and single leaf tree"){
    EncodingTreeNode* a = new EncodingTreeNode('A');
    EncodingTreeNode* b = nullptr;
    EXPECT(!areEqual(a, b));
    deallocateTree(a);
}

STUDENT_TEST("areEqual, compare two single leaf trees, same character"){
    EncodingTreeNode* a = new EncodingTreeNode('A');
    EncodingTreeNode* b = new EncodingTreeNode('A');

    EXPECT(areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("areEqual, compare two single leaf trees, different character"){
    EncodingTreeNode* a = new EncodingTreeNode('A');
    EncodingTreeNode* b = new EncodingTreeNode('B');

    EXPECT(!areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("areEqual, compare a single leaf with the example tree"){
    EncodingTreeNode* a = new EncodingTreeNode('A');
    EncodingTreeNode* b = createExampleTree();

    EXPECT(!areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("areEqual, compare the example tree with its copy"){
    EncodingTreeNode* a = createExampleTree();
    EncodingTreeNode* b = createExampleTree();

    EXPECT(areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("areEqual, compare the example tree with its sub tree"){
    EncodingTreeNode* a = createExampleTree();
    EncodingTreeNode* b = a->one;

    EXPECT(!areEqual(a, b));
    deallocateTree(a);
}

STUDENT_TEST("decodeText, single node tree") {
    EncodingTreeNode* tree = new EncodingTreeNode('A'); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1, 1, 1, 1, 1, 1 }; // AAAAAAA
    EXPECT_EQUAL(decodeText(tree, messageBits), "AAAAAAA");

    messageBits = { 0, 0, 0, 0, 0, 0, 0 }; // AAAAAAA
    EXPECT_EQUAL(decodeText(tree, messageBits), "AAAAAAA");

    messageBits = { 1, 0, 1, 1, 1, 0, 1 }; // AAAAAAA
    EXPECT_EQUAL(decodeText(tree, messageBits), "AAAAAAA");

    messageBits = { 1, 0, 1}; // AAA
    EXPECT_EQUAL(decodeText(tree, messageBits), "AAA");

    deallocateTree(tree);
}

#ifdef stack_ver
STUDENT_TEST("traverseTree: stack version, small example ") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Map<char, Stack<Bit>> bitSequenceMap;
    Stack<Bit> curSequence;
    traverseTree(tree, bitSequenceMap, curSequence);

    Map<char, Stack<Bit>> correctMap;
    correctMap.put('E',{1, 1});
    correctMap.put('T',{0});
    correctMap.put('R',{1, 0, 0});
    correctMap.put('S',{1, 0, 1});


    EXPECT_EQUAL(bitSequenceMap, correctMap);

    deallocateTree(tree);

}
#endif

#ifdef vector_ver
STUDENT_TEST("traverseTree: stack version, small example ") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Map<char, Vector<Bit>> bitSequenceMap;
    Vector<Bit> curSequence;
    traverseTree(tree, bitSequenceMap, curSequence);

    Map<char, Vector<Bit>> correctMap;
    correctMap.put('E',{1, 1});
    correctMap.put('T',{0});
    correctMap.put('R',{1, 0, 0});
    correctMap.put('S',{1, 0, 1});


    EXPECT_EQUAL(bitSequenceMap, correctMap);

    deallocateTree(tree);

}
#endif

STUDENT_TEST("auxBuildHuffmanTree, small example"){
    Map<char, int> charWeightMap;
    charWeightMap.put('R', 1);
    charWeightMap.put('S', 2);
    charWeightMap.put('E', 3);
    charWeightMap.put('T', 4);
    PriorityQueue<char> charPQ;
    for(char key : charWeightMap){
        charPQ.enqueue(key, charWeightMap[key]);
    }
    Map<char, EncodingTreeNode*> charPtrMap;

    auxBuildHuffmanTree(charPQ, charWeightMap, charPtrMap);


    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = charPtrMap.get(charPQ.dequeue());
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);

}

STUDENT_TEST("compress, handle invalid data input"){
    string message="AAAAAAAAA";
    EXPECT_ERROR(compress(message));
    message="A";
    EXPECT_ERROR(compress(message));
    message="";
    EXPECT_ERROR(compress(message));
    message="AB";
    EXPECT_NO_ERROR(compress(message));
    message="AAAAAAAAB";
    EXPECT_NO_ERROR(compress(message));
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}


PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
