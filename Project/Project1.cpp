#include <iostream>
#include <fstream>
using namespace std;

// Structure of the SparseRow and SparseMatrix classes
class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //Value
    public:
    SparseRow(); //Default Constructor
    SparseRow(int r, int c, int v); //Constructor with Arguments
    void display(); // Print Row#, Column#, value
    
    friend ostream& operator<<(ostream& s, const SparseRow& sr);
    
    // Getters and Setters
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
        int commonValue; //Common Value
        int noNonSparseValues; //#Non-sparse values
        SparseRow* myMatrix; //Matrix
    public:
    SparseMatrix (); // default constructor
    SparseMatrix (int n, int m, int cv, int nsv); // Constructor with Arguments
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
    cout << row << "," << col << "," << value << endl;
}

ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << sr.row << ", " << sr.col << ", " << sr.value;
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



// SparseMatrix Methods

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

// Deconstructor
SparseMatrix::~SparseMatrix(){    
    if (myMatrix != nullptr) {
        delete[] myMatrix;
        myMatrix = nullptr; 
    }
}

// Read Matrix
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

// Transpose Matrix
SparseMatrix* SparseMatrix::Transpose (){
    // Creates a new SparseMatrix with transposed dimensions
    SparseMatrix* result = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

    // Iterates through each non-zero entry in the original matrix
    for (int i = 0; i < noNonSparseValues; ++i) {
        // Sets the transposed row and column to be the column and row of the original matrix
        result->myMatrix[i].setRow(myMatrix[i].getCol());
        result->myMatrix[i].setCol(myMatrix[i].getRow());
        result->myMatrix[i].setValue(myMatrix[i].getValue());
    }
    return result;
}

// Multiply Matrix
SparseMatrix* SparseMatrix::Multiply (SparseMatrix& M){
    // Check if matrix multiplication is possible
    if (noCols != M.noRows || commonValue != M.commonValue){
        cout << "Matrix multiplication is not possible" << endl;
        return nullptr;
    }
    
    // Initialize the result matrix with an estimated size
    SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, noNonSparseValues * M.noNonSparseValues);

    // Initialize a temporary result matrix to hold intermediate results
    SparseRow* tempResult = new SparseRow[noRows * M.noCols];
    int* tempCount = new int[noRows * M.noCols]();
    
    // Iterates through each non-sparse entry in the first matrix
    for (int i = 0; i < noNonSparseValues; ++i) {
        int row1 = myMatrix[i].getRow();
        int col1 = myMatrix[i].getCol();
        int value1 = myMatrix[i].getValue();

        // Iterates through each non-sparse entry in the second matrix
        for (int j = 0; j < M.noNonSparseValues; ++j) {
            int row2 = M.myMatrix[j].getRow();
            int col2 = M.myMatrix[j].getCol();
            int value2 = M.myMatrix[j].getValue();

            // Checks if the columns of matrix 1 match rows of matrix 2
            if (col1 == row2) {
                int rowRes = row1;
                int colRes = col2;
                int valueRes = value1 * value2;

                // Calculates index for the result matrix by using number of columns, temp row, and temp column.
                // This finds the position/ index of the resulting element in the temporary tempResult array.
                int index = rowRes * M.noCols + colRes;

                // Accumulate result values
                tempResult[index].setRow(rowRes);
                tempResult[index].setCol(colRes);
                tempResult[index].setValue(tempResult[index].getValue() + valueRes);
                tempCount[index]++;
            }
        }
    }

    // Copies results from tempResult to the result matrix
    int count = 0;
    for (int i = 0; i < noRows * M.noCols; ++i) {
        if (tempCount[i] > 0 && tempResult[i].getValue() != commonValue) {
            if (count >= result->noNonSparseValues) {
                cout << "Matrix multiplication is not possible" << endl;
                delete[] tempResult;
                delete[] tempCount;
                delete result;
                return nullptr;
            }
            result->myMatrix[count++] = tempResult[i];
        }
    }

    // Sets the count of non-sparse values in the result matrix
    result->noNonSparseValues = count; 

    // Finally, it is neccessary to delete the temporary arrays
    delete[] tempResult;
    delete[] tempCount;

    return result;
}

