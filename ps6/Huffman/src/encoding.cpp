#include "encoding.h"
#include "filelib.h"
#include "pqueue.h"

/*
 * Reads input from a given istream (which could be a file on disk, a string
 * buffer, etc.). It then counts and returns a mapping from each character
 * (represented as int here) to the number of times that character appears
 * in the file. It also adds a single occurrence of the fake character
 * PSEUDO_EOF into the map. We assume that the input file exists and can
 * be read, though the file might be empty. An empty file would cause the
 * function to return a map containing only the 1 occurrence of PSEUDO_EOF.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    // Initialize the currChar from istream
    int currChar = input.get();

    // Loop over file and get characters one by one
    while (currChar != -1) {

        // if character doesn't already exist in map, initialize as 1
        if (!freqTable.containsKey(currChar)) {
            freqTable.put(currChar, 1);
        }

        // else increment value of that key by 1
        else {
            int currVal = freqTable[currChar];
            freqTable.put(currChar, currVal + 1);
        }

        // Update currChar
        currChar = input.get();
    }

    // Add PSEUDO_EOF
    freqTable.put(PSEUDO_EOF, 1);

    return freqTable;
}

/*
 * This function will accept a frequency table and uses it to create a Huffman
 * encoding tree based on those frequencies. It returns a pointer to the node
 * representing the root of the tree.
 * It assumes that the frequency table is valid: that it does not contain any
 * keys other than char values, PSEUDO_EOF, and NOT_A_CHAR; that all counts are
 * positive integers; and that it contains at least one key/value pairing. When
 * building the encoding tree, it uses a priority queue to keep track of which
 * nodes to process next. It uses the PriorityQueue collection provided by the
 * Stanford libraries, defined in library header pqueue.h. This allows each
 * element to be enqueued along with an associated priority. The dequeue function
 * always returns the element with the most urgent priority number.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    // Initialize priority queue
    PriorityQueue<HuffmanNode*> pq;

    // Populate priority queue with node pointers
    for (int currChar : freqTable) {
        HuffmanNode* h = new HuffmanNode;
        h->character = currChar;
        h->count = freqTable[currChar];
        h->zero = nullptr;
        h->one = nullptr;
        pq.enqueue(h, h->count);
    }

    // Build tree
    while (pq.size() > 1) {

        // Pull front two node pointers
        HuffmanNode* left = pq.dequeue();
        HuffmanNode* right = pq.dequeue();

        // Create new parent node
        HuffmanNode* parent = new HuffmanNode;
        parent->count = left->count + right->count;
        parent->zero = left;
        parent->one = right;

        // Enqueue the parent
        pq.enqueue(parent, parent->count);
    }

    HuffmanNode* root = pq.peek();

    return root;
}

/*
 * Helper function to buildEncodingMap which uses recursive backtracking
 * out the map with the nodes in the tree.
 */
void fillEncodingMap(Map<int, string>& encodingMap,
                     HuffmanNode* currBranch, string& currString) {

    // Base Case
    if (currBranch->character != NOT_A_CHAR) {
        encodingMap.add(currBranch->character, currString);
    }

    else {

        // Recurse to left child
        currString += "0";
        fillEncodingMap(encodingMap, currBranch->zero, currString);
        currString.pop_back();

        // Recurse to right child
        currString += "1";
        fillEncodingMap(encodingMap, currBranch->one, currString);
        currString.pop_back();
    }
}

/*
 * This function accepts a pointer to the root node of a Huffman tree and uses it
 * to create and returns a Huffman encoding map based on the tree's structure. Each
 * key in the map is a character, and each value is the binary encoding for that
 * character represented as a string. For example, if the character 'a' has binary
 * value 10 and 'b' has 11, the map stores the key/value pairs 'a':"10" and
 * 'b':"11". If the encoding tree is nullptr, it returns an empty map.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;

    string currString = "";

    // Fill the map
    fillEncodingMap(encodingMap, encodingTree, currString);

    return encodingMap;
}

/*
 * Writes string to binary stream bit by bit
 */
