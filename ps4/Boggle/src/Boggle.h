// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>

#include "lexicon.h"
#include "grid.h"
#include "hashset.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Grid<string> boggleBoard;
    Lexicon dict;
    int getHumanWordsSize();
    void getHumanWords(HashSet<string>& humanWords);

    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    string shuffleBoard();
    HashSet<string> humanWords;
    void setBoggleBoard(Grid<string>& boggleBoard);
    int scoreHuman = 0;
    int scoreComputer = 0;
};

#endif // _boggle_h
