#include "Program.h"

Program::Program() {
    this->currentMenuPage=0;
    createMenu();
    this->network=Network();
}

/**Functionality:
 *
 * Description: This is an auxiliary function, it will create a string and ask the user to insert anything when he wants to continue the program. That
 * way the program will be on hold until the user tells it to continue.
 */
void Program::wait() const {
    cout<<"\nEnter anything to go back:";
    string wait;
    cin>>wait;
}

void Program::clear() const {
    for(int i = 0; i < 20; i++){
        cout << endl;
    }
}

void Program::createMenu() {
    this->menus.push_back(Menu("../menu/files/mainMenu.txt", "Train Network"));//Initialize main menu
    /*this->menus.push_back(Menu("../Menus/scheduleSubMenu.txt"));// Initialize schedule submenu
    this->menus.push_back(Menu("../Menus/requestsMenu"));*/

}

int Program::getCurrentPage() const {
    return this->currentMenuPage;
}

/**
 *
 * @param newCurrentPage
 * This function receive the index of the new menu page we are in and set it as the current menu page
 */
void Program::setCurrentPage(int newCurrentPage)  {
    this->currentMenuPage=newCurrentPage;
}

/**Functionality: draw the current page of the menu we are in
 * Description: Take the attribute menus get the current menu and use the method draw of the menu to draw it
 */
void Program::draw(){
    this->menus[this->currentMenuPage].draw(); //draw the current menu
}

