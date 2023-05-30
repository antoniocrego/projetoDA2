#ifndef PROJETODA_NETWORK_H
#define PROJETODA_NETWORK_H


#include "data_structures/Graph.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <cmath>

using namespace std;


class Network {
public:

    ///@brief Empty Constructor.
    Network();

    /**
     * @brief Store the information about the Stations from a Data File into a graph.
     * Time Complexity: O(N), N being the number of stations on the file.
     */
    void readDataset(string path, bool isReal);

    /**
     * @brief Store the information about the connections between Stations from a Data File into a graph.
     * Time Complexity: O(N), N being the number of connections between stations on the file.
     */
    void readNodes(const string& graph);

    /**
     * @brief Function that returns the graph related to the connections between Stations.
     * @return Graph.
     */
    Graph getCurrentGraph();

    void backtracking(const Graph& test, double &min_cost, double actual_cost, int currPos, vector<int>& path, const vector<int>& currentPath);
    void nearestNeighbor(double &min_cost, vector<int>& path);

private:
    ///@brief Bidirectional Graph that represents the connections between Stations.
    Graph currentGraph;
    unordered_map<int,int> mapIDtoIndex;
};


#endif //PROJETODA_NETWORK_H
