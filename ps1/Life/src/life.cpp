/*
 * CS 106B, Homework 1, Game of Life
 *
 * life.cpp source file
 *
 * Implements the Game of Life simulation conceived by J. H. Conway
 * in 1970.  A user selects an input text file which describes the
 * initial configuration of a "colony of bacteria," and then
 * interactively propagates the lifecycle of that colony forward
 * according to a set of rules.
 *
 * Written by: Joshua Sullivan, 005878651
 * Change log:
 *      04/09/2018: Added bare-bones structure to program.
 *      04/11/2018: Filled in structure with support functions.
 *      04/12/2018: Testing and debugging.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"

using namespace std;

void printWelcomeMessage();
string promptUserForInputs(ifstream& stream, bool& wrapFlag);
void promptForWrapping(string& wrapping);
void printGridIntStateToConsole(Grid<size_t>& gridState);
void transformDataToGrid(ifstream& stream, Grid<size_t>& gridState);
void printGridStateToConsole(Grid<size_t>& gridState);
void promptUserForGameplay(string& gameplayMode, Grid<size_t>& gridState, bool& wrapFlag);
void promptForFrames(size_t& numFrames);
void propagateOneGeneration(Grid<size_t>& gridState, bool& wrapFlag);
void propagateMultipleGenerations(size_t numGen, Grid<size_t>& gridState, bool& wrapFlag);

const int NUM_MS_PAUSE = 50;

int main() {
    printWelcomeMessage();

    bool wrapFlag = false;
    ifstream stream;
    string filename = promptUserForInputs(stream, wrapFlag);

    Grid<size_t> gridState;
    transformDataToGrid(stream, gridState);

    string gameplayMode;
    promptUserForGameplay(gameplayMode, gridState, wrapFlag);

    cout << "Have a nice Life!" << endl;
    return 0;
}

void printWelcomeMessage(){
    //  Prints the welcome message, initiating the CS 106B Game of Life
    //  simulation and explaining the rules of the game.

    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << endl;
}

string promptUserForInputs(ifstream& stream, bool& wrapFlag){
    // Function for prompting the user for filename and wrapping inputs.

    string filename = promptUserForFile(stream, "Grid input file name? ");
    string wrapping;
    promptForWrapping(wrapping);

    // Sets a boolean flag denoting if wrapping is occuring (1) or not occuring (0)
    wrapFlag = toLowerCase(wrapping).compare("n");

    return filename;
}

void promptForWrapping(string& wrapping){
    //  Recursively prompts for user input on wrapping.  Checks if anything other than
    //  y or n is given, and repeatedly prompts until a proper input is provided.

    wrapping = getLine("Should the simulation wrap around the grid (y/n)? ");

    if (toLowerCase(wrapping).compare("y") !=0 && toLowerCase(wrapping).compare("n") != 0){
        promptForWrapping(wrapping);
    }
}

void printGridIntStateToConsole(Grid<size_t>& gridState){
    // A debugging utility to see the internal grid state
    // in terms of 1's and 0's on the console.
    cout << gridState.toString2D() << endl;
}

void transformDataToGrid(ifstream& stream, Grid<size_t>& gridState){
    //  Takes input sream and transforms text data to grid data

    string content;
    size_t whileCnt = 0;
    size_t nRows = 0;
    size_t nCols = 0;

    // Loop through the stream content and extract data
    while(getline(stream, content)){
        // First extract number of rows entry from first line
        if (whileCnt == 0){
            nRows = stringToInteger(content);
        }
        // Then extract number of cols entry from second line
        else if (whileCnt == 1){
            nCols = stringToInteger(content);
            gridState.resize(nRows, nCols);
        }
        // Then disregard any other entries in the text file
        // past the bounds of the grid data
        else if (whileCnt == nRows + 2){
            break;
        }
        // For each row, evaluate each column entry and convert
        // to 1 or 0 based on whether the cell contains a live
        // agent in the simulation.
        else{
            cout << content << endl;
            size_t rowCnt = whileCnt - 2;
            size_t colCnt = 0;
            for (char ch : content){
                if (ch == 'X'){
                    gridState.set(rowCnt, colCnt, 1);
                }
                else {
                    gridState.set(rowCnt, colCnt, 0);
                }
                colCnt++;
            }
        }
        whileCnt++;
    }

    stream.close();
}

void printGridStateToConsole(Grid<size_t>& gridState){
    // Prints the current display state of the grid (in terms
    // of '-' and 'X') to the console

    for (int row = 0; row < gridState.numRows(); row++){
        for (int col = 0; col < gridState.numCols(); col++){
            if (gridState[row][col] == 1){
                cout << "X";
            }
            else {
                cout << "-";
            }
        }
        cout << endl;
    }
}

void promptUserForGameplay(string& gameplayMode, Grid<size_t>& gridState, bool& wrapFlag){
    // Recursively prompts the user for gameplay next steps and
    // propagates the generations accordingly or quits

    gameplayMode = getLine("a)nimate, t)ick, q)uit? ");
    if (toLowerCase(gameplayMode).compare("a") == 0){
        // Animated case: prompts user recursively for numFrames and then
        // repeats the single generation propagation case.
        size_t numFrames = 0;
        promptForFrames(numFrames);
        propagateMultipleGenerations(numFrames, gridState, wrapFlag);
        promptUserForGameplay(gameplayMode, gridState, wrapFlag);
    }
    else if (toLowerCase(gameplayMode).compare("t") == 0){
        // Tick case: computes one generation at a time.
        propagateOneGeneration(gridState, wrapFlag);
        printGridStateToConsole(gridState);
        promptUserForGameplay(gameplayMode, gridState, wrapFlag);
    }
    else if (toLowerCase(gameplayMode).compare("q") == 0){
        return;
    }
    else {
        cout << "Invalid choice; please try again." << endl;
        promptUserForGameplay(gameplayMode, gridState, wrapFlag);
    }
}

void promptForFrames(size_t& numFrames){
    // Recursive function for prompting the user for number of frames in animated case.
    string numFrames_str = getLine("How many frames? ");
    // Uses a try/catch block to ensure the user is providing a string that translates
    // cleanly to an integer number of frames.
    try {
        numFrames = stringToInteger(numFrames_str);
        return;
    }
    catch (ErrorException){
        cout << "Illegal integer format. Try again." << endl;
        promptForFrames(numFrames);
    }
}
void propagateOneGeneration(Grid<size_t>& gridState, bool& wrapFlag){
    // Propagates forward by one generation.

    // Creates a temporary copy of gridState so as to not alter the original until the end
    Grid<size_t> gridState_copy = gridState;

    int nRows = gridState.numRows();
    int nCols = gridState.numCols();

    for (int row = 0; row < nRows; row++){
        for (int col = 0; col < nCols; col++){
            size_t sumNeighbors = 0;
            for (int row_delta = -1; row_delta < 2; row_delta++){
                for (int col_delta = -1; col_delta < 2; col_delta++){
                    size_t rowIdx;
                    size_t colIdx;
                    if (wrapFlag){
                        rowIdx = (row + row_delta + nRows) % nRows;
                        colIdx = (col + col_delta + nCols) % nCols;
                    }
                    else{
                        rowIdx = row + row_delta;
                        colIdx = col + col_delta;
                    }
                    if (gridState.inBounds(rowIdx, colIdx)){
                        if (!(row_delta == 0 && col_delta == 0)){
                            sumNeighbors += gridState[rowIdx][colIdx];
                        }
                    }
                }
            }
            if ((sumNeighbors < 2) || (sumNeighbors > 3)){
                gridState_copy[row][col] = 0;
            }
            else if (sumNeighbors == 3){
                gridState_copy[row][col] = 1;
            }
        }
    }
    gridState = gridState_copy;
}
void propagateMultipleGenerations(size_t numGen, Grid<size_t>& gridState, bool& wrapFlag){
    // Propagates forward by numGen generations.  Uses propagateOneGeneration repeatedly.
    // Additional brings in the plotting to console utilities so that the console can be cleared.

    for (size_t gen = 0; gen < numGen; gen++){
        clearConsole();
        propagateOneGeneration(gridState, wrapFlag);
        printGridStateToConsole(gridState);
        pause(NUM_MS_PAUSE);
    }
}
