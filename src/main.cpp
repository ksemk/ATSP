#include "../include/main.h"

// Global variables
int numSamples;
bool isRandom, doBNB, doBF;
int initial_size, final_size, minValue, maxValue, symmetricity, asymRangeMin, asymRangeMax, step;
std::string input_path;

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
                Matrix mat(0);
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
            BranchAndBound bnb(mat);
            Util util1;
            util1.getStartTime();
            bnb.runBranchAndBound();
            util1.getEndTime();
            std::cout << "Branch and Bound algorithm completed for sample " << file_name << std::endl;
            bnb.printSolution();
            util1.printElapsedTimeMilliseconds();

            BruteForce bf(mat);
            Util util2;
            util2.getStartTime();
            bf.runBruteForce();
            util2.getEndTime();
            std::cout << "Brute Force algorithm completed for sample " << file_name << std::endl;
            bf.printSolution();
            util2.printElapsedTimeMilliseconds();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


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
    }
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
        }
    }
}

void readConfig(const nlohmann::json& config_json) {
    try {
        // Required configuration fields
        numSamples = config_json.at("configurations").at("numSamples").get<int>();
        isRandom = config_json.at("configurations").at("isMatrixRandom").get<bool>();

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
            doBNB = config_json.at("configurations").at("doBNB").get<bool>();
            doBF = config_json.at("configurations").at("doBF").get<bool>();
        } else {
            // Ensure input file path is available when isMatrixRandom is false
            input_path = "../" + config_json.at("configurations").at("inputFilePath").get<std::string>();
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
        exit(1);
    }
}
