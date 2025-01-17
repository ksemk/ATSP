import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Ścieżka do pliku CSV (zmień na odpowiednią ścieżkę)
file_path = "../results/resultsGA_53x53_rates.csv"

# Wczytywanie pliku CSV
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

# Przycięcie danych do maksymalnie 50 pierwszych wierszy na rozmiar populacji
def limit_data(data):
    return data.groupby("populationSize").head(50)

# Rysowanie wykresu czasu wykonania w zależności od rozmiaru populacji
def plot_execution_time(data, output_path):
    # Grupowanie danych po rozmiarze populacji i obliczanie średniego czasu wykonania
    avg_time_data = data.groupby("populationSize")["time"].mean()
    
    # Wykres łączący punkty
    plt.plot(avg_time_data.index, avg_time_data.values, marker="o", linestyle="-", color="blue", label="Średni czas wykonania")
    
    # Opis osi i tytułów
    plt.title("Czas wykonania w zależności od rozmiaru populacji")
    plt.xlabel("Rozmiar populacji")
    plt.ylabel("Średni czas wykonania (s)")
    plt.grid(True, linestyle="--", alpha=0.7)
    
    # Dodanie legendy
    plt.legend(loc="upper left", fontsize=10)
    
    # Zapis wykresu do pliku PNG
    plt.tight_layout()
    plt.gcf().set_size_inches(10, 6)
    plt.savefig(output_path)
    print(f"Wykres zapisano do pliku: {output_path}")
    plt.close()

# Główna funkcja
def main():
    data = read_csv_file(file_path)
    if data is not None:
        print("Dane załadowane pomyślnie.")
        limited_data = limit_data(data)
        output_path = "execution_time_connected_plot.png"
        plot_execution_time(limited_data, output_path)

if __name__ == "__main__":
    main()
