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
    void readDataset(string path, string type);

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

    void preorderTraversal(vector<Edge *> mst, Vertex * v, vector<bool>& visited, vector<Vertex *>& preorder);

    void nearestNeighbor(double &min_cost, vector<int>& path);

    vector<int> tspChristofides(double& minCost);

    vector<int> findEulerianCircuit(Graph g);

    vector<Edge *> findMinimumWeightMatching(vector<Vertex *> odds);

    std::vector<Vertex*> getOddDegreeVertices(Graph g);

    void make_hamilton(vector<int>& path, double &path_dist);

    double calcPath(vector<Vertex *> path);


private:
    ///@brief Bidirectional Graph that represents the connections between Stations.
    Graph currentGraph;
    unordered_map<int,int> mapIDtoIndex;
};


#endif //PROJETODA_NETWORK_H
