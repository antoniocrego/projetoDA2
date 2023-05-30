#include <set>
#include <unordered_set>
#include "network.h"

Network::Network(){
}

void Network::readDataset(string path, bool isReal) {
    currentGraph=Graph(); // clear current graph
    mapIDtoIndex.clear(); // clear map
    string url = "../src/data/"+path;

    if (isReal){
        readNodes(url);
        url+="/edges.csv";
    }

    ifstream in(url);
    string aLine, origin, dest, distance;
    getline(in, aLine);
    while (getline(in, aLine))
    {
        istringstream inn(aLine);
        getline(inn, origin, ',');
        getline(inn, dest, ',');
        getline(inn, distance, ',');

        if (!isReal && currentGraph.findVertex(stoi(origin))==nullptr){
            currentGraph.addVertex(stoi(origin));
            mapIDtoIndex.emplace(stoi(origin),currentGraph.getNumVertex()-1);
        }
        if (!isReal && currentGraph.findVertex(stoi(dest))==nullptr){
            currentGraph.addVertex((stoi(dest)));
            mapIDtoIndex.emplace(stoi(dest),currentGraph.getNumVertex()-1);
        }
        currentGraph.addBidirectionalEdge(stoi(origin),stoi(dest),stod(distance));

    }
}

void Network::readNodes(const string& graph) {
    ifstream in(graph+"/nodes.csv");
    string aLine, node;
    getline(in, aLine);
    while (getline(in, aLine))
    {
        istringstream inn(aLine);
        getline(inn,node,',');
        currentGraph.addVertex(stoi(node));
        mapIDtoIndex.emplace(stoi(node),currentGraph.getNumVertex()-1);
    }
}

Graph Network::getCurrentGraph() {
    return currentGraph;
}
void Network::backtracking(const Graph& test, double &min_cost, double actual_cost,int currPos, vector<int>& path, const vector<int>& currentPath) {
    // Check if a Hamiltonian cycle is found
    if (currentPath.size() == test.getNumVertex()) {
        // Check if the last and first vertices are adjacent
        for (auto edge: test.indexVertex(mapIDtoIndex.at(currPos))->getAdj()) {
            if (edge->getDest()->getId() == 0) {
                double cost = actual_cost + edge->getWeight();
                if (min_cost > cost) {
                    min_cost = cost;
                    path=currentPath;
                    path.push_back(0);
                }
                return;
            }
        }
    }

    for (auto edge: test.indexVertex(mapIDtoIndex.at(currPos))->getAdj()) {
        if (edge->getDest()->getId()==0) continue;
        if (!edge->getDest()->isVisited()) {
            edge->getDest()->setVisited(true);
            vector<int> other = currentPath;
            other.push_back(edge->getDest()->getId());
            backtracking(test, min_cost, edge->getWeight()+actual_cost, edge->getDest()->getId(), path, other);
            edge->getDest()->setVisited(false);
        }
    }
}

std::vector<Edge> Network::prim(vector<Vertex *> vertexSet) {

    vector<Edge> mst;
    if (vertexSet.empty()) {
        return mst;
    }

    // Reset auxiliary info
    for (auto v: vertexSet) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    // start with an arbitrary vertex
    Vertex *s = vertexSet.at(0);
    s->setDist(0);

    // initialize priority queue
    MutablePriorityQueue<Vertex> q;
    q.insert(s);
    // process vertices in the priority queue
    while (!q.empty()) {
        auto v = q.extractMin();
        v->setVisited(true);
        for (auto &e: v->getAdj()) {
            Vertex *w = e->getDest();
            if (!w->isVisited()) {
                auto oldDist = w->getDist();
                if (e->getWeight() < oldDist) {
                    w->setDist(e->getWeight());
                    w->setPath(e);
                    if (oldDist == INF) {
                        q.insert(w);
                    } else {
                        q.decreaseKey(w);
                    }
                }
            }
        }
    }

    return mst;
}

void Network::nearestNeighbor(double &min_cost, vector<int>& path){
    while(path.size()<=currentGraph.getNumVertex()){
        int nearestNode = -1;
        double minDistance = numeric_limits<double>::max();
        Vertex* currentSearch = currentGraph.indexVertex(mapIDtoIndex.at(path.back()));
        for (Edge* e: currentSearch->getAdj()){
            if(!e->getDest()->isVisited() && e->getWeight()<minDistance){
                nearestNode=e->getDest()->getId();
                minDistance=e->getWeight();
            }
        }
        if (nearestNode!=-1) {
            min_cost += minDistance;
            path.push_back(nearestNode);
            currentGraph.indexVertex(mapIDtoIndex.at(path.back()))->setVisited(true);
        }
        else{
            for (Edge* e: currentSearch->getAdj()){
                if(e->getDest()->getId()==0){
                    path.push_back(0);
                    min_cost+=e->getWeight();
                    break;
                }
            }
            break;
        }
    }
}

// Function to approximate the TSP using the Christofides algorithm
std::vector<int> Network::tspChristofides(double& minCost) {
    // Step 1: Find the minimum-weight perfect matching
    vector<Edge *> matching = prim(currentGraph.getVertexSet());

    // Step 2: Create a graph with the matching edges
    Graph newGraph = Graph();
    for (auto e: matching){
        newGraph.addVertex(e->getOrig()->getId());
        newGraph.addVertex(e->getDest()->getId());
        newGraph.addBidirectionalEdge(e->getOrig()->getId(),e->getDest()->getId(),e->getWeight());
    }

    // Step 3: Find the Eulerian circuit in the graph
    std::vector<int> eulerianCircuit = findEulerianCircuit(newGraph);

    // Step 4: Construct the TSP tour from the Eulerian circuit
    std::vector<int> tour = eulerianCircuit;

    for (int i = 0; i<tour.size(); i++){
        Vertex* current = newGraph.findVertex(tour.at(i));
        for (auto e : current->getAdj()){
            if (i!=tour.size()-1) {
                if (e->getDest()->getId() == tour.at(i + 1)) {
                    minCost += e->getWeight();
                    break;
                }
            }
            else{
                if (e->getDest()->getId() == 0) {
                    minCost += e->getWeight();
                    break;
                }
            }
        }
    }

    return tour;
}

// Function to find the Eulerian circuit in a graph
std::vector<int> Network::findEulerianCircuit(Graph g) {
    vector<int> circuit = g.dfs(1);

    reverse(circuit.begin(), circuit.end());
    return circuit;
}

// Function to find the minimum-weight perfect matching in a graph
vector<Edge *> Network::findMinimumWeightMatching() {
    vector<Edge *> matching;

    // Find the minimum-weight perfect matching using a greedy algorithm
    vector<int> nearestNeighbors(currentGraph.getNumVertex(), -1);

    for (int i = 1; i < currentGraph.getNumVertex(); ++i) {
        int u = -1;
        double currentMin = std::numeric_limits<double>::max();
        Vertex* current = currentGraph.indexVertex(mapIDtoIndex.at(i));
        Edge* closestEdge;
        for(auto e : current->getAdj()){
            if (!e->getDest()->isVisited() && (currentMin>e->getWeight())){
                u=e->getDest()->getId();
                closestEdge=e;
            }
        }
        matching.push_back(closestEdge);
        if (u!=-1) currentGraph.indexVertex(mapIDtoIndex.at(u))->setVisited(true);
    }

    return matching;
}




