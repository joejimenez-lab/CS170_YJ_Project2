#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random
#include <vector>  // For vector
#include <algorithm> // For find and remove
#include <iomanip> // For std::fixed and std::setprecision
#include <iterator> // For ostream_iterator

using namespace std;

// Function declarations
// Stub evaluation function
double evaluateFeatureSet(const vector<int>& featureSet) {
    return (rand() % 10000) / 100.0; // Generates a random floating-point value between 0.00 and 99.99
}

bool isFeatureSelected(const vector<int>& selectedFeatures, int feature) {
    // Returns true if the feature is already in the selectedFeatures list
    return std::find(selectedFeatures.begin(), selectedFeatures.end(), feature) != selectedFeatures.end();
}

void forwardSelection(int totalFeatures) {
    cout << "Using no features and \"random\" evaluation, I get an accuracy of " 
         << fixed << setprecision(1) << evaluateFeatureSet({}) << "%" << endl;
    cout << "Beginning search." << endl;

    vector<int> selectedFeatures; // Keep track of selected features
    double bestAccuracy = 0.0;    // Best accuracy so far

    for (int i = 0; i < totalFeatures; ++i) {
        int bestFeature = -1;   // Store the feature giving the best score
        double maxAccuracy = 0; // Store the best score in this iteration

        for (int feature = 1; feature <= totalFeatures; ++feature) {
            // Check if the feature is already selected
            if (!isFeatureSelected(selectedFeatures, feature)) {
                // Simulate evaluation with a stub function
                vector<int> tempSet = selectedFeatures;
                tempSet.push_back(feature);
                double accuracy = evaluateFeatureSet(tempSet);

                cout << "Using feature(s) {";
                copy(tempSet.begin(), tempSet.end(), ostream_iterator<int>(cout, ","));
                cout << "\b} accuracy is " << fixed << setprecision(1) << accuracy << "%" << endl;

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

            bestAccuracy = maxAccuracy;
            cout << "Feature set {";
            copy(selectedFeatures.begin(), selectedFeatures.end(), ostream_iterator<int>(cout, ","));
            cout << "\b} was best, accuracy is " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else {
            cout << "No further improvements possible." << endl;
            break;
        }
    }

    cout << "Finished search!! The best feature subset is {";
    copy(selectedFeatures.begin(), selectedFeatures.end(), ostream_iterator<int>(cout, ","));
    cout << "\b}, which has an accuracy of " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
}

void backwardElimination(int totalFeatures) {
    cout << "Using all features and \"random\" evaluation, I get an accuracy of " 
         << fixed << setprecision(1) << evaluateFeatureSet(vector<int>(totalFeatures)) << "%" << endl;
    cout << "Beginning search." << endl;

    vector<int> selectedFeatures;
    for (int i = 1; i <= totalFeatures; ++i) {
        selectedFeatures.push_back(i); // Start with all features
    }

    double bestAccuracy = evaluateFeatureSet(selectedFeatures); // Initial accuracy
    while (selectedFeatures.size() > 1) {
        int worstFeature = -1;  // Feature to remove
        double maxAccuracy = 0; // Accuracy after removal

        for (size_t i = 0; i < selectedFeatures.size(); ++i) {
            int feature = selectedFeatures[i];
            
            // Create a temporary subset without the current feature
            vector<int> tempSet = selectedFeatures;
            tempSet.erase(tempSet.begin() + i);
            double accuracy = evaluateFeatureSet(tempSet);

            cout << "Using feature(s) {";
            copy(tempSet.begin(), tempSet.end(), ostream_iterator<int>(cout, ","));
            cout << "\b} accuracy is " << fixed << setprecision(1) << accuracy << "%" << endl;

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

            cout << "Feature set {";
            copy(selectedFeatures.begin(), selectedFeatures.end(), ostream_iterator<int>(cout, ","));
            cout << "\b} was best, accuracy is " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else {
            cout << "No further improvements possible." << endl;
            break;
        }
    }

    cout << "Finished search!! The best feature subset is {";
    copy(selectedFeatures.begin(), selectedFeatures.end(), ostream_iterator<int>(cout, ","));
    cout << "\b}, which has an accuracy of " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    cout << "Welcome to Joe's and Yahirs Feature Selection Algorithm." << endl << endl;
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
