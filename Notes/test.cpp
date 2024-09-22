#include <iostream>
#include <string>

using namespace std;

class Chip {
private:
    char chipType;          // Type of the chip (A: Addition, S: Subtraction, etc.)
    string id;              // Unique identifier for the chip
    Chip* input1;           // Pointer to the first input chip
    Chip* input2;           // Pointer to the second input chip (can be NULL)
    Chip* output;           // Ptr to the output chip (is NULL for output chips)
    double inputValue;      // For input chips only
    double outputValue;     // Stores computed output value

public:
    // Constructor
    Chip(char type, const string& id) : chipType(type), id(id), input1(nullptr), input2(nullptr), output(nullptr), inputValue(0), outputValue(0) {}

    // Setters for inputs and output
    void setInput1(Chip* inputChip) { input1 = inputChip; }
    void setInput2(Chip* inputChip) { input2 = inputChip; }
    void setOutput(Chip* outputChip) { output = outputChip; }

    // Set value for input chips
    void setInputValue(double value) { inputValue = value; }

    // Get the chip ID
    string getId() const { return id; }

    // Perform the operation based on the chip type
    void compute() {
        switch (chipType) {
            case 'A':  // Addition
                outputValue = input1->getOutputValue() + input2->getOutputValue();
                break;
            case 'S':  // Subtraction
                outputValue = input1->getOutputValue() - input2->getOutputValue();
                break;
            case 'M':  // Multiplication
                outputValue = input1->getOutputValue() * input2->getOutputValue();
                break;
            case 'D':  // Division
                outputValue = input1->getOutputValue() / input2->getOutputValue();
                break;
            case 'N':  // Negation
                outputValue = -input1->getOutputValue();
                break;
            case 'I':  // Input chip (returns its value directly)
                outputValue = inputValue;
                break;
            case 'O':  // Output chip, display the final result
                cout << "I am output chip number " << id << ", and the value I received is " << input1->getOutputValue() << endl;
                return;
        }
        if (output != nullptr) {
            output->compute();  // Recursively trigger the next chip
        }
    };

    // Get output value (for input to other chips)
    double getOutputValue() const { return outputValue; }

    // Display connections for the chip
    void display() const {
        if (chipType == 'I') {
            cout << id << ", Output = " << output->getId() << endl;
        } else if (chipType == 'O') {
            cout << id << ", Input 1 = " << input1->getId() << endl;
        } else {
            cout << id << ", Input 1 = " << input1->getId() << ", Input 2 = " << (input2 ? input2->getId() : "NULL") << ", Output = " << (output ? output->getId() : "NULL") << endl;
        }
    }
};

int main() {
    int numChips;
    cin >> numChips;

    Chip** allChips = new Chip*[numChips];

    // Create all chip objects
    for (int i = 0; i < numChips; i++) {
        string chipId;
        cin >> chipId;

        char type = chipId[0]; // Identify chip type from ID
        allChips[i] = new Chip(type, chipId);
    }

    int numCommands;
    cin >> numCommands;

    // Process commands to build the circuit
    for (int i = 0; i < numCommands; i++) {
        string command;
        cin >> command;

        if (command == "A") {  // A means connect chips
            string chipId1, chipId2;
            cin >> chipId1 >> chipId2;

            Chip* chip1 = nullptr;
            Chip* chip2 = nullptr;

            // Find chip pointers in allChips array
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipId1) chip1 = allChips[j];
                if (allChips[j]->getId() == chipId2) chip2 = allChips[j];
            }

            // Chip 2 is the destination chip (output), so we set input connections
            if (chip2->getId()[0] == 'O') {
                chip2->setInput1(chip1);  // Output chip only needs one input
            } else {
                if (!chip2->getOutputValue()) {
                    chip2->setInput1(chip1);
                } else {
                    chip2->setInput2(chip1);
                }
            }

            chip1->setOutput(chip2);  // Set the output of chip1 to chip2
        } else if (command == "I") {  // I means input a value
            string chipId;
            double value;
            cin >> chipId >> value;

            // Find the corresponding input chip
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipId) {
                    allChips[j]->setInputValue(value);
                    break;
                }
            }
        } else if (command == "O") {  // O means compute and output the result
            string chipId;
            cin >> chipId;

            // Find the output chip and execute computation
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipId) {
                    allChips[j]->compute();  // This triggers computation starting from output chip
                    break;
                }
            }
        }
    }

    cout << "***** Showing the connections that were established *****" << endl;

    // Display connections for each chip
    for (int i = 0; i < numChips; i++) {
        allChips[i]->display();
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < numChips; i++) {
        delete allChips[i];
    }
    delete[] allChips;

    return 0;
}