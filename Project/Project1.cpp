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
    cout << "Row:" << row << "Column:" << col << "Value" << value << endl;
}

ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << "Row: " << sr.row << " Column: " << sr.col << " Value: " << sr.value;
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
}

SparseMatrix::SparseMatrix(int n, int m, int cv, int nsv){
    noRows = n;
    noCols = m;
    commonValue = cv;
    noNonSparseValues =nsv;
}

SparseMatrix::~SparseMatrix(){
    delete[] myMatrix;
}


// fix this
void SparseMatrix::readMatrix() {
    int count = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            int value;
            cin >> value;
            if (value != commonValue) {
                myMatrix[count++] = SparseRow(i, j, value);
            }
        }
    }
}

// Matrix Operations

// fix this transpose
SparseMatrix* SparseMatrix::Transpose (){
SparseMatrix* transposed = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);
    for (int i = 0; i < noNonSparseValues; ++i) {
    transposed->myMatrix[i].setRow(myMatrix[i].getCol());
    transposed->myMatrix[i].setCol(myMatrix[i].getRow());
    transposed->myMatrix[i].setValue(myMatrix[i].getValue());
    }
return transposed;
}



// fix this multiplication
SparseMatrix* SparseMatrix::Multiply (SparseMatrix& M){
if (noCols != M.noRows) {
    cout << "Matrix dimensions do not allow multiplication!" << endl;
    return nullptr;
}

int maxSize = noNonSparseValues * M.noNonSparseValues; // Overestimate
SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, 0);

int count = 0;
for (int i = 0; i < noNonSparseValues; ++i) {
    for (int j = 0; j < M.noNonSparseValues; ++j) {
        if (myMatrix[i].getCol() == M.myMatrix[j].getRow()) {
            int r = myMatrix[i].getRow();
            int c = M.myMatrix[j].getCol();
            int value = myMatrix[i].getValue() * M.myMatrix[j].getValue();
            bool found = false;
            for (int k = 0; k < count; ++k) {
                if (result->myMatrix[k].getRow() == r && result->myMatrix[k].getCol() == c) {
                    result->myMatrix[k].setValue(result->myMatrix[k].getValue() + value);
                    found = true;
                    break;
                }
            }

            if (!found && value != commonValue) {
                 result->myMatrix[count++] = SparseRow(r, c, value);
            }
        }
    }
}

result->noNonSparseValues = count;
return result;
}


// fix this addition
SparseMatrix* SparseMatrix::Addition (SparseMatrix& M){
if (noRows != M.noRows || noCols != M.noCols || commonValue != M.commonValue) {
    cout << "Matrix dimensions or common values do not match!" << endl;
    return nullptr;
}

int maxSize = noNonSparseValues + M.noNonSparseValues;
SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0);

int i = 0, j = 0, count = 0;
while (i < noNonSparseValues && j < M.noNonSparseValues) {
    if (myMatrix[i].getRow() == M.myMatrix[j].getRow() && myMatrix[i].getCol() == M.myMatrix[j].getCol()) {
        int sumValue = myMatrix[i].getValue() + M.myMatrix[j].getValue();
        if (sumValue != commonValue) {
            result->myMatrix[count++] = SparseRow(myMatrix[i].getRow(), myMatrix[i].getCol(), sumValue);
        }
        i++;
        j++;
    } 
    else if ((myMatrix[i].getRow() < M.myMatrix[j].getRow()) ||
        (myMatrix[i].getRow() == M.myMatrix[j].getRow() && myMatrix[i].getCol() < M.myMatrix[j].getCol())) {
        result->myMatrix[count++] = myMatrix[i++];
    } 
    else{
        result->myMatrix[count++] = M.myMatrix[j++];
    }
}
while (i < noNonSparseValues) {
    result->myMatrix[count++] = myMatrix[i++];
}
while (j < M.noNonSparseValues) {
    result->myMatrix[count++] = M.myMatrix[j++];
}
result->noNonSparseValues = count;
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
    
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    //Write the Statements to read in the first matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    
    firstOne->readMatrix();


    //Write the Statements to read in the second matrix
    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();
    cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);
    
    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();
    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);
    
    cout << "Transpose of the first one in matrix" << endl;
    cout << (*(*firstOne).Transpose());
    
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Addition(*secondOne);
    if (temp) {
        cout << *temp;
        temp->displayMatrix();
        delete temp;
    }

    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply(*secondOne);
    if (temp) {
        cout << *temp;
        temp->displayMatrix();
        delete temp;
    }

}