import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.preprocessing import MinMaxScaler

# Define column names based on the structure of your CSV
columns = [
    "algoName", "problemSize", "time", "bestCost", "populationSize",
    "mutationRate", "randomRate", "crossoverRate", "mutationType",
    "iterationNum", "crossingSegmentSizeRate", "mutationSegmentSizeRate",
    "randomRateNewGen"
]

# Load the CSV without headers and assign column names
file_path = "../results/resultsGA_53x53_testing.csv"  # Replace with your file path
df = pd.read_csv(file_path, header=None, names=columns)

# Sort by 'bestCost' in ascending order and pick the row with the lowest cost
df = df.sort_values(by="bestCost", ascending=True)
selected_row = df.iloc[0]  # Row with the best (lowest) cost

# Define the parameters to include in the radar chart (excluding 'mutationType')
parameters = [
    "populationSize", "mutationRate", "randomRate", "crossoverRate",
    "iterationNum", "crossingSegmentSizeRate", "mutationSegmentSizeRate",
    "randomRateNewGen"
]

# Calculate max value for normalization (100 for rates, max value for others)
max_values = {param: 100 if "Rate" in param else df[param].max() for param in parameters}

# Normalize the selected row
normalized_values = [
    (selected_row[param] / max_values[param]) * 100 for param in parameters
]

# Append the first value to close the radar chart loop
normalized_values = np.append(normalized_values, normalized_values[0])

# Define labels and close the loop for labels
labels = parameters + [parameters[0]]

# Create the angles for the radar chart
angles = np.linspace(0, 2 * np.pi, len(labels), endpoint=True)

# Colors for each parameter
colors = [
    "red", "blue", "green", "purple", "orange",
    "brown", "pink", "cyan"
]

# Create the radar chart
fig, ax = plt.subplots(figsize=(10, 10), subplot_kw=dict(polar=True))

# Plot the radar chart with normalized values
ax.plot(angles, normalized_values, color="blue", linewidth=2, linestyle="solid")
ax.fill(angles, normalized_values, color="blue", alpha=0.25)

# Hide parameter names on the chart
ax.set_xticks([])
ax.set_yticks([])

# Add a legend with parameter names, ranges, and colors
legend_entries = [
    f"{param}: {selected_row[param]} (Range: 0 - {max_values[param]}) ({colors[i]})"
    for i, param in enumerate(parameters)
]
plt.legend(
    legend_entries,
    loc="upper left",
    bbox_to_anchor=(1.1, 1.05),
    frameon=True,
    fontsize=9,
    title="Parameter Values and Ranges"
)

# Add the bestCost value in the middle of the chart
ax.text(0, 0, f"Target Best Cost:\n{selected_row['bestCost']}", ha="center", va="center", fontsize=14, color="black")

# Title
plt.title("Radar Chart for GA Parameters (Normalized)", size=16)

# Adjust layout and show the plot
plt.tight_layout()
plt.show()
