#include "Network.h"
#include "Program.h"

int main(){
    Network network = Network();
    network.readDataset(0);
    Graph copy = network.getCurrentGraph();
    for(auto c : copy.getVertexSet()){
        cout << c->getId() << endl;
    }
    /*
    Program p = Program();
    p.run();`*/;

    return 0;
}