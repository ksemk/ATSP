#include "main.h"

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

    try {
        // Extract the input path from the JSON object
        std::string input_path = "../" + config_json.at("configurations").at("inputFilePath").get<std::string>();

        Matrix mat(0); // Initialize with size 0, as it will be set from the file
        mat.readFromFile(input_path);

        std::string file_name = input_path.substr(input_path.find_last_of("/\\") + 1);
        std::cout << "Matrix loaded from file: " << file_name << std::endl;
        mat.display();
        // Generate a random matrix for testing (you can remove this if unnecessary)
        int size = config_json.at("configurations").at("matrixGeneration").at("size").get<int>();
        int minValue = config_json.at("configurations").at("matrixGeneration").at("minValue").get<int>();
        int maxValue = config_json.at("configurations").at("matrixGeneration").at("maxValue").get<int>();
        int symmetricity = config_json.at("configurations").at("matrixGeneration").at("symmetricity").get<int>();
        int asymRangeMin = config_json.at("configurations").at("matrixGeneration").at("asymRangeMin").get<int>();
        int asymRangeMax = config_json.at("configurations").at("matrixGeneration").at("asymRangeMax").get<int>();

        mat.generateRandomMatrix(size, minValue, maxValue, symmetricity, asymRangeMin, asymRangeMax);
        std::cout << "Random matrix generated:" << std::endl;
        mat.display();

        // Run the Branch and Bound algorithm
        BranchAndBound bnb(mat);

        Util util1;
        util1.getStartTime();
        bnb.runBranchAndBound();
        util1.getEndTime();
        std::cout << "Branch and Bound algorithm completed." << std::endl;
        bnb.printSolution();
        util1.elapsedTime();
        

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}
