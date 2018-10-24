//#include "Item.h"
#include "Container.h"
#include "Room.h"
#include "Creature.h"
#include "Trigger.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


int main(int argc, char* argv[] ){

    xml_document<>doc;
    xml_node<> * root_node;

    ifstream file(argv[1]);
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    string content(buffer.str());

    /*vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0'); */
    doc.parse<0>(&content[0]);
/**
    file<> xml_l("sample.txt.xml");
    if(&xml_l == NULL){
        throw std::runtime_error("File not found!");
    }
    doc.parse<0>(xml_l.data());
**/

    // Find our root node (aka Map)
    root_node = doc.first_node("map");
    if (root_node == NULL){
        cout << "cant get root node" << endl;
        return 0;
    }

    //creating hashmaps of items
    //map<std::string, Item> allItems;

    //debugger is giving me an error in the for loop because it is saying that root_node is null and thats why
    //it is unable to find the root_node, therefore i think the actual problem is with downloading the file,
    //getting the vector of strings, and then buffering it - ayushi

    //so just added that if statement checking if it could get root node, and yeah the error is there
    //code cant get root ode rn

   //iterating over each item first
    for (xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item"))
    {

        //creating temp Item to put into hashmap later
        /**Item temp;
        temp.name = item_node->first_attribute("name")->value();
        temp.descrip = item_node->first_attribute("description")->value();
        temp.writing = item_node->first_attribute("writing")->value();
        temp.status = item_node->first_attribute("status")->value();
        cout << "Item name is: " << temp.name << endl;
        cout << "Item description is: " << temp.descrip << endl;
        cout << "Item writing is: " << temp.writing << endl;
        cout << "Item status is: " << temp.status << endl;**/

        //pretty sure above code is right, and the exit code 11 is coming from a different place (see above)
        //but i commented that out and tried to print as I parsed through it rather than create a map first

        string name = item_node->first_attribute("name")->value();
        string descrip = item_node->first_attribute("description")->value();
        string writing = item_node->first_attribute("writing")->value();
        string status = item_node->first_attribute("status")->value();
        cout << "Item name is: " << name << endl;
        cout << "Item description is: " << descrip << endl;
        cout << "Item writing is: " << writing << endl;
        cout << "Item status is: " << status << endl;

        //allItems[temp.name] = temp;


    }
/**
    //print out all items
    for (const auto &p: allItems){
        p.second.printItem();
    } **/

    //allItems.clear();
};