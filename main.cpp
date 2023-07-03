#include <iostream>
#include <vector>
#include <string>

using namespace std;

// function to evaluate the circuit
bool evaluate_circuit(const vector<pair<string, string>>& circuit, const vector<int>& input_vector) {
    vector<int> values(circuit.size());  // store the values of each net in the circuit

    for (int i = 0; i < circuit.size(); i++) {
        string expression = circuit[i].second;
        string net = circuit[i].first;

        if (expression[0] == '~') {
            // NOT gate operation
            int input_index = -1;
            for (int j = 0; j < circuit.size(); j++) {
                if (circuit[j].first == expression.substr(1)) {
                    input_index = j;
                    break;
                }
            }
            values[i] = 1 - values[input_index];
        }
        else {
            // gate operations (AND, OR, XOR)
            size_t op_pos = string::npos;
            for (int j = 0; j < expression.size(); j++) {
                if (expression[j] == '&' || expression[j] == '|' || expression[j] == '^') {
                    op_pos = j;
                    break;
                }
            }
            string left_net = expression.substr(0, op_pos);
            string right_net = expression.substr(op_pos + 1);

            int left_index = -1;
            int right_index = -1;
            for (int j = 0; j < circuit.size(); j++) {
                if (circuit[j].first == left_net) {
                    left_index = j;
                }
                if (circuit[j].first == right_net) {
                    right_index = j;
                }
            }

            if (expression[op_pos] == '&') {
                // AND gate operation
                values[i] = values[left_index] & values[right_index];
            }
            else if (expression[op_pos] == '|') {
                // OR gate operation
                values[i] = values[left_index] | values[right_index];
            }
            else if (expression[op_pos] == '^') {
                // XOR gate operation
                values[i] = values[left_index] ^ values[right_index];
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

    string fault_node = "net_f";  // fault node location
    string fault_type = "SA0";    // fault type

    vector<int> input_vector(4, 0);  // initialize the input vector with zeros

    for (int i = 0; i < input_vector.size(); i++) {
        input_vector[i] = 1;  // set the i-th input to 1

        bool original_output = evaluate_circuit(circuit, input_vector);

        input_vector[i] = 0;  // set the i-th input to 0

        bool modified_output = evaluate_circuit(circuit, input_vector);

        if ((fault_type == "SA0" && modified_output != original_output) ||
            (fault_type == "SA1" && modified_output == original_output)) {
            // print the input test vector and expected output
            cout << "[" << input_vector[0] << ", " << input_vector[1] << ", " << input_vector[2] << ", " << input_vector[3] << "], Z = " << original_output << endl;
        }

        input_vector[i] = 1;  // reset the i-th input back to 1
    }

    return 0;
}
