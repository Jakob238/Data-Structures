#include <iostream>
#include <vector>
using namespace std;

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

    void insert_sorted(DT value) {
        auto it = values.begin();
        while (it != values.end() && *it < value) {
            ++it;
        }
        values.insert(it, value);
    }

public:
    MTree(int M) : M(M) {}

    ~MTree() {
        for (int i = 0; i < children.size(); i++) {
            delete children[i];
        }
    }

    bool is_leaf() const { // Check if the current node is a leaf
        return children.empty();
    }

    void insert(const DT& value) {
         if (find(values.begin(), values.end(), value) != values.end()) {
            throw duplicateInsertion();
        }
        values.push_back(value);
    }

    void remove(const DT& value) {
         auto it = find(values.begin(), values.end(), value);
        if (it != values.end()) {
            values.erase(it);
        }
    }

    void split_node() {
        int midIndex = values.size() / 2;
        DT midValue = values[midIndex];

        // Create a new child node
        MTree* newChild = new MTree(M);
        for (int i = midIndex + 1; i < values.size(); ++i) {
            newChild->values.push_back(values[i]);
        }

        values.resize(midIndex); // Keep only the first half in the current node

        // If the current node is not a root, propagate the midValue to the parent
        if (!is_leaf()) {
            children.push_back(newChild);
        }
        // Otherwise, this node becomes a new root
        if (values.size() == 0) {
            values.push_back(midValue);
            children.push_back(newChild);
        } else {
            int insertIndex = 0;
            while (insertIndex < values.size() && midValue > values[insertIndex]) {
                insertIndex++;
            }
            values.insert(values.begin() + insertIndex, midValue);
            children.insert(children.begin() + insertIndex + 1, newChild);
        }
    }

    MTree* find_child(const DT& value) {
        for (int i = 0; i < values.size(); ++i) {
            if (value < values[i]) {
                return children[i];
            }
        }
        return children[values.size()]; // Last child
    }

    bool search(const DT& value) {
        for (const DT& v : values) {
            if (v == value) {
                return true;
            }
        }

        for (MTree* child : children) {
            if (child->search(value)) {
                return true;
            }
        }
        return false;
    }

    void buildTree(vector<DT>& input_values) {
        for (DT value : input_values) {
            insert(value);
        }
    }

    bool find(DT value) const {
        // Look for value in the current node
        for (const DT& v : values) {
            if (v == value) {
                return true;
            }
        }
        // Look for value in child nodes
        for (MTree* child : children) {
            if (child->find(value)) {
                return true;
            }
        }
        return false;
    }

    vector<DT>& collect_values() {
        static vector<DT> collected_values;
        collected_values.clear();

        if (is_leaf()) {
            collected_values = values;
        } else {
            for (MTree* child : children) {
                vector<DT>& child_values = child->collect_values();
                collected_values.insert(collected_values.end(), child_values.begin(), child_values.end());
            }
        }

        return collected_values;
    }
};

int main() {
    int M;
    cin >> M;

    MTree<int>* myTree = new MTree<int>(M);

    // Build the tree (dummy input for testing)
    vector<int> mySortedValues = {5, 8, 10, 13, 14, 35, 37, 40}; // Example input
    myTree->buildTree(mySortedValues);

    int numCommands;
    cin >> numCommands; // Read the number of commands

    // Read each command and process it
    for (int i = 0; i < numCommands; i++) {
        char command;
        int value;
        cin >> command; // Read the command type
        switch (command) {
        case 'I': { // Insert
            cin >> value;
            try {
                myTree->insert(value);
            } catch (duplicateInsertion& e) {
                cout << "The value = " << value << " already in the tree." << endl;
            }
            break;
        }

        case 'R': { // Remove
            cin >> value;
            try {
                myTree->remove(value);
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

        case 'B': { // Rebuild tree
            vector<int> myValues = myTree->collect_values();
            myTree->buildTree(myValues);
            break;
        }

        default:
            cout << "Invalid command!" << endl;
        }
    }

    delete myTree;
    return 0;
}

/*
cd Project\Project 4
g++ test.cpp
a.exe < input1.txt
*/