#include "network.h"
#include "Program.h"
#include "network.h"

int main(){

    Network network = Network();
    network.readDataset("../data/Toy-Graphs/shipping.csv",false);
    vector<Edge *> mst = network.prim(network.getCurrentGraph().getVertexSet());
    for(auto e : mst){
        cout << e->getOrig()->getId() << "->" << e->getDest()->getId() << endl;
    }

    vector<bool> visited;
    visited.resize(network.getCurrentGraph().getVertexSet().size(), false);
    vector<Vertex *> preorder;
    network.preorderTraversal(mst, network.getCurrentGraph().indexVertex(0),visited,preorder);
    for(auto v : preorder){
        if(v->getId() == 0) cout << v->getId();
        else cout << "->" << v->getId();
    }


    /*
    Program p = Program();
    p.run();
    */

    return 0;
}