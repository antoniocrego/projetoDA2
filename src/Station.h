//
// Created by António Rego on 3/21/2023.
//

#ifndef PROJETODA_STATION_H
#define PROJETODA_STATION_H

#include <string>
using namespace std;

class Station {
public:
    ///@brief Empty constructor.
    Station();
    /**
     * @brief Constructor with 5 parameters refering to the Station.
     * @param name The name of the Station.
     * @param district The District where the Station is located.
     * @param municipality The Municipality where the Station is located.
     * @param township The Towship where the Station is located.
     * @param line The line where the Station belongs to.
     */
    Station(string name, string district, string municipality, string township, string line);
    /**
     * @brief Function that returns the Name of the Station.
     * @return Name.
     */
    string getName();
    /**
     * @brief Function that returns the District where the Station is located.
     * @return District.
     */
    string getDistrict();

    /**
     * @brief Function that returns the Municipality where the Station is located.
     * @return The Municipality.
     */
    string getMunicipality();
    /**
     * @brief Function that returns the Township where the Station is located.
     * @return The Township.
     */
    string getTownship();
    /**
     * @brief Function that returns the Line where the Station belong to.
     * @return The Line.
     */
    string getLine();
    /**
     * @brief Function that compares 2 Stations by the following order: Township,Municipality,District,Name
     * @param s1
     * @return The 2 Stations ordered.
     */
    bool operator<(Station s1) const;
private:
    ///@brief The name of the Station.
    string name;
    ///@brief The District where the Station is located.
    string district;
    ///@brief The Municipality where the Station is located.
    string municipality;
    ///@brief The Towship where the Station is located.
    string township;
    ///@brief The line where the Station belongs to.
    string line;
};


#endif //PROJETODA_STATION_H
