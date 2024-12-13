/*

- Group: Joe Jimenez - jjime206 and Yahir Amaral – yamar003 
- Small Dataset Results:  
- Forward: Feature Subset: {5, 3, 7, 1, 6, 2, 4, 9, 10, 8}, Acc: 68.0%  
- Backward: Feature Subset: {5}, Acc: 75.0%  
- Custom Algorithm: Feature subset: {5,3,7,1,6,2,4,9,10,8}, Acc: 68.0% 

- Large Dataset Results:  
- Forward: Feature Subset: {27,1,15,19,37,40,28,24,13,33,12,6,17,29,34,16,8,2,32,30,3,21,4,39,22,25,10,23,26,20,35,9,18,5,36,7,11,31,14,38}, Acc: 69.3%  
- Backward: Feature Subset: {27}, Acc: 84.7% 
- Custom Algorithm: Feature subset: {27,1,15,19,37,40,28,24,13,33,12,6,17,29,34,16,8,2,32,30,3,21,4,39,22,25,10,23,26,20,35,9,18,5,36,7,11,31,14,38}, Acc: 69.3% 

- Titanic Dataset Results:  
- Forward: Feature Subset: {2, 5, 4, 1, 3, 6}, Acc: 73.9%  
- Backward: Feature Subset: {2}, Acc: 78% 
- Custom Algorithm: Feature subset: {2,5,4,1,3,6}, Acc: 73.9% 

*/

#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random
#include <vector>  // For vector
#include <algorithm> // For find and remove
#include <iomanip> // For std::fixed and std::setprecision
#include <fstream> // For file operations
#include <sstream> // For string stream processing
#include <cmath>   // For mathematical operations
#include <set>     // For set data structure
#include <limits>  // For numeric limits

using namespace std;

// Stub evaluation function to simulate feature subset evaluation
double evaluateFeatureSet(const vector<int>& featureSet) {
    return (rand() % 10000) / 100.0; // Generates a random floating-point value between 0.00 and 99.99
}

// Helper function to check if a feature is already in the selected set
bool isFeatureSelected(const vector<int>& selectedFeatures, int feature) {
    return std::find(selectedFeatures.begin(), selectedFeatures.end(), feature) != selectedFeatures.end();
}

// Struct to represent an instance with features and label
struct Instance {
    vector<double> features; // Stores feature values
    int label;               // Class label
};

// Function to parse the dataset from a file
void parseDataset(const string& filename, vector<Instance>& instances) {
    ifstream file(filename); // Open the file
    if (!file.is_open()) { // Check if file opens successfully
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1); // Exit if file cannot be opened
    }

    string line;
    while (getline(file, line)) { // Read each line from file
        stringstream ss(line); // Process the line using stringstream
        double value;
        vector<double> features; // To store feature values
        int label; // To store class label

        ss >> label; // Extract label (first column)
        while (ss >> value) { // Extract feature values
            features.push_back(value);
        }

        if (!features.empty() && features[0] == 0) {
            features.erase(features.begin()); // Remove extraneous initial values
        }

        instances.push_back({features, label}); // Add instance to dataset
    }

    file.close(); // Close the file
}

// Function to normalize features in the dataset
void normalizeFeatures(vector<Instance>& instances) {
    size_t numFeatures = instances[0].features.size(); // Get number of features
    vector<double> minValues(numFeatures, numeric_limits<double>::max()); // Initialize minimum values
    vector<double> maxValues(numFeatures, numeric_limits<double>::lowest()); // Initialize maximum values

    // Find min and max for each feature
    for (const auto& instance : instances) {
        for (size_t i = 0; i < numFeatures; ++i) {
            minValues[i] = min(minValues[i], instance.features[i]);
            maxValues[i] = max(maxValues[i], instance.features[i]);
        }
    }

    // Normalize features
    for (auto& instance : instances) {
        for (size_t i = 0; i < numFeatures; ++i) {
            if (maxValues[i] != minValues[i]) {
                instance.features[i] = (instance.features[i] - minValues[i]) / (maxValues[i] - minValues[i]);
            } else {
                instance.features[i] = 0.0; // Set to 0 if min and max are the same
            }
        }
    }
}

// Function to calculate Euclidean distance between two feature vectors
double calculateDistance(const vector<double>& a, const vector<double>& b) {
    double distance = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += pow(a[i] - b[i], 2); // Sum of squared differences
    }
    return sqrt(distance); // Return square root of sum
}

// Filters dataset to retain only selected features
vector<Instance> filterFeatures(const vector<Instance>& data, const set<int>& selectedFeatures) {
    vector<Instance> filteredData; // Filtered dataset

    for (const auto& instance : data) {
        vector<double> filteredFeatures;
        for (int feature : selectedFeatures) {
            filteredFeatures.push_back(instance.features[feature - 1]); // Convert 1-based index to 0-based
        }
        filteredData.push_back({filteredFeatures, instance.label});
    }

    return filteredData;
}

