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

// Represents a single data instance with its features and label
struct Instance {
    vector<double> features; // The list of feature values for the instance
    int label;               // The class label (1 or 2)
};

// Reads the dataset file and stores the rows as instances of features and labels.
// The first column is treated as the label, and the rest are feature values.
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

        // Extract the label (first column)
        ss >> label;

        // Extract the feature values (remaining columns)
        while (ss >> value) {
            features.push_back(value);
        }

        // Remove any extraneous initial values (e.g., 0) from the feature list
        if (!features.empty() && features[0] == 0) {
            features.erase(features.begin());
        }

        // Add the instance to the dataset
        instances.push_back({features, label});
    }

    file.close();

    // Print the first 5 parsed instances for debugging and verification
    /*cout << "First 5 parsed instances:" << endl;
    for (size_t i = 0; i < min(instances.size(), size_t(5)); ++i) {
        cout << "Label: " << instances[i].label << " Features: ";
        for (double feature : instances[i].features) {
            cout << feature << " ";
        }
        cout << endl;
    }
    */
}

// Normalizes the feature values for all instances to the range [0, 1].
// This is done by computing the minimum and maximum for each feature across all instances
// and then scaling the values using the formula: (value - min) / (max - min).
void normalizeFeatures(vector<Instance>& instances) {
    size_t numFeatures = instances[0].features.size();
    vector<double> minValues(numFeatures, numeric_limits<double>::max());
    vector<double> maxValues(numFeatures, numeric_limits<double>::lowest());

    // Compute the minimum and maximum for each feature
    for (const auto& instance : instances) {
        for (size_t i = 0; i < numFeatures; ++i) {
            minValues[i] = min(minValues[i], instance.features[i]);
            maxValues[i] = max(maxValues[i], instance.features[i]);
        }
    }

    // Normalize each feature value for all instances
    for (auto& instance : instances) {
        for (size_t i = 0; i < numFeatures; ++i) {
            if (maxValues[i] != minValues[i]) { // Avoid division by zero
                instance.features[i] = (instance.features[i] - minValues[i]) / (maxValues[i] - minValues[i]);
            } else {
                instance.features[i] = 0.0; // If all values are the same, set the feature to 0
            }
        }
    }

    // Print the first 5 normalized instances for debugging
    /*
    cout << "First 5 normalized instances:" << endl;
    for (size_t i = 0; i < min(instances.size(), size_t(5)); ++i) {
        cout << "Label: " << instances[i].label << " Features: ";
        for (double feature : instances[i].features) {
            cout << feature << " ";
        }
        cout << endl;
    }
    */
}

// Filters the dataset to only include selected features based on their indices.
// Indices in `selectedFeatures` are 1-based and are adjusted to 0-based for internal processing.
vector<Instance> filterFeatures(const vector<Instance>& data, const set<int>& selectedFeatures) {
    vector<Instance> filteredData;

    for (const auto& instance : data) {
        vector<double> filteredFeatures;
        for (int feature : selectedFeatures) {
            filteredFeatures.push_back(instance.features[feature - 1]); // Adjust 1-based index to 0-based
        }
        filteredData.push_back({filteredFeatures, instance.label});
    }

    // Print the first 5 filtered instances for debugging
    /**
    cout << "First 5 filtered instances:" << endl;
    for (size_t i = 0; i < min(filteredData.size(), size_t(5)); ++i) {
        cout << "Label: " << filteredData[i].label << " Features: ";
        for (double feature : filteredData[i].features) {
            cout << feature << " ";
        }
        cout << endl;
    }
    */

    return filteredData;
}

// Calculates the Euclidean distance between two feature vectors.
// Sums the squared differences for each feature and takes the square root of the total.
double calculateDistance(const vector<double>& a, const vector<double>& b) {
    double distance = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += pow(a[i] - b[i], 2);
    }
    return sqrt(distance);
}

