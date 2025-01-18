import pandas as pd
import matplotlib.pyplot as plt

# Path to the CSV file (change to the appropriate path)
file_path = "../results/resultsGA_53x53_rates.csv"

# Reading the CSV file
def read_csv_file(file_path):
    try:
        data = pd.read_csv(file_path, names=[
            "algoName", "problemSize", "time", "bestCost", "populationSize",
            "mutationRate", "randomRate", "crossoverRate", "mutationType",
            "iterationNum", "crossingSegmentSizeRate", "mutationSegmentSizeRate",
            "randomRateNewGen"
        ])
        return data
    except Exception as e:
        print(f"Error reading file: {e}")
        return None

# Limiting the data to a maximum of the first 50 rows per population size
def limit_data(data):
    return data.groupby("populationSize").head(50)

# Plotting the execution time versus population size
def plot_execution_time(data, output_path):
    # Grouping data by population size and calculating the average execution time
    avg_time_data = data.groupby("populationSize")["time"].mean()
    std_time_data = data.groupby("populationSize")["time"].std()
    times = avg_time_data.index
    
    # Line plot
    plt.plot(avg_time_data.index, avg_time_data.values, marker="o", linestyle="", color="blue", label="Average execution time")
    
    # Axis labels and titles
    plt.figure(figsize=(10, 6))
    plt.errorbar(times, avg_time_data.values, yerr=std_time_data.values, fmt='o', label='Average execution time', markersize=5, capsize=5)
    plt.title("Execution time versus population size")
    plt.xlabel("Population size")
    plt.ylabel("Average execution time (s)")
    plt.grid(True, linestyle="--", alpha=0.7)
    
    # Adding legend
    plt.legend(loc="upper left", fontsize=10)
    
    # Saving the plot to a PNG file
    plt.tight_layout()
    plt.gcf().set_size_inches(10, 6)
    plt.savefig(output_path)
    print(f"Plot saved to file: {output_path}")
    plt.close()

# Main function
def main():
    data = read_csv_file(file_path)
    if data is not None:
        print("Data loaded successfully.")
        limited_data = limit_data(data)
        output_path = "execution_time_plot.png"
        plot_execution_time(limited_data, output_path)

if __name__ == "__main__":
    main()
