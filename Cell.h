#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>

using namespace std;

class Cell {
private:
	char input;
	bool locked;
	bool validRow;
	bool validCol;
	bool validBox;
	bool aiMove;

public:
	Cell();
	vector< char > attempted;
	bool oNotes[9]; // list of numbers user decides are possible to use in cell
	bool xNotes[9]; // list of numbers user decides cannot be used in cell
	bool getLocked();
	void lockCell();
	char getInput();
	void setInput(char newInput);
	bool getAIMove();
	void resetCell();
	void setRowValid(bool);
	void setColValid(bool);
	void setBoxValid(bool);
	bool isValid();
};

#endif
