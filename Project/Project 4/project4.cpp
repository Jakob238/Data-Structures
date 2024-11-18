#include <vector>
#include <iostream>
using namespace std;

// /***** Write your exception class here ******* SEE main function *****/

// Duplicate Insertion Exception - For when a value is already in the tree
class duplicateInsertion : public std::exception {
    virtual const char* what() const throw() {
        return "The value is already in the tree.";
    }
};

// Not Found Exception - For when a value is not found in the tree
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
        bool is_leaf() const; // Checks if the current node is a leaf node
        void insert(const DT& value); // Insert a value into the MTree
        void split_node(); // Split the node if it exceeds capacity (i.e., >=M)
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


// /************** Implement the MTree Methods here ***************//

//Is Leaf - Checks if the current node is a leaf
template <class DT>
bool MTree<DT>::is_leaf() const{
    return children.empty();
}

//Insert - Inserts a value into the MTree
template <class DT>
void MTree<DT>::insert(const DT& value){
    if (search(value)){
        throw duplicateInsertion();
    }

    // Checks if the current node is a leaf and inserts the value
    if (is_leaf()){
        if(values.size() < M - 1){
            //Insert the value in the correct position
            values.push_back(value);
            // Sort the values directly by shifting values and inserting in order
            for (int i = values.size() - 1; i > 0; i--) {
                if (values[i] < values[i - 1]) {
                    swap(values[i], values[i - 1]);
                } else {
                    break;
                }
            }
        } 
        //Splits the node if it exceeds capacity
        else {
            split_node();
            // After splitting, insert into the correct child node
            find_child(value)->insert(value);
        }
    }
    
    // If it is not a leaf, it inserts the value in the correct child node
    else {
        find_child(value)->insert(value);
    }
}

//Split Node - Splits the node if it exceeds capacity (i.e >=M)
template <class DT>
void MTree<DT>::split_node(){
    //Find the middle value of the node and create a new child
    int midIndex = values.size() / 2;
    DT midValue = values[midIndex];
    MTree* newChild = new MTree(M);

    //Assign the middle value to the current node
    newChild->values.assign(values.begin() + midIndex + 1, values.end());
    values.resize(midIndex);
    //If the node is not a leaf, assign the children to the new child
    if(!is_leaf()){
        newChild->children.assign(children.begin() + midIndex + 1, children.end());
        children.resize(midIndex + 1);
    }
}

//Find Child - Finds the correct child to follow
template <class DT>
MTree<DT>* MTree<DT>::find_child(const DT& value){
    for (int i = 0; i < values.size(); i++){
        if (value <= values[i]){
            return children[i];
        }
    }
    return children[children.size()-1];
}

//Search - Searches for a value in the MTree
template <class DT>
bool MTree<DT>::search(const DT& value){
    //Check if the current node is a leaf and search for the value in the values
    if(is_leaf()){
        for (const DT& v : values) {
            if (v == value) {
                return true;
            }
        }
    } 
    //If the current node is not a leaf, find the child and recursively search
    else{
        return find_child(value)->search(value);
    }
    return false;
}

//Remove - Remove a value from the MTree
template <class DT>
void MTree<DT>::remove(const DT& value){
    // if the value cannot be found in the tree, throw an exception
    if(!search(value)){
        throw NotFoundException();
    }

    // if the current node is a leaf, remove the value from the values
    if(is_leaf()){
         for (auto it = values.begin(); it != values.end(); ++it) {
            if (*it == value) {
                values.erase(it);
                return;
            }
        }   
    } 
    // if the current node is not a leaf, find the child and recursively remove from there
    else{
        MTree* child = find_child(value);
        child->remove(value);
    }

    vector<DT> every_value = collect_values();
    buildTree(every_value);
}

