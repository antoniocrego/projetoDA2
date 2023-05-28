#include "Menu.h"

Menu::Menu(string file_, string title) {
    this->title = title;
    ifstream file(file_);
    for(string button; getline(file,button);){
        this->buttons.push_back(button);
    }
}

Menu::Menu(vector<string> options, string title, string inputLabel){
    this->title = title;
    this->inputLabel = inputLabel;
    for(string option : options){
        buttons.push_back(option);
    }
}

Menu::Menu(unordered_set<string> options, string title, string inputLabel){
    this->title = title;
    this->inputLabel = inputLabel;
    for(string option : options){
        buttons.push_back(option);
    }
}

vector<string> Menu::getButtons() const {
    return buttons;
}

int Menu::getOption() const {
    return option;
}

/**Functionality: Setter for the buttons
 * Description: Receives a vector of string as a parameter and replaces the current buttons of the new ones
 *
 * @param  buttons_ a vector of strings
 */
void Menu::setButtons(vector<string> buttons_) {
    this->buttons = buttons_;
}

void Menu::addButton(string button) {
    this->buttons.push_back(button);
}

void Menu::clear() const {
    for(int i = 0; i < 20; i++){
        cout << endl;
    }
}

/**Functionality:Draw a menu
 *
 * Description:This function will get the attribute of the Menu buttons and will go through all of them printing them in a pleasant way
 * it will also ask the user for a input however it will not handle the user input.
 *
 */
void Menu::draw(){
    string o;
    clear();
    string display;
    display =" _____________________________________________ \n"
             "|<<<<<<<<<<<<<<<<<>  Menu  <>>>>>>>>>>>>>>>>>>|\n"
             "|---------------------------------------------|\n";
    display+= "| "+title+"     ";
    for(int j=0; j<(39-this->title.length());j++) display+=" "; //adding spaces to format the menu
    display+="|\n";
    display += "|---------------------------------------------|\n";

    for(int i=0; i<this->buttons.size();i++){
        display+="| ["+to_string(i+1)+"] "+this->buttons[i];
        for(int j=0; j<(39-this->buttons[i].length());j++) display+=" "; //adding spaces to format the menu
        if(i<9) display+=' ';
        display+="|\n";
    }
    display+="|_____________________________________________|\n";
    cout << display <<endl;
    if(inputLabel != "") cout << "Selecting " <<inputLabel << endl;
    cout << "Choose an option: ";
    while (true){
        cin >> o;
        try {
            option = stoi(o);
            if(option >= 1 and option <= buttons.size()) break;
            cout << "\nInvalid option! Please introduce a valid one: ";
        }
        catch (std::invalid_argument ia){
            cout << "\nInvalid option! Please introduce a valid one: ";
        }
    }
}
