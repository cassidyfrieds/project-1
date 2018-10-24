#include "Item.h"

using namespace std;

Item::Item(){
    name="";
    descrip="";
    writing="";
    status="";
    turnon.print="";
    turnon.action = "";
}   

Item::~Item(){}

void Item::printItem() const {
    cout << "Item name is: " << name << endl;
    cout << "Item description is: " << descrip << endl;
    cout << "Item writing is: " << writing << endl;
    cout << "Item status is: " << status << endl;
    cout << "Item turnon print is: " << turnon.print << endl;
    cout << "Item turnon action is: " << turnon.action << endl;
    cout << "\n" << endl;
};