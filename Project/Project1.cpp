#include <iostream>
using namespace std;

// Structure of the SparseRow and SparseMatrix classes
class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //Values
    public:
    SparseRow(); //default constructor; row=-1;col=-1;value=0
    SparseRow(int r, int c, int v); //constructor with arguments
    void display(); // print Row#, Column#, value
    
    friend ostream& operator<<(ostream& s, const SparseRow& sr);
    
    int getRow() const;
    int getCol() const;
    int getValue() const;
    
    void setRow(int r);
    void setCol(int c);
    void setValue(int v);
};

class SparseMatrix {
    protected:
        int noRows; //#Rows of the original matrix
        int noCols; //#Columns of the original matrix
        int commonValue; //Value
        int noNonSparseValues;
        SparseRow* myMatrix; //Array
    public:
    SparseMatrix ();
    SparseMatrix (int n, int m, int cv, int nsv);
    ~SparseMatrix();

    SparseMatrix* Transpose (); //Matrix Transpose
    SparseMatrix* Multiply (SparseMatrix& M); // Matrix Multiplication
    SparseMatrix* Addition (SparseMatrix& M); // Matrix Addiiton
    
    friend ostream& operator<< (ostream& s, const SparseMatrix& sm);
    void displayMatrix (); //Displays the matrix in its original format
    void readMatrix();
};


// SparseRow methods

// Constructor
SparseRow::SparseRow(){
    row = -1;
    col = -1;
    value = 0;
}

// Constructor with arguments
SparseRow::SparseRow(int r, int c, int v){
    row = r;
    col = c;
    value = v;
}

// Display
void SparseRow::display(){
    cout << "" << row << "," << col << "," << value << endl;
}

ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << "" << sr.row << ", " << sr.col << ", " << sr.value;
    return s;
}

//Getters
int SparseRow::getRow() const {
    return row;
}
int SparseRow::getCol() const {
    return col;
}
int SparseRow::getValue() const {
    return value;
}

//Setters
void SparseRow::setRow(int r) {
    row = r;
}
void SparseRow::setCol(int c) {
    col = c;
}
void SparseRow::setValue(int v) {
    value = v;
}


// SparseMatrix methods

// Constructors
SparseMatrix::SparseMatrix(){
    noRows = 0;
    noCols = 0;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = nullptr;
}

SparseMatrix::SparseMatrix(int n, int m, int cv, int nsv){
    noRows = n;
    noCols = m;
    commonValue = cv;
    noNonSparseValues = nsv;
    if (noNonSparseValues > 0) {
        myMatrix = new SparseRow[noNonSparseValues];
    } 
    else {
        myMatrix = nullptr;
    }
}

SparseMatrix::~SparseMatrix(){    
if (myMatrix != nullptr) {
        delete[] myMatrix;
        myMatrix = nullptr; // Optional: Set to nullptr to avoid double deletion
    }
}


// fix this
void SparseMatrix::readMatrix() {
int count = 0;
for (int i = 0; i < noRows; ++i) {
    for (int j = 0; j < noCols; ++j) {
        int value;
        cin >> value;
        if (value != commonValue && count < noNonSparseValues) {
                myMatrix[count++] = SparseRow(i, j, value);
        }
    }
}
}


// Matrix Operations
SparseMatrix* SparseMatrix::Transpose (){

// Creates a new SparseMatrix with transposed dimensions
SparseMatrix* transposed = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

// Iterates through each non-zero entry in the original matrix
for (int i = 0; i < noNonSparseValues; ++i) {

    // Sets the transposed row and column to be the column and row of the original matrix
    transposed->myMatrix[i].setRow(myMatrix[i].getCol());
    transposed->myMatrix[i].setCol(myMatrix[i].getRow());
    transposed->myMatrix[i].setValue(myMatrix[i].getValue());
}
return transposed;
}


