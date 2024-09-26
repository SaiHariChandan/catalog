#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

// Function to decode the base value
long long decodeBaseValue(const string& value, int base) {
    try {
        return stoll(value, nullptr, base);  // Using stoll for long long
    } catch (const invalid_argument&) {
        throw invalid_argument("Invalid argument: The string '" + value + "' cannot be converted to an integer in base " + to_string(base));
    } catch (const out_of_range&) {
        throw out_of_range("Out of range error: The value '" + value + "' is too large to fit in a long long.");
    }
}

// Function to perform Gaussian elimination and back substitution
vector<double> gaussianElimination(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();

    // Forward elimination
    for (int i = 0; i < n; ++i) {
        // Find the row with the largest pivot and swap rows
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }
        swap(A[i], A[maxRow]);
        swap(b[i], b[maxRow]);

        // Make all rows below this one 0 in current column
        for (int k = i + 1; k < n; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }

    // Back substitution
    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i] / A[i][i];
        for (int k = i - 1; k >= 0; --k) {
            b[k] -= A[k][i] * x[i];
        }
    }

    return x;
}

int main() {
    // Open and parse the JSON file
    ifstream inputFile("input.json"); // Ensure your input JSON filename is correct
    if (!inputFile) {
        cerr << "Failed to open the input file!" << endl;
        return 1;
    }

    json jsonData;
    inputFile >> jsonData;

    // Extract n and k
    int n = jsonData["keys"]["n"];
    int k = jsonData["keys"]["k"];

    // Vectors to store decoded x and y values
    vector<int> x_values;
    vector<long long> y_values;  // Use long long for y values

    // Iterate through the JSON to decode the values
    for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if (it.key() == "keys") continue; // Skip the keys section
        
        // Ensure that the key is an integer
        int x;
        try {
            x = stoi(it.key()); // The key represents x
        } catch (const invalid_argument&) {
            cerr << "Error: The key '" << it.key() << "' is not a valid integer." << endl;
            return 1;
        }

        // Decode base and value properly
        int base = stoi(it.value()["base"].get<string>()); // Decode base
        string value = it.value()["value"].get<string>();   // The value to decode

        try {
            long long y = decodeBaseValue(value, base); // Decode the y value
            x_values.push_back(x);
            y_values.push_back(y);
        } catch (const exception& e) {
            cerr << "Error decoding value for key " << x << ": " << e.what() << endl;
            return 1;
        }
    }

    // We have k points, we need to construct a k x k system of equations
    vector<vector<double>> A(k, vector<double>(k));
    vector<double> b(k);

    // Fill the matrix A and vector b using the x_values and y_values
    for (int i = 0; i < k; ++i) {
        int x = x_values[i];
        b[i] = static_cast<double>(y_values[i]); // Convert long long to double
        for (int j = 0; j < k; ++j) {
            A[i][j] = pow(x, k - 1 - j); // A[i][j] = x^j
        }
    }

    // Solve the system using Gaussian elimination
    vector<double> coefficients = gaussianElimination(A, b);

    // The constant term c is the last coefficient (because we constructed the matrix in reverse order)
    double c = coefficients[k - 1];
    cout << "The constant term c is: " << c << endl;

    return 0;
}
