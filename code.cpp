#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <numeric>

using namespace std;

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
        long long gcd = std::gcd(num, den);
        num /= gcd;
        den /= gcd;
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
    // Define the number of points and the minimum required
    int n = 0;
    int k = 0;

    // Input format choice
    cout << "Enter number of points (n) and minimum required (k): ";
    cin >> n >> k;

    // Hardcoded map of points (with base and encoded value)
    map<int, pair<int, string>> test_case;

    // Reading input based on the defined number of points
    cout << "Enter " << n << " entries in the format (base value):" << endl;
    for (int i = 1; i <= n; ++i) {
        int base;
        string value;
        cin >> base >> value;
        test_case[i] = {base, value};
    }

    vector<pair<int, int>> points;

    // Decode the values from the given base
    for (auto& entry : test_case) {
        int x = entry.first;  // The key of the map
        int base = entry.second.first;  // The base
        string encoded_value = entry.second.second;  // The encoded value

        // Decode the encoded value using the base
        long long decoded_value = decode_value(encoded_value, base);
        points.push_back({x, decoded_value});
    }

    // Allocate arrays for x and y values
    int* x = new int[k];
    int* y = new int[k];

    // Use the first k points for Lagrange interpolation
    for (int i = 0; i < k; ++i) {
        x[i] = points[i].first;
        y[i] = points[i].second;
    }

    // Get the secret constant term 'c'
    long long secret = generate_secret(x, y, k);

    cout << "The constant term (secret) is: " << secret << endl;

    // Clean up
    delete[] x;
    delete[] y;

    return 0;
}
