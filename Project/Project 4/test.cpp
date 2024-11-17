#include <vector>
#include <iostream>
using namespace std;

// /***** Write your exception class here ******* SEE main function *****/

//Exception Classes 
class duplicateInsertion : public std::exception {
    virtual const char* what() const throw() {
        return "The value is already in the tree.";
    }
};

class NotFoundException : public std::exception {
    virtual const char* what() const throw() {
        return "The value was not found in the tree.";
    }
};

template <class DT>
class MTree {
    protected:
        int M; // Maximum number of children per node (M+1 way split)
        vector<DT> values; // Values stored in the node (M-1 values)
        vector<MTree*> children; // Pointers to child MTrees (M+1 children)
    public:
        MTree(int M); // Constructor
        ~MTree(); // Destructor
        bool is_leaf() const; // Check if the current node is a leaf
        void insert(const DT& value); // Insert a value into the MTree
        void split_node(); // Split the node if it exceeds capacity
        MTree* find_child(const DT& value); // Find the correct child to follow
        bool search(const DT& value); // Search for a value in the MTree
        void remove(const DT& value); // Remove a value from the MTree
        void buildTree(const vector<DT>& input_values); // Build the MTree from a list of values
        vector<DT>& collect_values(); // Collect all values from the MTree
        bool find(DT& value); // Find a value in the MTree
};

//Constructor
template <class DT>
MTree<DT>::MTree(int M)  : M(M) {}

//Destructor
template <class DT>
MTree<DT>::~MTree(){
    for (auto child : children) {
        delete child;
    }
    children.clear();
}

//Check if the current node is a leaf
template <class DT>
bool MTree<DT>::is_leaf() const{
    return children.empty();
}

//Insert a value into the MTree
template <class DT>
void MTree<DT>::insert(const DT& value){
    if (search(value)){
        throw duplicateInsertion();
    }

    if (is_leaf()){
        if(values.size() < M - 1){
            //Insert the value in the correct position
            values.push_back(value);
            for(size_t i = 0; i < values.size(); i++){
                DT current = values[i];
                int j = i - 1;
                while(j >= 0 && values[j] > current){
                    values[j+1] = values[j];
                    j--;
                }
                values[j + 1] = current;
            }
        } 

        else {
            //Split the node if it exceeds capacity
            split_node();
            find_child(value)->insert(value);
        }
    }
    else {
        //Find the correct child to follow
        find_child(value)->insert(value);
    }
}

//Split the node if it exceeds capacity (i.e >=M)
template <class DT>
void MTree<DT>::split_node(){
    int midIndex = values.size() / 2;
    DT midValue = values[midIndex];

    MTree* newChild = new MTree(M);
    newChild->values.assign(values.begin() + midIndex + 1, values.end());
    values.resize(midIndex+1);

    if(!is_leaf()){
        newChild->children.assign(children.begin() + midIndex + 1, children.end());
        children.resize(midIndex + 1);
    }
}

//Find the correct child to follow
template <class DT>
MTree<DT>* MTree<DT>::find_child(const DT& value){
    for (int i = 0; i < values.size(); i++){
        if (value <= values[i]){
            return children[i];
        }
    }
    return children[children.size()-1];
}

//Search for a value in the MTree
template <class DT>
bool MTree<DT>::search(const DT& value){
    if(is_leaf()){
        for (const DT& v : values) {
            if (v == value) {
                return true;
            }
        }
    } else{
        return find_child(value)->search(value);
    }
    return false;
}

//Remove a value from the MTree
template <class DT>
void MTree<DT>::remove(const DT& value){
    if(!search(value)){
        throw NotFoundException();
    }

    if(is_leaf()){
         for (auto it = values.begin(); it != values.end(); ++it) {
            if (*it == value) {
                values.erase(it);
                return;
            }
        }   
    } 
    else{
        MTree* child = find_child(value);
        child->remove(value);
    }

    vector<DT> all_values = collect_values();
    buildTree(all_values);
}

//Build the MTree from a list of values
template <class DT>
void MTree<DT>::buildTree(const vector<DT>& input_values){
    for (auto& child : children) {
        delete child;
    }
    values.clear();
    children.clear();
    
    if (input_values.size() <= M - 1) {
            values = input_values;
    } else {
        int partition = input_values.size() / M;
        for (int i = 0; i < M; i++) {
            int start = partition * i;
            int end;
            if (i == M - 1) {
                end = input_values.size() - 1;
            } else {
                end = start + partition - 1;
                values.push_back(input_values[end]);
            }
            vector<DT> child_values(input_values.begin() + start, input_values.begin() + end + 1);
            MTree<DT>* child = new MTree<DT>(M);
            child->buildTree(child_values);
            children.push_back(child);
        }
    }
}

//Collect all values from the MTree
template<class DT>
vector<DT>& MTree<DT>::collect_values(){
    vector<DT>* myValues = new vector<DT>();
    if(is_leaf()){
       for(int i = 0; i < values.size(); i++){
           myValues->push_back(values[i]);
       }
    } else{
       for(auto child : children){
           vector<DT>& c_num = child->collect_values();
            myValues->insert(myValues->end(), c_num.begin(), c_num.end());
       }
   }
   return *myValues;
}

//Find a value in the MTree
template <class DT>
bool MTree<DT>::find(DT& value){
    return search(value);
}

int main() {
    int n; // Number of numbers in the initial sorted array
    int MValue;
    int numCommands;
    char command;
    int value;

    cin >> n; // Read the number of numbers in the initial sorted array
    vector<int> mySortedValues(n);
    for (int i = 0; i < n; i++) {
        cin >> mySortedValues[i];
    }

    cin >> MValue; // Read the M value
    MTree<int>* myTree = new MTree<int>(MValue);
    myTree->buildTree(mySortedValues);

    cin >> numCommands;
    // /************** Read each command Process ***************//

    for (int i = 0; i < numCommands; i++) {
        cin >> command;
        switch (command) {
            case 'I': {
                cin >> value;
                try {
                    myTree->insert(value);
                } catch (duplicateInsertion& e) {
                    cout << "The value = " << value << " already in the tree." << endl;
                }
                break;
            }

            case 'R': {
                cin >> value;
                try {
                    myTree->remove(value);
                    cout << "The value = " << value << " has been removed." << endl;
                } catch (NotFoundException& e) {
                    cout << "The value = " << value << " not found." << endl;
                }
                break;
            }

            case 'F': {
                cin >> value;
                if (myTree->find(value)) {
                    cout << "The element with value = " << value << " was found." << endl;
                } else {
                    cout << "The element with value = " << value << " not found." << endl;
                }
                break;
            }

            case 'B': {
                vector<int> myValues = myTree->collect_values();
                myTree->buildTree(myValues);
                cout << "The tree has been rebuilt." << endl;
                break;
            }

            default:
                cout << "Invalid command!" << endl;
        }
    }

    // Output the final list
    cout << "Final list: ";
    vector<int> outputValues = myTree->collect_values();
    for (size_t i = 0; i < outputValues.size(); i++) {
        cout << outputValues[i] << " ";
        if (i % 20 == 19 && i != 0) {
            cout << endl;
        }
    }
    cout << endl;

    delete myTree;
    return 0;
}

/*LLM and GitHub Copilot Usage Documentation :

Prompt 1:

Answer:

Explanation:

*/


/* Debugging and Testing Plan :

*/

/*
cd Project\Project 4
g++ test.cpp
a.exe < input1.txt
*/

