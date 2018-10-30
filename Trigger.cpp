#include "Trigger.h"

using namespace std;

Trigger::Trigger(){
    type = "";
    print = "";
    triggered = false;
}

Trigger::~Trigger(){

}

void Trigger::printTrigger() const {
    cout << "\t* Trigger details: " << endl;
    cout << "\t\t- type: " << type << endl;
    cout << "\t\t- print '" << print.substr(0, 50) << "...'" << endl;
    cout << "\t\t- commands:" << endl;
    for(int i = 0; i < commands.size(); i++) {
        cout << "\t\t\t~ " << commands[i] << endl;
    }
    cout << "\t\t- conditions:" << endl;
    for(int i = 0; i < conditions.size(); i++) {
        cout << "\t\t\t~ ";
        conditions[i].printCondition();
        
    }
    cout << "\n" << endl;
};
