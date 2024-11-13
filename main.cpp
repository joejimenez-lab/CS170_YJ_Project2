#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random

using namespace std;

// Function declarations
// Stub evaluation function
double evaluateFeatureSet(const vector<int>& featureSet) {
    // Generate a random score for now
    return rand() % 100 + 1; // Random score between 1 and 100
}
void forwardSelection(int totalFeatures) {
    cout << "Starting Forward Selection...\n";

    vector<int> selectedFeatures; // Keep track of selected features
    double bestAccuracy = 0.0;    // Best accuracy so far

    for (int i = 0; i < totalFeatures; ++i) {
        int bestFeature = -1;   // Store the feature giving the best score
        double maxAccuracy = 0; // Store the best score in this iteration

        cout << "Evaluating features...\n";
        for (int feature = 1; feature <= totalFeatures; ++feature) {
            // Skip features already selected
            if (find(selectedFeatures.begin(), selectedFeatures.end(), feature) != selectedFeatures.end()) {
                continue;
            }

            // Simulate evaluation with a stub function
            vector<int> tempSet = selectedFeatures;
            tempSet.push_back(feature);
            double accuracy = evaluateFeatureSet(tempSet);

            cout << "Using feature(s) { ";
            for (int f : tempSet) cout << f << " ";
            cout << "} accuracy is " << accuracy << "%\n";

            if (accuracy > maxAccuracy) {
                maxAccuracy = accuracy;
                bestFeature = feature;
            }
        }

        if (bestFeature != -1) {
            selectedFeatures.push_back(bestFeature);
            bestAccuracy = maxAccuracy;
            cout << "Feature set { ";
            for (int f : selectedFeatures) cout << f << " ";
            cout << "} was best, accuracy is " << bestAccuracy << "%\n";
        } else {
            cout << "No further improvements possible.\n";
            break;
        }
    }

    cout << "Finished search!! The best feature subset is { ";
    for (int f : selectedFeatures) cout << f << " ";
    cout << "}, which has an accuracy of " << bestAccuracy << "%\n";
}
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
