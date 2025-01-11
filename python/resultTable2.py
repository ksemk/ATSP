import pandas as pd
import glob
import numpy as np

def read_and_average_csv(file_pattern):
    # Read all CSV files matching the pattern
    files = glob.glob(file_pattern)
    if not files:  # No files found
        raise FileNotFoundError(f"No files match the pattern: {file_pattern}")
    
    data_frames = [pd.read_csv(file, header=None, names=['algorithm', 'city_size', 'time', 'best_path']) for file in files]
    
    # Concatenate all data frames
    all_data = pd.concat(data_frames)
    
    # Group by city size and calculate the average best path and mean elapsed time
    average_path = all_data.groupby('city_size').agg(
        best_path=('best_path', 'mean'),
        time=('time', 'mean')
    ).reset_index()
    
    # Round the time to 2 decimal places
    average_path['time'] = average_path['time'].round(2)
    
    return average_path


def generate_sorted_csv(file_pattern, correct_answers):
    # Read and average data from CSV files
    average_path = read_and_average_csv(file_pattern)
    
    # Convert correct_answers to DataFrame
    correct_answers_df = pd.DataFrame(correct_answers, columns=['city_size', 'correct_answer'])
    
    # Merge average_path with correct_answers_df on city_size
    result = pd.merge(correct_answers_df, average_path, on='city_size')
    
    # Calculate the absolute and relative errors
    result['absolute_error'] = np.ceil(abs(result['best_path'] - result['correct_answer']) * 100)/100
    result['relative_error_percent'] = np.ceil((result['absolute_error'] / result['correct_answer']) * 10000)/100
    
    # Group by city_size for mean errors
    result_summary = result.groupby('city_size').agg(
        correct_answer=('correct_answer', 'first'),
        mean_best_path=('best_path', 'mean'),
        mean_absolute_error=('absolute_error', 'mean'),
        mean_relative_error_percent=('relative_error_percent', 'mean'),
        mean_elapsed_time=('time', 'mean')
    ).reset_index()

    # Save the result to a CSV file
    result_summary.to_csv('results/tabuResultsTable.csv', index=False)

    
def main():
    correct_answers = [
        [17, 39],
        [34, 1286],
        [39, 1530],
        [45, 1613],
        [48, 14422],
        [53, 6905],
        [65, 1839],
        [71, 1950],
    ]   

    # Generate sorted CSV with error calculations
    generate_sorted_csv('results/resultsTabu_*.csv', correct_answers)

if __name__ == '__main__':
    main()
