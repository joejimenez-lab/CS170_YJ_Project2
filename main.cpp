#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random
#include <vector>  // For vector
#include <algorithm> // For find and remove
#include <iomanip> // For std::fixed and std::setprecision

using namespace std;

// Stub evaluation function to simulate feature subset evaluation
double evaluateFeatureSet(const vector<int>& featureSet) {
    return (rand() % 10000) / 100.0; // Generates a random floating-point value between 0.00 and 99.99
}

// Helper function to check if a feature is already in the selected set
bool isFeatureSelected(const vector<int>& selectedFeatures, int feature) {
    return std::find(selectedFeatures.begin(), selectedFeatures.end(), feature) != selectedFeatures.end();
}

// Function to print feature set
void printFeatureSet(const vector<int>& featureSet) {
    cout << "{";
    for (size_t i = 0; i < featureSet.size(); ++i) {
        cout << featureSet[i];
        if (i != featureSet.size() - 1) { // Add comma only if not the last element
            cout << ",";
        }
    }
    cout << "}";
}

// Forward Selection Algorithm
void forwardSelection(int totalFeatures) {
    cout << "Using no features and \"random\" evaluation, I get an accuracy of " 
         << fixed << setprecision(1) << evaluateFeatureSet({}) << "%" << endl;
    cout << "Beginning search." << endl;

    vector<int> selectedFeatures; // Vector to store the current best feature set
    double bestAccuracy = 0.0;    // Variable to track the highest accuracy achieved so far

    for (int i = 0; i < totalFeatures; ++i) {
        int bestFeature = -1;   // Store the feature that gives the best accuracy in this step
        double maxAccuracy = 0; // Store the highest accuracy in this step

        for (int feature = 1; feature <= totalFeatures; ++feature) {
            if (!isFeatureSelected(selectedFeatures, feature)) { // Skip already selected features
                vector<int> tempSet = selectedFeatures; // Copy the current feature set
                tempSet.push_back(feature); // Add the current feature for evaluation
                double accuracy = evaluateFeatureSet(tempSet); // Evaluate the new feature set

                // Print the trace for this evaluation
                cout << "Using feature(s) ";
                printFeatureSet(tempSet);
                cout << " accuracy is " << fixed << setprecision(1) << accuracy << "%" << endl;

                // Update the best feature for this step if it improves accuracy
                if (accuracy > maxAccuracy) {
                    maxAccuracy = accuracy;
                    bestFeature = feature;
                }
            }
        }

        if (bestFeature != -1) {
            selectedFeatures.push_back(bestFeature);

            if (maxAccuracy < bestAccuracy) {
                cout << "(Warning, Accuracy has decreased!)" << endl;
            }

            bestAccuracy = maxAccuracy; // Update the global best accuracy
            cout << "Feature set ";
            printFeatureSet(selectedFeatures);
            cout << " was best, accuracy is " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else {
            cout << "No further improvements possible." << endl;
            break;
        }
    }

    // Final output: Best feature subset and its accuracy
    cout << "Finished search!! The best feature subset is ";
    printFeatureSet(selectedFeatures);
    cout << ", which has an accuracy of " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
}

// Backward Elimination Algorithm
void backwardElimination(int totalFeatures) {
    cout << "Using all features and \"random\" evaluation, I get an accuracy of " 
         << fixed << setprecision(1) << evaluateFeatureSet(vector<int>(totalFeatures)) << "%" << endl;
    cout << "Beginning search." << endl;

    vector<int> selectedFeatures;
    for (int i = 1; i <= totalFeatures; ++i) {
        selectedFeatures.push_back(i); // Start with all features
    }

    double bestAccuracy = evaluateFeatureSet(selectedFeatures); // Evaluate the full set initially

    while (selectedFeatures.size() > 1) {
        int worstFeature = -1;  // Track the feature whose removal gives the best improvement
        double maxAccuracy = 0; // Track the accuracy after removing a feature

        for (size_t i = 0; i < selectedFeatures.size(); ++i) {
            int feature = selectedFeatures[i]; // Feature to evaluate removal

            // Create a temporary subset without the current feature
            vector<int> tempSet = selectedFeatures;
            tempSet.erase(tempSet.begin() + i);
            double accuracy = evaluateFeatureSet(tempSet); // Evaluate the new subset

            // Print the trace for this evaluation
            cout << "Using feature(s) ";
            printFeatureSet(tempSet);
            cout << " accuracy is " << fixed << setprecision(1) << accuracy << "%" << endl;

            // Update the worst feature for this step if accuracy improves
            if (accuracy > maxAccuracy) {
                maxAccuracy = accuracy;
                worstFeature = feature;
            }
        }

        if (worstFeature != -1) {
            selectedFeatures.erase(remove(selectedFeatures.begin(), selectedFeatures.end(), worstFeature), selectedFeatures.end());
            if (maxAccuracy < bestAccuracy) {
                cout << "(Warning, Accuracy has decreased!)" << endl;
            }
            bestAccuracy = maxAccuracy;

            cout << "Feature set ";
            printFeatureSet(selectedFeatures);
            cout << " was best, accuracy is " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else {
            cout << "No further improvements possible." << endl;
            break;
        }
    }

    // Final output: Best feature subset and its accuracy
    cout << "Finished search!! The best feature subset is ";
    printFeatureSet(selectedFeatures);
    cout << ", which has an accuracy of " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator for consistent results

    cout << "Welcome to Joe's and Yahir's Feature Selection Algorithm." << endl << endl;
    cout << "Please enter the total number of features: ";
    int totalFeatures;
    cin >> totalFeatures;
    cout << endl;

    while (totalFeatures <= 0) {
        cout << "Invalid number of features." << endl;
        cout << "Please enter the total number of features: ";
        cin >> totalFeatures;
        cout << endl;
    }

    cout << "Type the number of the algorithm you want to run." << endl << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backward Elimination" << endl;
    cout << "3. Special Algorithm (Not Implemented)" << endl;

    int choice;
    cin >> choice;

    cout << endl << endl;

    if (choice == 1) {
        forwardSelection(totalFeatures);
    } else if (choice == 2) {
        backwardElimination(totalFeatures);
    } else if (choice == 3) {
        cout << "Special Algorithm is not implemented yet." << endl;
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    return 0;
}