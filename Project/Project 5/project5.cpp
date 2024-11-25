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
    
    //get each token and store them in the unordered_map (or map) increment
    //its frequencies. You MAY be able to do myTokens[aToken]++. Work on this
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


/* LLM and GitHub Copilot Usage Documentation :

Prompt 1: Help me with sorting the tokens by frequency and alphabetically in the code snippet provided.

Answer: This program reads tokens (words or strings) from the input, tracks their frequency, and sorts them by the following criteria:
Descending frequency: Tokens that appear more often are ranked higher.
Alphabetical order (in case of ties): Tokens with the same frequency are sorted in ascending lexicographical order.
Steps Explained:
Token Frequency Count:
The program uses a map<string, int> (tokenFrequency) to count the frequency of each token. Each token read from the input increments its 
corresponding count in the map.

Tracking Original Token Order:
A second map, map<int, string> (originalTokens), stores tokens in their original input order. The integer key represents the position of the token.
Copying Token-Frequency Pairs:
The map tokenFrequency is copied into a fixed-size array of pair<string, int> (sortedTokens) for sorting. The size is assumed to be no more than
1000 unique tokens.
Sorting Tokens:
The array of token-frequency pairs is sorted using the sort function with a custom comparator sortByFrequency.
Comparator Logic:
If two tokens have the same frequency (a.second == b.second), sort them alphabetically (a.first < b.first).
Otherwise, sort by descending frequency (a.second > b.second).
Mapping Tokens to Ranks:
The sorted tokens are mapped to their rank (1 for the most frequent, 2 for the next, etc.) in map<string, int> (tokenToIndex).
Output:

Sorted Tokens: Prints the tokens in sorted order based on frequency and alphabetical tie-breaking.
Indices in Original Order: Prints the rank of each token in the original input order.
Code:
cpp
Copy code
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


Explanation: The provided code snippet reads tokens from the input, tracks their frequency, and sorts them based on frequency and alphabetical 
order. The sortByFrequency comparator function is used to define the sorting criteria. Tokens with higher frequency are ranked first, and in case 
of ties, they are sorted alphabetically. The program maintains the original order of tokens and outputs the sorted tokens along with their ranks 
in the original order. This approach ensures that tokens are sorted effectively based on the specified criteria, providing a clear and organized
output of token frequencies and ranks.


Prompt 2: Help me with mapping each frequency-sorted token to its rank in the original order in the code snippet provided.

Answer: 
The code maps frequency-sorted tokens to ranks and associates those ranks with the tokens in their original input order. Here's how it works 
step by step:

Step 1: Frequency-Sorted Tokens
After sorting the sortedTokens array by frequency and alphabetically (using sortByFrequency), the tokens are arranged from highest frequency to 
lowest. Tokens with the same frequency are ordered alphabetically.
Example of sortedTokens after sorting:
plaintext
Copy code
[("apple", 3), ("banana", 2), ("orange", 1)]
Step 2: Map Tokens to Their Rank
The code uses a map<string, int> (tokenToIndex) to assign a rank to each token based on its position in the sortedTokens array:
The most frequent token gets rank 1.
The second most frequent gets rank 2, and so on.

Code snippet:
cpp
Copy code
map<string, int> tokenToIndex;
int rank = 1;
for (int i = 0; i < sortedIndex; ++i) {
    tokenToIndex[sortedTokens[i].first] = rank++;
}
Analysis:
sortedTokens[i].first retrieves the token string.
rank++ assigns the current rank to the token and then increments the rank for the next iteration.

After this loop, tokenToIndex will look like this:
cpp
Copy code
{
    "apple": 1,
    "banana": 2,
    "orange": 3
}
Step 3: Print Ranks Based on Original Input Order
The originalTokens map keeps track of tokens in their original input order, with the position of the token as the key and the token itself as the 
value:
cpp
Copy code
{
    1: "apple",
    2: "banana",
    3: "apple",
    4: "orange",
    5: "banana",
    6: "apple"
}
To map these original tokens to their frequency-based ranks:

Iterate through originalTokens.
For each token (value in originalTokens), look up its rank in tokenToIndex.

Code snippet:
cpp
Copy code
for (const auto& pair : originalTokens) {
    cout << tokenToIndex[pair.second] << " ";
}

Analysis:
pair.second is the token in its original order.
tokenToIndex[pair.second] retrieves the rank of the token from the tokenToIndex map.
Output: The ranks of the tokens are printed in the order they originally appeared in the input.
Example Walkthrough
Input:
plaintext
Copy code
apple banana apple orange banana apple

Token Frequencies:
cpp
Copy code
{
    "apple": 3,
    "banana": 2,
    "orange": 1
}

Frequency-Sorted Tokens:
cpp
Copy code
[("apple", 3), ("banana", 2), ("orange", 1)]

Ranks Assigned:
cpp
Copy code
{
    "apple": 1,
    "banana": 2,
    "orange": 3
}

Original Tokens:
cpp
Copy code
{
    1: "apple",
    2: "banana",
    3: "apple",
    4: "orange",
    5: "banana",
    6: "apple"
}
Ranks Printed Based on Original Order:
plaintext
Copy code
1 2 1 3 2 1
Key Insight
The program maps tokens to their frequency-based rank and uses the original order of tokens to output their ranks. This combines the logical 
sorting (by frequency and alphabetically) with the sequence of the tokens as they were first read.


Explanation: The code maps each frequency-sorted token to its rank in the original order of appearance. It uses a map<string, int>
(tokenToIndex) to store the rank of each token based on its frequency-sorted position. By iterating through the originalTokens map and
retrieving the rank of each token from tokenToIndex, the code prints the ranks of tokens in the order they were originally read. This
approach ensures that each token is associated with its corresponding rank, providing a clear and structured output of token ranks based
on their original order of appearance.

*/


/* Debugging and Testing Plan: At the beginning of the debugging process, I would start by examining the input files and ensuring that they
contain the expected data. I would then run the program with the input files redirected to fstream to verify that the program reads the
input correctly. However, I realized that the autograder was not able to use fstream input, so I had to modify the program to read input from
standard input. After making the necessary changes, I would run the program with the provided input files: input1, input2, input3. This allowed me 
to check if the output matches the expected results by using: output1, output2, output3. If the output does not match the expected results, 
I would examine the code to identify any logical errors or bugs that may be causing the discrepancies. While I was creating the code, the
sort sortedTokens function was not working as expected. I realized that the sortByFrequency function was not correctly sorting the tokens
based on frequency and alphabetically. After reviewing the sortByFrequency function, I found that the comparison logic was incorrect. I
adjusted the comparison logic to correctly sort the tokens, and the sorting function worked as expected.


*/


/*
cd Project\Project 5
g++ project5.cpp
a.exe < input1.txt
*/