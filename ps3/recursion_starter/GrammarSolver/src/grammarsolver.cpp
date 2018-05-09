/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Josh Sullivan
 * Section leader: Luciano Gonzalez
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"
#include "map.h"
#include "random.h"
#include "strlib.h"

using namespace std;

Map<string, Vector<string>> buildRuleMap(istream& input);
void generateSequence(string symbol, const Map<string, Vector<string>>& ruleMap, string& phrase);

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    Map<string, Vector<string>> ruleMap = buildRuleMap(input);

    Vector<string> phrases;
    for(int ii = 0; ii < times; ii++){
        string phrase;
        generateSequence(symbol, ruleMap, phrase);
        phrases.add(trim(phrase));
    }

    return phrases;           // This is only here so it will compile
}

Map<string, Vector<string>> buildRuleMap(istream& input){

    string rule;
    Vector<string> tempRule;
    string mapKey;
    Vector<string> mapValues;
    Map<string, Vector<string>> ruleMap;

    while(getline(input, rule)){
        tempRule = stringSplit(rule, "::=");
        mapKey = trim(tempRule[0]);
        if (ruleMap.containsKey(mapKey)){
            throw "Map already contains key.";
        }
        mapValues = stringSplit(tempRule[1], "|");
        for(int ii = 0; ii < mapValues.size(); ii++){
            mapValues[ii] = trim(mapValues[ii]);
        }
    ruleMap.put(mapKey, mapValues);
    }

    return ruleMap;

}

void generateSequence(string symbol, const Map<string, Vector<string>>& ruleMap, string& phrase){

    // Base Case when symbol is terminal
    if (!ruleMap.containsKey(symbol)){
        phrase += symbol;
        phrase += " ";
    }
    else{
        // Recursively walk through the ruleMap
        Vector<string> symbolValues = ruleMap.get(symbol);
        int randIdx = randomInteger(0, symbolValues.size() - 1);
        string value = symbolValues[randIdx];
        Vector<string> valueSplit = stringSplit(value, " ");
        for (int ii = 0; ii < valueSplit.size(); ii++){
            generateSequence(valueSplit[ii], ruleMap, phrase);
        }
    }
}
