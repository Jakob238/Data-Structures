#include <vector>
#include <iostream>
using namespace std;

// /***** Write your exception class here ******* SEE main function *****/

class duplicateInsertion : public exception {
public:
    const char* what() const noexcept override {
        return "The value is already in the tree.";
    }
};

class NotFoundException : public exception {
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
            for (int i = 0; i < children.size(); i++) {
                delete children[i];
            }
        }
        bool is_leaf() const{ // Check if the current node is a leaf
            return children.size() == 0;
        } 
        void insert(const DT& value); // Insert a value into the MTree
        void split_node(); // Split the node if it exceeds capacity (i.e >=M)
        MTree* find_child(const DT& value); // Find the correct child to follow
        bool search(const DT& value){ // Search for a value in the MTree
            return false;
        } 
        void remove(const DT& value); // Delete a value from the MTree
        void buildTree(vector<DT>& input_values); // Build the tree
        vector<DT>& collect_values(); // Collect values from all leaf nodes
        bool find (DT& value) const {
            if (std::find(values.begin(), values.end(), value) != values.end()) {
                return true;
            }
            for (const auto& child : children) {
                if (child->find(value)) {
                    return true;
                }
            }
            return false;
        }
};




int main() {
    int n; // number of numbers in the initial sorted array
    int MValue;
    int numCommands;
    char command;
    int value;
    vector<int> mySortedValues(n);
    //read n numbers from the input and add them to the vector mySortedValues
    //Get the M value
    int M;
    cin >> M;
    MTree<int>* myTree = new MTree<int>(M);
    //Build the tree
    (*myTree).buildTree (mySortedValues);
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
                cout << "The value = " << value <<  " already in the tree. ";
                cout << endl;
            }
            break;
        }
        
        case 'R': { // Remove
            cin >> value;
            try {
                (*myTree).remove(value);
            }
            catch (NotFoundException& e) {
                cout << "The value = " << value << " not found.";
            }
            break;
        }
        case 'F': { // Find
            cin >> value;
            if ((*myTree).find(value))
                cout << "The element with value = " << value << " was found.";
            else
                cout << "The element with value = " << value << " not found.";
            cout << endl;
            break;
        }
        case 'B': { // rebuild tree
            vector<int> myValues = (*myTree).collect_values();
            (*myTree).buildTree (myValues);
            break;
        }
        default:
            cout << "Invalid command!" << endl;
        }
    }
    delete myTree;
    return 0;
}





