#include <set>
#include <unordered_set>
#include "Network.h"

Network::Network(){
    readStations();
    readNetwork();
}

void Network::readNetwork() {
    ifstream in("../data/network.csv");
    string aLine, origin, dest, capacity, service, trash;
    getline(in, aLine);
    while (getline(in, aLine))
    {
        istringstream inn(aLine);
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, origin, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, origin, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, dest, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, dest, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, capacity, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, capacity, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, service, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, service, ',');
        double gordo = stod(capacity);
        trainNetwork.addBidirectionalEdge(stationToID.at(origin),stationToID.at(dest),gordo, service);
    }
}

void Network::readStations() {
    set<string> seenDistricts;
    set<string> seenMunicipalities;
    ifstream in("../data/stations.csv");
    string aLine, name, district, municipality, township, line, trash;
    getline(in, aLine);
    while (getline(in, aLine))
    {
        istringstream inn(aLine);
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, name, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, name, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, district, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, district, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, municipality, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, municipality, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, township, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, township, ',');
        if (inn.peek()=='\"'){
            getline(inn, trash, '\"');
            getline(inn, line, '\"');
            getline(inn,trash,',');
        }
        else getline(inn, line, ',');
        Station station = Station(name,district,municipality,township,line);
        if(seenDistricts.find(district) == seenDistricts.end()) {
            districts.push_back(district);
            seenDistricts.insert(district);
        }
        if(seenMunicipalities.find(municipality) == seenMunicipalities.end()) {
            municipalities.push_back({district,municipality});
            seenMunicipalities.insert(municipality);
        }
        stationInfo.insert({name,station});
        stationToID.insert({name,stationToID.size()});
        trainNetwork.addVertex(stationToID.size()-1);
    }
}

Graph Network::getTrainNetwork() {
    return trainNetwork;
}

string Network::IDtoStation(int id) {
    for(const auto& c : stationToID){
        if (c.second==id) return c.first;
    }
    return "Station not found";
}

double Network::maxFlow(const std::string& source, const std::string& dest){
    int srcID;
    int destID;
    try {
        srcID = stationToID.at(source);
        destID = stationToID.at(dest);
    }
    catch (const std::out_of_range& oor){
        cout << "Invalid source or destination stations.\n";
        return -1;
    }
    return trainNetwork.edmondsKarp(srcID, destID);
}

Station Network::getStationInfo(string name){
    auto iter = stationInfo.find(name);
    if(iter == stationInfo.end()){
        cout << "Notfound";
        return Station();
    }
    return iter->second;
}

double Network::maxFlowPairs(vector<pair<std::string, std::string>>& stationPairs){
    double currentMax = 0;
    double iterationFlow;
    for (int i = 0; i<stationToID.size(); i++){
        for (int j=i; j<stationToID.size(); j++){
            if (i==j) continue;
            iterationFlow= trainNetwork.edmondsKarp(i,j);
            if (iterationFlow>currentMax){
                currentMax=iterationFlow;
                stationPairs.clear();
                stationPairs.emplace_back(IDtoStation(i), IDtoStation(j));
            }
            else if (iterationFlow==currentMax) stationPairs.emplace_back(IDtoStation(i), IDtoStation(j));
        }
    }
    return currentMax;
}

vector<pair<double,string>> Network::multiMaxFlowDistricts() {
    vector<pair<double,string>>maxflows;

    for(const string& district : this->districts){
        vector<int> sources;
        vector<int> sinks;
        for(auto vertex : this->trainNetwork.getVertexSet()){
            Station station = this->getStationInfo(this->IDtoStation(vertex->getId()));
            if(station.getDistrict() == district){
                sinks.push_back(vertex->getId());
            }else{
                sources.push_back(vertex->getId());
            }
        }
        int megaSourceId = this->trainNetwork.megaSource(sources);
        int megaSinkId = this->trainNetwork.megaSink(sinks);
        double mf = this->trainNetwork.edmondsKarp(megaSourceId,megaSinkId);
        this->trainNetwork.removeVertex(megaSourceId);
        this->trainNetwork.removeVertex(megaSinkId);

        maxflows.emplace_back(mf, district);
    }
    std::sort(maxflows.begin(), maxflows.end());
    return maxflows;
}

vector<pair<double,string>> Network::multiMaxFlowMunicipalities(const std::string &district) {
    vector<pair<double,string>>maxflows;
    vector<string> municipalities;
    for(auto pair : this->municipalities){
        if(pair.first == district){
            municipalities.push_back(pair.second);
        }
    }
    for(const string& munic : municipalities){
        vector<int> sources;
        vector<int> sinks;
        for(auto vertex : this->trainNetwork.getVertexSet()){
            Station station = this->getStationInfo(this->IDtoStation(vertex->getId()));
            if(station.getMunicipality() == munic){
                sinks.push_back(vertex->getId());
            }else{
                sources.push_back(vertex->getId());
            }
        }
        int megaSourceId = this->trainNetwork.megaSource(sources);
        int megaSinkId = this->trainNetwork.megaSink(sinks);
        double mf = this->trainNetwork.edmondsKarp(megaSourceId,megaSinkId);
        this->trainNetwork.removeVertex(megaSourceId);
        this->trainNetwork.removeVertex(megaSinkId);

        maxflows.emplace_back(mf, munic);
    }
    std::sort(maxflows.begin(), maxflows.end());
    return maxflows;
}

