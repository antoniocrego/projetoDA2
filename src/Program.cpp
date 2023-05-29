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
    this->menus.push_back(Menu("../src/menu/files/mainMenu.txt", "TSP Analyzer"));//Initialize main menu
    this->menus.push_back(Menu("../src/menu/files/graphTypeMenu.txt", "Graph Type Selector")); // Initialize Graph type submenu
    this->menus.push_back(Menu("../src/menu/files/toyMenu.txt", "Toy Graph Selector"));
    this->menus.push_back(Menu("../src/menu/files/extraMenu.txt", "Extra Fully Connected Selector"));
    this->menus.push_back(Menu("../src/menu/files/realMenu.txt", "Real World Graph Selector"));
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
        string path = "";
        int option = menus.at(currentMenuPage).getOption();
        switch (this->currentMenuPage) {
            case 0: // Is on main menu
                switch (option) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        this->currentMenuPage = 1;
                        break;
                    case 5:
                        this->currentMenuPage = -1;
                        break;
                }
                break;
            case 1:
                switch (option) {
                    case 1:
                        this->currentMenuPage = 2;
                        break;
                    case 2:
                        this->currentMenuPage = 3;
                        break;
                    case 3:
                        this->currentMenuPage = 4;
                        break;
                }
                break;
            case 2:
                path = "Toy-Graphs/";
                switch (option) {
                    case 1:
                        path += "shipping.csv";
                        break;
                    case 2:
                        path += "stadiums.csv";
                        break;
                    case 3:
                        path += "tourism.csv";
                        break;
                }
                if(path != "Toy-Graphs/"){
                    network.readDataset(path, false);
                    currentMenuPage = 0;
                }
                break;
            case 3:
                path = "Extra_Fully_Connected_Graphs/edges_";
                switch (option) {
                    case 1:
                        path += "25.csv";
                        break;
                    case 2:
                        path += "50.csv";
                        break;
                    case 3:
                        path += "75.csv";
                        break;
                    case 4:
                        path += "100.csv";
                        break;
                    case 5:
                        path += "200.csv";
                        break;
                    case 6:
                        path += "300.csv";
                        break;
                    case 7:
                        path += "400.csv";
                        break;
                    case 8:
                        path += "500.csv";
                        break;
                    case 9:
                        path += "600.csv";
                        break;
                    case 10:
                        path += "700.csv";
                        break;
                    case 11:
                        path += "800.csv";
                        break;
                    case 12:
                        path += "900.csv";
                        break;
                }
                if(path != "Extra_Fully_Connected/edges_") {
                    network.readDataset(path, false);
                    currentMenuPage = 0;
                }
                break;
            case 4:
                path = "Real-world Graphs/";
                switch (option) {
                    case 1:
                        path += "graph1/";
                        break;
                    case 2:
                        path += "graph2/";
                        break;
                    case 3:
                        path += "graph3/";
                        break;
                }
                if(path != "Real-world Graphs/") {
                    network.readDataset(path, true);
                    currentMenuPage = 0;
                }
                break;
        }
// runs until quit option is selected
    }
}