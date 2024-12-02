#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Comparator function for sorting tokens by frequency and alphabetically
bool sortByFrequency(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second == b.second)
        return a.first < b.first; // Alphabetical order for ties
    return a.second > b.second;  // Descending frequency
}

int main() {
    // --- Encoding Section ---
    map<string, int> tokenFrequency;  // Tracks frequency of tokens
    map<int, string> originalTokens; // Stores original order of tokens
    string token;
    int position = 1;                // Position to track token order

    // Read tokens from input
    while (cin >> token) {
        if (!token.empty()) {
            tokenFrequency[token]++;        // Increment frequency
            originalTokens[position++] = token; // Store token with its position
        }
    }

    // Copy token-frequency pairs into an array for sorting
    pair<string, int> sortedTokens[1000]; // Assume no more than 1000 unique tokens
    int sortedIndex = 0;
    for (const auto& pair : tokenFrequency) {
        sortedTokens[sortedIndex++] = pair;
    }

    // Sort the array of token-frequency pairs
    sort(sortedTokens, sortedTokens + sortedIndex, sortByFrequency);

    // Map each token to its rank in the sorted order
    map<string, int> tokenToIndex;
    vector<string> sortedTokenList; // To store sorted tokens in order
    for (int i = 0; i < sortedIndex; ++i) {
        tokenToIndex[sortedTokens[i].first] = i + 1;
        sortedTokenList.push_back(sortedTokens[i].first);
    }

    // Encode indices based on original order
    vector<int> encodedIndices;
    for (const auto& pair : originalTokens) {
        encodedIndices.push_back(tokenToIndex[pair.second]);
    }

    // --- Decoding Section ---
    map<int, string> sortedTokenMap; // Map for decoding sorted tokens
    for (size_t i = 0; i < sortedTokenList.size(); ++i) {
        sortedTokenMap[i + 1] = sortedTokenList[i];
    }

    // Reconstruct and output the original text
    for (size_t i = 0; i < encodedIndices.size(); ++i) {
        int tokenIndex = encodedIndices[i] - 1; // Convert to 0-based index
        cout << sortedTokenMap[tokenIndex + 1];

        // Add space unless it's the end of the sequence or punctuation
        if (i < encodedIndices.size() - 1) {
            string nextToken = sortedTokenMap[encodedIndices[i + 1]];
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
a.exe < input1.txt
*/