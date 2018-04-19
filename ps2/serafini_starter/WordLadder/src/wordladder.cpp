/*
 * CS 106B, Homework 2, Serafini
 *
 * wordladder.cpp source file
 *
 * Implements a word ladder from one user-provided word to
 * another user-provided word.  A word ladder is a bridge between
 * two words formed by changing one letter at a time, with the
 * constraint that each intermediate sequence of letters still forms
 * a valid word.
 *
 * Written by: Joshua Sullivan, 005878651
 * Change log:
 *      04/17/2018: Created user interface functionality.
 *      04/18/2018: Implemented word ladder algorithm and tested.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "hashset.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "stack.h"
#include "strlib.h"

using namespace std;

void printWelcomeMessage();
Lexicon createLexicon(ifstream& stream);
void deployUserInterface(Lexicon& lex);
bool promptUserForWords(Lexicon& lex, string& word1, string& word2);
bool runWordLadderAlgo(Lexicon& lex, const string& word1, const string& word2, Stack<string>& solnStack);
Stack<string> findNeighborWords(Lexicon& lex, const string& word);
void printWordLadderSolution(const string& word1, const string& word2, Stack<string>& solnStack);

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";

int main(){
// Main function interface

    printWelcomeMessage();

    ifstream stream;
    Lexicon lex = createLexicon(stream);
    stream.close();

    deployUserInterface(lex);

    cout << "Have a nice day." << endl;
    return 0;
}

void printWelcomeMessage(){
// Prints the starting welcome message to the user.

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

Lexicon createLexicon(ifstream& stream){
// Initializes new lexicon with data stream from user prompt.

    promptUserForFile(stream, "Dictionary file name? ");
    cout << endl;

    // Creating the lexicon.
    Lexicon lex(stream);

    return lex;
}

void deployUserInterface(Lexicon& lex){
// Recursive function which deploys all of the user interface/prompting utilities.

    string word1;
    string word2;
    // Prompts user for words and returns boolea if user specifies to terminate.
    bool progTerminated = promptUserForWords(lex, word1, word2);
    if (progTerminated){
        return;
    }
    // Create solution stack object
    Stack<string> solnStack;
    // Run the word ladder algorithm and return a boolean on whether solution has been found.
    bool solnFound = runWordLadderAlgo(lex, word1, word2, solnStack);
    if (solnFound){
        printWordLadderSolution(word1, word2, solnStack);
    }
    else {
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
        cout << endl;
    }

    // Recursively call the UI utility until program has been terminated by user.
    deployUserInterface(lex);
}

bool promptUserForWords(Lexicon& lex, string& word1, string& word2){
// Recursive function which prompts the user for the two words and runs error checks on them

    word1 = trim(toLowerCase(getLine("Word #1 (or Enter to quit): ")));
    // Check if enter was hit.
    if (word1.length() == 0){
        return true;
    }
    word2 = trim(toLowerCase(getLine("Word #2 (or Enter to quit): ")));
    // Check if enter was hit.
    if (word2.length() == 0){
        return true;
    }

    // Error handling to check if the words are of same length
    if (word1.length() != word2.length()){
        cout << "The two words must be the same length." << endl;
        cout << endl;
        promptUserForWords(lex, word1, word2);
    }
    // Error handling to check if the words are the same.
    else if (word1 == word2){
        cout << "The two words must be different." << endl;
        cout << endl;
        promptUserForWords(lex, word1, word2);
    }
    // Error handling to check if both words exist in lexicon
    else if (!lex.contains(word1) || !lex.contains(word2)){
        cout << "The two words must be found in the dictionary." << endl;
        cout << endl;
        promptUserForWords(lex, word1, word2);
    }

    return false;
}

bool runWordLadderAlgo(Lexicon& lex, const string& word1, const string& word2, Stack<string>& solnStack){
// Function which runs the word ladder algorithm using two user-provided words and a lexicon.

    // Creating a queue of stacks
    Queue<Stack<string> > stackQ;

    // Initializing a stack that contains word1
    Stack<string> stack1;
    stack1.push(word1);

    // Initialiizing the queue with this initial stack
    stackQ.enqueue(stack1);

    // Creating a hashset of used words to ensure words are not repeated in a ladder
    HashSet<string> usedWords;
    usedWords.add(word1);

    Stack<string> tempStack_copy;

    // While the queue is not empty...
    while (!stackQ.isEmpty()){
        // Dequeue the partial-ladder stack from the front of the queue
        Stack<string> tempStack = stackQ.dequeue();
        // Peek the word from the top of the stack
        string tempWord = tempStack.peek();
        // Create a stack of all of this words neighbors
        Stack<string> neighborStack = findNeighborWords(lex, tempWord);
        // For each of these neighbor words...
        while (!neighborStack.isEmpty()){
            // Pop them off one at a time
            string tempNeighbor = neighborStack.pop();
            // and check if they have already been used in a prior partial-ladder
            if (!usedWords.contains(tempNeighbor)){
                // add them to the used words set
                usedWords.add(tempNeighbor);
                // if we found our second word, populate solution stack and return true
                if (tempNeighbor == word2){
                    tempStack.push(word2);
                    solnStack = tempStack;
                    return true;
                }
                // if we didn't find our second word, add the new neighbor and continue on.
                else {
                    tempStack_copy = tempStack;
                    tempStack_copy.push(tempNeighbor);
                    stackQ.enqueue(tempStack_copy);
                }
            }
        }
    }
    // If no solution is obtained, no path exists between the two words.
    return false;
}

Stack<string> findNeighborWords(Lexicon& lex, const string& word){
// Function which finds all the dictionary word neighbors to an input word.

    Stack<string> neighborStack;

    // Looping over the words characters.
    for (size_t ii = 0; ii < word.length(); ii++){
        // Looping over the alphabet.
        for (size_t jj = 0; jj < ALPHABET.length(); jj++){
            string neighborWord = word;
            neighborWord[ii] = ALPHABET.at(jj);

            // Checking the lexicon for the new word.
            if (lex.contains(neighborWord)){
                neighborStack.push(neighborWord);
            }
        }
    }
    return neighborStack;
}

void printWordLadderSolution(const string& word1, const string& word2, Stack<string>& solnStack){
// Function which prints the solution stack.

    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while (!solnStack.isEmpty()){
        string nextWord = solnStack.pop();
        cout << nextWord << " ";
    }
    cout << endl;
    cout << endl;
}
