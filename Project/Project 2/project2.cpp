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
    if(!(input2 == NULL))
        delete input2;
}

// Setters
void Chip::setInput1(Chip* inputChip){
    input1 = inputChip;
}

void Chip::setInput2(Chip* inputChip){
    input2 = inputChip;
}

void Chip::setOutput(Chip* outputChip) {
    output = outputChip;
}

void Chip::setInputValue(double value) { 
    inputValue = value; 
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


// Display Method
void Chip::display() const{

// Input Chip
if(chipType == 'I'){
cout << id << ", Output = " << output->getId() << std::endl;
}

// Output Chip
if(chipType == 'O'){
cout << id << ", Input 1 = " << input1->getId() << std::endl;
}

// Arithmetic Chips
if(chipType == 'A' || chipType == 'S' || chipType == 'M'  || chipType == 'D'){ 
    cout << id << ", Input 1 = " << input1->getId() << ", Input 2 = " << input2->getId() << ", Output = " << output->getId() << std::endl;
}

// Negation Chip - (Needs Only One Input)
if(chipType == 'N')
{
    cout << id << ", Input 1 = " << input1->getId() << ", Input 2 = None" << ", Output = " << output->getId() << std::endl;
}

}


// Compute Method
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
        
        
        case 'I': // Input chip
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
    string chipID;
    string type;
    int numChips;
    int numCommands;
    double output;
    Chip** allChips;
    
    // Read the number of chips from input
    cin >> numChips;
    
    //Creates an array Chip objects pointers
    allChips = new Chip*[numChips];

    //Each array location is a pointer to a Chip Object
    for (int i=0; i < numChips; i++) {
        //Read the chip ID
        cin >> chipID;
        char chipType = chipID[0];
        //Create the chip object and initialize it appropriately and store it an array
        allChips[i] = new Chip(chipType, chipID);
    }
    

    cin >> numCommands;

    // Uses the total number of commands in order to determine the type of each command
        for (int i = 0; i < numCommands; i++) {
        string command;
        cin >> command; 

        if (command == "A") {  //Association
            string chipID1, chipID2;
            cin >> chipID1 >> chipID2; // Read two chip IDs to associate
            
            Chip* chip1 = nullptr;
            Chip* chip2 = nullptr;
            
            // Search for the chips by their IDs in the allChips array
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID1) {
                    chip1 = allChips[j];
                }
                if (allChips[j]->getId() == chipID2) {
                    chip2 = allChips[j];
                }
            }

            // If both chips are found, make the connection between them
            if (chip1 && chip2) {
                chip1->setOutput(chip2);

                // Handle special case for 'O' (Output) or 'N' (Negation) chips
                if (chip2->getId()[0] == 'O') {
                    chip2->setInput1(chip1);
                } 
                else if (chip2->getId()[0] == 'N') {
                    chip2->setInput1(chip1);
                } 

                else {
                    // For chips requiring two inputs, set the first or second input accordingly
                    if(chip2->getInput1() == nullptr){
                        chip2->setInput1(chip1);
                    }
                    else{
                        chip2->setInput2(chip1);
                    }
                }
            }
        }

        else if (command == "I") {  //Input
            string chipID;
            double value;
            cin >> chipID >> value;

            // Search for the chip by its ID and set its input value
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID) {
                    allChips[j]->setInputValue(value);
                }
            }
        } 

        else if (command == "O") {  //Output
            string chipID;
            cin >> chipID;

            // Search for the chip by its ID and call the compute() method
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID) {
                    allChips[j]->compute();
                }
            }
        }
    }

    // Find the output chip and gets the output value
    output = 0;
    for (int j = 0; j < numChips; j++) {
    if (allChips[j]->getId()[0] == 'O') { // Only will take the output chip value.
        output = allChips[j]->getInputValue(); 
    }
} 

    // Outputs the results
    std::cout << "Computation Starts" << std::endl;
    std::cout << "The output value from this circuit is " << output << std::endl;
    std::cout << "***** Showing the connections that were established" << std::endl;
    

    // Display the connections, but waits to display the output chip last
    Chip* outputChip;
    for(int i =0; i < numChips; i++){
        if(allChips[i]->getId()[0] == 'O'){
            outputChip = allChips[i];
        }
        else{
            allChips[i]->display();
        }
    }
    outputChip->display();
    delete outputChip;


    //Flush the output
    cout.flush();

    //Deletes all the chips
    for (int i = 0; i < numChips; i++) {
        delete allChips[i];
    }
    delete[] allChips;


    return 0;
}










