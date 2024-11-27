#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>

using namespace std;

int main() {
    string tokensLine, indicesLine, separatorLine;

    // Read the line containing tokens
    getline(cin, tokensLine);

    // Read the separator line (asterisks)
    getline(cin, separatorLine);

    // Read the line containing encoded indices
    getline(cin, indicesLine);

    // Parse the tokens
    map<int, string> sortedTokens; // Using map to emulate the behavior of a vector
    int tokenIndex = 0;
    string token;
    size_t pos = 0, start = 0;
    while ((pos = tokensLine.find(' ', start)) != string::npos) {
        token = tokensLine.substr(start, pos - start);
        sortedTokens[++tokenIndex] = token;
        start = pos + 1;
    }
    sortedTokens[++tokenIndex] = tokensLine.substr(start);

    // Parse the encoded indices
    map<int, int> encodedIndices; // Using map instead of vector
    int indexPosition = 0;
    start = 0;
    while ((pos = indicesLine.find(' ', start)) != string::npos) {
        int index = stoi(indicesLine.substr(start, pos - start));
        encodedIndices[++indexPosition] = index;
        start = pos + 1;
    }
    encodedIndices[++indexPosition] = stoi(indicesLine.substr(start));

    // Reconstruct and output the original text
    for (int i = 1; i <= indexPosition; ++i) {
        int tokenIndex = encodedIndices[i] - 1; // Convert to 0-based index
        cout << sortedTokens[tokenIndex + 1];

        // Add space unless it's the end of the sequence or punctuation
        if (i < indexPosition) {
            string nextToken = sortedTokens[encodedIndices[i + 1]];
            if (nextToken != "." && nextToken != "," && nextToken != "?" &&
                nextToken != ":" && nextToken != ";") {
                cout << " ";
            }
        }
    }
    cout << endl;

    return 0;
}




/*
cd Project\Project 5
g++ project5_decompress.cpp
a.exe < output1.txt
*/