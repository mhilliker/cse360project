#include "AccountManager.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	Constructor defaults all variables to NULL.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AccountManager::AccountManager() {
	userAccounts = NULL;
	tailAccount = NULL;
	loggedIn = NULL;
	size = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	Destructor
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AccountManager::~AccountManager() {
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	login() : search for user in linked list, and compare passwords. If user
*	is not found, then return false. If user is found, but password does not
*	match, return false. Otherwise, return true, and set loggedIn user.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool AccountManager::login(string user, string pw) {
	User * loginUser = search(user);

	if (loginUser == NULL) return false;
	else if (pw.compare(loginUser->getPassword()) != 0) return false;
	else loggedIn = loginUser;

	return true;
}

void AccountManager::logout() {
	loggedIn = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	newAccount() : [before being called, password much be verified twice,
*	and have restrictions to username and password (length limit, char type,
*	etc.)] function will take in a user name and password parameters. User
*	will be added in alphabetical order.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool AccountManager::newAccount(string user, string pw) {

	// if: username exists in the list, return false.
	if (search(user) != NULL) return false;

	User * newAccount = new User(user, pw);
	User * ptr = userAccounts;

	// if: username comes after tail, point tail next to new user, and tail
	//		pointer equal to new user.
	//	elseif: username comes before head, point new user new to head, and
	//		head pointer equal to new user.
	//	else: increment pointer to node that new user comes after. Set next
	//		of new user to next of pointer. Set next of pointer to new user.
	if (userAccounts == NULL) {
		userAccounts = newAccount;
		tailAccount = newAccount;
	} else if (user.compare(tailAccount->getUserName()) > 0) {
		tailAccount->setNext(newAccount);
		tailAccount = newAccount;
	} else if (user.compare(userAccounts->getUserName()) < 0) {
		newAccount->setNext(userAccounts);
		userAccounts = newAccount;
	} else {
		while (user.compare(ptr->next()->getUserName()) > 0)
			ptr = ptr->next();
		newAccount->setNext(ptr->next());
		ptr->setNext(newAccount);
	}

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	loadData() : read userAccounts from sudoku_save file. If non-existent,
*	[...]
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool AccountManager::loadData() {
	string fileName = "sudoku_save.dat"; // file type? .txt? .bin? ...?
	// read userAccounts from file
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	saveData() : write userAccounts to sudoku_save file.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool AccountManager::saveData() {
	string fileName = "sudoku_save.dat"; //  "   "   "   "
	// write userAccounts to a file
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	search() : simple iteration search returns user if found, otherwise
*	return NULL.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
User * AccountManager::search(string user) {
	User * ptr = userAccounts;

	// while: ptr exists and they don't equal,
	while (ptr != NULL && user.compare(ptr->getUserName()) != 0)
		ptr = ptr->next();

	return ptr;
}

void AccountManager::saveGame(Board * game) {
	loggedIn->saveGame(game);
}

string AccountManager::getName() {
	return (loggedIn != NULL) ? loggedIn->getUserName() : "";
}

void AccountManager::setHiScore(int hs) {
	loggedIn->setHighScore(hs);
}

int AccountManager::getHiScore() {
	return (loggedIn != NULL) ? loggedIn->getHighScore() : -1;
}

void AccountManager::printList() {
	User * ptr = userAccounts;
	while (ptr != NULL) {
		ptr->printUser();
		ptr = ptr->next();
	}
}