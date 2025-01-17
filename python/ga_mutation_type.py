import os
import pandas as pd
import matplotlib.pyplot as plt

# Ścieżka do folderu z plikami CSV
folder_path = "../results/results_mut_type_bigger"

# Wartości poprawne (dla każdego rozmiaru problemu)
correct_answers = {
    17: 39,
    34: 1286,
    39: 1530,
    45: 1613,
    48: 14422,
    53: 6905,
    65: 1839,
    71: 1950,
}

# Wczytywanie wszystkich plików CSV z folderu
def load_csv_files(folder_path):
    all_files = [os.path.join(folder_path, file) for file in os.listdir(folder_path) if file.endswith(".csv")]
    data_frames = []
    for file in all_files:
        try:
            data = pd.read_csv(file, names=[
                "algoName", "problemSize", "time", "bestCost", "populationSize",
                "mutationRate", "randomRate", "crossoverRate", "mutationType",
                "iterationNum", "crossingSegmentSizeRate", "mutationSegmentSizeRate",
                "randomRateNewGen"
            ])
            data_frames.append(data)
        except Exception as e:
            print(f"Error reading file {file}: {e}")
    return pd.concat(data_frames, ignore_index=True) if data_frames else None

# Obliczanie średniego błędu względnego dla każdego rozmiaru problemu i typu mutacji
def calculate_relative_error(data):
    results = []
    grouped = data.groupby(["problemSize", "mutationType"])
    for (problem_size, mutation_type), group in grouped:
        correct_value = correct_answers.get(problem_size)
        if correct_value:
            avg_best_cost = group["bestCost"].mean()
            relative_error = ((avg_best_cost - correct_value) / correct_value) * 100
            results.append((problem_size, mutation_type, relative_error))
    return pd.DataFrame(results, columns=["problemSize", "mutationType", "relativeError"])

# Rysowanie wykresów dla dwóch typów mutacji
# Rysowanie wykresów dla dwóch typów mutacji i zapis do pliku PNG
def plot_relative_errors(data, output_path):
    plt.figure(figsize=(10, 6))

    for mutation_type in ["i", "s"]:
        subset = data[data["mutationType"] == mutation_type]
        plt.plot(
            subset["problemSize"], 
            subset["relativeError"], 
            marker="o", 
            label=f"Mutation: {mutation_type}"
        )

    # Opis osi i tytułów
    plt.title("Porównanie błędu względnego dla różnych typów mutacji dla małego segmentu (40% chromosomy)")
    plt.xlabel("Rozmiar problemu")
    plt.ylabel("Błąd względny [%]")
    plt.axhline(y=0, color="black", linestyle="--", linewidth=0.8, alpha=0.7, label="Brak błędu")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend()
    plt.tight_layout()

    # Zapis do pliku PNG
    plt.savefig(output_path, dpi=300)
    print(f"Wykres zapisano do pliku: {output_path}")
    plt.close()

# Główna funkcja
def main():
    data = load_csv_files(folder_path)
    if data is not None:
        print("Wczytano dane z plików CSV.")
        
        # Obliczanie błędu względnego
        relative_error_data = calculate_relative_error(data)
        
        # Ścieżka do zapisu wykresu
        output_path = "relative_error_comparison_bigger.png"
        
        # Rysowanie wykresów i zapis do pliku
        plot_relative_errors(relative_error_data, output_path)
    else:
        print("Brak danych do analizy.")

if __name__ == "__main__":
    main()
