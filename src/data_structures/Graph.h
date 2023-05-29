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
    /**
    * @brief This function implements the Dijkstra algorithm
    * Time Complexity: O(E + V*log(V)), V being the number of vertexes and E being the number of edges
    * @param source Id of the vertex to be considered as source
    * @param dest Id of the vertex to be considered as target
    * @return Pair of the minimum cost calculated and the max flow between the two vertexes
    */
    std::pair<int,double> Dijsktra(int source,int dest);
protected:
    std::vector<Vertex *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const int &id) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif /* DA_TP_CLASSES_GRAPH */