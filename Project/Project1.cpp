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
    SparseMatrix (int n, int m, int cv);
    
    SparseMatrix* Transpose (); //Matrix Transpose
    SparseMatrix* Multiply (SparseMatrix& M);
    SparseMatrix* Add (SparseMatrix& M);
    
    friend ostream& operator<< (ostream& s, const SparseMatrix& sm);
    void displayMatrix (); //Display the matrix in its original format
    //other methods that are necessary such as get and set
};









// SparseRow methods
// Constructor
SparseRow::SparseRow(){
    row = -1;
    col = -1;
    value =0;
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

SparseMatrix::SparseMatrix(int n, int m, int cv){
    noRows = n;
    noCols = m;
    commonValue = cv;
    noNonSparseValues = 0;
}

// Matrix Operations
SparseMatrix* Transpose (){

return nullptr;
}

SparseMatrix* Multiply (SparseMatrix& M){

return nullptr;
}

SparseMatrix* Add (SparseMatrix& M){

return nullptr;
}

ostream& operator<< (ostream& s, const SparseMatrix& sm){

return s;
}

// Display
void SparseMatrix::displayMatrix(){
    for(int i=0; i<noRows; i++){
        for( int j=0; j<noCols; j++){
            cout << myMatrix[i*noCols+j].getValue() << " "; //Fix this
        }
    }
}

// Getters


// Setters










//Main Method
int main () {
    int n, m, cv, noNSV;
    SparseMatrix* temp;
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    //Write the Statements to read in the first matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    
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
    temp = (*(*firstOne).Addition(secondOne));
    cout << temp;
    (*temp).displayMatrix();
    cout << "Matrix Multiplication Result" << endl;
    temp = (*(*firstOne).Multiply(secondOne));
    cout << temp;
    (*temp).displayMatrix();
}