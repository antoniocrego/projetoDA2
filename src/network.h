#ifndef PROJETODA_NETWORK_H
#define PROJETODA_NETWORK_H


#include "data_structures/Graph.h"
#include "Station.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <cmath>

using namespace std;


class Network {
public:

    ///@brief Empty Constructor.
    Network();

    /**
     * @brief Store the information about the Stations from a Data File into a graph.
     * Time Complexity: O(N), N being the number of stations on the file.
     */
    void readStations();

    /**
     * @brief Store the information about the connections between Stations from a Data File into a graph.
     * Time Complexity: O(N), N being the number of connections between stations on the file.
     */
    void readNetwork();

    /**
     * @brief Function that returns the graph related to the connections between Stations.
     * @return Graph.
     */
    Graph getTrainNetwork();

    /**
     * @brief Function that returns the Station related to the given id passed in the parameter.
     * Time Complexity: O(N), N being the number of stations.
     * @param id The id of the Station.
     * @return The Station.
     */
    string IDtoStation(int id);

    /**
     * @brief Function that returns information about the Station given the name.
     * Time Complexity: O(log(N)), N being the number of stations.
     * @param name The name of the Station.
     * @return The Station.
     */
    Station getStationInfo(string name);

    /**
     * @brief Function that returns the unordered_map related to the Stations.
     * @return unordered_map.
     */
    unordered_map<string,Station> getStations();

    /**
     * @brief Function that returns the vector related to the districts.
     * @return vector.
     */
    vector<string> getDistricts();

    /**
     * @brief Function that return the vector related to the municipalities.
     * @return vector.
     */
    vector<pair<string,string>> getMunicipalities();

    /**
     * @brief Function that returns the id of the Station given the name.
     * Time Complexity: O(N), N being the number of stations
     * @param name The name of the Station.
     * @return The Station ID.
     */
    int getStationID(const string& name);

    /**
     * @brief Function that takes two strings as parameters and convert them into id before calling the Edmonds Karp Algorithm.
     * Time Complexity: O(V*E^2) V being the number of vertexes and E being the number of edges.
     * @param source The source Station.
     * @param dest The destination Station.
     * @return The Edmonds Karp function with these two parameters.
     */
    double maxFlow(const std::string& source, const std::string& dest);

    /**
     * @brief Function that calculates the maximum maxflow in the entire railway.
     * Time Complexity: O(V^3 * E^2), V being the number of vertexes in the graph and E being the number of edges.
     * @param stationPairs The vector that is going to store our return.
     * @return The vector stationPairs with the names of the Stations where flow is max and the value of said maximum flow.
     */
    double maxFlowPairs(vector<pair<std::string, std::string>>& stationPairs);

    /**
     * @brief Function that calculates the maxFlow passing through a district.
     * Time Complexity: O(D*(V+V*E^2)), D being the number of districts, V being the number of vertexes and E being the number of edges.
     * @return A vector with pairs that associate the flow that passes through the district with the district's name.
     */
    vector<pair<double,string>> multiMaxFlowDistricts();

    /**
     * @brief Function that calculates the maxFlow for each municipality of a given district.
     * Time Complexity: O(M*(V+V*E^2)), M being the number of municipalities, V being the number of vertexes and E being the number of edges.
     * @param district Name of the district whose municipalities will be used.
     * @return A vector with pairs that associate the flow that passes through the municipality with the municipality's name.
     */
    vector<pair<double,string>> multiMaxFlowMunicipalities(const std::string& district);

    /**
     * @brief Function that calculates the maxFlow between two stations, assuming that certain edges have been "disabled".
     * Time Complexity: O(N+V*E^2) N being the number of segments to be removed, V being the number of vertexes and E being the number of edges.
     * @param source The source station.
     * @param dest The destination station.
     * @param segments The edges which are to be removed.
     * @return The maximum flow value between the two stations in the altered graph.
     */
    double reducedEdgesMaxFlow(const std::string& source, const std::string& dest, unordered_set<Edge *> segments);

    /**
     * @brief Function that calculates the maxFlow for each station once for each edge that indicated to be disabled.
     * Time Complexity: O(N* V^3 * E^2), N being the number of segments to be removed, V being the number of vertexes and E being the number of edges.
     * @param segments The edges which are to be removed.
     * @return Vector that stores pairs that associate a station's ID to its maxFlow before and after removing a specific edge. An entry exists for every edge in the segments unordered set.
     */
    vector<pair<int,pair<double,double>>> segmentFailureEvaluation(const vector<Edge *>& segments);

    /**
     * @brief Calculates how many trains can arrive to one specific station from the entire graph.
     * Time Complexity: O(V*E^2), V being the number of vertexes and E being the number of edges.
     * @param station The station whose maximum arrival will be calculated.
     * @return The maximum flow of trains that can arrive to the station in question.
     */
    double maxArrival(const std::string& station);
private:
    ///@brief Bidirection Graph that represents the connections between Stations.
    Graph trainNetwork;
    ///@brief A vector with all the districts
    vector<string> districts;
    ///@brief A vector of pairs where the first parameter is the district and the second one is the municipality
    vector<pair<string, string>> municipalities;
    ///@brief A map where the first parameter is the name of the Station and the second one is a Station Object.
    unordered_map<string,Station> stationInfo;
    ///@brief A map where the first parameter is the name of the Station and the second one is the Station ID.
    unordered_map<string,int> stationToID;
};


#endif //PROJETODA_NETWORK_H
