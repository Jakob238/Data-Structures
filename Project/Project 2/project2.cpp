#include <iostream>
#include <string>
using namespace std;
class Chip {
    private:
    char chipType; // Type of the chip (A: Addition, S: Subtraction, etc.)
    string id; // Unique identifier for the chip
    Chip* input1; // Pointer to the first input chip
    Chip* input2; // Pointer to the second input chip (can be NULL)
    Chip* output; // Ptr to the output chip (is NULL for output chips)
    double inputValue; //for the input chip
public:
    // Constructor and Decstructor
    Chip(char type, const string& id);
    ~Chip();
    // Method prototypes
    void setInput1(Chip* inputChip); // Sets the first input chip
    void setInput2(Chip* inputChip); // second input chip (can be NULL)
    void setOutput(Chip* outputChip); // Sets the output chip (can be NULL)
    void compute(); // Performs the operation based on the chip type
    void display() const; // Displays the chip's information
    //example: I6, Output = S600 --- for the input Chip
    //example: O50, Input 1 = S600 --- for the output Chip
    //example: A100, Input 1 = I1, Input 2 = I2, Output = M300
    string getId() const; // Returns the chip ID
    //****** OTHER METHODS AS NECESSARY ************//
    double getInputValue() const; // Returns the input value
    void setInputValue(double value);
    Chip* getInput1() const; // Returns the first input chip
};


// Constructor
Chip::Chip(char type, const string& id){
    chipType = type;
    this->id = id;
    input1 = nullptr;
    input2 = nullptr;
    output = nullptr;
    inputValue = 0;
    
}

// Deconstructor
Chip::~Chip(){
    if(!(input1 == NULL))
        delete input1;
    if(~(input2 == NULL))
        delete input2;
}

// Method ProtoTypes
void Chip::setInput1(Chip* inputChip){
    input1 = inputChip;
}

void Chip::setInput2(Chip* inputChip){
    input2 = inputChip;
}

void Chip::setOutput(Chip* outputChip) {
    output = outputChip;
}


// Getters
string Chip::getId() const { 
    return id; 
}
double Chip::getInputValue() const{
    return inputValue;
}

Chip* Chip::getInput1() const{
    return input1;
}

void Chip::setInputValue(double value) { 
    inputValue = value; 
}


// Display Method
void Chip::display() const{
if(chipType == 'I'){
std::cout << id << ", Output = " << output->getId() << std::endl;
}

if(chipType == 'O'){
std::cout << id << ", Input 1 = " << input1->getId() << std::endl;
}

if(chipType == 'A' || chipType == 'S' || chipType == 'M'  || chipType == 'D'){ 
    std::cout << id << ", Input 1 = " << input1->getId() << ", Input 2 = " << input2->getId() << ", Output = " << output->getId() << std::endl;
}

if(chipType == 'N')
{
    std::cout << id << ", Input 1 = " << input1->getId() << ", Input 2 = None" << ", Output = " << output->getId() << std::endl;
}

}


void Chip::compute(){
    switch (chipType) {
        case 'A': // Addition
            if(input1 != nullptr)
            {
                input1 -> compute();
            }
            if(input2 != nullptr)
            {
                input2 -> compute();
            }
            inputValue = input1->inputValue + input2->inputValue;
            break;
        case 'S': // Subtraction
            if(input1 != nullptr)
            {
                input1 -> compute();
            }
            if(input2 != nullptr)
            {
                input2 -> compute();
            }
            inputValue = input1->inputValue - input2->inputValue;
            break;
        case 'M': // Multiplication
            if(input1 != nullptr)
            {
                input1 -> compute();
            }
            if(input2 != nullptr)
            {
                input2 -> compute();
            }
            inputValue = input1->inputValue * input2->inputValue;
            break;
        case 'N': // Negation
        if(input1 != nullptr)
            {
                input1 -> compute();
            }
            inputValue = -input1->inputValue;
            break;
        case 'D': // Division
            if(input1 != nullptr)
            {
                input1 -> compute();
            }
            if(input2 != nullptr)
            {
                input2 -> compute();
            }
            inputValue = input1->inputValue / input2->inputValue;
            break;
        
        // fix these
        case 'I': // Input chip
            // inputValue = inputValue;
            break;
        case 'O': // Output chip
            if(input1 != nullptr)
            {
                input1 -> compute();
            }
            inputValue = input1->inputValue;
            break;
    }     
}


// Main Method
int main () {
    //**** ALL THE VARIABLES YOU NEED FOR THIS MAIN FUNCTION *****//
    int numChips;
    string chipID;
    string type;
    int id;
    int output;
    Chip** allChips;
    int numCommands;

    cin >> numChips;
    //create an array Chip objects pointers
    allChips = new Chip*[numChips];

    //each array location is a pointer to a Chip Object
    for (int i=0; i < numChips; i++) {
        //read the chip ID
        cin >> chipID;
        char chipType = chipID[0];
        //create the chip object and initialize it appropriately and store it an array
        allChips[i] = new Chip(chipType, chipID);
    }
    
    cin >> numCommands;

        for (int i = 0; i < numCommands; i++) {
        string command;
        cin >> command;

        if (command == "A") {
            string chipID1, chipID2;
            cin >> chipID1 >> chipID2;
            Chip* chip1 = nullptr;
            Chip* chip2 = nullptr;
            
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID1) {
                    chip1 = allChips[j];
                }
                if (allChips[j]->getId() == chipID2) {
                    chip2 = allChips[j];
                }
            }

            if (chip1 && chip2) {
                chip1->setOutput(chip2);
                if (chip2->getId()[0] == 'O') {
                    chip2->setInput1(chip1);
                } 
                else if (chip2->getId()[0] == 'N') {
                    chip2->setInput1(chip1);
                } 
                else{
                    // chip2->setInput1(chip1);
                    // chip2->setInput2(chip1);

                    if(chip2->getInput1() == nullptr){
                        chip2->setInput1(chip1);
                    }
                    else{
                        chip2->setInput2(chip1);
                    }
                }
            }
        }
        else if (command == "I") {
            string chipID;
            double value;
            cin >> chipID >> value;
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID) {
                    allChips[j]->setInputValue(value);
                }
            }
        } else if (command == "O") {
            string chipID;
            cin >> chipID;
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID) {
                    allChips[j]->compute();
                }
            }
        }
    }

    output = 0;
    // for (int j = 0; j < numChips; j++) {
    //     output += allChips[j]->getInputValue();
    // }
    for (int j = 0; j < numChips; j++) {
    if (allChips[j]->getId()[0] == 'O') {
        output = allChips[j]->getInputValue(); // Only take the output chip value.
    }
} 


    std::cout << "Computation Starts" << std::endl;
    std::cout << "The output value from this circuit is " << output << std::endl;
    std::cout << "***** Showing the connections that were established" << std::endl;
    

    for(int i =0; i < numChips; i++){
        allChips[i]->display();
    }

    for (int i = 0; i < numChips; i++) {
        delete allChips[i];
    }

    delete[] allChips;

    return 0;
}













/* LLM and GitHub Copilot Usage Documentation :


*/


/* Debugging and Testing Plan :



*/