import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def REP_vs_problemSize(table_path):
    # Load the result table
    data = pd.read_csv(table_path)

    # Create a line plot for Relative Error Percentage (REP) vs. Problem Size
    plt.figure(figsize=(10, 6))
    sns.lineplot(x='city_size', y='mean_relative_error_percent', data=data, marker='o', label='Mean REP (%)')

    # Add plot labels and title
    plt.title('Relative Error Percentage (REP) vs Problem Size for Tabu Search', fontsize=14)
    plt.xlabel('City Size', fontsize=12)
    plt.ylabel('Mean Relative Error Percentage (%)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()

    # Save the plot to a file
    plt.savefig('results/REP_vs_ProblemSize_TS.png')
    plt.show()

def executionTime_vs_problemSize(table_path):
    # Read all data
    data = pd.read_csv(table_path)

    # Plot execution time
    plt.figure(figsize=(10, 6))
    sns.lineplot(x='city_size', y='mean_elapsed_time', data=data, marker='o', label='Execution Time')

    # Add plot labels and title
    plt.title('Execution Time vs Problem Size for Tabu Search', fontsize=14)
    plt.xlabel('City Size', fontsize=12)
    plt.ylabel('Execution Time (ms)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()

    # Save the plot
    plt.savefig('results/ExecutionTime_vs_ProblemSize_TS.png')
    plt.show()


def main():
    REP_vs_problemSize('results/tabuResultsTable.csv')
    executionTime_vs_problemSize('results/tabuResultsTable.csv')
if __name__ == '__main__':
    main()