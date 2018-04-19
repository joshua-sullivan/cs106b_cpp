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
HashMap<Queue<string>, Vector<string> > promptUserForInitialInputs(ifstream& stream, int& N);
void buildNGramMap(ifstream& stream, const int N, HashMap<Queue<string>, Vector<string> >& NGramMap);
void parseWindowAndAdd(HashMap<Queue<string>, Vector<string> >& NGramMap, Queue<string>& window, Vector<string>& vectVal, const string word);
void deployRandomWriter(const HashMap<Queue<string>, Vector<string> >& NGramMap, const int N);
Queue<string> createTextSequence(const HashMap<Queue<string>, Vector<string> >& NGramMap, const int numRandWords);
void printTextSequence(Queue<string>& outputTextQ);

int main() {
// Main function interface
    printWelcomeMessage();

    ifstream stream;
    int N;
    HashMap<Queue<string>, Vector<string> > NGramMap = promptUserForInitialInputs(stream, N);

    stream.close();

    deployRandomWriter(NGramMap, N);

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

HashMap<Queue<string>, Vector<string> > promptUserForInitialInputs(ifstream& stream, int& N){
// Prompts the user for the input file name, N value, and then proceeds to build an N-gram map.

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

    HashMap<Queue<string>, Vector<string> > NGramMap;

    // Building N-Gram map from moving window of size N-1
    buildNGramMap(stream, N, NGramMap);

    return NGramMap;
}

void buildNGramMap(ifstream& stream, const int N, HashMap<Queue<string>, Vector<string> >& NGramMap){
// Builds N-Gram map, with keys given by a queue of strings which has size N-1, and values given by
// a vector of strings is composed of all Nth words following the moving window.

    const int dimWindow = N - 1;
    // Initializing
    string word;
    Queue<string> window;
    Queue<string> firstWords;

    // Creating a loop counter to enqueue the first N-1 words of the text for use in wrapping.
    int counter = 0;

    // Populating the initial window and also creating a first words queue to be used when wrapping.
    while (window.size() < dimWindow){
        stream >> word;
        window.enqueue(word);

        if (counter < dimWindow){
            firstWords.enqueue(word);
        }
    }

    Vector<string> vectVal;

    // Loop through all words in the file stream
    while(stream >> word){
        // Add the word to an existing key-value pair, or create a new key-value pair
        parseWindowAndAdd(NGramMap, window, vectVal, word);
    }

    // Carrying out additional iterations to finalize the wrapping.
    string firstWord;
    for (int ii = 0; ii < N - 1; ii++){
        // Uses the same methodology for populating the non-wrapped portion of the map,
        // just with a set of words dequeued from the firstWords queue.
        firstWord = firstWords.dequeue();
        parseWindowAndAdd(NGramMap, window, vectVal, firstWord);
    }

}

void parseWindowAndAdd(HashMap<Queue<string>, Vector<string> >& NGramMap, Queue<string>& window, Vector<string>& vectVal, const string word){
// Function which updates the key-value pairs in the N-Gram map.  It checks whether the current map contains the key (the window of text).
// If no key exists, a new one is created and the value stored.  If a key exists, the current value vector is pulled and the word is added.


    // Updating key-value pair that already exists
    if (NGramMap.containsKey(window)){
        vectVal = NGramMap.get(window);
        vectVal.add(word);
        NGramMap.put(window, vectVal);
        vectVal.clear();
    }
    // Create new key-value pair
    else{
        vectVal.add(word);
        NGramMap.put(window, vectVal);
        vectVal.clear();
    }

    // Update the window (slide it forward by one).
    window.dequeue();
    window.enqueue(word);

}

void deployRandomWriter(const HashMap<Queue<string>, Vector<string> >& NGramMap, const int N){
// Recursive function which prompts the user for the number of words to generate, and then generates the "random" text.
    int numRandWords;

    bool promptForNumRandWords = true;
    while (promptForNumRandWords){

        numRandWords = getInteger("# of random words to generate (0 to quit)? ");

        // Checking if user commands to terminate the program
        if (numRandWords == 0){
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

    // Calling the text sequence creator algorithm.
    Queue<string> outputTextQ = createTextSequence(NGramMap, numRandWords);

    // Printing the output sequence.
    printTextSequence(outputTextQ);

    // Recursively calling until the user terminates the program.
    deployRandomWriter(NGramMap, N);
}

Queue<string> createTextSequence(const HashMap<Queue<string>, Vector<string> >& NGramMap, const int numRandWords){
// Given a N-Gram map, this function initializes the random text sample from a random window, and then draws successive
// words for the output text by sampling the N-Gram map pairs.  This continues until the user-specified size of output text
// is achieved.

    // Creating a vector of the map keys (which are themselves queues of strings).
    Vector<Queue<string> > mapKeys = NGramMap.keys();

    // Generating a random starting point in the vector of keys.
    int randStartIdx = randomInteger(0, mapKeys.size() - 1);

    // Creating a queue which captures the complete output text from the random writer.
    Queue<string> outputTextQ = mapKeys[randStartIdx];

    // Initializing the moving window to the starting point in the map key queue.
    Queue<string> window = outputTextQ;
    Vector<string> vectVal;
    int randIdx;
    string addWord;

    // Loop until the desired output size is achieved
    while (outputTextQ.size() < numRandWords){
        // Get the values for a given key in the N-Gram map.
        vectVal = NGramMap.get(window);

        // Randomly index into that vector of strings and enqueue that word onto the output text queue.
        randIdx = randomInteger(0, vectVal.size() - 1);
        addWord = vectVal[randIdx];
        outputTextQ.enqueue(addWord);

        // Update the window queue.
        window.dequeue();
        window.enqueue(addWord);
        vectVal.clear();
    }

    return outputTextQ;
}

void printTextSequence(Queue<string>& outputTextQ){
// Function for printing the final output queue from the random writer algorithm.
    cout << "... ";
    while (!outputTextQ.isEmpty()){
        cout << outputTextQ.dequeue() << " ";
    }
    cout << "..." << endl;
    cout << endl;
}


