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
	int colRoots(int col);
	bool enforceRules(int row, int col, int input, bool init);
	bool checkBox(int rootRow, int rootCol, int row, int col, int input, bool init);

public:
	Board(int level);
	~Board();
	void newBoard();
	void endGame();
	void printSolution();
	vector< vector< Cell > > getSolution();
	vector< vector< Cell > > getBoard();
};