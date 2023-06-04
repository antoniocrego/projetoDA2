// By: Gonçalo Leão

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <climits>
#include "../data_structures/MutablePriorityQueue.h"

#include "VertexEdge.h"

class Graph {
public:
    ~Graph();
    /*
    * Auxiliary function to find a vertex with a given ID.
    */
    Vertex *findVertex(const int &id) const;
    Vertex* indexVertex(const int &id) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const int &id);

    /**
    * @brief This function removes as vertex from the graph
    * @param id Id of the vertex to be removed
    * Time Complexity: O(V * E), V being the amount of vertexes and E being the amount of Edges
    * @return True if the vertex is successfully removed, false otherwise
    */
    bool removeVertex(const int& id);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const int &sourc, const int &dest, const double &weight);
    bool addBidirectionalEdge(const int &sourc, const int &dest, const double &weight);
    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;

    std::string getType() const;

    void setType(std::string type);

    /**
     *
     * @return Vector of edges in the Minimum Spanning Tree of the graph
     * @brief Function that calculates the MST of the graph utilising the Prim algorithm
     * Time Complexity: O(V * E), V being the number of nodes in the graph and E being the number of edges
     */
    std::vector<Edge *> prim();

    /**
     *
     * @param orig Id of the origin vertex in the vertex set.
     * @param dest Id of the destination vertex.
     * @return The edge if found and null otherwise.
     * @brief Function that searches for an edge in the graph.
     * Time Complexity: O(E), E being the number of edges.
     */
    Edge * findEdge(int orig, int dest);
protected:
    std::vector<Vertex *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const int &id) const;
    std::string type = "";
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif /* DA_TP_CLASSES_GRAPH */