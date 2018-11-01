#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

#include "Item.h"
#include "Container.h"
#include "Room.h"
#include "Creature.h"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

map<std::string, Item> allItems;
map<std::string, Creature> allCreatures;
map<std::string, Container> allContainers;
map<std::string, Room> allRooms;
Room* currRoom;

//behind the scenes commands declarations
bool Add(Item*, Room*);
bool Add(Creature*, Room*);
bool Add(Item*, Container*);
bool Add(Container*, Room*);
bool Delete(Room*);
bool Delete(Item*);
bool Delete(Container*);
bool Delete(Creature*);
bool Update(Room*, string);
bool Update(Container*, string);
bool Update(Creature*, string);
bool Update(Item*, string);
void GameOver(bool);

vector<string> splitString(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

bool checkTriggerCondition(Trigger trig){
    bool tempTrig = true;
    for (int i=0; i<(trig.conditions.size()); i++){
        if (trig.conditions[i].has.compare("yes") ==  0 ){
            tempTrig = false;
            Container owner = allContainers[trig.conditions[i].owner];
            Item obj = allItems[trig.conditions[i].obj];
            for(int x=0; x<(owner.items.size()); x++){
                if (owner.items[x]->name.compare(obj.name) == 0){
                    tempTrig = true;
                }
            }
        }
        else if (trig.conditions[i].has.compare("no") ==  0){
            Container owner = allContainers[trig.conditions[i].owner];
            Item obj = allItems[trig.conditions[i].obj];
            for(int x=0; x<(owner.items.size()); x++){
                if (owner.items[x]->name.compare(obj.name) == 0){
                    tempTrig = false;
                }
            }
        }
        else{
            Container obj1;
            Item obj2;
            if (allItems.find(trig.conditions[i].obj) == allItems.end()){
                Container obj1 = allContainers[trig.conditions[i].obj];
            }
            else{
                Item obj2 = allItems[trig.conditions[i].obj];
            }

            if (obj1.status.compare(trig.conditions[i].status) == 0 || obj2.status.compare(trig.conditions[i].status) == 0){
                tempTrig = false;
            }
        }
    }
    return tempTrig;
}

bool isTriggered(string command) {
    //check if triggered
    bool triggered = false;
    // For each of the room's triggers
    for (int x=0; x<(currRoom->triggers.size()); x++){
        // For each of the trigger's commands
        for (int y=0; y<(currRoom->triggers[x].commands.size()); y++){
            // If the current command is in the trigger's commands
            if (currRoom->triggers[x].commands[y].compare(command) == 0){
                triggered = checkTriggerCondition(currRoom->triggers[x]);
                break;
            }
        }
        if (triggered==true){
            cout << currRoom->triggers[x].print << endl;
        }
    }
    return triggered;
}

bool parseAction(string action){
    if (action == ""){
        return false;
    }
    bool actionComp = false;

    // Split action phrase into words
    vector<string> tokens = splitString(action, ' ');

    //check if add funciton
    if (tokens[0].compare("Add") == 0 && tokens.size()== 4){
        //get first object (can be container or item)
        if (allItems.find(tokens[1]) != allItems.end()){
            //it is an item
            Item* obj1 = &allItems[tokens[1]];
            //get second obj (can be container or room)
            if (allContainers.find(tokens[3]) != allContainers.end()){
                //it is a container
                Container* obj2 = &allContainers[tokens[3]];
                //adding item to container
                actionComp = Add(obj1, obj2);
            }
            else{
                //canot be found in allContainers it is a room
                Room* obj2 = &allRooms[tokens[3]];
                //adding item to room
                actionComp = Add(obj1, obj2);
            }
        }
        else {
            //canot be found in allItems it is a container
            Container* obj1 = &allContainers[tokens[1]];
            //if it is a container it must be added to a room
            Room* obj2 = &allRooms[tokens[3]];
            //adding container to room
            actionComp = Add(obj1, obj2);
        }
    }

    //check if delete function
    if (tokens[0].compare("Delete") == 0 && tokens.size()== 2){
        if (allItems.find(tokens[1]) != allItems.end()){
            Item* obj = &allItems[tokens[1]];
            actionComp = Delete(obj);
        } 
        else if (allContainers.find(tokens[1]) != allContainers.end()) {
            Container* obj = &allContainers[tokens[1]];
            actionComp = Delete(obj);
        } 
        else if (allRooms.find(tokens[1]) != allRooms.end()) {
            Room* obj = &allRooms[tokens[1]];
            actionComp = Delete(obj);
        } 
        else if (allCreatures.find(tokens[1]) != allCreatures.end()) {
            Creature* obj = &allCreatures[tokens[1]];
            actionComp = Delete(obj);
        } 
        else {
            // Not found at all
            cout << "ERROR: obj " << tokens[1] << " not found." << endl;
        }
    }

    //check if update function
    if (tokens[0].compare("Update") == 0 && tokens.size() == 4){
        if (allItems.find(tokens[1]) != allItems.end()){
            //is a item
            Item* obj = &allItems[tokens[1]];
            actionComp = Update(obj, tokens[3]);
        }
        else if (allContainers.find(tokens[1]) != allContainers.end()){
            //is a container, update container status
            Container* obj = &allContainers[tokens[1]];
            actionComp = Update(obj, tokens[3]);
        }
        else {
            //not a container or item, must be a creature, update his status!
            Creature* obj = &allCreatures[tokens[1]];
            actionComp = Update(obj, tokens[3]);
        }
    }

    return actionComp;
}

int main(int argc, char* argv[] ){

    /*
        Read the file
    */
	xml_document<> doc;
	xml_node<> * root_node;
	ifstream theFile (argv[1]);         // Read the xml file into a vector
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    theFile.close();

	doc.parse<0>(&buffer[0]); 	        // Parse the buffer using the xml file parsing library into doc
	root_node = doc.first_node("map");  // Find our map node
    if (root_node == NULL){
        cout << "cant get root node" << endl;
        return 0;
    }

    /*
        Parse Items
    */
    for(xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
        Item temp;
        // string name
        if(item_node->first_node("name")) {
            temp.name = item_node->first_node("name")->value();
        }
        // string writing
        if(item_node->first_node("writing")) {
            temp.writing = item_node->first_node("writing")->value();
        }
        // string status
        if(item_node->first_node("status")) {
            temp.status = item_node->first_node("status")->value();
        }
        // string description
        if(item_node->first_node("description")) {
            temp.descrip = item_node->first_node("description")->value();
        }
        // TurnOn turnon
        if(item_node->first_node("turnon")) {
            if(item_node->first_node("turnon")->first_node("print")) {
                temp.turnon.print = item_node->first_node("turnon")->first_node("print")->value();
            }
            if(item_node->first_node("turnon")->first_node("action")) {
                temp.turnon.action = item_node->first_node("turnon")->first_node("action")->value();
            }
        }
        // vector<Trigger> triggers
        for(xml_node<> * trigger_node = item_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }
        //temp.printItem();
        allItems[temp.name] = temp;
    }

    /*
        Parse creatures
    */
    for(xml_node<> * creature_node = root_node->first_node("creature"); creature_node; creature_node = creature_node->next_sibling("creature")) {
        Creature temp;
        // string name
        if(creature_node->first_node("name")) {
            temp.name = creature_node->first_node("name")->value();
        }
        // string status
        if(creature_node->first_node("status")) {
            temp.status = creature_node->first_node("status")->value();
        }
        // string description
        if(creature_node->first_node("description")) {
            temp.descrip = creature_node->first_node("description")->value();
        }
        xml_node<> * attack_node = creature_node->first_node("attack");
        // Attack attack
        if(attack_node) {
            // vector<Condition> attack.conditions
            for(xml_node<> * condition_node = attack_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                temp.attack.conditions.push_back(tempCondition);
            }
            // string attack.print
            if(attack_node->first_node("print")) {
                temp.attack.print = attack_node->first_node("print")->value();
            }
            // vector<string> attack.actions
            for(xml_node<> * action_node = attack_node->first_node("action"); action_node; action_node = action_node->next_sibling("action")) {
                temp.attack.actions.push_back(action_node->value());
            }
        }
        // vector<String> vulner
        for(xml_node<> * vuln_node = creature_node->first_node("vulnerability"); vuln_node; vuln_node = vuln_node->next_sibling("vulnerability")) {
            temp.vulner.push_back(vuln_node->value());
        }
        // vector<Trigger> triggers
        for(xml_node<> * trigger_node = creature_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }
        //temp.printCreature();
        allCreatures[temp.name] = temp;
    }

    /*
        Parse containers
    */
    for(xml_node<> * container_node = root_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")) {
        Container temp;
        // string name
        if(container_node->first_node("name")) {
            temp.name = container_node->first_node("name")->value();
        }
        // string status
        if(container_node->first_node("status")) {
            temp.status = container_node->first_node("status")->value();
        }
        // string descrip
        if(container_node->first_node("description")) {
            temp.descrip = container_node->first_node("description")->value();
        }
        // vector <string> accepts
        for(xml_node<> * accept_node = container_node->first_node("accept"); accept_node; accept_node = accept_node->next_sibling("accept")) {
            temp.accepts.push_back(accept_node->value());
        }
        // vector <Item> items
        for(xml_node<> * item_node = container_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
            string itemName = item_node->value();
            temp.items.push_back(&allItems[itemName]);
        }
        // vector <Trigger> triggers;
        for(xml_node<> * trigger_node = container_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }

        //temp.printContainer();
        allContainers[temp.name] = temp;
    }

    /*
        Parse rooms
    */
    for(xml_node<> * room_node = root_node->first_node("room"); room_node; room_node = room_node->next_sibling("room")) {
        Room temp;
        if(room_node->first_node("name")) {
            temp.name = room_node->first_node("name")->value();
        }
        if(room_node->first_node("status")) {
            temp.status = room_node->first_node("status")->value();
        }
        if(room_node->first_node("description")) {
            temp.descrip = room_node->first_node("description")->value();
        }
        if(room_node->first_node("type")) {
            temp.type = room_node->first_node("type")->value();
        }

        // vector <Item> items
        for(xml_node<> * item_node = room_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
            string itemName = item_node->value();
            temp.items.push_back(&allItems[itemName]);
        }

        // vector <Creature> creatures
        for(xml_node<> * creature_node = room_node->first_node("creature"); creature_node; creature_node = creature_node->next_sibling("creature")) {
            string creatureName = creature_node->value();
            temp.creatures.push_back(&allCreatures[creatureName]);
        }

        // vector <Container> containers
        for(xml_node<> * container_node = room_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")) {
            string containerName = container_node->value();
            temp.containers.push_back(&allContainers[containerName]);
        }

         // vector <Border> borderss;
        for(xml_node<> * border_node = room_node->first_node("border"); border_node; border_node = border_node->next_sibling("border")) {
            Border tempBorder;
            //string border.direction
            if(border_node->first_node("direction")) {
                tempBorder.direction = border_node->first_node("direction")->value();
            }
            //string border.name
            if(border_node->first_node("name")) {
                tempBorder.name = border_node->first_node("name")->value();
            }
            temp.borders.push_back(tempBorder);
        }


        // vector <Trigger> triggers;
        for(xml_node<> * trigger_node = room_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }

            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }

            temp.triggers.push_back(tempTrigger);
        }

        //temp.printRoom();

        allRooms[temp.name] = temp;
    }

    // Make current room
    currRoom = &allRooms["Entrance"];
    //currRoom->printRoom();
    cout << currRoom->descrip <<endl;

    /* Start Reading Input */
    Container temp;
    allContainers["inventory"] = temp;

    while(true) {
        // Gather input string
        string input;
        cout << "> ";
        getline(cin, input);

        // Split string at spaces
        vector<string> commands = splitString(input, ' ');

        // Parse instructions
        if(commands.size() > 0) {
            string key = commands[0];
            // Move on if no triggers found
            if(isTriggered(key)) {
                // Block the action from happening
            }
            else if ((key == "n" || key == "s" || key == "e" || key == "w")) {
                bool roomChange = false;
                for (int i=0; i<(currRoom->borders.size()); i++){
                    if (key== "n" && (currRoom->borders[i].direction).compare("north")==0 ){
                        Room* temp = &allRooms[currRoom->borders[i].name];
                        currRoom = temp;
                        roomChange = true;
                        break;
                    }
                    if (key== "s" && (currRoom->borders[i].direction).compare("south")==0 ){
                        Room* temp = &allRooms[currRoom->borders[i].name];
                        currRoom = temp;
                        roomChange = true;
                        break;
                    }
                    if (key== "e" && (currRoom->borders[i].direction).compare("east")==0 ){
                        Room* temp = &allRooms[currRoom->borders[i].name];
                        currRoom = temp;
                        roomChange = true;
                        break;
                    }
                    if (key== "w" && (currRoom->borders[i].direction).compare("west")==0 ){
                        Room* temp = &allRooms[currRoom->borders[i].name];
                        currRoom = temp;
                        roomChange = true;
                        break;
                    }
                }
                if (roomChange == true){
                    cout << currRoom->descrip <<endl;
                    for (int x=0; x<(currRoom->triggers.size()); x++){
                        bool triggered = checkTriggerCondition(currRoom->triggers[x]);
                        if (triggered==true){
                                cout << currRoom->triggers[x].print << endl;
                        }
                    }
                }
                else{
                    cout << "Can't go that way." << endl;
                }
            }
            else if (key == "i") {
                // Print all items in inventory
                cout << "Inventory: ";
                vector<Item*> inventory = allContainers["inventory"].items;
                if(inventory.size() == 0) {
                    cout << "empty";
                }
                for(int i = 0; i < inventory.size(); i++) {
                    cout << inventory[i]->name;
                    if(i < inventory.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
            else if (key == "take" && commands.size() > 1) {
                // changes item ownership from room or container to inventory
                string itemName = commands[1];
                bool found = false;
                // Check if the item is in the room
                for(int i = 0; i < currRoom->items.size(); i++) {
                    if(currRoom->items[i]->name == itemName) {
                        allContainers["inventory"].items.push_back(currRoom->items.at(i));
                        currRoom->items.erase(currRoom->items.begin() + i);
                        found = true;
                        break;
                    }
                }
                // Check containers
                for(int i = 0; !found && i < currRoom->containers.size(); i++) {
                    if(currRoom->containers[i]->open) {
                        for(int j = 0; j < currRoom->containers[i]->items.size(); j++) {
                            if(currRoom->containers[i]->items[j]->name == itemName) {
                                allContainers["inventory"].items.push_back(currRoom->containers[i]->items[j]);
                                currRoom->containers[i]->items.erase(currRoom->containers[i]->items.begin() + j);
                                //update container in all container
                                //allContainers[currRoom->containers[i].name].items.erase(allContainers[currRoom->containers[i].name].items.begin()+j);
                                found = true;
                                break;
                            }
                        }
                    }
                }
                if(found) {
                    cout << "Item " << itemName << " added to inventory." << endl;
                } else {
                    cout << itemName << " not found." << endl;
                }
            }
            else if (key == "open" && commands.size() > 1) {
                bool over = false;

                //Update(*currRoom, "woohoo"); //FIX ME testing
                //cout<<currRoom->status<<endl; //FIX ME testing

                if(commands[1] == "exit") {
                    // Open Exit
                    if(currRoom->type == "exit") {
                        over = true;
                        GameOver(over);
                        cout << "Game Over" << endl;
                        return 0;
                    }
                    else {
                        cout << "This is not an exit." << endl;
                    }
                }
                else {
                    // Open CONTAINER
                    //need to make sure containername is in currentRoom containers and exists
                    bool foundContainer = false; //T if container is found in current room
                    for(int i = 0; i < currRoom->containers.size(); i++){
                        if(currRoom->containers[i]->name == commands[1]){
                            foundContainer = true; //container is found within room
                            Container* temp = currRoom->containers[i];
                            temp->open = true; //container was opened
                            //allContainers[temp->name].open = true; //updates in allContainers
                            if(temp->items.empty()) { //container is empty
                                cout << temp->name << " is empty. " << endl;
                            }
                            else{ //container contains items
                                cout << temp->name << " contains ";
                                cout << temp->items[0]->name;
                                for(int i = 1; i < temp->items.size(); i++){
                                    cout << ", " << temp->items[i]->name;
                                }
                                cout << endl;
                            }
                            break;
                        }
                    }
                    if(!foundContainer){ //if container is not found
                        cout << "Container not found" << endl; //or print error
                    }
                }
            }
            else if (key == "read" && commands.size() > 1) {
                //prints whatever is written on an object that the player has
                string itemName = commands[1];
                bool found = false;
                for(int i = 0; i < allContainers["inventory"].items.size(); i++) {
                    if(allContainers["inventory"].items[i]->name == itemName) {
                        if(allContainers["inventory"].items[i]->writing != "") {
                            cout << allContainers["inventory"].items[i]->writing << endl;
                        }
                        else {
                            cout << "Nothing written." << endl;
                        }
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    cout << itemName << " not in inventory." << endl;
                }
            }
            else if (key == "drop" && commands.size() > 1) {
                // changes item ownership from inventory to present room and prints “(item) dropped.”
                string itemName = commands[1];
                bool found = false;
                // Check if the item is in the inventory
                for(int i = 0; i < allContainers["inventory"].items.size(); i++) {
                    if(allContainers["inventory"].items[i]->name == itemName) {
                        currRoom->items.push_back(allContainers["inventory"].items[i]);
                        allContainers["inventory"].items.erase(allContainers["inventory"].items.begin() + i);
                        found = true;
                        cout << itemName << " dropped." << endl;
                        break;
                    }
                }
                if(!found) {
                    cout << itemName << " not found." << endl;
                }
            }
            else if (key == "put" && commands.size() > 3) {
                // adds the item to the containers inventory and and prints “Item (item) added to (container).
                string itemName = commands[1];
                string containerName = commands[3];
                bool foundItem = false;
                bool foundContainer = false;
                bool triggered = false;
                // Check if the item is in the inventory
                for(int i = 0; i < allContainers["inventory"].items.size(); i++) {
                    if(allContainers["inventory"].items[i]->name == itemName) {
                        foundItem = true;
                        // Check if the container is in the room
                        for(int k = 0; k < currRoom->containers.size(); k++) {
                            if(currRoom->containers[k]->name == containerName) {
                                foundContainer = true;
                                if(currRoom->containers[k]->open) {
                                    currRoom->containers[k]->items.push_back(allContainers["inventory"].items[i]);
                                    allContainers["inventory"].items.erase(allContainers["inventory"].items.begin() + k);
                                    //updating said container so that trigger can be checked
                                    //allContainers[containerName].items.push_back(allItems[itemName]);
                                    cout << "Item " << itemName <<  " added to " << containerName << "." << endl;
                                    // TODO: do we need to check if this opens the container?

                                    //checks if putting item in container sets off a container trigger
                                    for (int x=0; x<(currRoom->containers[k]->triggers.size()); x++){
                                        triggered = checkTriggerCondition(currRoom->containers[k]->triggers[x]);
                                        if (triggered){
                                            cout << currRoom->containers[k]->triggers[x].print << endl;
                                            bool triggerAction = parseAction(currRoom->containers[k]->triggers[x].action);
                                        }
                                    }
                                    break;
                                } else {
                                    cout << containerName << " is closed." << endl;
                                }
                            }
                        }
                        if(!foundContainer) {
                            cout << containerName << " isn't in this room." << endl;
                        } else {
                            break;
                        }
                    }
                }
                if(!foundItem) {
                    cout << itemName << " not in inventory." << endl;
                }
            }
            else if (key == "turn" && commands.size() > 2 && commands[1] == "on") {
                // activates an item if it is in the player’s inventory printing “You activate the (item).”
                string itemName = commands[2];
                // Check if the item is in the inventory
                bool foundItem = false;
                for(int i = 0; i < allContainers["inventory"].items.size(); i++) {
                    if(allContainers["inventory"].items[i]->name == itemName) {
                        foundItem = true;
                        cout << "You activate the " << itemName << endl;
                        cout << allContainers["inventory"].items[i]->turnon.print << endl;
                        //parse the action
                        bool action = parseAction(allContainers["inventory"].items[i]->turnon.action);
                        //cout << allContainers["inventory"].items[i]->status << endl;
                    }
                }
                if(!foundItem) {
                    cout << itemName << " not in inventory." << endl;
                }
            }
            else if (key == "attack" && commands.size() > 3 && commands[2] == "with") {
                // prints “You assault the (creature) with the (item).” and executes “attack” elements
                // if item matches creature’s “vulnerability” and existing conditions are met
                string creatureName = commands[1];
                string itemName = commands[3];
                // Check if the item is in the inventory
                bool foundItem = false, foundCreature = false, foundVulner = false;
                for(int i = 0; !foundItem && i < allContainers["inventory"].items.size(); i++) {
                    if(allContainers["inventory"].items[i]->name == itemName) {
                        foundItem = true;
                        
                        for(int j = 0; !foundCreature && j < currRoom->creatures.size(); j++) {
                            if(currRoom->creatures[j]->name == creatureName) {
                                // The creature is in the room!
                                foundCreature = true;

                                for(int v = 0; !foundVulner && v < allCreatures[creatureName].vulner.size(); v++) {
                                    if(allCreatures[creatureName].vulner[v] == itemName) {
                                        // The creature is vulnerable!
                                        foundVulner = true;
                                        cout << "You assault the " << creatureName << " with the " << itemName << "." << endl;
                                        for (int x = 0; x < allCreatures[creatureName].attack.actions.size(); x++) {
                                            parseAction(allCreatures[creatureName].attack.actions[x]);
                                        }
                                        break;
                                    }
                                }
                                if(!foundVulner) {
                                    cout << "The " << creatureName << " is not vulnerable to " << itemName << endl;
                                }
                                break;

                            }
                        }
                        if(!foundCreature) {
                            cout << creatureName << " not in room." << endl;
                        }
                        break;
                    }
                }
                if(!foundItem) {
                    cout << itemName << " not in inventory." << endl;
                }
            }
            else {
                cout << "Error" << endl;
            }
        }
    }
};
/*

BEHIND THE SCENES COMMANDS

*/
/* Add (object) to (room/container) */
//creates instance of object with a specific owner
//(does not work on the player's inventory).
bool Add(Item* itemA, Room* roomA) {
    roomA->items.push_back(itemA);
    return true;
}
bool Add(Creature* creatureA, Room* roomA) {
    roomA->creatures.push_back(creatureA);
    return true;
}
bool Add(Container* contA, Room* roomA) {
    roomA->containers.push_back(contA);
    return true;
}

bool Add(Item* itemA, Container* containerA) {
    containerA->items.push_back(itemA);
    return true;
}


/*
Delete (object) – removes object references from game, but the item can still be
brought back into the game using the 'Add' command. If a room is removed other rooms
will have references to the removed room as a 'border' that was removed, but there
is no means for adding a room back in.
*/
bool Delete(Room* room) {
    //remove from allRooms
    bool found = false;
    // Remove from rooms
    if (allRooms.find(room->name) != allRooms.end()){
        allRooms.erase(allRooms.find(room->name));
        found = true;

        //update borders
        for (map<string,Room>::iterator it=allRooms.begin(); it!=allRooms.end(); ++it) {
            // For every room in the map
            Room* room = &allRooms[it->first]; // it->first is the map key

            for(int i = 0; i < room->borders.size(); i++) {
                if(room->borders[i].name == room->name) {
                    room->borders.erase(room->borders.begin() + i);
                }
            }
        }
    }

    // This method leaves the container in allContainers in case it needs to be parsed later
    // TODO: decide if we need to delete it

    if(found) {
        cout << "Room " << room->name << " deleted from allRooms." << endl;
    } else {
        cout << room->name << " not found." << endl;
    }
    return found;
}
bool Delete(Item* item) {
    //remove from items in room and items in containers
    string itemName = item->name;
    bool found = false;

    // Check if the item is in the room
    for(int i = 0; i < currRoom->items.size(); i++) {
        if(currRoom->items[i]->name == itemName) {
            currRoom->items.erase(currRoom->items.begin() + i); // Erase from current room
            found = true;
            break;
        }
    }
    // Check containers
    for(int i = 0; !found && i < currRoom->containers.size(); i++) {
        for(int j = 0; j < currRoom->containers[i]->items.size(); j++) {
            if(currRoom->containers[i]->items[j]->name == itemName) {
                currRoom->containers[i]->items.erase(currRoom->containers[i]->items.begin() + j);
                found = true;
                break;
            }
        }
    }

    // This method leaves the item in allItems in case it needs to be parsed later
    // TODO: decide if we need to delete it

    if(found) {
        cout << "Item " << itemName << " deleted from room/container." << endl;
    } else {
        cout << itemName << " not found." << endl;
    }
    return found;
}
bool Delete(Container* container){
    //remove from containers in room
    bool found = false;
    // Check containers
    for(int i = 0; !found && i < currRoom->containers.size(); i++) {
        if(currRoom->containers[i]->name == container->name) {
            currRoom->containers.erase(currRoom->containers.begin() + i);
            found = true;
            break;
        }
    }

    // This method leaves the container in allContainers in case it needs to be parsed later
    // TODO: decide if we need to delete it

    if(found) {
        cout << "Container " << container->name << " deleted from room." << endl;
    } else {
        cout << container->name << " not found." << endl;
    }
    return found;
}
bool Delete(Creature* creature){
    //remove from creatures in room
    bool found = false;
    // Check containers
    for(int i = 0; !found && i < currRoom->creatures.size(); i++) {
        if(currRoom->creatures[i]->name == creature->name) {
            currRoom->creatures.erase(currRoom->creatures.begin() + i);
            found = true;
            break;
        }
    }

    // This method leaves the creature in allCreatures in case it needs to be parsed later
    // TODO: decide if we need to delete it

    if(found) {
        cout << "Creature " << creature->name << " deleted from room." << endl;
    } else {
        cout << creature->name << " not found." << endl;
    }
    return found;
}

//Update (object) to (status) – creates new status for object that can be checked by triggers 
bool Update(Room* room, string status){
    room->status = status;
    cout << "update room status to " << status << endl;
    return true;
}
bool Update(Container* cont, string status){
    cont->status = status;
    cout << "update container status to " << status << endl;
    return true;
}
bool Update(Creature* creature, string status){
    creature->status = status;
    cout << "update creature status to " << status << endl;
    return true;
}
bool Update(Item* item, string status){
    item->status = status;
    //cout << "update item status to " << status << endl;
    return true;
}

/* Game Over */
void GameOver(bool over) {
    if(over == true){
        cout<< "Victory!" << endl;
    }
}