void writeStringToBinFile(obitstream& output, string& binVal) {
    for (int i = 0; i < binVal.size(); i++) {
        output.writeBit(charToInteger(binVal[i]));
    }
}

/*
 * This function reads one character at a time from a given input file, and uses the
 * provided encoding map to encode each character to binary, then writes the
 * character's encoded binary bits to the given bit output bit stream.
 * After writing the file's contents, it writes a single occurrence of the binary
 * encoding for PSEUDO_EOF into the output. It assumes that the parameters
 * are valid: that the encoding map is valid and contains all needed data, that the
 * input stream is readable, and that the output stream is writable. The streams are
 * already opened and ready to be read/written.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    // Rewind stream
    rewindStream(input);

    // Initialize the currChar from istream
    int currChar = input.get();

    // Loop over file and get characters one by one
    while (currChar != -1) {
        string binVal = encodingMap.get(currChar);
        writeStringToBinFile(output, binVal);

        // Update currChar
        currChar = input.get();
    }

    // Add the PSEUDO_EOF key
    string binVal = encodingMap.get(PSEUDO_EOF);
    writeStringToBinFile(output, binVal);

}

/*
 * Helper function to buildDecodingMap which uses recursive backtracking
 * out the map with the nodes in the tree.
 */
void fillDecodingMap(Map<string, int>& decodingMap,
                     HuffmanNode* currBranch, string& currString) {

    // Base Case
    if (currBranch->character != NOT_A_CHAR) {
        decodingMap.add(currString, currBranch->character);
    }

    else {

        // Recurse to left child
        currString += "0";
        fillDecodingMap(decodingMap, currBranch->zero, currString);
        currString.pop_back();

        // Recurse to right child
        currString += "1";
        fillDecodingMap(decodingMap, currBranch->one, currString);
        currString.pop_back();
    }
}

/*
 * This function accepts a pointer to the root node of a Huffman tree and uses it
 * to create and returns a Huffman decoding map based on the tree's structure.
 */
Map<string, int> buildDecodingMap(HuffmanNode* encodingTree) {

    Map<string, int> decodingMap;
    string currString = "";

    // Fill the map
    fillDecodingMap(decodingMap, encodingTree, currString);

    return decodingMap;
}


/*
 * This function does the opposite of encodeData; it read bits from the given
 * input file one at a time, and recursively walks through the specified
 * decoding tree to write the original uncompressed contents of that file to
 * the given output stream.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    Map<string, int> decodingMap;
    decodingMap = buildDecodingMap(encodingTree);
    string currChunk = "";

    while(true) {
        int currChar = input.readBit();
        currChunk.append(integerToString(currChar));
        // Try converting to character using map
        if (decodingMap.containsKey(currChunk)) {
            int c = decodingMap.get(currChunk);
            if (c == PSEUDO_EOF) {
                break;
            }
            output.put(c);
            currChunk = "";
        }
    }
}

void compress(istream& input, obitstream& output) {

    // build frequency table
    Map<int, int> freqTable;
    freqTable = buildFrequencyTable(input);

    // write freq table to output
    output << freqTable;

    // build encoding tree
    HuffmanNode* rootNodePointer;
    rootNodePointer = buildEncodingTree(freqTable);

    // build encoding map
    Map<int, string> encodingMap;
    encodingMap = buildEncodingMap(rootNodePointer);

    // encode data
    encodeData(input, encodingMap, output);

    // free tree
    freeTree(rootNodePointer);

}

void decompress(ibitstream& input, ostream& output) {

    // read freq table
    Map<int, int> freqTable;
    input >> freqTable;

    // build encoding tree
    HuffmanNode* rootNodePointer;
    rootNodePointer = buildEncodingTree(freqTable);

    // decode data
    decodeData(input, rootNodePointer, output);

    // free tree
    freeTree(rootNodePointer);
}

void freeTree(HuffmanNode* node) {
    // Base case
    if (node->zero == nullptr and node->one == nullptr) {
        delete node;
    }

    else {

        // Recurse to left child
        freeTree(node->zero);

        // Recurse to right child
        freeTree(node->one);

        // Delete the parent node
        delete node;
    }
}
