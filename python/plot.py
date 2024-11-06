import pandas as pd
import matplotlib.pyplot as plt
import glob
import numpy as np
from numpy.polynomial.polynomial import Polynomial

def read_and_average_csv(file_pattern):
    # Read all CSV files matching the pattern
    files = glob.glob(file_pattern)
    data_frames = [pd.read_csv(file, header=None, names=['algorithm', 'city_size', 'time']) for file in files]
    
    # Concatenate all data frames
    all_data = pd.concat(data_frames)
    
    # Round city sizes to the nearest integer and filter out non-integer sizes
    all_data['city_size'] = all_data['city_size'].round().astype(int)
    
    # Group by city size and calculate the average time
    average_data = all_data.groupby('city_size')['time'].mean().reset_index()
    
    return average_data

def plot_algorithm_performance(data, algorithm_name, filename):
    plt.figure(figsize=(10, 6))
    
    # Plot algorithm performance with dots only
    plt.plot(data['city_size'], data['time'], label=f'{algorithm_name} Data', marker='o', linestyle='-')
    
    # # Annotate each point with its value, slightly offset to avoid covering the line
    # for i in range(len(data)):
    #     plt.text(data['city_size'][i], data['time'][i], f'{data["time"][i]:.2f}', fontsize=9, ha='center')
    # Add labels and title
    plt.xlabel('City Size')
    plt.ylabel('Average Time (ms)')
    plt.title(f'{algorithm_name} Performance (Logarithmic scale)')
    plt.legend()
    # Set y-axis to log scale
    plt.yscale('log')
    
    # Add grid
    plt.grid(True)
    
    # Set x-axis ticks to natural numbers
    plt.xticks(np.arange(data['city_size'].min(), data['city_size'].max() + 1, 1))
    
    # Save the plot as a PNG file
    plt.savefig(f'../results/{filename}_log.png')

def plot_combined_performance(branch_and_bound_data, brute_force_data, filename):
    plt.figure(figsize=(10, 6))
    
    # Plot Branch and Bound data
    plt.plot(branch_and_bound_data['city_size'], branch_and_bound_data['time'], label='Branch and Bound', marker='o', linestyle='None')
    p_bnb = Polynomial.fit(branch_and_bound_data['city_size'], branch_and_bound_data['time'], deg=10)
    x_bnb = np.linspace(branch_and_bound_data['city_size'].min(), branch_and_bound_data['city_size'].max(), 100)
    y_bnb = p_bnb(x_bnb)
    plt.plot(x_bnb, y_bnb, label='Branch and Bound Polynomial Fit', linestyle='-')
    
    # # Annotate each point with its value for Branch and Bound
    # for i in range(len(branch_and_bound_data)):
    #     plt.text(branch_and_bound_data['city_size'][i], branch_and_bound_data['time'][i], f'{branch_and_bound_data["time"][i]:.2f}', fontsize=9, ha='center')
    
    # Plot Brute Force data
    plt.plot(brute_force_data['city_size'], brute_force_data['time'], label='Brute Force', marker='o', linestyle='None')
    p_bf = Polynomial.fit(brute_force_data['city_size'], brute_force_data['time'], deg=10)
    x_bf = np.linspace(brute_force_data['city_size'].min(), brute_force_data['city_size'].max(), 100)
    y_bf = p_bf(x_bf)
    plt.plot(x_bf, y_bf, label='Brute Force Polynomial Fit', linestyle='-')
    
    # # Annotate each point with its value for Brute Force
    # for i in range(len(brute_force_data)):
    #     plt.text(brute_force_data['city_size'][i], brute_force_data['time'][i], f'{brute_force_data["time"][i]:.2f}', fontsize=9, ha='center')
    
    # Add labels and title
    plt.xlabel('City Size')
    plt.ylabel('Average Time (ms)')
    plt.title('Branch and Bound vs Brute Force Performance')
    plt.legend()
    
    # Add grid
    plt.grid(True)
    
    # Set x-axis ticks to natural numbers
    plt.xticks(np.arange(branch_and_bound_data['city_size'].min(), branch_and_bound_data['city_size'].max() + 1, 1))
    
    # Save the plot as a PNG file
    plt.savefig(f'../results/{filename}.png')


def create_tables(branch_and_bound_data, brute_force_data):
    # Round times to 3 decimal places
    branch_and_bound_data['time'] = branch_and_bound_data['time'].apply(lambda x: f"{x:.3e}")
    brute_force_data['time'] = brute_force_data['time'].apply(lambda x: f"{x:.3e}")
    
    # Rename columns
    branch_and_bound_data.columns = ['Problem Size, [N]', 'Elapsed Time, [ms]']
    brute_force_data.columns = ['Problem Size, [N]', 'Elapsed Time, [ms]']
    
    # Create a table for Branch and Bound data
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.axis('tight')
    ax.axis('off')
    table_data = branch_and_bound_data.values
    table = ax.table(cellText=table_data, colLabels=branch_and_bound_data.columns, cellLoc='center', loc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1.2, 1.2)
    plt.title('Branch and Bound Algorithm')
    plt.savefig('../results/branch_and_bound_table.png')
    plt.close()

    # Create a table for Brute Force data
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.axis('tight')
    ax.axis('off')
    table_data = brute_force_data.values
    table = ax.table(cellText=table_data, colLabels=brute_force_data.columns, cellLoc='center', loc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1.2, 1.2)
    plt.title('Brute Force Algorithm')
    plt.savefig('../results/brute_force_table.png')
    plt.close()

def main():
    # Read and average data from CSV files
    branch_and_bound_data = read_and_average_csv('../results/resultsBNB_*.csv')
    brute_force_data = read_and_average_csv('../results/resultsBF_*.csv')
    
    # Plot the performance for Branch and Bound
    plot_algorithm_performance(branch_and_bound_data, 'Branch and Bound', 'branch_and_bound_performance')
    
    # Plot the performance for Brute Force
    plot_algorithm_performance(brute_force_data, 'Brute Force', 'brute_force_performance')
    
    # Plot both Branch and Bound and Brute Force performance together
    plot_combined_performance(branch_and_bound_data, brute_force_data, 'combined_performance')

    # Create tables for the data
    create_tables(branch_and_bound_data, brute_force_data)

if __name__ == '__main__':
    main()