/* LLM and GitHub Copilot Usage Documentation :
Prompt 1: Explain how to create an association between chips. Here is some of the fundamental instruction about an asssociation of chips. 
If you have a collection of specialized computing chips designed to perform a specific elementary arithmetic operation. 
These operations include addition, subtraction, multiplication, negation, and division. Each chip, except for the negation chip, 
requires two input values and produces a single output value. Due to the nature of its operation, the negation chip only
needs one input and generates one output. Among these chips, there is a unique type called the output chip. This chip has one input and no
output, as its sole function is to display the value it receives through its input line. 
Here is the initial main method, 
#include <iostream>
#inlcude <string>
using namespace std;
//Define your Chip class and all its methods.
int main () {
int numChips;
Chip** allChips;
int numCommands;
cin >> numChips;
//create an array Chip objects pointers
allChips = new Chip*[numChips];
Page 6 of 7
//each array location is a pointer to a Chip Object
for (int i=0; i < numChips; i++) {
//read the chip ID based on the first letter to determine its type
//create the chip object and initialize it appropriately
//store the chip object in the allChips array
}
for (int i=0; i < numCommands; i++) {
// read from input the links and make the appropriate
//connections. You may need to search the array allChips
//to find the chip that is referred and connect.
// If the first letter is an O, then execute the compute method
// on the object referred.
}
cout << "***** Showing the connections that were established” << endl;
//for each component created call the display () method
return 0; }



Answer: In this circuit simulation, you're creating associations between different types of chips (addition, subtraction, 
multiplication, negation, division, and output chips). Each chip performs a specific operation, with most needing two inputs, 
except for the negation chip, which only requires one input. The output chip's role is to display the final value after computations. 
Here's how the association and connection between chips would be structured and executed based on your code and the instructions provided.

Key Concepts for Chip Association:
Arithmetic chips (addition, subtraction, multiplication, and division) require two input values and produce one output.
The negation chip requires one input and produces one output.
The output chip only has one input and no output, as it is used to display the final value.
Chip Class Setup:
Define the Chip class and subclasses for different chip types (addition, subtraction, etc.). Each subclass will implement operations 
specific to its chip type.

Main Method Structure:
1. Chip Array Creation:
After reading the number of chips (numChips), you need to create an array to store pointers to chip objects. You will then initialize each 
chip based on the input provided.

2. Reading Chip Types:
Based on the chip ID (from input), determine the type of chip you're dealing with (addition, subtraction, multiplication, negation, etc.) 
and create the respective chip object. For example, if the chip ID starts with 'A', it's an addition chip.

3. Associating Chips:
During the execution of commands, chips will be associated based on input and output connections. You will search through the chip array 
(allChips) to find the chips by ID and then connect them by setting input/output relationships.

4. Processing Commands:
Command A: Associates two chips, chipID1 (output chip) and chipID2 (input chip). It ensures the output of chip1 is connected to chip2, 
handling multiple inputs correctly.
Command I: Sets a numerical input for a chip by its ID.
Command O: For an output chip, triggers the compute() function to display the final computed value.
Code Walkthrough:
#include <iostream>
#include <string>
using namespace std;

// Base Chip class definition
class Chip {
public:
    virtual void compute() = 0;  // Pure virtual function to compute output
    virtual string getId() const = 0;  // Get chip ID
    virtual void setInput1(Chip* chip) = 0;  // Set first input
    virtual void setInput2(Chip* chip) = 0;  // Set second input (if needed)
    virtual void setOutput(Chip* chip) = 0;  // Set output for the chip
    virtual void setInputValue(double value) = 0;  // Set a numerical input value
    virtual void display() const = 0;  // Display connections and chip details
};

// Example of specific Chip types (Addition Chip)
class AddChip : public Chip {
    // AddChip-specific implementation of compute, setInput1, setInput2, etc.
};

// Similarly define SubChip, MulChip, NegChip, OutputChip, etc.

int main() {
    int numChips;
    Chip** allChips;
    int numCommands;
    
    cin >> numChips;

    // Create an array to store Chip object pointers
    allChips = new Chip*[numChips];

    // Initialize each chip based on the input type
    for (int i = 0; i < numChips; i++) {
        string chipID;
        cin >> chipID;

        if (chipID[0] == 'A') {
            // Create an Addition Chip
            allChips[i] = new AddChip(chipID);  // Assuming AddChip constructor takes chipID
        }
        else if (chipID[0] == 'S') {
            // Create a Subtraction Chip
            allChips[i] = new SubChip(chipID);
        }
        else if (chipID[0] == 'M') {
            // Create a Multiplication Chip
            allChips[i] = new MulChip(chipID);
        }
        else if (chipID[0] == 'N') {
            // Create a Negation Chip
            allChips[i] = new NegChip(chipID);
        }
        else if (chipID[0] == 'O') {
            // Create an Output Chip
            allChips[i] = new OutputChip(chipID);
        }
        // Add further types (division, etc.) as needed.
    }

    // Processing commands for associating and computing values
    cin >> numCommands;

    for (int i = 0; i < numCommands; i++) {
        string command;
        cin >> command;

        if (command == "A") {
            // Read chip IDs
            string chipID1, chipID2;
            cin >> chipID1 >> chipID2;

            Chip* chip1 = nullptr;
            Chip* chip2 = nullptr;

            // Find the chips by their IDs
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipID1) {
                    chip1 = allChips[j];
                }
                if (allChips[j]->getId() == chipID2) {
                    chip2 = allChips[j];
                }
            }

            // Establish connections
            if (chip1 && chip2) {
                chip1->setOutput(chip2);

                // Handle different chip types for input connections
                if (chip2->getId()[0] == 'O') {
                    chip2->setInput1(chip1);  // Output chip: 1 input
                }
                else if (chip2->getId()[0] == 'N') {
                    chip2->setInput1(chip1);  // Negation chip: 1 input
                }
                else {
                    // For other chips needing 2 inputs
                    if (chip2->getInput1() == nullptr) {
                        chip2->setInput1(chip1);  // Set first input
                    }
                    else {
                        chip2->setInput2(chip1);  // Set second input
                    }
                }
            }
        } 


Explanation: While the majority of the code is incorrect, the main method structure and the concept of associating chips are correctly
explained. The main method should create an array of Chip object pointers, initialize each chip based on the input type, and process
commands to establish connections between chips and compute values. The association between chips involves setting input/output relationships
and handling different chip types (e.g., output, negation, addition) appropriately. The provided code snippet outlines the basic structure
and logic required for creating associations between chips in a circuit simulation program. 



Prompt 2: Explain how to do a compute function for Chip::compute() method. Here is some of the fundamental instruction about the compute 
function for the Chip::compute() method. 
Consider a configuration where four chips are connected in sequence to perform a series of
operations. These chips are as follows:
1. A100 (Addition Chip): This chip adds two input values. It is connected to two input chips
labeled I1 and I2. These input chips supply the values to be added.
2. N110 (Negation Chip): The output from the addition chip A100 is sent to the input of the
negation chip N110. The negation chip takes this input value and negates it, producing a
single output that is the negative of the input value.
3. M120 (Multiplication Chip): This chip requires two inputs to perform multiplication. One
of its inputs is connected to the output of the negation chip N110, while the other input
comes from the output of another input chip labeled I3.
4. S130 (Subtraction Chip): The output from the multiplication chip M120 is fed into one of
the inputs of the subtraction chip S130. The other input to this chip comes from the output
of an additional input chip labeled I4. The subtraction chip then subtracts the second input
from the first and produces an output
In summary, the configuration of these chips allows a series of arithmetic operations to be
performed in sequence, where each chip's output is passed as an input to the next chip in the chain.
The entire process culminates in the output chip, which displays the final result of the computations


Answer: 
The Chip::compute() function implements the logic for computing the output of various types of chips based on their input values. Here's an explanation of how the function works and how each case operates:

1. Addition ('A')
The addition chip requires two input values (input1 and input2).
Before performing the addition, it recursively calls compute() on both input1 and input2 to ensure that their values are up-to-date.
After computing the values of the inputs, it adds them together: inputValue = input1->inputValue + input2->inputValue.
2. Subtraction ('S')
The subtraction chip works similarly to the addition chip.
It computes the values of both input1 and input2, and then subtracts the second value from the first: inputValue = input1->inputValue - input2->inputValue.
3. Multiplication ('M')
The multiplication chip also requires two input values.
It computes the values of input1 and input2 and multiplies them: inputValue = input1->inputValue * input2->inputValue.
4. Negation ('N')
The negation chip only requires one input (input1).
It computes the value of input1 and negates it: inputValue = -input1->inputValue.
5. Division ('D')
The division chip requires two input values.
It computes the values of input1 and input2 and divides the first value by the second: inputValue = input1->inputValue / input2->inputValue.
Note: You might want to add error handling here to avoid division by zero.
6. Input Chip ('I')
The input chip simply holds an externally provided value. It doesn't compute any new values since it is an input terminal.
This case could involve more logic for directly handling input values, but here it is a placeholder.
7. Output Chip ('O')
The output chip takes one input (input1).
It calls compute() on input1 to ensure that its value is up-to-date, and then stores input1->inputValue as its own value.
Compute Flow Explanation:
The compute() method is recursively called on the input chips to ensure that all upstream calculations are performed before the current 
chip computes its value.
This method essentially walks through the entire dependency chain of chips, updating the inputValue for each chip in the correct order.
Enhancements:
Error Handling: For division, you may want to check for division by zero and handle it appropriately to prevent runtime errors.
Sample Error Handling for Division:
cpp
Copy code
case 'D': // Division
    if(input1 != nullptr)
    {
        input1 -> compute();
    }
    if(input2 != nullptr)
    {
        input2 -> compute();
    }
    if(input2->inputValue != 0) { // Avoid division by zero
        inputValue = input1->inputValue / input2->inputValue;
    } else {
        cerr << "Error: Division by zero in chip " << getId() << endl;
        inputValue = 0; // or handle it in some other way
    }
    break;
In this compute function, you traverse the chip network, ensuring all dependent computations are done before computing the value 
of the current chip.


Explanation: For the compute() function in the Chip class, the logic for each chip type (addition, subtraction, multiplication, negation,
division, input, output) is correctly explained. The function recursively calls compute() on input chips to ensure that all upstream
calculations are performed before computing the value of the current chip. The explanation also highlights the specific operations
performed by each chip type and the handling of input values. It is neccessary to provide comments to explain the logic and flow of
the compute() function for better understanding and maintainability of the code.


*/


/* Debugging and Testing Plan :
For the test, I used the following input files: input1, input2, and input3. Then, I ran the program with the 
redirected input configuration. The program was able to read the input from the files and output the results. Finally, I compared
the output from my code with the expected output using the files output1, output2, and output3. This allowed me to have a better 
undertstanding of the project and verify the correctness of the program. Initially, I had some issues with the compute function,
but I was able to resolve them by carefully checking the logic and making the necessary corrections. For this function, I needed to
add a switch statement to handle the different chip types and perform the appropriate operations. Additonally, I had to add a condition
to check if the input1 and input2 pointers were not null before calling the compute function recursively. Another issue I had was with
the associations between the chips, I had to ensure that the input and output chips were correctly connected. By carefully checking the
connections and updating the pointers, I was able to resolve this issue. After making these changes, the code was able to handle all
test cases and produce the expected output. Finally, I had to flush the cout stream in order to display the output correctly.
*/