#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <cctype> // For isdigit
#include <algorithm> // For all_of

using namespace std;

// Helper function to trim whitespace
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Helper function to safely convert string to int
int safe_stoi(const string &str) {
    string trimmed = trim(str);
    if (trimmed.empty() || !all_of(trimmed.begin(), trimmed.end(), ::isdigit)) {
        throw invalid_argument("Invalid integer string: " + str);
    }
    return stoi(trimmed);
}

int main() {
    string tokensLine, indicesLine, separatorLine;

    // Read the line containing tokens
    getline(cin, tokensLine);

    // Read the separator line (asterisks)
    getline(cin, separatorLine);

    // Read the line containing encoded indices
    getline(cin, indicesLine);

    // Step 1: Parse the tokens
    map<int, string> sortedTokens;
    int tokenIndex = 0;
    size_t pos = 0, start = 0;
    while ((pos = tokensLine.find(' ', start)) != string::npos) {
        string token = tokensLine.substr(start, pos - start);
        token = trim(token);
        sortedTokens[++tokenIndex] = token;
        start = pos + 1;
    }
    sortedTokens[++tokenIndex] = trim(tokensLine.substr(start));

    // Step 2: Parse the encoded indices
    map<int, int> encodedIndices;
    int indexPosition = 0;
    start = 0;
    while ((pos = indicesLine.find(' ', start)) != string::npos) {
        string indexStr = indicesLine.substr(start, pos - start);
        int index = safe_stoi(indexStr);
        encodedIndices[++indexPosition] = index;
        start = pos + 1;
    }
    encodedIndices[++indexPosition] = safe_stoi(indicesLine.substr(start));

    // Step 3: Reconstruct and output the original text
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
g++ bonustest.cpp
a.exe < output1.txt
*/