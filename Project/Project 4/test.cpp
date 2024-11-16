#include <vector>
#include <iostream>
using namespace std;

// /***** Write your exception class here ******* SEE main function *****/

class duplicateInsertion : public std::exception {
public:
    const char* what() const noexcept override {
        return "The value is already in the tree.";
    }
};



class NotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "The value was not found in the tree.";
    }
};


template <typename DT>
class MTree {
    protected:
        int M; // Maximum number of children per node (M+1 way split)
        vector<DT> values; // Values stored in the node (M-1 values)
        vector<MTree*> children; // Pointers to child MTrees (M+1 children)

    public:
        MTree(int M){
            this->M = M;
        }

        ~MTree(){
            for (auto child : children) {
                delete child;
            }
            children.clear();
        }

        bool is_leaf() const{ // Check if the current node is a leaf
            return children.empty();
        } 

        void insert(const DT& value){ // Insert a value into the MTree
            
            if (search(value)){
                throw duplicateInsertion();
            }
            if (is_leaf()){
                
                if(values.size() < M-1){
                    values.push_back(value);
                    for(size_t i = 0; i < values.size(); i++){
                        DT key = values[i];
                        int j = i - 1;
                        while(j >= 0 && values[j] > key){
                            values[j+1] = values[j];
                            j = j - 1;
                        }
                        values[j+1] = key;
                    }
                } 
                else {
                    split_node();
                    find_child(value)->insert(value);
                }
            }
            else{
                find_child(value)->insert(value);
            }
        }

        void split_node(){ // Split the node if it exceeds capacity (i.e >=M)
            int mid = values.size() / 2;
            DT midValue = values[mid];

            MTree* newNode = new MTree(M);
            newNode->values.assign(values.begin() + mid + 1, values.end());
            values.resize(mid+1);

            if(!is_leaf()){
                newNode->children.assign(children.begin() + mid + 1, children.end());
                children.resize(mid + 1);
            }
        } 

        MTree* find_child(const DT& value){  // Find the correct child to follow
            for (int i = 0; i < values.size(); ++i) {
                if (value < values[i]) {
                    return children[i];
                }
            }
            return children[children.size()-1]; // Last child
        }

        bool search(const DT& value) { // Search for a value in the MTree  
            if(is_leaf()){
                for(int i=0; i< values.size(); i++){
                    if(values[i] == value){
                        return true;
                    }
                }
            } else{
                return find_child(value)->search(value);
            }
            return false;
        } 

        void remove(const DT& value){ // Delete a value from the MTree
            if(!search(value)){
                throw NotFoundException();
            }

            if(is_leaf()){
                for (int i =0; i < values.size(); i++){
                    if(values[i] == value){
                        values.erase(values.begin() + i);
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
        void buildTree(vector<DT>& input_values){ // Build the tree from a sorted vector of values
            for(int i = 0; i < children.size(); i++){
                delete children[i];
            }
            values.clear();
            children.clear();

            if (input_values.size() <= M - 1) {
                    values = input_values;
            } else {
                int D = input_values.size() / M;
                for (int i = 0; i < M; i++) {
                    int start = D * i;
                    //cout << "start: " << start << " - ";
                    int end;
                    if (i == M - 1) {
                        end = input_values.size() - 1;
                        //cout << "end: " << end << endl;
                    } else {
                        end = start + D - 1;
                        //cout << "end: " << end << endl;
                        values.push_back(input_values[end]);
                    }
                    vector<DT> child_values(input_values.begin() + start, input_values.begin() + end + 1);
                    MTree<DT>* child = new MTree<DT>(M);
                    child->buildTree(child_values);
                    children.push_back(child);
        }
    }
        } 
        vector<DT>& collect_values(){  // Collect values from all leaf nodes
            vector<DT>* myValues = new vector<DT>();
            for(auto value : values){
                myValues->push_back(value);
            }
            for(auto child : children){
                for(auto child : children){
                    vector<DT>& child_values = child->collect_values();
                    myValues->insert(myValues->end(), child_values.begin(), child_values.end());
                }
            }
            return *myValues;

        }

        bool find (DT& value) {
            return search(value);
        }

};




int main() {
    int n; // number of numbers in the initial sorted array
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
    (*myTree).buildTree(mySortedValues);

    cin >> numCommands; // Read the number of commands   
    // /************** Read each command Process ***************//
    
    for (int i = 0; i < numCommands; i++) {
        cin >> command; // Read the command type
        switch (command) {
        case 'I': { // Insert
            cin >> value;
            try {
                (*myTree).insert(value);
            }
            catch (duplicateInsertion& e) {
                cout << "The value = " << value <<  " already in the tree." << endl;
                // continue;
            }
            break;
        }
        
        case 'R': { // Remove
            cin >> value;
            try {
                (*myTree).remove(value);
                cout << "The value = " << value << " has been removed." << endl;
            }
            catch (NotFoundException& e) {
                cout << "The value = " << value << " not found." << endl;
            }
            break;
        }
        case 'F': { // Find
            cin >> value;
            if ((*myTree).find(value)){
                cout << "The element with value = " << value << " was found."<< endl;
            } else{
                cout << "The element with value = " << value << " not found."<< endl;
            }
            break;
        }
        case 'B': { // rebuild tree
            vector<int> myValues = (*myTree).collect_values();
            (*myTree).buildTree (myValues);
            cout << "The tree has been rebuilt." << endl;
            break;
        }
        default:
            cout << "Invalid command!" << endl;
        }
    }

    cout << "Final list: ";
    vector<int> allValuesOut = myTree->collect_values();
    for(int i=0; i<allValuesOut.size(); i++){
        cout << allValuesOut[i] << " ";
        if(i % 20 == 19 && i != 0){
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

