#include <iostream>
#include <vector>
#include <string>

using namespace std;

// function to evaluate the circuit
bool evaluateCircuit(const vector<pair<string, string>>& circuit, const vector<int>& inputVector) {
    vector<int> values(circuit.size());  // ftore the values of each net in the circuit

    for (int i = 0; i < circuit.size(); i++) {
        string expression = circuit[i].second;
        string net = circuit[i].first;

        if (expression[0] == '~') {
            // NOT gate operation
            int inputIndex = -1;
            for (int j = 0; j < circuit.size(); j++) {
                if (circuit[j].first == expression.substr(1)) {
                    inputIndex = j;
                    break;
                }
            }
            values[i] = 1 - values[inputIndex];
        }
        else {
            // gate operations (AND, OR, XOR)
            size_t opPos = string::npos;
            for (int j = 0; j < expression.size(); j++) {
                if (expression[j] == '&' || expression[j] == '|' || expression[j] == '^') {
                    opPos = j;
                    break;
                }
            }
            string leftNet = expression.substr(0, opPos);
            string rightNet = expression.substr(opPos + 1);

            int leftIndex = -1;
            int rightIndex = -1;
            for (int j = 0; j < circuit.size(); j++) {
                if (circuit[j].first == leftNet) {
                    leftIndex = j;
                }
                if (circuit[j].first == rightNet) {
                    rightIndex = j;
                }
            }

            if (expression[opPos] == '&') {
                // AND gate operation
                values[i] = values[leftIndex] & values[rightIndex];
            }
            else if (expression[opPos] == '|') {
                // OR gate operation
                values[i] = values[leftIndex] | values[rightIndex];
            }
            else if (expression[opPos] == '^') {
                // XOR gate operation
                values[i] = values[leftIndex] ^ values[rightIndex];
            }
        }
    }

    return values.back();  // return the value of the output(Z)
}

int main() {
    //circuit structure and logic
    vector<pair<string, string>> circuit = {
        { "net_e", "A & B" },
        { "net_f", "C | D" },
        { "net_g", "~net_f" },
        { "Z", "net_g ^ net_e"}
    };

    string faultNode = "net_f";  // fault node location
    string faultType = "SA0";    // fault type

    vector<int> inputVector(4, 0);  // initialize the input vector with zeros

    for (int i = 0; i < inputVector.size(); i++) {
        inputVector[i] = 1;  // set the i-th input to 1

        bool originalOutput = evaluateCircuit(circuit, inputVector);

        inputVector[i] = 0;  // set the i-th input to 0

        bool modifiedOutput = evaluateCircuit(circuit, inputVector);

        if ((faultType == "SA0" && modifiedOutput != originalOutput) ||
            (faultType == "SA1" && modifiedOutput == originalOutput)) {
            // print the input test vector and expected output
            cout << "[" << inputVector[0] << ", " << inputVector[1] << ", " << inputVector[2] << ", " << inputVector[3] << "], Z = " << originalOutput << endl;
        }

        inputVector[i] = 1;  // reset the i-th input back to 1
    }

    return 0;
}
