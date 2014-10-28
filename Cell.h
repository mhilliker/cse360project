#include <iostream>
#include <vector>

using namespace std;

class Cell {
private:
	int input;
	bool locked;
	bool badInput;
	bool validRow;
	bool validCol;
	bool validBox;

public:
	Cell();
	bool getLocked();
	void lockCell();
	int getInput();
	void setInput(int);
	void resetCell();
	void setRowValid(bool);
	void setColValid(bool);
	void setBoxValid(bool);
	bool getRowValid();
	bool getColValid();
	bool getBoxValid();
	vector<int> attempted;
};