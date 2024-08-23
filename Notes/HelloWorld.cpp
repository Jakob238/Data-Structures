#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    std::cout << "Hello World\n";

    vector<string> msg{"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}