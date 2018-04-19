/*
 * CS 106B, Homework 2, Serafini
 *
 * ngrams.cpp source file
 *
 * Implements a random writer style emulator.  This program takes as
 * input a user-provided text file, and randomly generates a set of
 * text which emulates the style of the provided prose. The program
 * makes use of N-grams, which are sequences of words from the input.
 *
 * Written by: Joshua Sullivan, 005878651
 * Change log:
 *      04/18/2018: Created user interface functionality.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "hashmap.h"
#include "queue.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"

using namespace std;

void printWelcomeMessage();
void promptUserForInitialInputs(ifstream& stream, int& N);
void deployRandomWriter(ifstream& stream, const int N);
HashMap<Queue<string>, Vector<string> > buildGramMap(ifstream stream, const int N);
void parseWindowAndAdd(HashMap<Queue<string>, Vector<string> >& NGramMap, Queue<string>& window, Vector<string>& vectVal, const string word);
Queue<string> createTextSequence(HashMap<Queue<string>, Vector<string> >& NGramMap, const int numRandWords);
void printTextSequence(Queue<string>& outputTextQ);

int main() {

    printWelcomeMessage();

    ifstream stream;
    int N;
    promptUserForInitialInputs(stream, N);

    deployRandomWriter(stream, N);

    cout << "Exiting." << endl;
    return 0;
}

void printWelcomeMessage(){
// Prints the starting welcome message to the user.

    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

void promptUserForInitialInputs(ifstream& stream, int& N){

    promptUserForFile(stream, "Input file name? ");

    bool promptForN = true;
    while (promptForN){

        N = getInteger("Value of N? ");

        // Error handling for N < 2
        if (N > 1){
            promptForN = false;
        }
        else {
            cout << "N must be 2 or greater." << endl;
        }
    }
    cout << endl;
}

void deployRandomWriter(ifstream& stream, const int N){

    int numRandWords;

    bool promptForNumRandWords = true;
    while (promptForNumRandWords){

        numRandWords = getInteger("# of random words to generate (0 to quit)? ");

        // Checking if user commands to terminate the program
        if (numRandWords == 0){
            stream.close();
            return;
        }

        // Error handling for numRandWords < N
        if (numRandWords >= N){
            promptForNumRandWords = false;
        }
        else{
            cout << "Must be at least " << N << " words." << endl;
            cout << endl;
        }
    }

    HashMap<Queue<string>, Vector<string> > NGramMap = buildGramMap(stream, N);

    Queue<string> outputTextQ = createTextSequence(NGramMap, numRandWords);
    printTextSequence(outputTextQ);

    deployRandomWriter(stream, N);
}

HashMap<Queue<string>, Vector<string> > buildGramMap(ifstream stream, const int N){

    HashMap<Queue<string>, Vector<string> > NGramMap;

    const int dimWindow = N - 1;

    string word;
    Queue<string> window;

    int counter = 0;
    Queue<string> firstWords;
    while (window.size() < dimWindow){
        stream >> word;
        window.enqueue(word);

        if (counter < dimWindow){
            firstWords.enqueue(word);
        }
    }

    Vector<string> vectVal;

    while(stream >> word){
        parseWindowAndAdd(NGramMap, window, vectVal, word);
    }

    string firstWord;
    for (int ii = 0; ii < N - 1; ii++){
        firstWord = firstWords.dequeue();
        parseWindowAndAdd(NGramMap, window, vectVal, firstWord);
    }

    return NGramMap;
}

void parseWindowAndAdd(HashMap<Queue<string>, Vector<string> >& NGramMap, Queue<string>& window, Vector<string>& vectVal, const string word){

    if (NGramMap.containsKey(window)){
        vectVal = NGramMap.get(window);
        vectVal.add(word);
        NGramMap.put(window, vectVal);
        vectVal.clear();
    }

    else{
        vectVal.add(word);
        NGramMap.put(window, vectVal);
        vectVal.clear();
    }

    window.dequeue();
    window.enqueue(word);

}

Queue<string> createTextSequence(HashMap<Queue<string>, Vector<string> >& NGramMap, const int numRandWords){

    Vector<Queue<string> > mapKeys = NGramMap.keys();
    int randStartIdx = randomInteger(0, mapKeys.size() - 1);
    Queue<string> outputTextQ = mapKeys[randStartIdx];
    Queue<string> window = outputTextQ;
    Vector<string> vectVal;
    int randIdx;
    string addWord;

    while (outputTextQ.size() < numRandWords){
        vectVal = NGramMap.get(window);
        randIdx = randomInteger(0, vectVal.size() - 1);
        addWord = vectVal[randIdx];
        outputTextQ.enqueue(addWord);
        window.dequeue();
        window.enqueue(addWord);
        vectVal.clear();
    }

    return outputTextQ;
}

void printTextSequence(Queue<string>& outputTextQ){

    cout << "... ";
    while (!outputTextQ.isEmpty()){
        cout << outputTextQ.dequeue() << " ";
    }
    cout << "..." << endl;
    cout << endl;
}


