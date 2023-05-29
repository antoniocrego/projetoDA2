#include <set>
#include <unordered_set>
#include "network.h"

Network::Network(){
}

void Network::readDataset(int num) {
    currentGraph=Graph(); // clear current graph;
    string url = "../src/data/";
    if (num>=0 && num<=2){
        url+="Toy-Graphs/";
        switch (num) {
            case 0:
                url += "shipping";
                break;
            case 1:
                url += "stadiums";
                break;
            case 2:
                url += "tourism";
                break;
        }
        url+=".csv";
    }
    else if (num>=3 && num<=14){
        url+="Extra_Fully_Connected_Graphs/edges_";
        switch (num) {
            case 3:
                url+="25";
                break;
            case 4:
                url+="50";
                break;
            case 5:
                url+="75";
                break;
            case 6:
                url+="100";
                break;
            case 7:
                url+="200";
                break;
            case 8:
                url+="300";
                break;
            case 9:
                url+="400";
                break;
            case 10:
                url+="500";
                break;
            case 11:
                url+="600";
                break;
            case 12:
                url+="700";
                break;
            case 13:
                url+="800";
                break;
            case 14:
                url+="900";
                break;
        }
        url+=".csv";
    }
    else if (num>=15 && num<=17){
        url+="Real-world Graphs/";
        switch (num){
            case 15:
                url+="graph1";
                break;
            case 16:
                url+="graph2";
                break;
            case 17:
                url+="graph3";
                break;
        }
        readNodes(url);
        url+="/edges.csv";
    }
    else{
        printf("Invalid ID for graph!");
        return;
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
        if (num<=14 && currentGraph.findVertex(stoi(origin))==nullptr) currentGraph.addVertex(stoi(origin));
        if (num<=14 && currentGraph.findVertex(stoi(dest))==nullptr) currentGraph.addVertex((stoi(dest)));
        currentGraph.addBidirectionalEdge(stoi(origin),stoi(dest),stoi(distance));


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

    }
}

Graph Network::getCurrentGraph() {
    return currentGraph;
}
void Network::backtracking(Graph test, double &min_cost, double actual_cost,int currPos, int count) {
    // Check if a Hamiltonian cycle is found
    if (count == test.getNumVertex()) {
        // Check if the last and first vertices are adjacent
        for (auto edge: test.findVertex(currPos)->getAdj()) {
            if (edge->getDest()->getId() == 0) {
                double cost = actual_cost + edge->getWeight();
                if (min_cost > cost) {
                    min_cost = cost;
                    cout<< min_cost << endl;
                    return;
                }
            }
        }
    }

    for (auto edge: test.findVertex(currPos)->getAdj()) {
        if (!edge->getDest()->isVisited()) {
            edge->getDest()->setVisited(true);
            backtracking(test, min_cost, edge->getWeight()+actual_cost, edge->getDest()->getId(), count + 1);
            edge->getDest()->setVisited(false);
        }
    }
}



