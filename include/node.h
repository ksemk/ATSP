#ifndef NODE_H
#define NODE_H

#include <cstring> // For memset

/**
 * @struct Node
 * @brief Represents a node in the search tree for the ATSP (Asymmetric Traveling Salesman Problem).
 *
 * This structure is used to store information about a specific state in the search tree,
 * including the current path of cities, the level of the node in the tree, the cost of the path,
 * the lower bound for the path, the current city, and a tracker for visited cities.
 */
struct Node {
    int* path;         // Dynamically allocated array for the path
    int level;         // Current level in the tree (depth of recursion)
    int cost;          // Cost of the path so far
    int bound;         // Lower bound for the path
    int currentCity;   // The current city in the path
    bool* visited;     // Tracker for visited cities
    int numCities;     // Number of cities (size of the problem)

    /**
     * @brief Constructor for the Node structure.
     *
     * Dynamically allocates memory for the path and visited arrays.
     * Initializes the path with -1 (unvisited) and the visited array with false.
     *
     * @param numCities The number of cities in the ATSP problem.
     */
    Node(int numCities);

    /**
     * @brief Destructor for the Node structure.
     *
     * Frees the dynamically allocated memory for path and visited arrays.
     */
    ~Node();

    /**
     * @brief Copy constructor for Node.
     *
     * Performs a deep copy of another Node instance.
     *
     * @param other The Node instance to copy from.
     */
    Node(const Node& other);

    /**
     * @brief Assignment operator for Node.
     *
     * Performs a deep copy of another Node instance.
     * Frees existing memory before copying.
     *
     * @param other The Node instance to assign from.
     * @return A reference to the current instance.
     */
    Node& operator=(const Node& other);

    /**
     * @brief Debugging utility to print the node's data.
     */
    void print() const;
};

#endif // NODE_H