double Network::reducedEdgesMaxFlow(const std::string& source, const std::string& dest, unordered_set<Edge *> segments){
    Graph reducedConnectivity;
    for (Vertex* v : trainNetwork.getVertexSet()){
        reducedConnectivity.addVertex(v->getId());
    }
    for (Vertex* v : trainNetwork.getVertexSet()) {
        for (Edge *e: v->getAdj()) {
            reducedConnectivity.addEdge(e->getOrig()->getId(),e->getDest()->getId(),e->getWeight(),e->getService());
        }
    }
    for (auto s : segments){
        reducedConnectivity.getVertexSet().at(s->getOrig()->getId())->removeEdge(s->getDest()->getId());
        reducedConnectivity.getVertexSet().at(s->getDest()->getId())->removeEdge(s->getOrig()->getId());
    }

    int srcID;
    int destID;
    try {
        srcID = stationToID.at(source);
        destID = stationToID.at(dest);
    }
    catch (const std::out_of_range& oor){
        cout << "Invalid source or destination stations.\n";
        return -1;
    }
    return reducedConnectivity.edmondsKarp(srcID,destID);
}

vector<pair<int,pair<double,double>>> Network::segmentFailureEvaluation(const vector<Edge *>& segments) {
    vector<pair<int,pair<double,double>>> stationBeforeAfter;
    pair<double,double> beforeAndAfter;
    Graph reducedConnectivity;
    for (Vertex* v: trainNetwork.getVertexSet()){
        reducedConnectivity.addVertex(v->getId());
    }
    for (Vertex* v: trainNetwork.getVertexSet()){
        for (Edge * e : v->getAdj()){
            reducedConnectivity.addEdge(e->getOrig()->getId(),e->getDest()->getId(),e->getWeight(),e->getService());
        }
    }
    double iterationFlow;
    for (int i = 0; i<stationToID.size(); i++){
        iterationFlow=0;
        for (int j=0; j<stationToID.size(); j++){
            if (i==j) continue;
            iterationFlow+=trainNetwork.edmondsKarp(i,j);
        }
        beforeAndAfter = {iterationFlow/(trainNetwork.getNumVertex()-1),0};
        for (int k=0; k<segments.size();k++) stationBeforeAfter.emplace_back(i,beforeAndAfter);
    }
    for (int m = 0; m<segments.size(); m++) {
        reducedConnectivity.getVertexSet().at(segments.at(m)->getOrig()->getId())->removeEdge(segments.at(m)->getDest()->getId());
        reducedConnectivity.getVertexSet().at(segments.at(m)->getDest()->getId())->removeEdge(segments.at(m)->getOrig()->getId());
        for (int i = 0; i < stationToID.size(); i++) {
            iterationFlow=0;
            for (int j = 0; j < stationToID.size(); j++) {
                if (i == j) continue;
                iterationFlow+=reducedConnectivity.edmondsKarp(i,j);
            }
            stationBeforeAfter.at((i*segments.size())+m).second.second=iterationFlow/(trainNetwork.getNumVertex()-1);
        }
        reducedConnectivity.addBidirectionalEdge(segments.at(m)->getOrig()->getId(),segments.at(m)->getDest()->getId(),segments.at(m)->getWeight(),segments.at(m)->getService());
    }
    return stationBeforeAfter;
}

int Network::getStationID(const string& name) {
    for (auto c : stationToID){
        if (c.first==name) return c.second;
    }
    return -1;
}

double Network::maxArrival(const std::string& station){
    int stationID;
    try {
        stationID = stationToID.at(station);
    }
    catch (const std::out_of_range& oor){
        cout << "Invalid station.\n";
        return -1;
    }
    std::vector<int> sources;
    for(auto v : this->trainNetwork.getVertexSet()){
        if(v->getId() != stationID) sources.push_back(v->getId());
    }
    int sourceId = this->trainNetwork.megaSource(sources);
    double result = this->trainNetwork.edmondsKarp(sourceId, stationID);
    this->trainNetwork.removeVertex(sourceId);
    return result;
}

vector<pair<string,string>> Network::getMunicipalities(){
    return this->municipalities;
}

vector<string> Network::getDistricts(){
    return this->districts;
}

unordered_map<string,Station> Network::getStations(){
    return this->stationInfo;
}
