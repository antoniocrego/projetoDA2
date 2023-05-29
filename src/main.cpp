#include "network.h"
#include "Program.h"

int main(){
    Network network = Network();
    network.readDataset("Toy-Graphs/shipping.csv", false);
    std::vector<int> path = {0};
    std::vector<double> dist;
    Graph c = network.getCurrentGraph();

    for (auto vertex: c.getVertexSet()){
        if (vertex->getId()==0) vertex->setVisited(true);
        vertex->setVisited(false);
    }
    double max_cost = std::numeric_limits<double>::max();
    double* min_cost = &max_cost;

    network.backtracking(c,*min_cost,0,0,path,{0});

    cout << *min_cost << endl;
/*
    Program p = Program();
    p.run();
*/
    return 0;
}