// Addition Matrix
SparseMatrix* SparseMatrix::Addition (SparseMatrix& M){
    // Checks if matrix addition is possible
    if (noRows != M.noRows || noCols != M.noCols || commonValue != M.commonValue) {
        cout << "Matrix addition is not possible" << endl;
        return nullptr;
    }

    // Allocates enough space in the result matrix for each scenario
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);
    result->noNonSparseValues = 0; // Initializes the count of non-sparse values

    int i = 0, j = 0;

    // Iterates through both matrices as long as there are non-zero elements
    // Additionally, it adds a corresponding element
    while (i < noNonSparseValues && j < M.noNonSparseValues) {
        if (myMatrix[i].getRow() == M.myMatrix[j].getRow() && myMatrix[i].getCol() == M.myMatrix[j].getCol()) {
            int sum = myMatrix[i].getValue() + M.myMatrix[j].getValue();
            if (sum != commonValue) {
                result->myMatrix[result->noNonSparseValues++] = SparseRow(myMatrix[i].getRow(), myMatrix[i].getCol(), sum);
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

    // Copies the remaining elements from the matrices
    while (i < noNonSparseValues) {
        result->myMatrix[result->noNonSparseValues++] = myMatrix[i++];
    }
    while (j < M.noNonSparseValues) {
        result->myMatrix[result->noNonSparseValues++] = M.myMatrix[j++];
    }

    return result;
}

ostream& operator<< (ostream& s, const SparseMatrix& sm){
    // Iterates through each non-zero entry in the matrix and output it
    for(int i=0; i<sm.noNonSparseValues; i++){
        s << sm.myMatrix[i] << endl;
    }
    return s;
}

// Display
void SparseMatrix::displayMatrix(){
    int count = 0;
    // Iterates through each element of the matrix
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            // If there's a non-zero value at this position, it will output it
            if (count < noNonSparseValues && myMatrix[count].getRow() == i && myMatrix[count].getCol() == j) {
                cout << myMatrix[count].getValue() << " ";
                ++count;
            } 
            else{
                // Output the common value for sparse elements
                cout << commonValue << " ";
                }
        }
        cout << endl; // Ensures that there is an end line after each row
    }
}


//Main Method
int main () {

    int n, m, cv, noNSV;
    SparseMatrix* temp;
    
    //Writes the Statements to read in the first matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
    firstOne->readMatrix();

    //Writes the Statements to read in the second matrix
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


    cout << "Matrix addition result" << endl;
    temp = firstOne->Addition(*secondOne);
    if (temp) {
        temp->displayMatrix();
        delete temp;
    }

    cout << "Matrix multiplication result" << endl;
    temp = firstOne->Multiply(*secondOne);
    if (temp) {
        temp->displayMatrix();
        delete temp;
    }

    cout.flush();

    delete firstOne;
    delete secondOne;
    
    return 0;
}





