#include "../include/main.h"

// Global variables
int numSamples;
bool isRandom, doBNB, doBF, doTabu, doGA;
int initial_size, final_size, minValue, maxValue, symmetricity, asymRangeMin, asymRangeMax, step, tabuSize, maxIterations;
std::string input_path, GAPath;

int main() {

    // Read the config file
    std::ifstream config_file("../config/config.json");
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config file." << std::endl;
        return 1;
    }
    // Parse the config file as JSON
    
    nlohmann::json config_json;
    config_file >> config_json;
    readConfig(config_json);

    try {
        if (isRandom) {
            for (int size = initial_size; size <= final_size; size += step) {
                Matrix mat(size);
                mat.generateRandomMatrix(size, minValue, maxValue, symmetricity, asymRangeMin, asymRangeMax);
                std::cout << "Random matrix generated:" << std::endl;
                mat.display();
                runMultipleAlgorithms(numSamples, mat);
            }
        } else {
            Matrix mat(0);

            mat.readFromFile(input_path);
            std::string file_name = input_path.substr(input_path.find_last_of("/\\") + 1);
            std::cout << "Matrix loaded from file: " << file_name << std::endl;
            mat.display();
            if (doBNB){
                BranchAndBound bnb(mat);
                Util util1;
                util1.getStartTime();
                bnb.runBranchAndBound();
                util1.getEndTime();
                std::cout << "Branch and Bound algorithm completed for sample " << file_name << std::endl;
                bnb.printSolution();
                util1.printElapsedTimeMilliseconds();
            }
            if (doBF){
                BruteForce bf(mat);
                Util util2;
                util2.getStartTime();
                bf.runBruteForce();
                util2.getEndTime();
                std::cout << "Brute Force algorithm completed for sample " << file_name << std::endl;
                bf.printSolution();
                util2.printElapsedTimeMilliseconds();
            }
            if (doTabu){
                runMultipleAlgorithms(numSamples, mat);
            }
            if (doGA){
                runMultipleAlgorithms(numSamples, mat);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout<<"Program completed successfully."<<std::endl;
    return 0;
}


void runMultipleAlgorithms(int numSamples, Matrix& mat) {
    for (int i = 0; i < numSamples; ++i) {
        std::cout << "Running sample " << (i + 1) << " of " << numSamples << std::endl;
        if(doBNB){
            // Run the Branch and Bound algorithm
            BranchAndBound bnb(mat);

            Util util1;
            util1.getStartTime();
            bnb.runBranchAndBound();
            util1.getEndTime();
            std::cout << "Branch and Bound algorithm completed for sample " << (i + 1) << std::endl;
            bnb.printSolution();
            util1.printElapsedTimeMilliseconds();
            util1.saveResults("../results/resultsBNB_" + std::to_string(mat.getSize()) + "x" + std::to_string(mat.getSize()) + ".csv", 
            "bnb", mat.getSize(), util1.returnElapsedTimeMilliseconds());
        };
        if(doBF){
            // Run the Brute Force algorithm
            BruteForce bf(mat);

            Util util2;
            util2.getStartTime();
            bf.runBruteForce();
            util2.getEndTime();
            std::cout << "Brute Force algorithm completed for sample " << (i + 1) << std::endl;
            bf.printSolution();
            util2.printElapsedTimeMilliseconds();
            util2.saveResults("../results/resultsBF_" + std::to_string(mat.getSize()) + "x" + std::to_string(mat.getSize()) + ".csv", 
            "bf", mat.getSize(), util2.returnElapsedTimeMilliseconds());
        };
        if(doTabu){
            // Run the Tabu Search algorithm
            TabuSearch tabu(mat, tabuSize, maxIterations);
            Util util3;
            util3.getStartTime();
            tabu.runTabuSearch();
            util3.getEndTime();
            std::cout << "Tabu Search algorithm completed for sample " << (i + 1) << std::endl;
            tabu.printSolutionTabu();
            util3.printElapsedTimeMilliseconds();
            util3.saveResultsTabuSearch("../results/resultsTabu_" + std::to_string(mat.getSize()) + "x" + std::to_string(mat.getSize()) + ".csv", 
            "tabu", mat.getSize(), util3.returnElapsedTimeMilliseconds(), tabu.getBestCost());
        }
        if(doGA){
            GeneticAlgorithm ga(mat, GAPath);
            Util util4;
            util4.getStartTime();
            ga.runGeneticAlgorithm();
            util4.getEndTime();
            std::cout << "Genetic algorithm completed for sample " << (i + 1) << std::endl;
            ga.printSolution();
            util4.printElapsedTimeMilliseconds();
            util4.saveResultsGA("../results/results_mut_type_bigger/resultsGA_" + std::to_string(mat.getSize()) + "x" + std::to_string(mat.getSize()) + ".csv", 
            "ga", mat.getSize(), util4.returnElapsedTimeMilliseconds(), ga.getBestSolution(), ga.getPopulationSize(),
            ga.getMutationRate(), ga.getCrossOverRate(), ga.getMutationType(), ga.getRandomRateNewGen(), ga.getIterationNum(),
            ga.getCrossingSegmentSizeRate(), ga.getMutationSegmentSizeRate(), ga.getRandomRate());
        }
    }
}


void readConfig(const nlohmann::json& config_json) {
    try {
        // Required configuration fields
        numSamples = config_json.at("configurations").at("numSamples").get<int>();
        isRandom = config_json.at("configurations").at("isMatrixRandom").get<bool>();
        doBNB = config_json.at("configurations").at("doBNB").get<bool>();
        doBF = config_json.at("configurations").at("doBF").get<bool>();
        doTabu = config_json.at("configurations").at("doTabu").get<bool>();
        tabuSize = config_json.at("configurations").at("tabuSearch").at("tabuSize").get<int>();
        maxIterations = config_json.at("configurations").at("tabuSearch").at("maxIterations").get<int>();
        doGA = config_json.at("configurations").at("doGA").get<bool>();
        GAPath = config_json.at("configurations").at("GA").at("GAPath").get<std::string>();

        // Conditional parsing based on whether the matrix is random or loaded from a file
        if (isRandom) {
            // Ensure that all expected fields for random matrix generation are present
            initial_size = config_json.at("configurations").at("matrixGeneration").at("initial_size").get<int>();
            final_size = config_json.at("configurations").at("matrixGeneration").at("final_size").get<int>();
            minValue = config_json.at("configurations").at("matrixGeneration").at("minValue").get<int>();
            maxValue = config_json.at("configurations").at("matrixGeneration").at("maxValue").get<int>();
            symmetricity = config_json.at("configurations").at("matrixGeneration").at("symmetricity").get<int>();
            asymRangeMin = config_json.at("configurations").at("matrixGeneration").at("asymRangeMin").get<int>();
            asymRangeMax = config_json.at("configurations").at("matrixGeneration").at("asymRangeMax").get<int>();
            step = config_json.at("configurations").at("matrixGeneration").at("step").get<int>();


        } else {
            // Ensure input file path is available when isMatrixRandom is false
            input_path = "../" + config_json.at("configurations").at("inputFilePath").get<std::string>();
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
        exit(1);
    }
}
