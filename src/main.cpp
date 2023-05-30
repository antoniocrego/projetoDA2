#include "network.h"
#include "Program.h"
#include "network.h"

int main(){

    Network network = Network();
    network.readDataset("../data/Toy-Graphs/shipping.csv",false);
    vector<Edge> mst = network.prim(network.getCurrentGraph().getVertexSet());


    /*
    Program p = Program();
    p.run();
    */

    return 0;
}