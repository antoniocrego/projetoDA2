#include "network.h"
#include "Program.h"

int main(){
    Network network = Network();
    network.readDataset(0);
    std::vector<int> path;
    std::vector<double> dist;
    Graph c = network.getCurrentGraph();
    for(auto edge : c.getVertexSet()){
        dist.push_back(edge->getDist());
    }

    double k = network.backtracking(c,INTMAX_MAX,0,path);
    cout << k << endl;
    /*
    Program p = Program();
    p.run();`*/;

    return 0;
}