// By: Gonçalo Leão

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
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
    bool addEdge(const int &sourc, const int &dest, double w, std::string service);
    bool addBidirectionalEdge(const int &sourc, const int &dest, double w, std::string service);

    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;

    /**
    * @brief This function creates a vertex that links all the vertexes to be considered as sinks
    * @param sinks Vector of the ids of the vertexes to be linked
    * Time Complexity: O(n), n being the size of the vector sinks
    * @return The id of the vertex created
    */
    int megaSink(const std::vector<int>& sinks);

    /**
    * @brief This function creates a vertex that links all the vertexes to be considered as sources
    * @param sources Vector of the ids of the vertexes to be linked
    * Time Complexity: O(n), n being the size of the vector sources
    * @return The id of the vertex created
    */
    int megaSource(const std::vector<int>& sources);

    /**
     * @brief This function sets a vertex to visited and adds it to the queue, setting the vertex's path to a given edge.
     * Time Complexity: O(1).
     * @param q Queue of vertexes.
     * @param e Edge to be set as vertex's path.
     * @param w Vertex to be tested.
     * @param residual Residual capacity of the edge.
     */
    void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);

    /**
     * @brief This function finds an augmenting path between two vertexes.
     * Time Complexity: O(V*E), V being the number of vertexes and E being the number of edges.
     * @param s The source vertex.
     * @param t The target vertex.
     * @return True if it finds an augmenting path between the two vertexes, false otherwise.
     */
    bool findAugmentingPath(Vertex *s, Vertex *t);

    /**
     * @brief This function returns the minimum residual capacity of the augmenting path.
     * Time Complexity: O(V), V being the number of vertexes.
     * @param s The source vertex.
     * @param t The target vertex.
     * @return The minimum residual capacity.
     */
    double findMinResidualAlongPath(Vertex *s, Vertex *t);

    /**
     * @brief This function augments a given flow along an augmentation path between two vertexes
     * Time Complexity: O(V), V being the number of vertexes.
     * @param s The  source vertex.
     * @param t The target vertex.
     * @param f The flow to augment
     */
    void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);

    /**
    * @brief This function implements the Edmonds-Karp algorithm
    * @param source Id of the vertex to be considered as source
    * @param target Id of the vertex to be considered as target
    * Time Complexity: O(V*E^2) V being the number of vertexes and E being the number of edges
    * @return The max flow between the source and target vertexes
    **/
    double edmondsKarp(int source, int target);

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