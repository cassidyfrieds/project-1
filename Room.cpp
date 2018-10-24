#include "Room.h"

using namespace std;

Room::Room( ){
    name = "";
    status = "";
    descrip = "";
    type = "";
    //borders.name = "";
    //borders.direction = ""; //idk if we need to change this bc border has to be a vector too
}

Room::~Room(){}

void Room::printRoom() const {
    cout << "Room name is: " << name << endl;
    cout << "Room description is: " << descrip << endl;
    cout << "Room status is: " << status << endl;
    cout << "Room type is: " << type << endl;
    //cout << "Room border name is: " << border.name << endl;
    //cout << "Room border direction is: " << border.direction << endl;

    cout << "\n" << endl;
};