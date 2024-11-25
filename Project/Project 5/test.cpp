#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>

using namespace std;

// Comparator function for sorting tokens by frequency and alphabetically
bool sortByFrequency(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second == b.second)
        return a.first < b.first; // Alphabetical order for ties
    return a.second > b.second;  // Descending frequency
}

int main() {
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
    int rank = 1;
    for (int i = 0; i < sortedIndex; ++i) {
        tokenToIndex[sortedTokens[i].first] = rank++;
    }

    // Print sorted tokens
    for (int i = 0; i < sortedIndex; ++i) {
        cout << sortedTokens[i].first << " ";
    }
    cout << endl << "**********" << endl;

    // Print indices of tokens based on their original order
    for (const auto& pair : originalTokens) {
        cout << tokenToIndex[pair.second] << " ";
    }
    cout << endl;

    return 0;
}

/*
cd Project\Project 5
g++ test.cpp
a.exe < input1.txt
*/

