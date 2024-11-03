#ifndef NODE_H
#define NODE_H

#include <vector>

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