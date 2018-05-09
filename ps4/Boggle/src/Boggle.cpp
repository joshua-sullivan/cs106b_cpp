// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <string>

#include "Boggle.h"
#include "hashset.h"
#include "random.h"
#include "strlib.h"

using namespace std;


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AEEAGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};



Boggle::Boggle(Lexicon& dictionary, string boardText) {

    Grid<string> boggleBoard(4, 4);

    if (boardText == "") {
        boardText = shuffleBoard();
    }

    for (int ii = 0; ii < 4; ii++) {
        for (int jj = 0; jj < 4; jj++) {
           boggleBoard.set(ii, jj, toLowerCase(boardText.substr((4 * ii) + jj, 1)));
        }
    }

    dict = dictionary;
    setBoggleBoard(boggleBoard);

//    cout << this->boggleBoard.toString2D() << endl;

}

char Boggle::getLetter(int row, int col) {

    if (row >= boggleBoard.numRows() or row < 0){
        throw row;
    }

    if (col >= boggleBoard.numCols() or col < 0){
        throw col;
    }

    string letter = boggleBoard[row][col];

    return letter[0];
}

bool Boggle::checkWord(string word) {

    if (!dict.contains(word)){
        return false;
    }

    if (word.length() < 4){
        return false;
    }

    if (humanWords.contains(word)){
        return false;
    }

    return true;

}

bool Boggle::humanWordSearch(string word) {
    // TODO: implement
    return false;   // remove this
}

int Boggle::getScoreHuman() {

    return scoreHuman;
}

Set<string> Boggle::computerWordSearch() {
    // TODO: implement
    Set<string> result;   // remove this
    return result;        // remove this
}

int Boggle::getScoreComputer() {

    return scoreComputer;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    // TODO: implement
    return out;
}

string Boggle::shuffleBoard(){

    setRandomSeed(106);

    HashSet<int> usedDice;

    string boardText = "";
    while (boardText.length() != 16){

        int randomDie = randomInteger(0, 15);

        if (!usedDice.contains(randomDie)) {

            usedDice.add(randomDie);

            string die = CUBES[randomDie];

            int randomSide = randomInteger(0, 5);

            boardText += die[randomSide];
        }

    }

    return boardText;
}

void Boggle::setBoggleBoard(Grid<string>& boggleBoard){
    this->boggleBoard = boggleBoard;
}

int Boggle::getHumanWordsSize(){

    return humanWords.size();
}

void Boggle::getHumanWords(HashSet<string>& humanWords){

    humanWords = this->humanWords;

}
