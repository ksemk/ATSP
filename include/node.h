#ifndef NODE_H
#define NODE_H

#include <vector>

/**
 * @struct Node
 * @brief Represents a node in the search tree for the ATSP (Asymmetric Traveling Salesman Problem).
 *
 * This structure is used to store information about a specific state in the search tree,
 * including the current path of cities, the level of the node in the tree, the cost of the path,
 * the lower bound for the path, the current city, and a tracker for visited cities.
 *
 * @var std::vector<int> Node::path
 * Stores the current path of cities. Initialized with -1 to indicate unvisited cities.
 *
 * @var int Node::level
 * Represents the current level in the tree (depth of recursion).
 *
 * @var int Node::cost
 * Stores the cost of the path so far.
 *
 * @var int Node::bound
 * Represents the lower bound for the path.
 *
 * @var int Node::currentCity
 * The current city in the path (used for the Nearest Neighbor algorithm).
 *
 * @var std::vector<bool> Node::visited
 * Tracker for visited cities. Initialized to false for all cities.
 *
 * @fn Node::Node(int numCities)
 * @brief Constructor for the Node structure.
 *
 * Initializes the path with -1 (unvisited) and the visited vector with false (all cities unvisited).
 *
 * @param numCities The number of cities in the ATSP problem.
 */
struct Node {
    std::vector<int> path;      // Stores the current path of cities
    int level;                  // Current level in the tree (depth of recursion)
    int cost;                   // Cost of the path so far
    int bound;                  // Lower bound for the path
    int currentCity;            // The current city in the path (for NN algorithm)
    std::vector<bool> visited;  // Tracker for visited cities

    Node(int numCities) : level(0), cost(0), bound(0), currentCity(0) {
        path.resize(numCities, -1);        // Initialize path with -1 (unvisited)
        visited.resize(numCities, false);  // Initialize all cities as unvisited
    }
};
extern Node initialNode;

#endif // NODE_H