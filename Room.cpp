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

void Room::printRoomBorders() const{
    cout << name << "has borders: " <<endl;
    for(int i = 0; i < borders.size(); i++) {
                //cout << borders[i].room.name << ", " << borders[i].direction << endl;
        }  
}

void Room::printRoom() const {
    cout << "Room name is: " << name << endl;
    cout << "Room description is: " << descrip << endl;
    cout << "Room status is: " << status << endl;
    cout << "Room type is: " << type << endl;
    cout << "Room containers:" << endl;
     for(int i = 0; i < containers.size(); i++) {
                containers[i].printContainer();
        }      
    cout << "Room items:" << endl;
     for(int i = 0; i < items.size(); i++) {
                items[i].printItem();
        }   
    cout << "Room creatures:" << endl;
     for(int i = 0; i < creatures.size(); i++) {
                creatures[i].printCreature();
        }    
    cout << "Room borders:" << endl;
     for(int i = 0; i < borders.size(); i++) {
            cout << borders[i].name << ", " << borders[i].direction << endl;
        }  
    cout << "Room triggers:" << endl;
     for(int i = 0; i < triggers.size(); i++) {
                triggers[i].printTrigger();
        }      

    //cout << "Room border name is: " << border.name << endl;
    //cout << "Room border direction is: " << border.direction << endl;

    cout << "\n" << endl;
};