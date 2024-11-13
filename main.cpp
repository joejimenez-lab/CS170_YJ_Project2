#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random

using namespace std;

// Function declarations
void forwardSelection(int totalFeatures);
void backwardElimination(int totalFeatures);

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    cout << "Welcome to the Feature Selection Algorithm.\n";
    cout << "Please enter the total number of features: ";
    int totalFeatures;
    cin >> totalFeatures;

    if (totalFeatures <= 0) {
        cout << "Invalid number of features. Exiting.\n";
        return 1;
    }

    cout << "Type the number of the algorithm you want to run.\n";
    cout << "1. Forward Selection\n";
    cout << "2. Backward Elimination\n";
    cout << "3. Special Algorithm (Not Implemented)\n";

    int choice;
    cin >> choice;

    if (choice == 1) {
        forwardSelection(totalFeatures);
    } else if (choice == 2) {
        backwardElimination(totalFeatures);
    } else if (choice == 3) {
        cout << "Special Algorithm is not implemented yet.\n";
    } else {
        cout << "Invalid choice. Exiting.\n";
        return 1;
    }

    return 0;
}
