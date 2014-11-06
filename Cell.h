#include <iostream>
#include <vector>

using namespace std;

class Cell {
private:
	int input;
	bool locked;
	bool validRow;
	bool validCol;
	bool validBox;
	vector< int > guesses;
	vector< int > unwanted;

public:
	Cell();
	vector< int > attempted;
	bool getLocked();
	void lockCell();
	int getInput();
	void setInput(int newInput);
	void resetCell();
	void setRowValid(bool);
	void setColValid(bool);
	void setBoxValid(bool);
	bool getRowValid();
	bool getColValid();
	bool getBoxValid();
	bool isValid();
};