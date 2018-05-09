// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <string>

#include "Boggle.h"
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "strlib.h"

using namespace std;

string promptUserForBoardText();
void printGameStatus(Boggle& boggleGame, string& upperMessage);
void promptUserForWord(Boggle& boggleGame, string& upperMessage);

void playOneGame(Lexicon& dictionary) {

    string boardText;
    // Prompt user and construct the Boggle game object
    if (!getYesOrNo("Do you want to generate a random board? ")){
        boardText = promptUserForBoardText();
    }
    else{
        boardText = "";
    }

    Boggle boggleGame(dictionary, boardText);

    string upperMessage = "It's your turn!";
    promptUserForWord(boggleGame, upperMessage);

}

string promptUserForBoardText(){

    const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string boardText;
    // Prompt user in while loop and parse for error checking
    bool properInput = false;
    while (!properInput){
        properInput = true;
        boardText = getLine("Type the 16 letters to appear on the board: ");

        boardText = toUpperCase(boardText);

        if (boardText.length() != 16){
            // Re-prompt user for proper number of letters
            properInput = false;
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            continue;
        }

        size_t found;
        for (int ii = 0; ii < boardText.length(); ii++) {
            found = ALPHABET.find(boardText[ii]);
            if (found == string::npos) {
                properInput = false;
            }
        }
        if (properInput == false){
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }


    return boardText;
}

void promptUserForWord(Boggle& boggleGame, string& upperMessage){

    printGameStatus(boggleGame, upperMessage);

    string word;
    word = getLine("Type a word (or Enter to stop): ");

    while(!word.empty()){
        printGameStatus(boggleGame, upperMessage);
        word = getLine("Type a word (or Enter to stop): ");
    }
}

void printGameStatus(Boggle& boggleGame, string& upperMessage){

    clearConsole();
    cout << upperMessage << endl;

    // Printing Boggle board status
    for (int ii = 0; ii < 4; ii++){
        for (int jj = 0; jj < 4; jj++){

            cout << toUpperCase(boggleGame.boggleBoard[ii][jj]);
        }
        cout << endl;
    }
    cout << endl;

    // Printing human words status
    HashSet<string> humanWords_local;
    boggleGame.getHumanWords(humanWords_local);

    cout << "Your words (" << boggleGame.getHumanWordsSize() << "): " << humanWords_local.toString() << endl;

    // Printing human score status
    cout << "Your score: " << boggleGame.getScoreHuman() << endl;
}