//Build Tree - Build the MTree from a list of values
template <class DT>
void MTree<DT>::buildTree(const vector<DT>& input_values){
    // Delete all children and clear the values
    for (auto& child : children) {
        delete child;
    }
    values.clear();
    children.clear();
    
    // if the input values are less than or equal to M-1, assign the values to the current node
    if (input_values.size() <= M - 1) {
            values = input_values;
    } 
    // if the input values are greater than M-1, split the values into M children
    else {
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

//Collect Values - Collect all values from the MTree
template<class DT>
vector<DT>& MTree<DT>::collect_values(){
    // create a vector to store the values
    vector<DT>* collectedValues = new vector<DT>();

    // if the current node is a leaf, return the values
    if(is_leaf()){
       for(int i = 0; i < values.size(); i++){
           collectedValues->push_back(values[i]);
       }
    } 
    // if the current node is not a leaf, collect the values from the children
    else{
       for(auto child : children){
            vector<DT>& c_num = child->collect_values();
            collectedValues->insert(collectedValues->end(), c_num.begin(), c_num.end());
       }
   }
   return *collectedValues;
}

//Find - Find a value in the MTree
template <class DT>
bool MTree<DT>::find(DT& value){
    return search(value);
}



int main() {
    int n; // Number of numbers in the initial sorted array
    int MValue; // M value
    int numCommands; // Number of commands to process
    char command; // Command to process
    int value; // Value to process

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
            case 'I': { // Insert
                cin >> value;
                try {
                    myTree->insert(value);
                    cout << "The value = " << value << " has been inserted." << endl;
                } catch (duplicateInsertion& e) {
                    cout << "The value = " << value << " already in the tree." << endl;
                }
                break;
            }

            case 'R': { // Remove
                cin >> value;
                try {
                    myTree->remove(value);
                    cout << "The value = " << value << " has been removed." << endl;
                } catch (NotFoundException& e) {
                    cout << "The value = " << value << " not found." << endl;
                }
                break;
            }

            case 'F': { // Find
                cin >> value;
                if (myTree->find(value)) {
                    cout << "The element with value = " << value << " was found." << endl;
                } else {
                    cout << "The element with value = " << value << " not found." << endl;
                }
                break;
            }

            case 'B': { // Rebuild
                vector<int> myValues = myTree->collect_values();
                myTree->buildTree(myValues);
                cout << "The tree has been rebuilt." << endl;
                break;
            }

            default: // Invalid command
                cout << "Invalid command!" << endl;
        }
    }

    // Final list - Collect all values from the MTree and display
    cout << "Final list: ";
    vector<int> outputValues = myTree->collect_values();
    for (size_t i = 0; i < outputValues.size(); i++) {
        cout << outputValues[i] << " ";
        if (i % 20 == 19 && i != 0) {
            cout << endl;
        }
    }
    cout << endl;

    // Clean up memory
    delete myTree;
    return 0;
}





