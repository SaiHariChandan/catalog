#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <fstream>
#include <nlohmann/json.hpp> // Include the JSON library

using namespace std;
using json = nlohmann::json;

// Function to compute GCD using the Euclidean algorithm
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to decode the value in the given base
long long decode_value(const string& value, int base) {
    return stoll(value, nullptr, base);
}

// Structure for handling fractions
struct fraction {
    long long num, den;

    // Constructor
    fraction(long long n, long long d) : num(n), den(d) {
        reduce_fraction();
    }

    // Reduce the fraction using GCD
    void reduce_fraction() {
        long long g = gcd(num, den);
        num /= g;
        den /= g;
    }

    // Multiplication of two fractions
    fraction operator*(const fraction& f) const {
        return fraction(num * f.num, den * f.den);
    }

    // Addition of two fractions
    fraction operator+(const fraction& f) const {
        return fraction(num * f.den + den * f.num, den * f.den);
    }

    // Function to return the numerator of the fraction
    long long get_num() const {
        return num;
    }
};

// Lagrange interpolation to recover the constant term 'c'
long long generate_secret(int x[], int y[], int M) {
    fraction ans(0, 1);

    // Iterate through the given points
    for (int i = 0; i < M; ++i) {
        fraction l(y[i], 1);
        for (int j = 0; j < M; ++j) {
            if (i != j) {
                fraction temp(-x[j], x[i] - x[j]);
                l = l * temp;
            }
        }
        ans = ans + l;
    }
    // Return the secret as an integer
    return ans.get_num();
}

// Main function
int main() {
    // Declare variables for number of points and minimum required
    int n = 0;
    int k = 0;

    // Read JSON input file
    ifstream input_file("input.json");
    if (!input_file) {
        cerr << "Unable to open input file." << endl;
        return 1;
    }

    json input_json;
    input_file >> input_json;

    n = input_json["n"];
    k = input_json["k"];

    // Prepare the points from the JSON input
    vector<pair<int, string>> points;
    for (const auto& point : input_json["points"]) {
        int base = point["base"];
        string value = point["value"];
        points.push_back({base, value});
    }

    // Allocate arrays for x and y values
    int* x = new int[k];
    int* y = new int[k];

    // Decode the values from the given base
    for (int i = 0; i < k; ++i) {
        long long decoded_value = decode_value(points[i].second, points[i].first);
        x[i] = i + 1; // Using sequential x values starting from 1
        y[i] = decoded_value;
    }

    // Get the secret constant term 'c'
    long long secret = generate_secret(x, y, k);

    cout << "The constant term (secret) is: " << -1 * secret << endl;

    // Clean up
    delete[] x;
    delete[] y;

    return 0;
}