// Leave-one-out validation function for accuracy computation
double leaveOneOutValidation(vector<Instance>& data, const vector<int>& featureSubset) {
    int correctPredictions = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        vector<Instance> trainingSet = data;
        Instance testInstance = trainingSet[i];
        trainingSet.erase(trainingSet.begin() + i); // Leave out one instance

        // Filter features for training and test sets
        for (auto& instance : trainingSet) {
            vector<double> filteredFeatures;
            for (int feature : featureSubset) {
                filteredFeatures.push_back(instance.features[feature - 1]);
            }
            instance.features = filteredFeatures;
        }
        vector<double> testFeatures;
        for (int feature : featureSubset) {
            testFeatures.push_back(testInstance.features[feature - 1]);
        }
        testInstance.features = testFeatures;

        // Find nearest neighbor
        double minDistance = numeric_limits<double>::max();
        int predictedLabel = -1;

        for (const auto& trainInstance : trainingSet) {
            double distance = calculateDistance(testInstance.features, trainInstance.features);
            if (distance < minDistance) {
                minDistance = distance;
                predictedLabel = trainInstance.label;
            }
        }

        if (predictedLabel == testInstance.label) { // Check if prediction is correct
            ++correctPredictions;
        }
    }

    return static_cast<double>(correctPredictions) / data.size() * 100.0; // Return accuracy
}

// Helper function to print feature sets
void printFeatureSet(const vector<int>& featureSet) {
    cout << "{";
    for (size_t i = 0; i < featureSet.size(); ++i) {
        cout << featureSet[i];
        if (i != featureSet.size() - 1) cout << ",";
    }
    cout << "}";
}

// Forward Selection Algorithm
void forwardSelection(vector<Instance>& data, int totalFeatures) {
    cout << "Running nearest neighbor with no features (default rate), using \"leave-one-out\" evaluation, I get an accuracy of "
         << fixed << setprecision(1) << leaveOneOutValidation(data, {}) << "%" << endl;

    cout << "Beginning search." << endl;

    vector<int> selectedFeatures; // Track selected features
    double bestOverallAccuracy = 0.0;

    for (int i = 1; i <= totalFeatures; ++i) {
        int bestFeature = -1;
        double bestAccuracy = 0.0;

        // Iterate through unselected features
        for (int feature = 1; feature <= totalFeatures; ++feature) {
            if (find(selectedFeatures.begin(), selectedFeatures.end(), feature) == selectedFeatures.end()) {
                vector<int> tempFeatures = selectedFeatures;
                tempFeatures.push_back(feature);

                double accuracy = leaveOneOutValidation(data, tempFeatures);
                cout << "Using feature(s) ";
                printFeatureSet(tempFeatures);
                cout << " accuracy is " << fixed << setprecision(1) << accuracy << "%" << endl;

                if (accuracy > bestAccuracy) {
                    bestAccuracy = accuracy;
                    bestFeature = feature; // Update best feature
                }
            }
        }

        if (bestFeature != -1) {
            selectedFeatures.push_back(bestFeature);
            if (bestAccuracy < bestOverallAccuracy) {
                cout << "(Warning, Accuracy has decreased!)" << endl;
            }
            bestOverallAccuracy = bestAccuracy;

            cout << "Feature set ";
            printFeatureSet(selectedFeatures);
            cout << " was best, accuracy is " << fixed << setprecision(1) << bestOverallAccuracy << "%" << endl;
        } else {
            break; // Stop if no features improve accuracy
        }
    }

    cout << "Finished search!! The best feature subset is ";
    printFeatureSet(selectedFeatures);
    cout << ", which has an accuracy of " << fixed << setprecision(1) << bestOverallAccuracy << "%" << endl;
}

