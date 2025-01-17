import pandas as pd
import matplotlib.pyplot as plt

# Ścieżka do pliku CSV (zmień na odpowiednią ścieżkę)
file_path = "../results/resultsGA_53x53.csv"

# Wczytywanie pliku CSV
def read_csv_file(file_path):
    try:
        data = pd.read_csv(file_path, names=[
            "algoName", "problemSize", "time", "bestCost", "populationSize",
            "mutationRate", "randomRate", "crossoverRate", "mutationType",
            "iterationNum", "crossingSegmentSizeRate", "mutationSegmentSizeRate",
            "randomRateNewGen"
        ])
        # Obliczenie współczynnika offspring rate
        data["offspringRate"] = 100 - data["mutationRate"] - data["randomRate"]
        return data
    except Exception as e:
        print(f"Error reading file: {e}")
        return None

# Przycięcie danych do maksymalnie 50 pierwszych wierszy na współczynnik offspring rate
def limit_data(data):
    return data.groupby("offspringRate").head(50)

# Rysowanie wykresu pudełkowego z liniami docelowymi
def plot_boxplot_with_target_lines(data, correct_answers, output_path):
    # Wykres pudełkowy dla współczynnika offspring rate
    data.boxplot(column="bestCost", by="offspringRate", grid=True, showmeans=True)
    
    # Obliczanie wartości ekstremalnych (minimum i maksimum) w danych
    min_cost = data["bestCost"].min()
    max_cost = data["bestCost"].max()
    
    # Dodanie linii docelowych dla odpowiadającego rozmiaru problemu
    for problem_size, correct_cost in correct_answers:
        if problem_size == data["problemSize"].iloc[0]:  # Sprawdzenie, czy problem_size w danych pasuje
            plt.axhline(
                y=correct_cost, color="red", linestyle="--", linewidth=1, alpha=0.7,
                label=f"Docelowy koszt dla problemu {problem_size}: {correct_cost}"
            )
            break  # Dodajemy tylko jedną linię i przerywamy pętlę
    
    # Dodanie informacji o ekstremalnych wartościach do legendy
    plt.axhline(
        y=min_cost, color="blue", linestyle=":", linewidth=1, alpha=0.7,
        label=f"Minimalny koszt: {min_cost}"
    )
    
    # Opis osi i tytułów
    plt.title("Wykres pudełkowy: Wartość najkrótszej ścieżki z analizą współczynnika offspring rate")
    plt.suptitle("")
    plt.xlabel("Offspring rate, %")
    plt.ylabel("Koszt najkrótszej ścieżki (mniej jest lepiej)")
    plt.grid(True, linestyle="--", alpha=0.7)
    
    # Dodanie legendy
    plt.legend(loc="upper right", fontsize=8)
    
    # Zapis wykresu do pliku PNG
    plt.tight_layout()
    plt.gcf().set_size_inches(10, 6)
    plt.savefig(output_path)
    print(f"Wykres zapisano do pliku: {output_path}")
    plt.close()

# Wartości prawidłowe
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

# Główna funkcja
def main():
    data = read_csv_file(file_path)
    if data is not None:
        print("Dane załadowane pomyślnie.")
        limited_data = limit_data(data)
        output_path = "boxplot_with_target_lines_offspring_rate.png"
        plot_boxplot_with_target_lines(limited_data, correct_answers, output_path)

if __name__ == "__main__":
    main()
