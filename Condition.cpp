#include "Condition.h"

Condition::Condition(){
    has="";
    obj="";
    owner="";
    status="";
}   

Condition::~Condition(){
}


void Condition::printCondition() const {
   if(has.compare("yes") == 0) {
        cout << owner << " has the " << obj << endl;
    }
    else if(has.compare("no") == 0) {
        cout << owner << " doesn't have " << obj << endl;
    }
    else {
        cout << obj << " must be " << status << endl;
    }  

}

using namespace std;
