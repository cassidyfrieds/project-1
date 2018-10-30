#pragma once

#include <iostream>
#include <string>

using namespace std;

class Condition {
    public:
        string obj; //object
        string owner;
        string has; //cass changed this, ayushi changed back
        string status;

        Condition();
        ~Condition();
        void printCondition() const;
};