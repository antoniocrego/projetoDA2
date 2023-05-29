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

    int chooseGraphType();
};


#endif