SparseMatrix* SparseMatrix::Multiply (SparseMatrix& M){

    // Check if matrix multiplication is possible
    if (noCols != M.noRows || commonValue != M.commonValue){
        cout << "Matrix multiplication is not possible" << endl;
        return nullptr;
    }
    
    // Initialize the result matrix with an estimated size
    SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, noNonSparseValues * M.noNonSparseValues);

    // Initialize a temporary result matrix to hold intermediate result
    SparseRow* tempResult = new SparseRow[noRows * M.noCols];
    int* tempCount = new int[noRows * M.noCols]();
    
    // Iterate through each non-zero entry in the first matrix
    for (int i = 0; i < noNonSparseValues; ++i) {
        int rowA = myMatrix[i].getRow();
        int colA = myMatrix[i].getCol();
        int valueA = myMatrix[i].getValue();

        // Iterate through each non-zero entry in the second matrix
        for (int j = 0; j < M.noNonSparseValues; ++j) {
            int rowB = M.myMatrix[j].getRow();
            int colB = M.myMatrix[j].getCol();
            int valueB = M.myMatrix[j].getValue();

            // Check if columns of A match rows of B
            if (colA == rowB) {
                int rowR = rowA;
                int colR = colB;
                int valueR = valueA * valueB;

                // Calculate index for the result matrix
                int index = rowR * M.noCols + colR;

                // Accumulate result values
                tempResult[index].setRow(rowR);
                tempResult[index].setCol(colR);
                tempResult[index].setValue(tempResult[index].getValue() + valueR);
                tempCount[index]++;
            }
        }
    }

    // Copy results from tempResult to the result matrix
    int count = 0;
    for (int i = 0; i < noRows * M.noCols; ++i) {
        if (tempCount[i] > 0 && tempResult[i].getValue() != commonValue) {
            if (count >= result->noNonSparseValues) {
                cout << "Error: result matrix out of bounds!" << endl;
                delete[] tempResult;
                delete[] tempCount;
                delete result;
                return nullptr;
            }
            result->myMatrix[count++] = tempResult[i];
        }
    }

    result->noNonSparseValues = count; // Update the count of non-zero entries

    delete[] tempResult;
    delete[] tempCount;

    return result;
}



SparseMatrix* SparseMatrix::Addition (SparseMatrix& M){

// Check if matrix addition is possible
if (noRows != M.noRows || noCols != M.noCols || commonValue != M.commonValue) {
    cout << "Matrix addition is not possible" << endl;
    return nullptr;
}

    // Allocate enough space in the result matrix for the worst-case scenario
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);
    result->noNonSparseValues = 0; // Initialize the count of non-sparse values

    int i = 0, j = 0;
    // Iterate through both matrices and add corresponding element
    while (i < noNonSparseValues && j < M.noNonSparseValues) {
        if (myMatrix[i].getRow() == M.myMatrix[j].getRow() && myMatrix[i].getCol() == M.myMatrix[j].getCol()) {
            int sumValue = myMatrix[i].getValue() + M.myMatrix[j].getValue();
            if (sumValue != commonValue) {
                result->myMatrix[result->noNonSparseValues++] = SparseRow(myMatrix[i].getRow(), myMatrix[i].getCol(), sumValue);
            }
            i++;
            j++;
        } 
        else if ((myMatrix[i].getRow() < M.myMatrix[j].getRow()) ||
                   (myMatrix[i].getRow() == M.myMatrix[j].getRow() && myMatrix[i].getCol() < M.myMatrix[j].getCol())) {
            result->myMatrix[result->noNonSparseValues++] = myMatrix[i++];
        } 
        else {
            result->myMatrix[result->noNonSparseValues++] = M.myMatrix[j++];
        }
    }

    // Copy the remaining elements from the matrices
    while (i < noNonSparseValues) {
        result->myMatrix[result->noNonSparseValues++] = myMatrix[i++];
    }
    while (j < M.noNonSparseValues) {
        result->myMatrix[result->noNonSparseValues++] = M.myMatrix[j++];
    }

    return result;
}


ostream& operator<< (ostream& s, const SparseMatrix& sm){
// Iterate through each non-zero entry in the matrix and output it
for(int i=0; i<sm.noNonSparseValues; i++){
    s << sm.myMatrix[i] << endl;
}
return s;
}

