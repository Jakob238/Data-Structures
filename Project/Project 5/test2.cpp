#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

// Comparator for sorting by frequency, then alphabetically
bool sortByFrequency(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second == b.second)
        return a.first < b.first; // Alphabetical order for ties
    return a.second > b.second;  // Descending frequency
}

int main() {
    unordered_map<string, int> tokenFrequency; // To store token frequencies
    vector<string> originalTokens;            // To maintain order of tokens
    string token;

    // Read tokens from standard input
    while (cin >> token) {
        if (!token.empty()) {
            originalTokens.push_back(token);  // Add every token to the original list
            tokenFrequency[token]++;          // Increment frequency
        }
    }

    // Sort tokens by frequency and alphabetically
    vector<pair<string, int>> sortedTokens(tokenFrequency.begin(), tokenFrequency.end());
    sort(sortedTokens.begin(), sortedTokens.end(), sortByFrequency);

    // Map tokens to their positions in sorted order
    unordered_map<string, int> tokenToIndex;
    int position = 1;
    for (const auto& pair : sortedTokens) {
        tokenToIndex[pair.first] = position++;
    }

    // Print sorted tokens
    for (const auto& pair : sortedTokens) {
        cout << pair.first << " ";
    }
    cout << endl << "**********" << endl;

    // Print indices of tokens from the original order (including duplicates)
    for (const string& originalToken : originalTokens) {
        cout << tokenToIndex[originalToken] << " ";
    }
    cout << endl;

    return 0;
}

/*
cd Project\Project 5
g++ test2.cpp
a.exe < input1.txt
*/

// produces identical to expected output