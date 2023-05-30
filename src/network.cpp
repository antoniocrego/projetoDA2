#include <set>
#include <unordered_set>
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

double Network::calcPath(vector<Vertex *> path){
    double result = 0;
    for(int i = 0; i < path.size()-1; i++){
        Edge * edge = currentGraph.findEdge(path[i]->getId(), path[i+1]->getId());
        if(edge == nullptr){
            result += path[i]->getCoordinate().distance(path[i+1]->getCoordinate());
        }else{
            result += edge->getWeight();
        }
    }
    return result;
}




