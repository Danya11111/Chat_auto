#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // ��� std::remove
#include "User.h"
#include "Message.h"
#include "TrieNode.h"
using namespace std;

class Chat {
private:
    map<string, User> users;
    vector<string> onlineUsers;
    vector<Message> publicMessages;
    map<string, vector<Message>> privateMessages;
public:
    unique_ptr<Graph> friends = make_unique<Graph>();
    unique_ptr<Trie> trie = make_unique<Trie>();

    bool registerUser(const string& login, const string& password, const string& name) {
        
        if (users.count(login)) {
            cout << "Error: login is already taken." << endl;
            return false; // ����� ��� �����
        }
        users[login] = User(login, password, name);
        string log = login;
        friends->vname.push_back(log);
        int value = friends->vname.size() - 1;
        friends->addVertex(value);
        cout << "User  " << name << " successfully registered!" << endl;
        return true;
    }

    bool loginUser(const string& login, const string& password) {
        if (!users.count(login) || users[login].password != password) {
            cout << "Error: incorrect login or password." << endl;
            return false; // �������� ����� ��� ������
        }
        onlineUsers.push_back(login); // ��������� ������������ � ������ ������
        cout << "User  " << login << " logged in." << endl;
        return true;
    }

    void logoutUser(const string& login) {
        onlineUsers.erase(remove(onlineUsers.begin(), onlineUsers.end(), login), onlineUsers.end());
        cout << "User  " << login << " logged out." << endl;
    }

    void sendMessage(const string& senderLogin, const string& message, const string& recipient = "") {

        if (!recipient.empty()) { // ������ ���������
            if (!users.count(recipient)) {
                cout << "Error: user " << recipient << " not found." << endl;
                return;
            }
            privateMessages[senderLogin].emplace_back(senderLogin, message, recipient);
            privateMessages[recipient].emplace_back(senderLogin, message, recipient);
            cout << users[senderLogin].name << " (to " << recipient << "): " << message << endl;
        }

        else { // ��������� ����
            publicMessages.emplace_back(senderLogin, message);
            cout << users[senderLogin].name << " (to everyone): " << message << endl;
        }
    }

    void listUsers(string login) const {
        friends->findMinDistancesFloyd(login);
    }

    void viewMessages(const string& login) const {
        cout << "Chat messages for " << users.at(login).name << ":" << endl;

        // ��������� ���������
        for (const auto& msg : publicMessages) {
            cout << msg.sender << " (to everyone): " << msg.content << endl;
        }

        // ������ ��������� ��� ������� ������������
        if (privateMessages.count(login)) {
            for (const auto& msg : privateMessages.at(login)) {
                cout << msg.sender << " (to " << msg.recipient << "): " << msg.content << endl;
            }
        }
    }
    void addFriend(const string& user_name) {
        cout << endl << "Who do you want to add as a friend? If you want to cancel press - 0." << endl;
        listUsers(user_name);
        bool found = false;
        string friend_name;

        do {
            cout << "Enter the username of the user you want to add as a friend: ";
            cin >> friend_name;

            if (friend_name == "0") {
                break; // ������������ ������� ����������
            }

            if (users.find(friend_name) == users.end()) {
                cout << "Error: user " << friend_name << " not found." << endl;
                continue; // ������������ �� ������
            }

            if (friend_name == user_name) {
                cout << "You cannot add yourself as a friend." << endl;
                continue; // ������ �������� ������ ����
            }
            int index_st;
            int index_user;
            for (int i = 0; i < friends->vname.size(); i++) {
                if (friends->vname[i] == friend_name)
                    index_st = i;
                if (friends->vname[i] == user_name)
                    index_user = i;
            }
            if (friends->edgeExists(index_user, index_st)) {
                cout << "You're already friends!" << endl;
                break;
            }

            friends->addEdge(user_name, friend_name);
            cout << "You have successfully added a friend: " << friend_name << endl;
            found = true;

        } while (!found);
    }

    string T9() {
        string input;
        cout << "Enter a prefix to autocomplete: " << endl;
        string itog;
        while (true) {
            cout << "> ";
            input = "";
            cin.ignore();
            getline(cin, input);
            vector<string> suggestions = trie->autocomplete(input);
            if (suggestions.empty()) {
                cout << "No suggestions found." << endl;
                cout << "Write your own message to send: ";
                input = "";
                getline(cin, input);
                itog = input;
                trie->insert(input);
                break;
            }
            else {
                cout << "Suggestions: ";
                int i = 1;
                for (const string& suggestion : suggestions) {
                    cout << i << " - " << suggestion << " ";
                }
                cout << endl;
                cout << "Enter the supplement that suits you, write its number or write 0 and write your own message: ";
                int choice;
                cin >> choice;
                if (choice == 0) {
                    cout << "Enter the message you wanted to enter : ";
                    string str;
                    cin.ignore();
                    getline(cin, str);
                    trie->insert(str);
                    itog = str;
                    break;
                }
                else {
                    itog = suggestions[choice - 1];
                    break;
                }
            }
        }
        return itog;
    }

    void insert_lib() {
        trie->insert("Hello");
        trie->insert("How");
        trie->insert("are");
        trie->insert("you");
        trie->insert("Hi");
    }
};