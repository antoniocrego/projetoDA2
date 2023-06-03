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
    if(type != "extra") getline(in, aLine);
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
    string aLine, node, longitude, latitude;
    getline(in, aLine);
    while (getline(in, aLine))
    {
        istringstream inn(aLine);
        getline(inn,node,',');
        getline(inn,longitude,',');
        getline(inn,latitude,',');
        currentGraph.addVertex(stoi(node));
        Coordinate coordinate = Coordinate(stod(latitude), stod(longitude));
        mapIDtoIndex.emplace(stoi(node),currentGraph.getNumVertex()-1);
        Vertex * last = currentGraph.getVertexSet().at(currentGraph.getNumVertex()-1);
        last->setCoordinate(coordinate);
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
    findMinimumWeightMatching(vertex,newGraph);

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
void Network::findMinimumWeightMatching(vector<Vertex *> odds, Graph g) {
    double length;
    Vertex* closest;
    vector<Vertex *>::iterator tmp, first;

    while(!odds.empty()){
        first = odds.begin();
        auto it = odds.begin() + 1;
        auto end = odds.end();
        length = std::numeric_limits<double>::max();
        for(;it!=end;++it){
            Edge* current = currentGraph.findEdge((*first)->getId(),(*it)->getId());
            if(current->getWeight()<length){
                length=current->getWeight();
                closest=*it;
                tmp=it;
            }
        }
        (*first)->addEdge(*tmp,length);
        (*tmp)->addEdge(*first,length);
        odds.erase(tmp);
        odds.erase(first);
    }
}

std::vector<Vertex *> Network::generateRandomPath() {
    vector<Vertex *> path;
    vector<Vertex *> remaining = currentGraph.getVertexSet();
    Vertex * v0 = remaining.at(mapIDtoIndex.at(0));
    path.push_back(v0);
    remaining.erase(remaining.begin() + mapIDtoIndex.at(0));

    std::srand(clock());
    while(!remaining.empty()){
        int rand = std::rand();
        rand = rand%remaining.size();
        path.push_back(remaining.at(rand));
        remaining.erase(remaining.begin()+rand);
    }

    path.push_back(v0);

    return path;
}

std::vector<Vertex *> Network::generateNext(vector<Vertex *> current, double &val){
    std::srand(clock());
    int firstId = std::rand() % current.size();
    while ((firstId == 0) || (firstId == (current.size() - 1))){
        firstId = std::rand() % current.size();
    }

    int v = current.size();

    std::srand(clock());
    int lastId = std::rand() % current.size();
    while ((lastId == 0) || (lastId == (current.size() - 1)) || (lastId == firstId)){
        lastId = std::rand() % current.size();
    }

    Vertex * v1 = current.at(firstId);
    Vertex * v2 = current.at(lastId);

    double keep1 = currentGraph.findEdge(mapIDtoIndex.at(current[firstId - 1]->getId()), current[firstId]->getId())->getWeight();
    keep1 += currentGraph.findEdge(mapIDtoIndex.at(current[firstId]->getId()), current[firstId + 1]->getId())->getWeight();
    keep1 += currentGraph.findEdge(mapIDtoIndex.at(current[lastId - 1]->getId()), current[lastId]->getId())->getWeight();
    keep1 += currentGraph.findEdge(mapIDtoIndex.at(current[lastId]->getId()), current[lastId + 1]->getId())->getWeight();

    current[firstId] = v2;
    current[lastId] = v1;

    double keep2 = currentGraph.findEdge(mapIDtoIndex.at(current[firstId - 1]->getId()), current[firstId]->getId())->getWeight();
    keep2 += currentGraph.findEdge(mapIDtoIndex.at(current[firstId]->getId()), current[firstId + 1]->getId())->getWeight();
    keep2 += currentGraph.findEdge(mapIDtoIndex.at(current[lastId - 1]->getId()), current[lastId]->getId())->getWeight();
    keep2 += currentGraph.findEdge(mapIDtoIndex.at(current[lastId]->getId()), current[lastId + 1]->getId())->getWeight();


    val = keep2 - keep1;

    return current;
}

vector<Vertex *> Network::simulated_annealing(){
    for (auto vertex: currentGraph.getVertexSet()){
        if (vertex->getId()==0) vertex->setVisited(true);
        else vertex->setVisited(false);
    }
    vector<int> path = {0};
    double val = 0;
    vector<Vertex *> currentPath;
    nearestNeighbor(val, path);
    for(int i : path){
        currentPath.push_back(currentGraph.indexVertex(mapIDtoIndex.at(i)));
    }
    vector<Vertex *> bestPath = currentPath;
    double best_val = val;
    double temperature = 1000;
    while (temperature >= 1){
        for(int i = 0; i < 500; i++) {
            std::srand(clock());
            double next_val = 0;
            vector<Vertex *> nextPath = generateNext(currentPath, next_val);
            double prob = 1 / (1 + std::exp((next_val)/temperature));
            int rand = std::rand() % 100;
            if (rand <= prob * 100) {
                currentPath = nextPath;
                val += next_val;
            }
            next_val += val;
            if(next_val < best_val){
                best_val = next_val;
                bestPath = nextPath;
            }
        }
        temperature = temperature * 0.99;
    }
    return bestPath;
}

