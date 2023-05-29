#include <set>
#include <unordered_set>
#include "network.h"

Network::Network(){
}

void Network::readDataset(string path, bool isReal) {
    currentGraph=Graph(); // clear current graph;
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
        if (!isReal && currentGraph.findVertex(stoi(origin))==nullptr) currentGraph.addVertex(stoi(origin));
        if (!isReal && currentGraph.findVertex(stoi(dest))==nullptr) currentGraph.addVertex((stoi(dest)));
        currentGraph.addBidirectionalEdge(stoi(origin),stoi(dest),stoi(distance));
    }
    for(auto node : currentGraph.getVertexSet()){
        cout << node->getId() << endl;
    }
    string temp;
    cin >> temp;
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
    }
}

Graph Network::getCurrentGraph() {
    return currentGraph;
}
