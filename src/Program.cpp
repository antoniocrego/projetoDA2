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

                    case 2:

                    case 3:

                    case 4:

                    case 5:

                    case 6:

                    case 7:

                    case 8:

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