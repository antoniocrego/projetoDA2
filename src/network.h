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
     * @brief Reads the edges in a file that includes only edges, adding the new found nodes along the way.
     * Time Complexity: O(N), N being the number of connections in the file.
     */
    void readDataset(string path, string type);

    /**
     * @brief Reads the nodes in a file that includes only nodes, i.e. real world graphs.
     * Time Complexity: O(N), N being the number of nodes in the file.
     */
    void readNodes(const string& graph);

    /**
     * @brief Function that returns the currently selected graph.
     * @return Graph.
     */
    Graph getCurrentGraph();

    /**
     *
     * @param test The graph to be used in the backtrack.
     * @param min_cost Reference variable that stores the actual lowest cost found.
     * @param actual_cost Variable passed down through recursion that stores the current paths cost.
     * @param currPos Variable that indicates what node to explore next.
     * @param path Vector reference that stores the path corresponding to the lowest cost found.
     * @param currentPath Vector passed down through recursion that stores the current explored path.
     * @brief Backtracking algorithm which returns the optimal solution through brute force, stopping an explored path when the cost gets too high.
     * Time Complexity: O(2^n), N being the number of nodes in the graph.
     */
    void backtracking(const Graph& test, double &min_cost, double actual_cost, int currPos, vector<int>& path, const vector<int>& currentPath);

    /**
     *
     * @param mst Tree structure, specifically a MST, to be traversed in preorder.
     * @param v Vertex to evaluate in current recursion.
     * @param visited Boolean vector reference that stores if a vertex has been visited yet.
     * @param preorder Vector reference that returns the resulting preorder traversal of the tree
     * @brief Function that returns the preorder traversal of a given tree structure.
     * Time Complexity: O(n), N being the number of nodes in the tree.
     */
    void preorderTraversal(vector<Edge *> mst, Vertex * v, vector<bool>& visited, vector<Vertex *>& preorder);

    /**
     *
     * @param min_cost Reference variable that returns the value of the path found.
     * @param path  Reference vector that returns the path found.
     * @brief Function that traverses the currently selected graph using the nearest neighbor algorithm.
     * Time Complexity: O(V*E), V being the number of nodes in the graph and E the number of edges.
     */
    void nearestNeighbor(double &min_cost, vector<int>& path);

    /**
     *
     * @param minCost Reference variable that returns the cost of the path found.
     * @param path Reference vector that returns the path found through the algorithm.
     * @param runs Maximum amount of 2-opt optimizations permitted (-1 if as many as needed).
     * @return Amount of 2-opt optimizations utilized.
     * @brief Function that traverses the currently selected graph using the Christofides algorithm and 2-opt.
     * Time Complexity: O(V^3), V being the number of nodes in the graph.
     */
    int tspChristofides(double& minCost, std::vector<int>& path, int runs);

    /**
     *
     * @param g Graph to be toured.
     * @return Vector with the order in which the Eulerian tour explored the graph.
     * @brief Finds the euler circuit of a given graph.
     * Time Complexity: O(V+E), V being the number of nodes in the graph and E the number of edges.
     */
    vector<int> findEulerianCircuit(Graph g);

    /**
     *
     * @param odds Vector of vertices whose degree is odd.
     * @param g Graph, specifically an MST, that the vertices belong to.
     * @brief Finds perfect matches for the odd degree vertices in a graph, making them all, ideally, even degree.
     * Time Complexity: O(V*E), V being the number of nodes in the graph and E the number of edges.
     */
    void findMinimumWeightMatching(vector<Vertex *> odds, Graph g);

    /**
     *
     * @param path Reference vector that stores the path to be optimized and, eventually, the optimized path.
     * @param cost Reference variable that stores the cost of the current best path.
     * @param max_runs Variable that defines the maximum amount of optimizations the algorithm should do.
     * @return Amount of optimizations done.
     * @brief Function that uses the 2-opt optimization algorithm to improve a path.
     * Time Complexity: O(E*V^2), V being the number of nodes in the given path and E the amount of edges in the graph.
     */
    int twoOpt(vector<int>& path, double& cost, int max_runs);

    /**
     *
     * @param g Graph, specifically a MST, to be explored.
     * @return Vector with references to the vertices with odd degree.
     * @brief Function that returns the odd degree vertices in a graph.
     * Time Complexity: O(V), V being the number of nodes in the graph.
     */
    std::vector<Vertex*> getOddDegreeVertices(Graph g);

    /**
     *
     * @param path Reference vector that stores the original path and, eventually, the updated path
     * @param path_dist Reference variable that returns the cost of traversing the Hamiltonian cycle.
     * @brief Function that turns a Eulerian circuit into a Hamiltonian cycle.
     * Time Complexity: O(N*E), N being the number of nodes in the original path and E the number of edges in the graph.
     */
    void make_hamilton(vector<int>& path, double &path_dist);

    /**
     *
     * @param path vector that stores the path of which we want to calculate the length.
     * @return Length of the path
     * @brief Function that calculates the length of a given path.
     * Time Complexity: O(V), V being the number of nodes in the path.
     */
    double calcPath(vector<Vertex *> path);

    /**
     *
     * @param current Current path being used in the simulated annealing.
     * @param val Reference value that returns the difference in length before and after the change.
     * @return Vector with the new path after the random changes.
     * @brief Function that generates the next neighbor for the current path in the simulated annealing.
     * Time Complexity: O(V), V being the number of nodes in the graph
     */
    vector<Vertex *> generateNext(vector<Vertex *> current, double &val);

    /**
     *
     * @return Vector with references to the vertexes in the path
     * @brief Function that calculates an approximate solution using the Simulated Annealing algorithm.
     * Time Complexity: O(V), V being the number of nodes in the graph
     */
    std::vector<Vertex *> simulated_annealing();

private:
    ///@brief Bidirectional Graph that represents the connections between Stations.
    Graph currentGraph;
    ///@brief Unordered map used to identify the index of a vertex in the graph vertex vector.
    unordered_map<int,int> mapIDtoIndex;


};


#endif //PROJETODA_NETWORK_H