// Backward Elimination Algorithm
void backwardElimination(vector<Instance> dataset, int totalFeatures) {
    // Start with all features
    vector<int> selectedFeatures;
    for (int i = 1; i <= totalFeatures; ++i) {
        selectedFeatures.push_back(i);
    }

    // Evaluate the full set initially
    double bestAccuracy = leaveOneOutValidation(dataset, selectedFeatures);

    cout << "Using all features and \"leave-one-out\" evaluation, I get an accuracy of "
         << fixed << setprecision(1) << bestAccuracy << "%" << endl;
    cout << "Beginning search." << endl;

    while (selectedFeatures.size() > 1) {
        int worstFeature = -1;  // Track the feature whose removal gives the best improvement
        double maxAccuracy = 0; // Track the best accuracy after removing a feature

        for (size_t i = 0; i < selectedFeatures.size(); ++i) {
            int feature = selectedFeatures[i]; // Feature to evaluate removal

            // Create a temporary subset without the current feature
            vector<int> tempSet = selectedFeatures;
            tempSet.erase(tempSet.begin() + i);

            // Evaluate accuracy for the new subset
            double accuracy = leaveOneOutValidation(dataset, tempSet);

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
            // Remove the identified "worst" feature permanently
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

// Bidirectional search combines forward selection and backward elimination
void bidirectionalSearch(vector<Instance>& data, int totalFeatures) {
    cout << "Starting Bidirectional Search..." << endl;

    vector<int> forwardSelectedFeatures; // Features selected during forward selection
    vector<int> backwardSelectedFeatures;
    for (int i = 1; i <= totalFeatures; ++i) {
        backwardSelectedFeatures.push_back(i); // Initialize with all features
    }

    double bestAccuracy = leaveOneOutValidation(data, {}); // Initial accuracy with no features
    vector<int> bestFeatureSet;

    while (!backwardSelectedFeatures.empty() || forwardSelectedFeatures.size() < totalFeatures) {
        int bestFeatureToAdd = -1, bestFeatureToRemove = -1;
        double bestForwardAccuracy = 0.0, bestBackwardAccuracy = 0.0;

        // Forward selection step
        for (int feature = 1; feature <= totalFeatures; ++feature) {
            if (find(forwardSelectedFeatures.begin(), forwardSelectedFeatures.end(), feature) == forwardSelectedFeatures.end()) {
                vector<int> tempFeatures = forwardSelectedFeatures;
                tempFeatures.push_back(feature);

                double accuracy = leaveOneOutValidation(data, tempFeatures);
                if (accuracy > bestForwardAccuracy) {
                    bestForwardAccuracy = accuracy;
                    bestFeatureToAdd = feature; // Feature to add
                }
            }
        }

        // Backward elimination step
        for (size_t i = 0; i < backwardSelectedFeatures.size(); ++i) {
            vector<int> tempFeatures = backwardSelectedFeatures;
            tempFeatures.erase(tempFeatures.begin() + i);

            double accuracy = leaveOneOutValidation(data, tempFeatures);
            if (accuracy > bestBackwardAccuracy) {
                bestBackwardAccuracy = accuracy;
                bestFeatureToRemove = backwardSelectedFeatures[i]; // Feature to remove
            }
        }

        // Decide the better action (add or remove)
        if (bestForwardAccuracy > bestBackwardAccuracy) {
            forwardSelectedFeatures.push_back(bestFeatureToAdd);
            backwardSelectedFeatures.erase(remove(backwardSelectedFeatures.begin(), backwardSelectedFeatures.end(), bestFeatureToAdd), backwardSelectedFeatures.end());
            bestAccuracy = bestForwardAccuracy;
            bestFeatureSet = forwardSelectedFeatures;
            cout << "Added feature " << bestFeatureToAdd << ", accuracy: " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else if (bestBackwardAccuracy >= bestForwardAccuracy) {
            backwardSelectedFeatures.erase(remove(backwardSelectedFeatures.begin(), backwardSelectedFeatures.end(), bestFeatureToRemove), backwardSelectedFeatures.end());
            forwardSelectedFeatures.erase(remove(forwardSelectedFeatures.begin(), forwardSelectedFeatures.end(), bestFeatureToRemove), forwardSelectedFeatures.end());
            bestAccuracy = bestBackwardAccuracy;
            bestFeatureSet = backwardSelectedFeatures;
            cout << "Removed feature " << bestFeatureToRemove << ", accuracy: " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
        } else {
            break; // Exit if neither action improves accuracy
        }
    }

    cout << "Finished Bidirectional Search! Best feature subset: ";
    printFeatureSet(bestFeatureSet);
    cout << " with accuracy: " << fixed << setprecision(1) << bestAccuracy << "%" << endl;
}

// Exports selected features to a CSV file
void exportSelectedFeatures(const vector<Instance>& instances, const vector<int>& selectedFeatures, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    // Write CSV header
    for (size_t i = 0; i < selectedFeatures.size(); ++i) {
        file << "Feature" << selectedFeatures[i];
        if (i < selectedFeatures.size() - 1) file << ",";
    }
    file << ",Label" << endl;

    // Write feature values and class labels
    for (const auto& instance : instances) {
        for (size_t i = 0; i < selectedFeatures.size(); ++i) {
            file << instance.features[selectedFeatures[i] - 1]; // Convert 1-based index to 0-based
            if (i < selectedFeatures.size() - 1) file << ",";
        }
        file << "," << instance.label << endl; // Add label
    }

    file.close();
    cout << "Exported selected features to " << filename << endl;
}

// Main function to drive the feature selection process
int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator for consistent results

    vector<Instance> instances; // Dataset instances
    string datasetFilename;

    cout << "Welcome to Joe's and Yahir's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    cin >> datasetFilename;

    parseDataset(datasetFilename, instances); // Parse dataset from file
    normalizeFeatures(instances); // Normalize feature values

    int totalFeatures = instances[0].features.size(); // Number of features in the dataset

    cout << "Type the number of the algorithm you want to run." << endl << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backward Elimination" << endl;
    cout << "3. Custom Bidirectional Algorithm" << endl;

    int choice;
    cin >> choice;

    cout << endl << endl;

    // Run the selected algorithm
    if (choice == 1) {
        forwardSelection(instances, totalFeatures);
    } else if (choice == 2) {
        backwardElimination(instances, totalFeatures);
    } else if (choice == 3) {
        bidirectionalSearch(instances, totalFeatures);
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }
    //exportSelectedFeatures(instances, {2, 1}, "good_features.csv"); // Features that separate well
    //exportSelectedFeatures(instances, {3, 6}, "bad_features.csv"); // Features that don’t separate well
    return 0;
}