/*
LLM Documentation

Prompt 1: Provide me a guide about using redirected Input, a way to send a file to the standard input of a program
without typing it using the keyboard, to visual studio code.  
(NOTE THIS IS NOT THE CORRECT WAY TO DO IT, I USED THE VIDEO LECTURE LATER)

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


My Explanation: In order to use redirected input, I needed to be able to to create a configuration within the json file . 
When I finished creating a configuration for each of the input files, I was able to begin the debugging process by 
testing each of the input files. Since the instructions said that I couldn't use the keyboard to input the data, this
step was neccessary in order to complete the task. However, I later learned that this was not the correct way to do file redirection, 
so I watched the video lecture in order to learn the correct way to do it.



Prompt 2: Explain the purpose of a ostream operator and explain how a ostream& operator<< (ostream& s, const SparseRow); would work

Answer: The ostream operator (<<) is used to print objects of custom classes in a human-readable format. 
When overloading the << operator for a class, you're enabling the class to be output using cout (or any other output stream), 
just like built-in types such as integers and strings. By defining this operator for custom classes, you allow users to 
print objects with a simple and intuitive syntax like:
SparseRow myRow(1, 2, 100);
cout << myRow;  // Instead of writing a custom print function
Without overloading the << operator, the default behavior would not print meaningful information about the object. 
Instead, it might print a memory address or cause a compilation error.
The function ostream& operator<< (ostream& s, const SparseRow& row) is responsible for defining how a SparseRow object 
is printed to an output stream (such as cout or a file stream). Let's break down how this function works and how it's structured.
1. Function Signature
ostream& operator<< (ostream& s, const SparseRow& row);
ostream& is the return type because the function returns a reference to the output stream. This allows chaining of 
multiple output operations like cout << obj1 << obj2;.
ostream& s is the output stream (e.g., cout, file stream, etc.) to which the information about the object will be written.
const SparseRow& row is a constant reference to the SparseRow object that we want to output. The const keyword ensures 
that the object is not modified during the output process.
2. Printing the SparseRow Object
In the implementation of the operator<<, you define what specific information about the SparseRow object is written to the output stream.
A SparseRow object typically has three properties: row, col, and value. The goal is to print this information 
in a readable format, such as:
Copy code
Row: 1, Col: 2, Value: 100
The implementation might look like this:

ostream& operator<< (ostream& s, const SparseRow& row) {
    // Print the row number, column number, and value in a formatted way
    s << "Row: " << row.getRow() << ", Col: " << row.getCol() << ", Value: " << row.getValue();
    return s;
}

3. Detailed Breakdown of the Code
Accessing the Data:
row.getRow(), row.getCol(), and row.getValue() are used to retrieve the private members of the SparseRow object. 
These are getter functions that you would need to define in the SparseRow class.
Stream Insertion:
The << operator is used multiple times to insert text into the stream (s). For example, s << "Row: " writes 
the string "Row: " to the stream, and s << row.getRow() writes the row number.
Chaining:
After each insertion into the stream, the function returns s, which allows for chaining multiple << operators in a single line like:
cout << row1 << row2 << row3;
Returning the Stream:
The function returns s, allowing the operator to be used in an expression like cout << row1;.
Example Usage
Suppose you have a SparseRow object like this:
SparseRow row(1, 2, 100);  // Row 1, Col 2, Value 100
When you run:
cout << row << endl;
The output might be:
Row: 1, Col: 2, Value: 100
This allows for an intuitive and simple way to output the contents of a SparseRow object.
Summary of Key Points:
The ostream& operator<< allows you to define custom output behavior for your class.
It makes printing objects of that class straightforward and consistent with built-in types.
For SparseRow, the function retrieves the row, column, and value, and prints them in a clear format.
Returning the stream enables chaining, allowing multiple output operations in a single statement.
This approach enhances code readability and makes debugging and logging more convenient, as you can easily inspect the 
values stored in your objects.


My Explanation: For the project, it was necessary to learn the purpose of a ostream operator and how it works in order to complete the 
project in a readable format. It is important to overload the << operator in order to print the SparseRow and SparseMatrix objects. 
After understanding the purpose of the ostream operator, I was able to implement the operator in both of the classes in order to
print the objects in the correct format.


Prompt 3: Explain how to create a multiply function using the signature SparseMatrix* SparseMatrix::Multiply (SparseMatrix& M.

Answer: The Multiply function in the SparseMatrix class is designed to perform matrix multiplication between the current matrix and 
another sparse matrix M. Here’s a detailed breakdown of how it works:
Pre-checks for Multiplication Validity: The function first ensures that matrix multiplication is feasible by checking if the number 
of columns in the current matrix (noCols) matches the number of rows in matrix M (M.noRows). It also verifies that both matrices 
share the same value type (commonValue). If these conditions are not met, it prints an error message and returns nullptr, indicating 
that multiplication cannot be performed.
Initialization: If multiplication is possible, the function initializes a new SparseMatrix object named result with the appropriate 
dimensions (noRows of the current matrix and noCols of M). The estimated size for the result matrix is set based on the product of the
non-zero entries of both matrices (noNonSparseValues * M.noNonSparseValues). Additionally, temporary arrays are created: tempResult to
hold intermediate results and tempCount to keep track of the number of occurrences for each position in the result matrix.
Matrix Multiplication Logic: The function iterates over each non-zero entry of the current matrix (myMatrix) and each non-zero entry of
matrix M (M.myMatrix). For each pair of entries, it checks if the column of the current matrix matches the row of matrix M 
(i.e., colA == rowB). If they match, it calculates the product of the values (valueA * valueB) and accumulates this product into the
appropriate position in the tempResult matrix. The tempCount array is used to keep track of how many times a position has been updated.
Finalizing the Result Matrix: After accumulating all the intermediate results in tempResult, the function copies the non-zero entries
from tempResult to the result matrix. It ensures that only entries with a non-zero value are included. If the number of non-zero entries
exceeds the allocated space for result, it prints an error message, deletes the temporary arrays, and returns nullptr. Otherwise, it
sets the noNonSparseValues of the result matrix to the count of valid non-zero entries.
Cleanup: Finally, the function releases the memory allocated for the temporary arrays (tempResult and tempCount) and returns the result
matrix.
In summary, this function carefully handles matrix multiplication for sparse matrices by managing intermediate results and ensuring that
the multiplication conditions are met, while maintaining efficiency by working with non-zero elements only.



My explanation: The multiply function was one of the most important functions in the project. It was necessary to understand how to
implement the matrix multiplication logic in order to complete the project. Since I was having issues with the creation of the mulitplication
function, I asked for clear steps in order to understand the necessary requirements for the function. For example, I learned the importance of 
using the temporary arrays, because  I was able to store the intermediate results and then copy the results into the result matrix. 
*/



/*
Debugging:

For the test, I used the following input files: input1, input2, input3, input4, and inpu5. Then, I ran the program with the 
redirected input configuration. The program was able to read the input from the files and output the results. Finally, I compared
the output with the expected results in order to verify the correctness of the program. Initially, I had some issues with the
matrix multiplication  and addition functions, but I was able to resolve them by carefully checking the logic and making the necessary 
corrections. For both matrix multiplication and addition, I needed to add a condition to check if that would check if 
matrix multiplication/addition is possible. If it was not possible, the program should output a message and return nullptr. 
This helped to handle cases where the matrices were not compatible for multiplication or addition. After making these changes, 
the program was able to handle all test cases.  Another issue I had was with redirecting the input files, it took me a while to 
figure out how to do it, but the video helped me to finish that part of the project. Finally, I had to flush the cout stream in order to
display the output correctly. After making these changes, my program was able to read the input from the files, perform the matrix 
operations, and output the results as expected.


*/