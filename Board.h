#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "Cell.h"

using namespace std;

class Board {
private:
	int startTime;
	double fillPercent;
	int difficulty;
	int gridSize;
	int score;
	vector< vector< Cell > > solution;
	vector< vector< Cell > > gameBoard;
	bool enforceRules(int row, int col, int newInput);
	void check(int col, int row);

public:
	Board(int level);
	//~Board();
	void newBoard();
	void endGame();
	void newInput(int row, int col, int newInput);
	vector< vector< Cell > > getSolution();
	vector< vector< Cell > > getBoard();
	void printSolution(); // for testing
	void playBoardRndm(); // for testing
	void playBoardReal(); // for testing
};