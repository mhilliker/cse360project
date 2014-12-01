#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include "Cell.h"

using namespace std;

class Board {
private:
	int startTime;
	double fillPercent;
	int difficulty;
	bool enableAI;
	int gridSize;
	int score;
	vector< vector< Cell > > solution;
	vector< vector< Cell > > gameBoard;
	bool enforceRules(int row, int col, char newInput, bool aiCheck);
	void check(int col, int row);

public:
	Board(int level, bool ai, bool test);
	~Board();
	void newBoard();
	void endGame();
	bool newInput(int row, int col, char newInput);
	vector< vector< Cell > > getSolution();
	vector< vector< Cell > > getBoard();
	void oNote(int col, int row, char note, bool add);
	bool* getoNote(int col, int row);
	void xNote(int col, int row, char note, bool add);
	bool* getxNote(int col, int row);
	void playAI();
	//void printSolution(); // for testing
	//void playBoardRndm(); // for testing
	//void playBoardReal(); // for testing
};

#endif
