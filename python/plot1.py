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
    plt.figure(figsize=(15, 9))
    
    # Plot algorithm performance with dots only
    plt.plot(data['city_size'], data['time'], label=f'{algorithm_name} Data', marker='o', linestyle='-')
    
    # # Annotate each point with its value
    # for i in range(len(data)):
    #     plt.text(data['city_size'][i], data['time'][i], f'{data["time"][i]:.2f}', fontsize=9, ha='center')
    
    # Add labels and title
    plt.xlabel('City Size')
    plt.ylabel('Average Time (ms)')
    plt.title(f'{algorithm_name} Performance (logarithmic scale)')
    plt.legend()
    
    # Add grid
    plt.grid(True)
    
    # Set x-axis ticks to natural numbers
    plt.xticks(np.arange(data['city_size'].min(), data['city_size'].max() + 1, 1))

    # Set y-axis to log scale
    plt.yscale('log')
    
    # Save the plot as a PNG file
    plt.savefig(f'../results/{filename}.png')
    
    # Show the plot
    plt.show()

def plot_combined_performance(branch_and_bound_data, brute_force_data, filename):
    plt.figure(figsize=(15, 9))
    
    # Plot Branch and Bound data
    plt.plot(branch_and_bound_data['city_size'], branch_and_bound_data['time'], label='Branch and Bound', marker='o', linestyle='None')
    p_bnb = Polynomial.fit(branch_and_bound_data['city_size'], branch_and_bound_data['time'], deg=10)
    x_bnb = np.linspace(branch_and_bound_data['city_size'].min(), branch_and_bound_data['city_size'].max(), 100)
    y_bnb = p_bnb(x_bnb)
    plt.plot(x_bnb, y_bnb, label='Branch and Bound Polynomial Fit', linestyle='-')
    
    # Plot Brute Force data
    plt.plot(brute_force_data['city_size'], brute_force_data['time'], label='Brute Force', marker='o', linestyle='None')
    p_bf = Polynomial.fit(brute_force_data['city_size'], brute_force_data['time'], deg=10)
    x_bf = np.linspace(brute_force_data['city_size'].min(), brute_force_data['city_size'].max(), 100)
    y_bf = p_bf(x_bf)
    plt.plot(x_bf, y_bf, label='Brute Force Polynomial Fit', linestyle='-')
    
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
    
    # Show the plot
    plt.show()

def plot_combined_performance_prediction(branch_and_bound_data, brute_force_data, filename):
    plt.figure(figsize=(15, 9))
    
    # Plot Branch and Bound data
    plt.plot(branch_and_bound_data['city_size'], branch_and_bound_data['time'], label='Branch and Bound', marker='o', linestyle='None')
    p_bnb = Polynomial.fit(branch_and_bound_data['city_size'], branch_and_bound_data['time'], deg=4)
    x_bnb = np.linspace(branch_and_bound_data['city_size'].min(), 18, 100)
    y_bnb = p_bnb(x_bnb)
    plt.plot(x_bnb, y_bnb, label='Branch and Bound Polynomial Fit', linestyle='-')
    
    # Plot Brute Force data
    plt.plot(brute_force_data['city_size'], brute_force_data['time'], label='Brute Force', marker='o', linestyle='None')
    p_bf = Polynomial.fit(brute_force_data['city_size'], brute_force_data['time'], deg=4)
    x_bf = np.linspace(brute_force_data['city_size'].min(), 18, 100)
    y_bf = p_bf(x_bf)
    plt.plot(x_bf, y_bf, label='Brute Force Polynomial Fit', linestyle='-')
    
    # Add labels and title
    plt.xlabel('City Size')
    plt.ylabel('Average Time (ms)')
    plt.title('Branch and Bound vs Brute Force Performance Prediction up to 18 Cities')
    plt.legend()
    
    # Add grid
    plt.grid(True)
    
    # Set x-axis ticks to natural numbers
    plt.xticks(np.arange(branch_and_bound_data['city_size'].min(), 19, 1))
    
    # Save the plot as a PNG file
    plt.savefig(f'../results/{filename}_prediction.png')
    
    # Show the plot
    plt.show()

def main():
    # Read and average data from CSV files
    branch_and_bound_data = read_and_average_csv('../results/resultsBNB_*.csv')
    brute_force_data = read_and_average_csv('../results/resultsBF_*.csv')
    
    # Debugging: Print the first few rows of the data to check the columns
    print("Branch and Bound Data:")
    print(branch_and_bound_data.head())
    print("Brute Force Data:")
    print(brute_force_data.head())
    
    # Plot the performance for Branch and Bound
    plot_algorithm_performance(branch_and_bound_data, 'Branch and Bound', 'branch_and_bound_performance')
    
    # Plot the performance for Brute Force
    plot_algorithm_performance(brute_force_data, 'Brute Force', 'brute_force_performance')
    
    # Plot both Branch and Bound and Brute Force performance together
    plot_combined_performance(branch_and_bound_data, brute_force_data, 'combined_performance')
    
    # Plot combined performance prediction up to 24 cities
    plot_combined_performance_prediction(branch_and_bound_data, brute_force_data, 'combined_performance_prediction')

if __name__ == '__main__':
    main()