// Display
void SparseMatrix::displayMatrix(){

int count = 0;
// Iterate through each element of the matrix
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            // If there's a non-zero value at this position, output it
            if (count < noNonSparseValues && myMatrix[count].getRow() == i && myMatrix[count].getCol() == j) {
                cout << myMatrix[count].getValue() << " ";
                ++count;
            } 
            else{
                // Output the common value for sparse elements
                cout << commonValue << " ";
                }
        }
        cout << endl; // end line after each row
    }
}


//Main Method
int main () {

    int n, m, cv, noNSV;
    SparseMatrix* temp;
    
    //Write the Statements to read in the first matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
    firstOne->readMatrix();

    //Write the Statements to read in the second matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    secondOne->readMatrix();
    
    cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);
    cout << "After transpose" << endl;
    cout << (*(*firstOne).Transpose());
    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

    
    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);
    cout << "After transpose" << endl;
    cout << (*(*secondOne).Transpose());
    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    // just matrix not sparse matrix too
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Addition(*secondOne);
    if (temp) {
        temp->displayMatrix();
        delete temp;
    }

    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply(*secondOne);
    if (temp) {
        temp->displayMatrix();
        delete temp;
    }
    delete firstOne;
    delete secondOne;
    
    return 0;
}





/*
LLM Documentation

Prompt 1: Provide me a guide about using redirected Input, a way to send a file to the standard input of a program
without typing it using the keyboard, to visual studio code. 

Answer: Here's a step-by-step guide to setting up redirected input in Visual Studio Code (VSCode) for a C++ program that reads input from a file:
1. Create/Open a C++ Project in VSCode
Open Visual Studio Code.
If you don’t have a project already, create a new directory and open it in VSCode.
Add a new C++ file (e.g., main.cpp) where you'll write your code.
2. Write the C++ Program
3. Create an Input File
Create a text file (e.g., input.txt) in the same directory as your C++ file.
Add the input data for the matrix. 
4. Configure VSCode for Redirected Input
In VSCode, click on the Run and Debug icon on the sidebar or press Ctrl+Shift+D to open the Debug panel.
Click on create a launch.json file if you don't have one already.
In the launch.json file, you will configure your debugging settings.
Modify the args property in the launch.json file to use redirected input:

json
Copy code
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C++: g++.exe build and debug active file",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}.exe",
            "args": ["<", "input.txt"],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "C/C++: g++.exe build active file",
            "miDebuggerPath": "gdb",
            "logging": {
                "trace": true,
                "traceResponse": true,
                "engineLogging": true
            },
            "internalConsoleOptions": "openOnSessionStart"
        }
    ]
}
5. Run and Debug
Save the launch.json file.
Go back to the Debug panel and click on the green play button (Start Debugging).
The program will run, reading the matrix from input.txt, and output the result.
6. Check Output
The output should be displayed in the terminal or an external console, depending on your launch.json settings.
This setup will allow your C++ program to take input from a file, as if the user had typed it directly into the console.





Explanation: In order to use redirected input, I needed to be able to to create a configuration within the json file . 
When I finished creating a configuration for each of the input files, I was able to begin the debugging process by 
testing each of the input files. Since the instructions said that I couldn't use the keyboard to input the data, this
step was neccessary in order to complete the task.


*/



/*
Debugging

For the test, I used the following input files: input1, input2, input3, input4, and inpu5. Then, I ran the program with the 
redirected input configuration. The program was able to read the input from the files and output the results. Finally, I compared
the output with the expected results in order to verify the correctness of the program. Initially, I had some issues with the
matrix multiplication  and addition functions, but I was able to resolve them by carefully checking the logic and making the necessary 
corrections. For both matrix multiplication and addition, I needed to add a condition to check if that Check if matrix multiplication/addition
is possible. If it is not possible, the program should output a message and return nullptr. This helped to handle cases where the
matrices were not compatible for multiplication or addition. After making these changes, the program was able to handle all test cases.


*/