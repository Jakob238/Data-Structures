#include <iostream>
using namespace std;

class Point{
    public:
        int x;
        int y;
    // constructors
    Point();
    Point(int s, int t);
    Point(Point& P);
    Point& operator = (Point& P); //overloaded assigment operator
    //display
    void display();
    //destructor
    ~Point();

};

Point& Point::operator =(Point& P){
    x = P.x;
    y = P.y;
}


Point::Point(Point& P){
    x = P.x;
    y = P.y;
}


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
    Point* pointPtr; // statically defined
    Point* cPoint; // statically defined


    aPoint.display();
    bPoint.display();

    pointPtr = new Point (100,200); //  dynamically deleted
    cPoint = new Point(aPoint);

    delete pointPtr;
    delete cPoint; // the box cPoint is not deleted; what is deleted is the object cPoint is referring to

    cPoint = new Point(10,33);

    aPoint =  bPoint = *cPoint; 

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

