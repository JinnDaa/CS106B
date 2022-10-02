// This is a .cpp program that finds a way out of a given maze.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function finds all the neighboring locations
 * of the current location and stores them in a Set<GridLocation> variable.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    // Move one step towards the 4 possible directions.
    GridLocation east(cur.row, cur.col + 1);
    GridLocation west(cur.row, cur.col - 1);
    GridLocation north(cur.row + 1, cur.col);
    GridLocation south(cur.row - 1, cur.col);
    // Judge if the movements are within maze range and along the paths.
    if(east.col <= maze.numCols() - 1 && maze[east.row][east.col]){
        neighbors.add(east);
    }
    if(west.col >=0 && maze[west.row][west.col]){
        neighbors.add(west);
    }
    if(north.row <= maze.numRows() - 1 && maze[north.row][north.col]){
        neighbors.add(north);
    }
    if(south.row >=0 && maze[south.row][south.col]){
        neighbors.add(south);
    }

    return neighbors;
}

/* This function test if a path is a way out of the maze.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
     int pathSize = path.size();
     //Use a Set to include all the distinct points of a path.
     Set<GridLocation> pathLocations;
     for(int i = 0; i < pathSize - 1; i++){
         GridLocation thisLocation = path.pop();
         pathLocations.add(thisLocation);
         GridLocation previousLocation = path.peek();
         if(pathLocations.contains(previousLocation)){
             error("This path goes through a point more than one time");
         }
         if(!generateValidMoves(maze, previousLocation).contains(thisLocation)){
             error("This move does not satisfy rules");
         }
     }
     GridLocation firstLocation = path.peek();
     GridLocation entrance = {0, 0};
     if(firstLocation != entrance){
         error("Path does not start at maze entrance");
     }

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* This fucntion finds a correct way out of the maze.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Queue<Stack<GridLocation>> pathQueue;
    GridLocation entrance = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    path.push(entrance);
    pathQueue.enqueue(path);
    while(!pathQueue.isEmpty()){
       Stack<GridLocation> currentPath = pathQueue.dequeue();
       MazeGraphics::highlightPath(currentPath, "green", 0);
       if(currentPath.peek() == exit){
           path = currentPath;
           break;
       }
       else{
           Stack<GridLocation> pathCopy = currentPath;
           //get the Set of points contained in current path
           Set<GridLocation> pathLocations = {};
           while(!pathCopy.isEmpty()){
               pathLocations.add(pathCopy.pop());
           }
           Set<GridLocation> validMoves = generateValidMoves(maze, currentPath.peek());
           //remove moves that contains points in previous path
           validMoves -= pathLocations;
           for(GridLocation location : validMoves){
               pathCopy = currentPath;
               pathCopy.push(location);
               pathQueue.enqueue(pathCopy);
           }
       }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("generateValidMoves on location on the LEFT side of 3x3 grid with walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {true, true, true}};
    GridLocation side = {1, 0};
    Set<GridLocation> expected = {{0,0}, {2,0}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the RIGHT side of 3x3 grid with walls and without acceptable moves") {
    Grid<bool> maze = {{true, true, false},
                       {true, false, true},
                       {true, true, false}};
    GridLocation side = {1, 2};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the TOP side of 3x3 grid with walls") {
    Grid<bool> maze = {{true, true, false},
                       {true, false, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the BOTTOM side of 3x3 grid with walls") {
    Grid<bool> maze = {{true, true, false},
                       {true, false, true},
                       {true, true, true}};
    GridLocation side = {2, 2};
    Set<GridLocation> expected = {{2, 1}, {1, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze;
    Stack<GridLocation> not_end_at_exit;
    Stack<GridLocation> not_begin_at_entry;
    Stack<GridLocation> go_through_wall;
    Stack<GridLocation> teleport;
    Stack<GridLocation> revisit;

    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7_invalid_1.soln", not_end_at_exit);
    readSolutionFile("res/5x7_invalid_2.soln", not_begin_at_entry);
    readSolutionFile("res/5x7_invalid_3.soln", go_through_wall);
    readSolutionFile("res/5x7_invalid_4.soln", teleport);
    readSolutionFile("res/5x7_invalid_5.soln", revisit);

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
