#ifndef PROJETO_SCHEDULE_MENU_H
#define PROJETO_SCHEDULE_MENU_H

#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace std;

class Menu {
public:
    /**
     * @brief Constructor of the Menu given a file path and a title.
     * @param file_ File path with the buttons.
     * @param title Menu Title.
     */
    Menu(string file_, string title);

    /**
     * @brief Constructor of the Menu given a vector and a title.
     * @param options Vector with the buttons.
     * @param title Menu Title.
     * @param inputLabel Menu label.
     */
    Menu(vector<string> options, string title, string inputLabel = "");

    /**
     * @brief Constructor of the Menu given an unordered_set and a title
     * @param options Unordered_set with the buttons.
     * @param title Menu title.
     * @param inputLabel Menu label.
     */
    Menu(unordered_set<string> options, string title, string inputLabel = "");

    /**
     * @brief This function returns the buttons of the menu.
     * @return Vector.
     */
    vector<string> getButtons() const;

    /**
     * @brief This function return the option chosen in the Menu.
     * @return Int.
     */
    int getOption() const;

    /**
     * @brief This function adds a button to the Menu.
     * @param button Button to be added.
     */
    void addButton(string button);

    /**
     * @brief This function sets buttons to the Menu.
     * @param buttons_ Buttons to be set.
     */
    void setButtons(vector<string> buttons_);

    /**
     * @brief This funtion draws the Menu.
     */
    void draw();

    /**
     * @brief This function "clears" the screen.
     */
    void clear() const;
private:

    ///@brief Vector of buttons.
    vector<string> buttons;

    ///@brief Option chosen by the user.
    int option;

    ///@brief Menu title.
    string title;

    ///@brief Menu label.
    string inputLabel;
};


#endif //PROJETO_SCHEDULE_MENU_H
