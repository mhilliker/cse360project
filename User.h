#ifndef USER_H
#define USER_H

#include <iostream>
//#include "Board.h"

using namespace std;

class User {
private:
	User * nextUser;
	string userName;
	string password;
	int highScore;
	//Board * savedGame;

public:
	User(string name, string pw);
	~User();
	User * next();
	string getUserName();
	string getPassword();
	int getHighScore();
	void setHighScore(int hs);
	void setNext(User * newUser);
	//void saveGame(Board * game);
	void printUser();

};