void Program:: run() {
    while (this->currentMenuPage != -1) {
        draw(); //draw the current menu
        int option = menus.at(currentMenuPage).getOption();
        string station1 = "";
        string station2 = "";
        vector<pair<string, string>> pairs;
        unordered_set<Edge *> segments;
        vector<Edge *> segments2;
        Edge * segment;
        double returnValue;
        vector<pair<int,pair<double,double>>> specialReturn;
        vector<pair<int,double>> temp;
        pair<int,double> dijkstra;
        vector<pair<double,string>> flowByDistrict;
        string o;
        int maxDisplay;
        switch (this->currentMenuPage) {
            case 0: // Is on main menu
                switch (option) {
                    case 1:
                        station1 = chooseStation(false,"the origin station");
                        station2 = chooseStation(false, "the destination station");
                        clear();
                        returnValue = network.maxFlow(station1,station2);
                        if(returnValue == -1){
                            cout << "Invalid origin and/or destination station. Please note that the origin and destination can't be the same!\n";
                            this->wait();
                            currentMenuPage=0;
                            break;
                        }
                        cout << "The maximum flow between " << station1 << " and " << station2 << " is " << returnValue << ".\n";
                        this->wait();
                        currentMenuPage=0;
                        break;
                    case 2:
                        station1 = chooseStation(false, "the station to evaluate");
                        clear();
                        returnValue = this->network.maxArrival(station1);
                        cout << station1 << " Max Arrival: " << returnValue << endl;
                        this->wait();
                        currentMenuPage = 0;
                        break;
                    case 3:
                        clear();
                        returnValue = this->network.maxFlowPairs(pairs);
                        cout << "The highest flow between stations in the network is " << returnValue << " for the following stations:\n";
                        for (auto pair: pairs) {
                            cout << "\t" << pair.first << " & " << pair.second << endl;
                        }
                        this->wait();
                        currentMenuPage = 0;
                        break;
                    case 4:
                        clear();
                        flowByDistrict = this->network.multiMaxFlowDistricts();
                        std::sort(flowByDistrict.begin(), flowByDistrict.end(), [](const pair<double,string>& a, const pair<double,string>& b) -> bool{return a.first<b.first;});
                        cout << "From highest transportation need (lowest flow), to lowest need (highest flow):" << endl;
                        for (int i = 1; i<=flowByDistrict.size(); i++){
                            cout << "\t[" << i << "] " << flowByDistrict.at(i-1).second << " - " << flowByDistrict.at(i-1).first << endl;
                        }
                        this->wait();
                        currentMenuPage = 0;
                        break;
                    case 5:
                        station1 = chooseDistrict(false, "the district to evaluate");
                        clear();
                        flowByDistrict = this->network.multiMaxFlowMunicipalities(station1);
                        std::sort(flowByDistrict.begin(), flowByDistrict.end(), [](const pair<double,string>& a, const pair<double,string>& b) -> bool{return a.first<b.first;});
                        cout << "From highest transportation need (lowest flow), to lowest need (highest flow) in the district of " << station1 << ":" << endl;
                        for (int i = 1; i<=flowByDistrict.size(); i++){
                            cout << "\t[" << i << "] " << flowByDistrict.at(i-1).second << " - " << flowByDistrict.at(i-1).first << endl;
                        }
                        this->wait();
                        currentMenuPage = 0;
                        break;
                    case 6:
                        station1 = chooseStation(false, "the origin station");
                        station2 = chooseStation(false, "the destination station");
                        if(station1 == station2){
                            clear();
                            cout << "Invalid origin and/or destination station. Please note that the origin and destination can't be the same!\n";
                            this->wait();
                            currentMenuPage=0;
                            break;
                        }
                        while (true){
                            segment = chooseEdge(true, "the edge to be removed");
                            if(segment == NULL) break;
                            segments.insert(segment);
                        }
                        clear();
                        returnValue = this->network.reducedEdgesMaxFlow(station1, station2, segments);
                        cout << "The maximum flow between " << station1 << " and " << station2 << " without the selected edges is " << returnValue << "." << endl;
                        this->wait();
                        currentMenuPage = 0;
                        break;
                    case 7:
                        while (true){
                            segment = chooseEdge(true, "the edge to be removed");
                            if(segment == NULL) break;
                            segments2.push_back(segment);
                        }
                        if (segments2.empty()) break;
                        cout << "Choose a number of stations up to " << network.getTrainNetwork().getNumVertex() << " to display per segment: ";
                        while (true){
                            cin >> o;
                            try {
                                maxDisplay = stoi(o);
                                if(maxDisplay >= 1 and maxDisplay<= network.getTrainNetwork().getNumVertex()) break;
                                cout << "\nInvalid option! Please introduce a valid one: ";
                            }
                            catch (std::invalid_argument ia){
                                cout << "\nInvalid option! Please introduce a valid one: ";
                            }
                        }
                        clear();
                        specialReturn = this->network.segmentFailureEvaluation(segments2);
                        for (int i = 0; i<segments2.size();i++) {
                            returnValue=0.0;
                            for (int j = i; j < specialReturn.size(); j += segments2.size()) {
                                temp.emplace_back(specialReturn.at(j).first,
                                                  specialReturn.at(j).second.first - specialReturn.at(j).second.second);
                                if (temp.at(temp.size()-1).second!=0) returnValue++;
                            }
                            cout << "For the failure of segment: " << network.IDtoStation(segments2.at(i)->getOrig()->getId()) << " <-> " << network.IDtoStation(segments2.at(i)->getDest()->getId()) << endl;
                            std::sort(temp.begin(), temp.end(), [](const pair<int,double>& a, const pair<int,double>& b) -> bool{return a.second>b.second;});
                            for (int cnt = 1; cnt<=maxDisplay; cnt++){
                                cout << "\t[" << cnt << "] " << network.IDtoStation(temp.at(cnt-1).first) << " | " << std::ceil(temp.at(cnt-1).second*100.0)/100.0 << " flow is lost on average | " << std::ceil(specialReturn.at(i+segments2.size()*temp.at(cnt-1).first).second.first*100.0)/100.0 << "-" << std::ceil(specialReturn.at(i+segments2.size()*temp.at(cnt-1).first).second.second*100.0)/100.0 << endl;
                            }
                            cout << "This failure affects " << std::ceil((returnValue/network.getTrainNetwork().getNumVertex())*100.0*100.0) / 100.0 << "% of all stations.\n";
                            temp.clear();
                            cout << endl;
                        }
                        this->wait();
                        currentMenuPage=0;
                        break;
                    case 8:
                        station1 = chooseStation(false, "the origin station");
                        station2 = chooseStation(false, "the destination station");
                        if(station1 == station2){
                            clear();
                            cout << "Invalid origin and/or destination station. Please note that the origin and destination can't be the same!\n";
                            this->wait();
                            currentMenuPage=0;
                            break;
                        }
                        clear();
                        dijkstra = network.getTrainNetwork().Dijsktra(network.getStationID(station1),network.getStationID(station2));
                        if(dijkstra.first == -1){
                            cout << "There is no path from " << station1 << " to " << station2 << ".\n";
                            this->wait();
                            break;
                        }
                        cout << "For the cheapest cost of " << dijkstra.first << "€, there is a maximum flow of " << dijkstra.second << " in between " << station1 << " and " << station2 << ".\n";
                        this->wait();
                        currentMenuPage=0;
                        break;
                    case 9:
                        this->network = Network();
                        break;
                    case 10:
                        this->currentMenuPage = -1;
                        break;
                }
                break;
        }
// runs until quit option is selected
    }
}

