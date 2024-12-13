import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the exported CSV file
def load_data(filename):
    return pd.read_csv(filename)

# Plot scatter plot for features
def plot_scatter(data, feature_x, feature_y, title):
    plt.figure(figsize=(8, 6))
    sns.scatterplot(
        x=data[feature_x],
        y=data[feature_y],
        hue=data["Label"],
        palette="viridis",  # Color palette for class labels
        style=data["Label"],
        s=50
    )
    plt.title(title)
    plt.xlabel(feature_x)
    plt.ylabel(feature_y)
    plt.legend(title="Class")
    plt.grid(True)
    plt.savefig(f"{title.replace(' ', '_')}.png")  # Save as PNG file
    plt.show()  # Display the plot

# Load datasets for good and bad features
good_data = load_data("good_features.csv")  # Replace with your CSV file for good features
bad_data = load_data("bad_features.csv")  # Replace with your CSV file for bad features

# Plot scatter plots for good features
plot_scatter(good_data, "Feature2", "Feature1", "Scatter Plot: Good Features (Feature2 vs Feature1, TITANIC DATASET)")

# Plot scatter plots for bad features
plot_scatter(bad_data, "Feature3", "Feature6", "Scatter Plot: Bad Features (Feature3 vs Feature6, TITANIC DATASET)")
