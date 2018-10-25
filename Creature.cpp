#include "Creature.h"

Creature::Creature(){
    name="";
    status="";
    descrip="";
}   

Creature::~Creature(){
}

void Creature::printCreature() const {
    cout << "Creature name is: " << name << endl;
    cout << "Creature description is: " << descrip << endl;
    cout << "Creature status is: " << status << endl;
    cout << "Creature attack is: " << endl;
        cout << "\t* conditon: ";
        attack.condition.printCondition();
        cout << "\t* print '" << attack.print.substr(0, 50) << "...'" << endl;
        cout << "\t* actions: " << endl;
        for(int i = 0; i < attack.actions.size(); i++) {
            cout << "\t\t- " << attack.actions[i] << endl;
        }
    cout << "Creature vulnerabilities are:" << endl;
    for(int i = 0; i < vulner.size(); i++) {
        cout << "\t* " << vulner[i] << endl;
    }
    cout << "Creature triggers are:" << endl;
    for(int i = 0; i < triggers.size(); i++) {
        triggers[i].printTrigger();
    }
    //cout << "Creature triggers is: "<< triggers << endl;
    cout << "\n" << endl;
};