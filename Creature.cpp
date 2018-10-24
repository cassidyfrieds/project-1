#include "Creature.h"

using namespace std;

Creature::Creature(){
    name="";
    status="";
    descrip="";
    attack="";

}   

Creature::~Creature(){
}
void Creature::printCreature() const {
    cout << "Creature name is: " << name << endl;
    cout << "Creature description is: " << descrip << endl;
    cout << "Creature status is: " << status << endl;
    cout << "Creature attack is: " << attack << endl;
    //cout << "Creature triggers is: "<< triggers << endl;
    cout << "\n" << endl;
};