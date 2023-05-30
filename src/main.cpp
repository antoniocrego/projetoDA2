#include "network.h"
#include "Program.h"
#include "network.h"

int main(){

    Network network = Network();
    network.readDataset("../data/Toy-Graphs/stadiums.csv",false);
    double mincost = 0;
    vector<int> tour = network.tspChristofides(mincost);
    for (auto c : tour){
        cout << "->" << c;
    }
    cout << endl << mincost;


    /*
    Program p = Program();
    p.run();
    */

    return 0;
}