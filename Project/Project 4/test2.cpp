#include <vector>
#include <iostream>
#include <exception>
using namespace std;
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
        std::vector<DT> values; // Values stored in the node (M-1 values)
        std::vector<MTree*> children; // Pointers to child MTrees (M+1 children)
    public:
    MTree(int M);
    ~MTree();
    bool is_leaf() const;
    void insert(const DT& value);
    void split_node();
    MTree* find_child(const DT& value);
    bool search(const DT& value);
    void remove(const DT& value);
    void buildTree(const vector<DT>& input_values);
    std::vector<DT>& collect_values();
    bool find(DT& value);
};

template <class DT>
MTree<DT>::MTree(int M){
    this->M = M;
}

template <class DT>
MTree<DT>::~MTree(){
    for (int i=0; i<children.size(); i++){
        delete children[i];
    }
}

template <class DT>
bool MTree<DT>::is_leaf() const{
    return children.empty();
}

template <class DT>
void MTree<DT>::insert(const DT& value){
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

template <class DT>
void MTree<DT>::split_node(){
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

template <class DT>
MTree<DT>* MTree<DT>::find_child(const DT& value){
    for (int i = 0; i < values.size(); i++){
        if (value <= values[i]){
            return children[i];
        }
    }
    return children[children.size()-1];
}

template <class DT>
bool MTree<DT>::search(const DT& value){
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

template <class DT>
void MTree<DT>::remove(const DT& value){
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

template <class DT>
void MTree<DT>::buildTree(const vector<DT>& input_values){
    values.clear();
    for(int i = 0; i < children.size(); i++){
        delete children[i];
    }
    children.clear();
    
    if (input_values.size() <= M - 1) {
            values = input_values;
    } else {
        int D = input_values.size() / M;
        for (int i = 0; i < M; i++) {
            int start = D * i;
            int end;
            if (i == M - 1) {
                end = input_values.size() - 1;
            } else {
                end = start + D - 1;
                values.push_back(input_values[end]);
            }
            vector<DT> child_values(input_values.begin() + start, input_values.begin() + end + 1);
            MTree<DT>* child = new MTree<DT>(M);
            child->buildTree(child_values);
            children.push_back(child);
        }
    }
}

template <class DT>
bool MTree<DT>::find(DT& value){
    return search(value);
}

template<class DT>
vector<DT>& MTree<DT>::collect_values(){
    vector<DT>* myValues = new vector<DT>();
   if(is_leaf()){
       for(int i = 0; i < values.size(); i++){
           myValues->push_back(values[i]);
       }
   } else{
       for(auto child : children){
           vector<DT>& child_values = child->collect_values();
            myValues->insert(myValues->end(), child_values.begin(), child_values.end());
       }
   }
   return *myValues;
}

int main(){
    int n;
    int MValue;
    int numCommands;
    char command;
    int value;


    cin >> n;
    vector<int> mySortedValues(n);

    for(int i = 0; i < n; i++){
        cin >> mySortedValues[i];
    }

    cin >> MValue;
    MTree<int>* myTree = new MTree<int>(MValue);
    myTree->buildTree(mySortedValues);

    cin >> numCommands;

    for(int i = 0; i < numCommands; i++){
        cin >> command;
        switch(command){
            case 'I': {
                cin >> value;
                try{
                    myTree->insert(value);
                } catch(duplicateInsertion& e){
                    cout << "The value = " << value << " already in the tree." << endl;
                }
                break;
            }
            case 'R': {
                cin >> value;
                try{
                    myTree->remove(value);
                    cout << "The value = " << value << " has been removed." << endl;
                } catch(NotFoundException& e){
                    cout << "The value = " << value << " not found." << endl;
                }
                break;
            }
            case 'F': {
                cin >> value;
                if(myTree->find(value)){
                    cout << "The element with value = " << value << " was found." << endl;
                } else{
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


/*
cd Project\Project 4
g++ test2.cpp
a.exe < input1.txt
*/


