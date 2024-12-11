#include "../include/node.h"
#include <iostream>

Node::Node(int numCities) : level(0), cost(0), bound(0), currentCity(0), numCities(numCities) {
    path = new int[numCities];
    visited = new bool[numCities];

    // Initialize the path with -1 and visited with false
    for (int i = 0; i < numCities; ++i) {
        path[i] = -1;
        visited[i] = false;
    }
}

Node::~Node() {
    delete[] path;
    delete[] visited;
}

Node::Node(const Node& other) : level(other.level), cost(other.cost),
    bound(other.bound), currentCity(other.currentCity), numCities(other.numCities) {
    path = new int[numCities];
    visited = new bool[numCities];
    for (int i = 0; i < numCities; ++i) {
        path[i] = other.path[i];
        visited[i] = other.visited[i];
    }
}

Node& Node::operator=(const Node& other) {
    if (this == &other) return *this;

    // Free existing memory
    delete[] path;
    delete[] visited;

    // Copy data from the other instance
    numCities = other.numCities;
    level = other.level;
    cost = other.cost;
    bound = other.bound;
    currentCity = other.currentCity;

    path = new int[numCities];
    visited = new bool[numCities];
    for (int i = 0; i < numCities; ++i) {
        path[i] = other.path[i];
        visited[i] = other.visited[i];
    }

    return *this;
}

void Node::print() const {
    std::cout << "Level: " << level << ", Cost: " << cost
              << ", Bound: " << bound << ", Current City: " << currentCity << "\n";
    std::cout << "Path: ";
    for (int i = 0; i < numCities; ++i) {
        if (path[i] == -1) break;
        std::cout << path[i] << " ";
    }
    std::cout << "\nVisited: ";
    for (int i = 0; i < numCities; ++i) {
        std::cout << visited[i] << " ";
    }
    std::cout << "\n";
}
