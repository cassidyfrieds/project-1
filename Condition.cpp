#include "Condition.h"

Condition::Condition(){
    has=false;
    obj="";
    owner="";
    status="";
}   

Condition::~Condition(){
}


void Condition::printCondition() const {
    if(has) {
        cout << owner << " must have the " << obj << endl;
    }
    else {
        cout << obj << " must be " << status << endl;
    }
}

using namespace std;
