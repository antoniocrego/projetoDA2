#include <set>
#include <unordered_set>
#include <stack>
#include <cstring>
#include "network.h"

Network::Network(){
}

void Network::readDataset(string path, string type) {
    currentGraph=Graph(); // clear current graph
    mapIDtoIndex.clear(); // clear map
    string url = "../src/data/"+path;

    cout << "Loading graph..." << endl;

    if (type == "real"){
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

        if (type != "real" && currentGraph.findVertex(stoi(origin))==nullptr){
            currentGraph.addVertex(stoi(origin));
            mapIDtoIndex.emplace(stoi(origin),currentGraph.getNumVertex()-1);
        }
        if (type != "real" && currentGraph.findVertex(stoi(dest))==nullptr){
            currentGraph.addVertex((stoi(dest)));
            mapIDtoIndex.emplace(stoi(dest),currentGraph.getNumVertex()-1);
        }
        currentGraph.addBidirectionalEdge(stoi(origin),stoi(dest),stod(distance));

    }
    currentGraph.setType(type);
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

void Network::preorderTraversal(vector<Edge *> mst, Vertex * v, vector<bool>& visited, vector<Vertex *>& preorder){
    visited[v->getId()] = true;
    preorder.push_back(v);

    for(auto edge : mst){
        if(edge->getOrig()->getId() == v->getId() && !visited[edge->getDest()->getId()]){
            preorderTraversal(mst, edge->getDest(), visited, preorder);
        }
    }
}

double Network::calcPath(vector<Vertex *> path){
    double result = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        Vertex *origin = path[i];
        Vertex *dest = path[i + 1];
        Edge *edge = currentGraph.findEdge(mapIDtoIndex.at(origin->getId()), dest->getId());
        if (edge == nullptr) {
            double dist = path[i]->getCoordinate().distance(path[i + 1]->getCoordinate());
            result += dist;
        } else {
            result += edge->getWeight();
        }
    }
    return result;
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
    vector<Edge *> mst = currentGraph.prim();

    // Step 2: Create a graph with the matching edges
    Graph newGraph = Graph();
    for(auto v : currentGraph.getVertexSet()){
        newGraph.addVertex(v->getId());
    }
    for(auto e : mst){
        newGraph.addBidirectionalEdge(e->getOrig()->getId(),e->getDest()->getId(),e->getWeight());
    }

    vector<Vertex *> vertex = getOddDegreeVertices(newGraph);
    vector<Edge *> matching = findMinimumWeightMatching(vertex);

    for(auto e : matching){
        newGraph.addBidirectionalEdge(e->getOrig()->getId(),e->getDest()->getId(),e->getWeight());
    }

    // Step 3: Find the Eulerian circuit in the graph
    std::vector<int> eulerianCircuit = findEulerianCircuit(newGraph);

    // Step 4: Construct the TSP tour from the Eulerian circuit
    make_hamilton(eulerianCircuit, minCost);


    return eulerianCircuit;
}

std::vector<Vertex*> Network::getOddDegreeVertices(Graph g) {
    vector<Vertex *> result;
    for (auto v: g.getVertexSet()) {
        if (v->getAdj().size() % 2 == 1) result.push_back(v);
    }
    return result;
}

// Function to find the Eulerian circuit in a graph
std::vector<int> Network::findEulerianCircuit(Graph g) {

    Graph copy = Graph(g);
    stack<int> stack;
    vector<int> circuit;

    int pos = 0;

    while(!stack.empty() || copy.findVertex(pos)->getAdj().size()>0){
        Vertex* currentVertex=copy.findVertex(pos);

        if(currentVertex->getAdj().size()==0){
            circuit.push_back(pos);
            int last = stack.top();
            stack.pop();
            pos = last;
        }
        else{
            stack.push(pos);
            Vertex* neighbor = currentVertex->getAdj().back()->getDest();
            currentVertex->removeEdge(neighbor->getId());
            neighbor->removeEdge(currentVertex->getId());
            pos=neighbor->getId();
        }
    }
    circuit.push_back(pos);
    reverse(circuit.begin(),circuit.end());
    return circuit;
}

void Network::make_hamilton(vector<int>& path, double &path_dist){
    bool visited[currentGraph.getNumVertex()];
    memset(visited,0,currentGraph.getNumVertex()*sizeof(bool));
    path_dist = 0;

    int root = path.front();
    vector<int>::iterator curr = path.begin();
    vector<int>::iterator next = path.begin()+1;
    visited[root]=true;

    while(next!=path.end()){
        if(!visited[*next]){
            for(auto e: currentGraph.getVertexSet().at(mapIDtoIndex.at(*curr))->getAdj()){
                if(e->getDest()->getId()==*next){
                    path_dist+=e->getWeight();
                    break;
                }
            }
            curr=next;
            visited[*curr]=true;
            next=curr+1;
        }
        else next=path.erase(next);
    }
    for(auto e : currentGraph.getVertexSet().at(mapIDtoIndex.at(*curr))->getAdj()){
        if(e->getDest()->getId()==0){
            path.push_back(0);
            path_dist+=e->getWeight();
            break;
        }
    }
}



// Function to find the minimum-weight perfect matching in a graph
vector<Edge *> Network::findMinimumWeightMatching(vector<Vertex *> odds) {
    vector<Edge *> matching;
    set<int> check;

    // Find the minimum-weight perfect matching using a greedy algorithm
    for (auto v : odds){
        check.insert(v->getId());
    }

    for (auto v : currentGraph.getVertexSet()){
        v->setVisited(false);
    }

    for (int i = 0; i < odds.size(); ++i) {
        int u = -1;
        if (currentGraph.getVertexSet().at(mapIDtoIndex.at(odds.at(i)->getId()))->isVisited()) continue;
        double currentMin = std::numeric_limits<double>::max();
        Edge* closestEdge;
        for(auto e : currentGraph.getVertexSet().at(mapIDtoIndex.at(odds.at(i)->getId()))->getAdj()){
            if (!e->getDest()->isVisited() && (currentMin>e->getWeight()) && (check.find(e->getDest()->getId())!=check.end())){
                u=e->getDest()->getId();
                closestEdge=e;
            }
        }
        bool exists = false;
        for (auto e : odds.at(i)->getAdj()){
            if (e->getDest()->getId()==u){
                exists=true;
                break;
            }
        }
        if(!exists) matching.push_back(closestEdge);
        if (u!=-1){
            currentGraph.getVertexSet().at(mapIDtoIndex.at(odds.at(i)->getId()))->setVisited(true);
            currentGraph.getVertexSet().at(mapIDtoIndex.at(closestEdge->getDest()->getId()))->setVisited(true);
        }
    }
    return matching;
}



