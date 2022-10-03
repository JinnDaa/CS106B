/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * This function receives three vertices of a triangle and order of this Sierpinski graph,
 * returns the total number of drawn black triangles.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
   // int numberOfBlackTriangle = 0;
    if(order == 0){
        //base case
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else{
        // midpoint of the opposite site of point one;
        GPoint midpointOne;
        midpointOne.x = (two.x + three.x) / 2;
        midpointOne.y = (two.y + three.y) / 2;
        GPoint midpointTwo;
        midpointTwo.x = (one.x + three.x) / 2;
        midpointTwo.y = (one.y + three.y) / 2;
        GPoint midpointThree;
        midpointThree.x = (two.x + one.x) / 2;
        midpointThree.y = (two.y + one.y) / 2;
        // draw the three Sierpinski triangles of order - 1;
        return drawSierpinskiTriangle(window, one, midpointTwo, midpointThree, order - 1)
                + drawSierpinskiTriangle(window, two, midpointOne, midpointThree, order - 1)
                + drawSierpinskiTriangle(window, three, midpointOne, midpointTwo, order - 1);
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

