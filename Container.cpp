#include "Container.h"

Container::Container(){
        name = "";
        status = "";
        descrip = "";
        open = false;
}   

Container::~Container(){
}

void Container::printContainer() const {
        cout << "Container name is: " << name << endl;
        cout << "Container status is: " << status << endl;
        cout << "Container description is: " << descrip << endl;
        cout << "Container accepts:" << endl;
        for(int i = 0; i < accepts.size(); i++) {
                cout << "\t* " << accepts[i] << endl;
        }
        cout << "Container items:" << endl;
        for(int i = 0; i < items.size(); i++) {
                items[i].printItem();
        }        
        cout << "Container triggers:" << endl;
        for(int i = 0; i < triggers.size(); i++) {
                triggers[i].printTrigger();
        }      
        cout << "\n" << endl;
}

using namespace std;
