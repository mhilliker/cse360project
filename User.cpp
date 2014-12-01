#include "User.h"

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	Constructor takes in parameters for username and password. Other
*	variables are set to default values.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
User::User(string name, string pw) {
	nextUser = NULL;
	userName = name;
	password = pw;
	highScore = 0;
	//savedGame = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	Destructor
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
User::~User() {
}

User * User::next() {
	return nextUser;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	getUserName() : returns the userName.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
string User::getUserName() {
	return userName;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	getPassword() : returns the password.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
string User::getPassword() {
	return password;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	getHighScore() : returns the highscore.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int User::getHighScore() {
	return highScore;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	setHighScore() : sets the highscore equal to the integer parameter 'hs'.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void User::setHighScore(int hs) {
	highScore = hs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	setNext() : sets the next user in the linked list equal to the User
*	pointer parameter 'newUser'.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void User::setNext(User * newUser) {
	nextUser = newUser;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	saveGame() : sets the savedGame equal to current gameboard to be stored.
*	This should be called periodically, together with saveData() in
*	AccountManager.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
void User::saveGame(Board * game) {
	savedGame = game;
}*/

void User::printUser() {
	//cout << userName << " " << highScore << " ";
}
