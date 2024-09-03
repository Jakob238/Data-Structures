#include <iostream>
using namespace std;

// Structure of the SparseRow and SparseMatrix classes
class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //We will assume that all our values will be integers
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
        int noRows; //Number of rows of the original matrix
        int noCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int noNonSparseValues;
        SparseRow* myMatrix; //Array
    public:
    SparseMatrix ();
    SparseMatrix (int n, int m, int cv, int nsv);
    ~SparseMatrix();

    SparseMatrix* Transpose (); //Matrix Transpose
    SparseMatrix* Multiply (SparseMatrix& M);
    SparseMatrix* Addition (SparseMatrix& M);
    
    friend ostream& operator<< (ostream& s, const SparseMatrix& sm);
    void displayMatrix (); //Display the matrix in its original format
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



// fix this addition
SparseMatrix* SparseMatrix::Addition (SparseMatrix& M){
if (noRows != M.noRows || noCols != M.noCols || commonValue != M.commonValue) {
    cout << "Matrix addition is not possible" << endl;
    return nullptr;
}

// Allocate enough space in the result matrix for the worst-case scenario
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);
    result->noNonSparseValues = 0; // Initialize the count of non-sparse values

    int i = 0, j = 0;
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

// fix this ostream
ostream& operator<< (ostream& s, const SparseMatrix& sm){
for(int i=0; i<sm.noNonSparseValues; i++){
    s << sm.myMatrix[i] << endl;
}
return s;
}

// Display
void SparseMatrix::displayMatrix(){
int count = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            if (count < noNonSparseValues && myMatrix[count].getRow() == i && myMatrix[count].getCol() == j) {
                cout << myMatrix[count].getValue() << " ";
                ++count;
            } 
            else{
                cout << commonValue << " ";
                }
        }
        cout << endl;
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