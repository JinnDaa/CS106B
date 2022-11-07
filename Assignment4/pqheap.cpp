#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQHeap class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQHeap class. The array
 * memory that was allocated for the PQHeap is deleted here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * This function inserts a new DataPoint element to a sorted array
 * and enlarge the size of the array if necessary.
 */
void PQHeap::enqueue(DataPoint elem) {
    if(_numFilled == _numAllocated){
        arrayExpansion();
    }
    // add the new elem to the end of the array.
    _elements[_numFilled] = elem;
    _numFilled++;
    if(_numFilled == 1){
        return;
    }
    // compare the cur elem with its parent elem
    // and swap their locations if necessary
    int i = _numFilled - 1;
    while(getParentIndex(i) != NONE){
        if(_elements[getParentIndex(i)].priority > elem.priority){
            swap(getParentIndex(i), i);
            i = getParentIndex(i);
        }
        else{
            break;
        }
    }

}

/*
 * This function doubles the size of the array.
 */
void PQHeap::arrayExpansion(){
    // backup
    DataPoint *oldArr = _elements;
    // renew
    _numAllocated *= 2;
    _elements = new DataPoint[_numAllocated];
    // move data
    for(int i = 0; i < _numFilled; i++){
        _elements[i] = oldArr[i];
    }
    delete [] oldArr;
}

/*
 * Private member function. This helper exchanges the element at
 * indexA with the element at indexB.
 */
void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}


/*
 * The array elements are stored in increasing order of priority value.
 * The element at index 0 is the most urgent (minimum priority value),
 * this element is frontmost. peek returns
 * the frontmost element (most urgent, minimum priority value).
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[0];
}

/*
 * This function returns the value of the frontmost element and removes
 * it from the queue. The frontmost element was at the first-filled index.
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = peek();
    swap(0, _numFilled - 1);
    _numFilled--;
    int i = 0;
    while(getLeftChildIndex(i) != NONE){
        int iSwap = getLeftChildIndex(i);
        // compare the two child elements and select the smaller one
        if(getRightChildIndex(i) != NONE){
            if(_elements[getLeftChildIndex(i)].priority > _elements[getRightChildIndex(i)].priority){
                 iSwap = getRightChildIndex(i);
            }
        }
        // if the selceted child is smaller than the current parent, swap them
        if(_elements[i].priority > _elements[iSwap].priority){
            swap(i, iSwap);
        }
        i = iSwap;
    }
    return front;
}

/*
 * Returns true if no elements in the queue, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * Updates internal state to reflect that the queue is empty, e.g. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The previously stored elements do not need to
 * be cleared; the slots would be overwritten when additional elements
 * are enqueued, but as a defensive programming technique, we mark
 * each with a sentinel value as a debugging aid.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * This function prints the contents of internal array for debugging purposes.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * This function checks that the elements in the array are stored in
 * priority order. Report an error if problem iis found.
 */
void PQHeap::validateInternalState() const {
    // If there are more elements than spots in the array, we have a problem.
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");
    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than its child and smaller priority than its parent, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    for (int i = 0; i < size(); i++) {
        if(getParentIndex(i) != NONE){
            if (_elements[i].priority < _elements[getParentIndex(i)].priority){
                printDebugInfo("validateInternalState");
                error("PQHeap has Parent element out of order at index " + integerToString(i));
            }
        }
        if(getLeftChildIndex(i) != NONE){
            if (_elements[i].priority > _elements[getLeftChildIndex(i)].priority){
                printDebugInfo("validateInternalState");
                error("PQHeap has LeftChild element out of order at index " + integerToString(i));
            }
        }
        if(getRightChildIndex(i) != NONE){
            if (_elements[i].priority > _elements[getRightChildIndex(i)].priority){
                printDebugInfo("validateInternalState");
                error("PQHeap has RightChild element out of order at index " + integerToString(i));
            }
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if(child == 0){
        return NONE;
    }
    else{
        return (child - 1) / 2;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    if(2*parent + 1 > size() - 1){
        return NONE;
    }
    else{
        return 2*parent + 1;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    if(2*parent + 2 > size() - 1){
        return NONE;
    }
    else{
        return 2*parent + 2;
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("PQHeap, enqueue only, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

STUDENT_TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("PQHeap: dequeue or peek on empty queue raises error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}


STUDENT_TEST("PQHeap, dequeue, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

STUDENT_TEST("PQHeap, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

STUDENT_TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

STUDENT_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

STUDENT_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;
    int nIn= 30000;
    //for(int nIn = 50000; nIn <= 80000; nIn += 10000){
        TIME_OPERATION(nIn, fillQueue(pq, nIn));
        TIME_OPERATION(nIn, emptyQueue(pq, nIn));
   // }
}
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
