#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include "User.h"

using namespace std;

class AccountManager {
private:
	User * userAccounts; // head of linked list
	User * tailAccount;
	User * loggedIn;
	int size;
	User * search(string user);

public:
	AccountManager();
	~AccountManager();
	bool login(string user, string pw);
	void logout();
	bool newAccount(string user, string pw,int score); // if taken username, return false.
	bool loadData();
	bool saveData();
	//void saveGame(Board * game); // for loggedin
	string getName(); // for loggedin
	void setHiScore(int hs);
	int getHiScore(); // for loggedin
	void printList();
};

#endif