// Predicts the label of a test instance using the Nearest Neighbor algorithm.
// Finds the closest instance in the training set (based on Euclidean distance) and returns its label.
int predictLabel(const Instance& testInstance, const vector<Instance>& trainingSet) {
    double minDistance = numeric_limits<double>::max();
    int predictedLabel = -1;

    // Compare the test instance to each training instance
    for (const auto& trainInstance : trainingSet) {
        double distance = calculateDistance(testInstance.features, trainInstance.features);
        if (distance < minDistance) { // Update if a closer instance is found
            minDistance = distance;
            predictedLabel = trainInstance.label;
        }
    }
    return predictedLabel;
}

// Evaluates the classifier using Leave-One-Out Validation (LOO).
// For each instance in the dataset, uses the remaining instances as the training set and
// the current instance as the test instance, and checks the prediction.
double leaveOneOutValidation(vector<Instance>& data) {
    int correctPredictions = 0;

    cout << "Instance | Predicted: | Actual: | Distance | Result:" << endl;

    for (size_t i = 0; i < data.size(); ++i) {
        // Prepare the training set by excluding the current test instance
        vector<Instance> trainingSet = data;
        Instance testInstance = trainingSet[i];
        trainingSet.erase(trainingSet.begin() + i);

        // Predict the label for the test instance
        int predictedLabel = predictLabel(testInstance, trainingSet);
        double minDistance = calculateDistance(testInstance.features, trainingSet[0].features);

        // Determine whether the prediction is correct
        string result = (predictedLabel == testInstance.label) ? "Correct" : "Incorrect";

        // Output the prediction result for the current instance
        cout << setw(9) << i + 1 << " | "
             << setw(10) << predictedLabel << " | "
             << setw(7) << testInstance.label << " | "
             << setw(8) << fixed << setprecision(6) << minDistance << " | "
             << result << endl;

        if (predictedLabel == testInstance.label) {
            ++correctPredictions; // Increment count of correct predictions
        }
    }

    // Return the overall accuracy as a percentage
    return static_cast<double>(correctPredictions) / data.size() * 100.0;
}

int main() {
    vector<Instance> instances;
    string datasetFilename;

    // Prompt the user to select which dataset to use
    cout << "Please Select Which Dataset to Use: (Input number)" << endl;
    cout << "1. Small Dataset" << endl;
    cout << "2. Large Dataset" << endl;
    cout << "Choice: ";
    int datasetChoice;
    cin >> datasetChoice;

    // Determine the appropriate filename based on user choice
    if (datasetChoice == 1) {
        datasetFilename = "small-test-dataset.txt";
    } else if (datasetChoice == 2) {
        datasetFilename = "large-test-dataset.txt";
    } else {
        cerr << "Invalid choice. Exiting..." << endl;
        return 1;
    }

    // Parse the dataset
    parseDataset(datasetFilename, instances);

    // Normalize the dataset features
    normalizeFeatures(instances);

    // Allow the user to select the feature set
    cout << "Parse entire dataset or only certain features?" << endl;
    cout << "1. Entire dataset" << endl;
    if (datasetChoice == 1) {
        cout << "2. Only features {3, 5, 7}" << endl;
    } else {
        cout << "2. Only features {1, 15, 27}" << endl;
    }
    cout << "Choice: ";
    int featureChoice;
    cin >> featureChoice;

    // Filter the dataset to include only the selected features if applicable
    set<int> selectedFeatures;
    if (featureChoice == 2) {
        if (datasetChoice == 1) {
            selectedFeatures = {3, 5, 7};
        } else {
            selectedFeatures = {1, 15, 27};
        }
        instances = filterFeatures(instances, selectedFeatures);
    }

    // Perform Leave-One-Out Validation and output the accuracy
    double accuracy = leaveOneOutValidation(instances);
    cout << "Accuracy: " << fixed << setprecision(2) << accuracy << "%" << endl;

    return 0;
}