/*LLM and GitHub Copilot Usage Documentation :

Prompt 1: Using the code that I have already provided, fix my collect_values function so that it correctly collects all values from the MTree.

Answer:The collect_values method you implemented is designed to gather and return all the values in the M-Tree as a single list. 
It works by traversing through the M-Tree recursively. Here's how it works:
If the current node is a leaf, it simply gathers all values stored at that node and returns them in a vector.
If the current node is not a leaf, it calls collect_values() on each child node, collecting values from those child nodes and 
merging them into a single vector. The method ultimately returns a vector of values containing all elements present in the M-Tree.
Here's a collect_values method:

cpp
Copy code
// Collect all values from the MTree
template<class DT>
vector<DT>& MTree<DT>::collect_values(){
    // if the current node is a leaf, return the values and if not, collect the values from the children
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
Description of the Code:
Leaf Node Handling: If the node is a leaf (checked using is_leaf()), the method adds each value in the node to a 
vector<DT>* myValues and returns it.Non-Leaf Node Handling: If the node is not a leaf, the method collects values from each 
child node by calling their collect_values() methods recursively. It merges these values into the vector myValues using insert().
Memory Management: myValues is dynamically allocated. Be cautious, as this approach may introduce potential memory management issues 
if not handled properly. Consider using vector<DT> as a stack-allocated object to avoid new and delete.

Explanation: The collect_values method is a crucial function in the MTree class, responsible for gathering all values stored in the MTree.
The method recursively traverses the MTree, collecting values from leaf nodes and children nodes. By correctly handling leaf and non-leaf nodes,
the collect_values function ensures that all values are correctly gathered and returned in a single vector. If the node is a leaf node, the method
simply adds the values to the vector. If the node is not a leaf, it recursively calls collect_values on each child node and merges the values into
the vector. This approach ensures that all values in the MTree are collected and returned in a single vector. In order to reduce runtime,
the method uses a vector<DT> to store the values and merges them from child nodes. This implementation provides an
efficient way to collect values from the MTree, maintaining the structure  of the tree.


Prompt 2: Using the code that I have already provided, create a insert function that removes a value from the MTree.

Answer: Here's a breakdown of the insert method and how it works within the MTree class: The insert method is designed to add a 
value to an M-ary tree (specifically, a tree that splits nodes when it exceeds a certain number of values). Here’s how it works:
Duplicate Check:
The method starts by checking if the value already exists in the tree using the search method. If it does, it throws a 
duplicateInsertion exception to signal that duplicate values are not allowed. Leaf Node Insertion: If the current node is a leaf node 
(checked using the is_leaf method), it proceeds to insert the value directly into the values vector. The value is inserted in sorted order 
using an insertion sort approach, where it finds the correct position and shifts other values as needed.
Splitting the Node:

If adding the value causes the node to exceed its capacity (defined as M-1 values), the node is split using the split_node method. 
The middle value becomes the new pivot, and the excess values are distributed to a new child node.
After splitting, the insert method recursively calls itself on the appropriate child node.
Internal Node Insertion:

If the node is not a leaf, the method finds the correct child node using find_child and recursively calls insert on that child.
insert Method Implementation
cpp
Copy code
// Insert a value into the MTree
template <class DT>
void MTree<DT>::insert(const DT& value){
    // Check for duplicates
    if (search(value)){
        throw duplicateInsertion();
    }

    // Insert into a leaf node
    if (is_leaf()){
        if (values.size() < M - 1){
            // Insert the value in sorted order
            values.push_back(value);
            for (size_t i = 0; i < values.size(); i++){
                DT current = values[i];
                int j = i - 1;
                while (j >= 0 && values[j] > current){
                    values[j + 1] = values[j];
                    j--;
                }
                values[j + 1] = current;
            }
        } else {
            // Split the node if it exceeds capacity
            split_node();
            find_child(value)->insert(value);
        }
    } else {
        // Insert into the correct child node
        find_child(value)->insert(value);
    }
}
Key Points
Duplicate Handling: The insert method ensures no duplicate values by leveraging the search method.
Leaf Node Handling: Values are inserted directly if the node is a leaf and has space.
Node Splitting: When the capacity is reached, split_node is invoked to maintain the structure of the tree.
Recursion in Non-Leaf Nodes: If the node is not a leaf, it identifies the appropriate child using find_child and recursively inserts the value there.


Explanation: The insert method is a important operation in the MTree class, which is responsible for adding values to 
the tree while maintaining its structure and order. By handling duplicates, leaf and non-leaf nodes, and node splitting, 
the insert method ensures that values are correctly inserted into the MTree. Node splitting is a key feature that allows
the tree to grow and maintain its balance. This is necessary to prevent nodes from becoming too large and to maintain the
M tree properties. This method inserts values in sorted order, ensuring that the tree remains balanced and efficient.
Recursion is used to navigate through the tree and insert values at the appropriate location. This implementation provides an 
effective way to add values to the tree while being memory-efficient and maintaining the structure of the tree structure. 
 


Prompt 3: Using the code that I have already provided, fix my collect values function so that it correctly collects all values from the MTree.

Answer:The remove method is responsible for deleting a value from the M-Tree (a multi-way tree with up to M children per node and M-1 values 
per node). Here’s a step-by-step breakdown of how the method works: Search for the Value: The first thing the remove method does is 
check if the value exists in the tree by calling the search method. If the value is not found, it throws the NotFoundException exception.

cpp
Copy code
if (!search(value)) {
    throw NotFoundException();
}
Handling Leaf Nodes: If the current node is a leaf (i.e., it has no children), the method proceeds to remove the value directly from the values vector of the node.

cpp
Copy code
if (is_leaf()) {
    for (auto it = values.begin(); it != values.end(); ++it) {
        if (*it == value) {
            values.erase(it);
            return;
        }
    }
}
The method searches through the values vector in the current node. Once it finds the matching value, it removes it using erase and 
returns immediately. If the value is not found in the current node's values, it will continue to search in the children.
Handling Non-Leaf Nodes: If the current node is not a leaf, the method finds the correct child node that contains the value using 
the find_child method and recursively calls remove on that child.

cpp
Copy code
else {
    MTree* child = find_child(value);
    child->remove(value);
}
Rebuilding the Tree: After the value is removed, the tree is rebuilt by collecting all the values in the tree using the collect_values 
method and then calling the buildTree method to reconstruct the tree.

cpp
Copy code
vector<DT> all_values = collect_values();
buildTree(all_values);
collect_values is a method that traverses the tree and collects all values in a vector.
buildTree takes this sorted list of values and rebuilds the tree from scratch. This ensures that the tree is always in a balanced 
state after a modification.
remove Method Code:
cpp
Copy code
// Remove a value from the MTree
template <class DT>
void MTree<DT>::remove(const DT& value) {
    // If the value cannot be found in the tree, throw an exception
    if (!search(value)) {
        throw NotFoundException();
    }

    // If the current node is a leaf, remove the value from the values
    if (is_leaf()) {
        for (auto it = values.begin(); it != values.end(); ++it) {
            if (*it == value) {
                values.erase(it);
                return;
            }
        }
    } else {
        // Otherwise, find the child and recursively remove from there
        MTree* child = find_child(value);
        child->remove(value);
    }

    // Collect all values from the tree and rebuild it
    vector<DT> all_values = collect_values();
    buildTree(all_values);
}
Key Points:
The method works recursively, ensuring that the correct child node is searched and modified when the tree is not a leaf.
After removing the value, the method rebuilds the tree to maintain its structure and balance.
Exceptions are used to handle cases where the value doesn't exist in the tree (NotFoundException) or when a duplicate insertion is attempted.

Explanation: The remove method is a critical operation in the MTree class, responsible for deleting values from the tree while maintaining 
its structure and balance. By handling leaf and non-leaf nodes, searching for the correct child node, and rebuilding the tree after removal, 
the remove method ensures that the MTree remains consistent and correctly reflects the modifications made. The use of exceptions for error
handling adds robustness to the method, allowing for clear and concise error reporting when values are not found or when duplicate insertions 
are attempted. This implementation provides an efficient and effective way to remove values from the MTree, ensuring that the tree remains 
balanced and maintains its integrity throughout the removal process. The remove method is a key component in managing the contents of the MTree 
and ensuring its correctness and consistency.

*/


/* Debugging and Testing Plan :
For the test, I used the following input files: input1, input2, and input3. Then, I ran the program with the 
redirected input configuration. The program was able to read the input from the files and output the results. 
Finally, I compared the outputs with the expected results: output1, output2, and output3. By using this method of 
testing the  program, it allowed me to verify the methods and functions in the program. Furthermore, I compared the
outputs with the expected results to ensure that the program was functioning correctly. Through this method, I was
able to identify and fix any issues in the program. For exmaple, I had issues with the collect_values function not
correctly collecting all values from the MTree. After identifying the issue, I was able to understand that I was not
properly collecting the values from the children. By revising the collect_values function to correctly collect the
values from the children, I was able to resolve the issue. As another example, I had issues with the remove function for
input1 and input 3 because the value was being found after it had already been removed. After identifying the issue, I
was able to correct the logic in the remove function to ensure that the value was removed before checking if it was found.
By making these changes, I was able to verify that the program was functioning correctly through following the debugging
and testing plan.
*/



/*
cd Project\Project 4
g++ project4.cpp
a.exe < input1.txt
*/
