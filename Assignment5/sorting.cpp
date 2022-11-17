#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "sorting.h"
#include "random.h"
#include <list>
using namespace std;

/*
 * This function rearranges the given link into sorted order.
 */
void quickSort(ListNode*& front) {
    ListNode* subFront1 = nullptr;
    ListNode* subFront2 = nullptr;
    ListNode* subFront3 = nullptr;
    if(front == nullptr){
        return;
    }
    //base case
    if(front->next == nullptr){
        return;
    }
    else{
        partitionList(front, subFront1, subFront2, subFront3);
        quickSort(subFront1);
        quickSort(subFront3);
        concatenateList(front, subFront1, subFront2, subFront3);
    }
}


// This fucntion divides the given list into three parts based on the firt value of the list.
// At the end of this fucntion, front becomes nullptr.
void partitionList(ListNode*& front, ListNode*& subFront1, ListNode*& subFront2, ListNode*& subFront3){
    int baseValue = front->data;
    while(front != nullptr){
        ListNode* temp = nullptr;
        if(front->data < baseValue){
            temp = subFront1;
            subFront1 = front;
            front = front->next; // update front in time, before it is changed by subFront
            subFront1->next = temp;
        }
        else if(front->data == baseValue){
            temp = subFront2;
            subFront2 = front;
            front = front->next;
            subFront2->next = temp;
        }
        else if(front->data > baseValue){
            temp = subFront3;
            subFront3 = front;
            front = front->next;
            subFront3->next = temp;
        }
    }
}

// This function merges three subLists into a single one.
void concatenateList(ListNode*& front, ListNode*& subFront1, ListNode*& subFront2, ListNode*& subFront3){
    ListNode* temp = nullptr;
    if(subFront1 != nullptr){
        // front starts from sublist1
        front = subFront1;
        temp = subFront1;
        while(temp->next != nullptr){
            temp = temp->next;
        }

        // Add sublist2 if not empty
        if(subFront2 != nullptr){
            temp->next = subFront2;
            while(temp->next != nullptr){
                temp = temp->next;
            }
        }

        // Add sublist3 if not empty
        if(subFront3 != nullptr){
            temp->next = subFront3;
            while(temp->next != nullptr){
                temp = temp->next;
            }
        }
    }
    else{
        if(subFront2 != nullptr){
            // front starts from sublist1
            front = subFront2;
            temp = subFront2;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            // Add sublist3 if not empty
            if(subFront3 != nullptr){
                temp->next = subFront3;
                while(temp->next != nullptr){
                    temp = temp->next;
                }
            }
        }
        else{
            if(subFront3 != nullptr){
                //front starts from sublist1
                front = subFront3;
            }
            else{
                front = nullptr;
            }
        }
    }

}


/* * * * * * Test Code Below This Point * * * * * */
STUDENT_TEST("This tests the partitionList function, ordianry case "){
     ListNode* studentList = createList({4, 1, 9, 5, 4, 4, 8, 3, 2});
     ListNode* subFront1 = nullptr;
     ListNode* subFront2 = nullptr;
     ListNode* subFront3 = nullptr;
     partitionList(studentList, subFront1,subFront2, subFront3);

     EXPECT(areEquivalent(subFront1, {2, 3, 1}));
     EXPECT(areEquivalent(subFront2, {4, 4, 4}));
     EXPECT(areEquivalent(subFront3, {8, 5, 9}));
     if(subFront1 != nullptr){
         deallocateList(subFront1);
     }
     if(subFront2 != nullptr){
         deallocateList(subFront2);
     }
     if(subFront3 != nullptr){
         deallocateList(subFront3);
     }
}

STUDENT_TEST("This tests the partitionList function, input list nodes have the same value "){
     ListNode* studentList = createList({4, 4, 4, 4, 4});
     ListNode* subFront1 = nullptr;
     ListNode* subFront2 = nullptr;
     ListNode* subFront3 = nullptr;
     partitionList(studentList, subFront1,subFront2, subFront3);
     EXPECT(areEquivalent(subFront1, {}));
     EXPECT(areEquivalent(subFront2, {4, 4, 4, 4, 4}));
     EXPECT(areEquivalent(subFront3, {}));
     if(subFront1 != nullptr){
         deallocateList(subFront1);
     }
     if(subFront2 != nullptr){
         deallocateList(subFront2);
     }
     if(subFront3 != nullptr){
         deallocateList(subFront3);
     }
}

STUDENT_TEST("This tests the concatenateList function, ordianry case "){
    ListNode* subFront1 = createList({2, 3, 1});
    ListNode* subFront2 = createList({4, 4, 4});
    ListNode* subFront3 = createList({8, 5, 9});

    ListNode* front = nullptr;
    concatenateList(front, subFront1, subFront2, subFront3);
    //printList(front);
    EXPECT(areEquivalent(front, {2, 3, 1, 4, 4, 4, 8, 5, 9}));
    deallocateList(front);

}

STUDENT_TEST("This tests the concatenateList function, contains empty sublists "){
    ListNode* subFront1 = createList({});
    ListNode* subFront2 = createList({4, 4, 4});
    ListNode* subFront3 = createList({});

    ListNode* front = nullptr;
    concatenateList(front, subFront1, subFront2, subFront3);
    //printList(front);
    EXPECT(areEquivalent(front, {4, 4, 4}));
    deallocateList(front);

}

STUDENT_TEST("End-to-end test, empty list."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, single element list."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, list sorted in ascending sequence."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1,5,7,9,10,78};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, list sorted in descending sequence."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {78,65,54,31,20,8,4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}
STUDENT_TEST("End-to-end test, list containing repeated elements."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {4,4,4,4,8,6,9,6,10,76,21};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, list with elements all the same."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {4,4,4,4,4,4,4,4,4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, list with neagtive values."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {4,-4,4,4,-8,6,9,6,-10,76,-21};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, list with random values."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for(int i = 0; i < 20; i++){
        values.add(randomInteger(0, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, large size list"){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for(int i = 0; i < 100000; i++){
        values.add(randomInteger(0, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);
    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("End-to-end test, time operation"){
    int startSize = 100000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }
        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        deallocateList(list);
    }
}


/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    while(front != nullptr){
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }
}

void deallocateListRec(ListNode* front){
    //base case
    if(front == nullptr){
        return;
    }
    else{
        deallocateListRec(front->next);
        delete front;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    // create list
    ListNode* front = nullptr;

    for(int i = values.size() - 1; i >= 0; i--){
        front= new ListNode(values[i], front);
    }

    return front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    for(int i = 0; i < v.size(); i++){
        //  if link size is smaller than the vector size, return false
        if(front == nullptr){
            return false;
        }

        // if vector size is smaller than the link size, return false
        if(i == v.size() - 1 && front->next != nullptr){
            return false;
        }

        if(v[i] != front->data){
            return false;
        }
        front = front->next;
    }
    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test deallocateListRec"){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});

    /* Test deallocate list functions(Recursive Version). There should be no memory leaks from this test. */
    deallocateListRec(studentList);
    deallocateListRec(testList);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