string Program::chooseDistrict(bool addStopButton, string label){
    int previousMenuPage = currentMenuPage;
    this->menus.push_back(Menu(network.getDistricts(), "Districts", label));// Initialize districts submenu
    currentMenuPage = menus.size() - 1;
    if(addStopButton) menus.at(currentMenuPage).addButton("Stop");
    draw();
    int option = menus.at(currentMenuPage).getOption();
    if((option-1) == this->network.getDistricts().size()) return "quit";
    string district = this->network.getDistricts().at(option-1);
    currentMenuPage = previousMenuPage;
    menus.pop_back();
    return district;
}

string Program::chooseMunicipality(bool addStopButton, string label) {
    string district = chooseDistrict(addStopButton,label);
    if(district == "quit") return "quit";
    int previousMenuPage = currentMenuPage;
    vector<string> municipalities;
    for(pair<string,string> pair : this->network.getMunicipalities()){
        if(pair.first == district){
            municipalities.push_back(pair.second);
        }
    }
    this->menus.push_back(Menu(municipalities, "Municipalities", label));// Initialize districts submenu
    currentMenuPage = menus.size() - 1;
    if(addStopButton) menus.at(currentMenuPage).addButton("Stop");
    draw();
    int option = menus.at(currentMenuPage).getOption();
    if((option-1) == municipalities.size()) return "quit";
    string municipality = municipalities.at(option-1);
    currentMenuPage = previousMenuPage;
    menus.pop_back();
    return municipality;
}

string Program::chooseStation(bool addStopButton, string label) {
    string municipality = chooseMunicipality(addStopButton, label);
    if(municipality == "quit") return "quit";
    int previousMenuPage = currentMenuPage;
    vector<string> stations;
    for(auto pair : this->network.getStations()){
        Station station = pair.second;
        if(station.getMunicipality() == municipality){
            stations.push_back(station.getName());
        }
    }
    menus.push_back(Menu(stations, "Stations", label));
    currentMenuPage = menus.size()-1;
    if(addStopButton) menus.at(currentMenuPage).addButton("Stop");
    draw();
    int option = menus.at(currentMenuPage).getOption();
    if((option-1) == stations.size()) return "quit";
    string stationName = stations.at(option-1);
    currentMenuPage = previousMenuPage;
    menus.pop_back();
    return stationName;
}

Edge * Program::chooseEdge(bool addStopButton, string label) {
    string source = chooseStation(addStopButton, label);
    if(source == "quit") return NULL;
    int sourceId = this->network.getStationID(source);
    int previousMenuPage = currentMenuPage;
    vector<string> destStations;
    Vertex * sourceVertex = this->network.getTrainNetwork().getVertexSet().at(sourceId);
    for(auto edge : sourceVertex->getAdj()){
        string destStation = this->network.IDtoStation(edge->getDest()->getId());
        destStations.push_back(destStation);
    }
    menus.push_back(Menu(destStations, "Adjacent Stations", label));
    currentMenuPage = menus.size()-1;
    if(addStopButton) menus.at(currentMenuPage).addButton("Stop");
    draw();
    int option = menus.at(currentMenuPage).getOption();
    if((option - 1) == destStations.size()){
        return NULL;
    }
    Edge * edge = sourceVertex->getAdj().at(option - 1);
    currentMenuPage = previousMenuPage;
    menus.pop_back();
    return edge;
}

