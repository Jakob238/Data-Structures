#include <iostream>
using namespace std;


class Point{
    public:
        int x;
        int y;
    // constructors
    Point();
    Point(int s, int t);
    //display
    void display();
    //destructor
    ~Point();

};

Point::~Point(){
    cout << " The point is (" << x << ", " << y << ") was killed" << endl; 
}


Point::Point(){
    x=0;
    y=0;
}

Point::Point(int s, int t){
    x=s;
    y=t;
} 

void Point::display(){
    cout << " The point is (" << x << ", " << y << ")" << endl;
}

//Questions on Quiz/Exam
// What is the prupose of a deconstructor?
// All statically defined objects are automatically deleted when program completes
// All dynamically defined objects are not deleted when the program completes

int main(){
    Point aPoint(10,20); // statically defined
    Point bPoint; // statically defined

    aPoint.display();
    bPoint.display();

    Point* pointPtr = new Point (100,200); //  dynamically deleted
    

    return 0;
}

/*
int main() {
    int myArray[10];

    int* someIntPtr;

    int x = 100;
    someIntPtr = &x;

    *someIntPtr = 200;

    cout << x << endl;
    cout << "THe starting address of the array myArray is " << myArray << endl;

    for(int i=0; i<10; i++){
        myArray[i] = 0;
    }

    return 0;
}
*/

/*
int x=100;
template <class DT>
void printThis(DT& x){
    cout << "The value of the integer is " << x << endl;
}

int main(){
    float r = 999.10;
    printThis<float>(r);
    cout << "The value in the box is " << r << endl;
    return 0;
}
*/



/*
int main(){
    int prime =3;
    int numRows;
    int row, col, value;

    cin >> numRows;
    cout << "Hello CS 2423\n";
    cout << "This number is a prime " << prime << endl;
    
    while(!cin.eof())
    {
        cin >> row >> col >> value;
        cout << row << ", " << col<< ", " << value << endl;
        numRows++;
    }
    
}
*/
// cd Notes
// g++ One.cpp -o One
// Get-Content One.txt | .\One.exe