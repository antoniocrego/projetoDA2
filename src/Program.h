#ifndef AERIALTRANSPORTATION_PROGRAM_H
#define AERIALTRANSPORTATION_PROGRAM_H

#include "menu/Menu.h"
#include "network.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Program {
public:

    ///@brief Empty Constructor.
    Program();

    /**
     * @brief This function returns the Current Page
     * @return Int
     */
    int getCurrentPage()const;

    /**
     * @brief This function sets the current page to a given one
     * @param newCurrentpage New current page
     */
    void setCurrentPage(int newCurrentpage);

    /**
     * @brief This function draws the current Menu
     */
    void draw();

    /**
     * @brief This function runs the program
     */
    void run();

    /**
     * @brief This function inicializes the menu
     */
    void createMenu();

    /**
     * @brief This function makes the program wait until the user wants it to continue
     */
    void wait() const;

    /**
     * @brief This function "clears" the Terminal
     */
    void clear() const;

private:

    ///@brief Current Menu Page
    int currentMenuPage;

    ///@brief Vector related to the Program's Menus
    vector<Menu> menus;

    ///@brief Program's Network
    Network network;

    /**
     * @brief This function asks the user to choose a Station
     * @param addStopButton
     * @param label Label to add to the menu presented to the user
     * @return The name of the chosen Station
     */
    string chooseStation(bool addStopButton, string label = "");

    /**
     * @brief This function asks the user to choose a District
     * @param addStopButton
     * @param label Label to add to the menu presented to the user
     * @return The name of the chosen District
     */
    string chooseDistrict(bool addStopButton, string label = "");

    /**
     * @brief This function asks the user to choose a Municipality
     * @param addStopButton
     * @param label Label to add to the menu presented to the user
     * @return The name of the chosen Station
     */
    string chooseMunicipality(bool addStopButton, string label = "");

    /**
     * @brief This function asks the user to choose a Edge
     * @param addStopButton
     * @param label Label to add to the menu presented to the user
     * @return The name of the chosen Edge
     */
    Edge *chooseEdge(bool addStopButton, string label = "");
};


#endif
