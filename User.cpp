#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
using namespace std;
class User
{
public:
	User next;
	String userName;
	String passWord;
	int highScore;
	String getUsername();
	String getPassword();
	int getHighScore();
	void setUsername(String name);
	void setPassword(String password);
	void setHighScore(int score);
	User(String name, String password, int score);
};
User(String name, String password, int score)
{
	name = "guest";
	password = "123456789";
	score = 0;
}
String getUsername()
{
	return userName;
}

String getPassword()
{
	return password;
}

int getHighScore()
{
	return highScore;
}

void setUsername(String name)
{
	userName = name;
}

void setPassword(String password)
{
	passWord = password;
}

void setHighScore(int score)
{
	highScore = score;
}



