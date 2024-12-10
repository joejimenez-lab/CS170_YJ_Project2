#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding random
#include <vector>  // For vector
#include <algorithm> // For find and remove
#include <iomanip> // For std::fixed and std::setprecision
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <set>
#include <limits>

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
    vector<double> features;
    int label;
};

// Function to parse the dataset from a file
void parseDataset(const string& filename, vector<Instance>& instances) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        double value;
        vector<double> features;
        int label;

        ss >> label; // Extract label (first column)
        while (ss >> value) { // Extract feature values
            features.push_back(value);
        }

        if (!features.empty() && features[0] == 0) {
            features.erase(features.begin()); // Remove extraneous initial values
        }

        instances.push_back({features, label});
    }

    file.close();
}

// Function to normalize features in the dataset
void normalizeFeatures(vector<Instance>& instances) {
    size_t numFeatures = instances[0].features.size();
    vector<double> minValues(numFeatures, numeric_limits<double>::max());
    vector<double> maxValues(numFeatures, numeric_limits<double>::lowest());

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
                instance.features[i] = 0.0;
            }
        }
    }
}

// Function to calculate Euclidean distance between two feature vectors
double calculateDistance(const vector<double>& a, const vector<double>& b) {
    double distance = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += pow(a[i] - b[i], 2);
    }
    return sqrt(distance);
}
vector<Instance> filterFeatures(const vector<Instance>& data, const set<int>& selectedFeatures) {
    vector<Instance> filteredData;

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
        trainingSet.erase(trainingSet.begin() + i);

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

        if (predictedLabel == testInstance.label) {
            ++correctPredictions;
        }
    }

    return static_cast<double>(correctPredictions) / data.size() * 100.0;
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

    vector<int> selectedFeatures;
    double bestOverallAccuracy = 0.0;

    for (int i = 1; i <= totalFeatures; ++i) {
        int bestFeature = -1;
        double bestAccuracy = 0.0;

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
                    bestFeature = feature;
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
            break;
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



int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator for consistent results

    vector<Instance> instances;
    string datasetFilename;

    cout << "Welcome to Joe's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    cin >> datasetFilename;

    parseDataset(datasetFilename, instances);
    normalizeFeatures(instances);

    int totalFeatures = instances[0].features.size();

    cout << "Type the number of the algorithm you want to run." << endl << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backward Elimination" << endl;
    cout << "3. Special Algorithm (Not Implemented)" << endl;

    int choice;
    cin >> choice;

    cout << endl << endl;

    if (choice == 1) {
        forwardSelection(instances, totalFeatures);
    } else if (choice == 2) {
        backwardElimination(instances, totalFeatures);
    } else if (choice == 3) {
        cout << "Special Algorithm is not implemented yet." << endl;
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    return 0;
}