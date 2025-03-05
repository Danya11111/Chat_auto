#pragma once
#include <iostream>
#include <string>
#include "Graph.h"
using namespace std;

class User {
public:
    string login;
    string password;
    string name;

    User() : login(""), password(""), name("") {}

    User(string login, string password, string name) : login(login), password(password), name(name) {}

    void sendMessage(const string& message, const string& recipient = "") const {
        if (recipient.empty()) {
            cout << name << " (to everyone): " << message << endl;
        }
        else {
            cout << name << " (to " << recipient << "): " << message << endl;
        }
    }
};