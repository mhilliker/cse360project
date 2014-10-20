#include "Board.h"

/*
TODO-----------------------------------------
Separate solution check from gameboard check.
*/

Board::Board(int level) {
	difficulty = level;

	switch (difficulty) {
	case 0:
		fillPercent = 50;   // easy
		gridSize = 9;
		break;
	case 1:
		fillPercent = 40;   // medium
		gridSize = 9;
		break;
	case 2:
		fillPercent = 30;   // hard
		gridSize = 9;
		break;
		// case 3:                 // case 3 will require a change in code for enforceRules() & checkBox()
		//     fillPercent = 30;   // evil
		//     gridSize = 16;
		//     break;
	default:
		break;
	}

	solution.resize(gridSize, vector<Cell>());
	gameBoard.resize(gridSize, vector<Cell>());
	for (int col = 0; col < gridSize; ++col) {
		solution[col].resize(gridSize, Cell());
		gameBoard[col].resize(gridSize, Cell());
	}
}

Board::~Board() {
	// delete stuff
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   newBoard() reinitializes board, and replaces current vector
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newBoard() {
	float randomizer;
	int randomInput;
	bool failedRules;
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> dist(1, 10);

	// forloop: loops through every cell to create a solution and gameBoard.
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			failedRules = true;
			randomizer = (float) rand() / RAND_MAX;

			// while: if the input failed the rules, redo the input.
			// if: rules don't fail, continue to next if, else randomize.
			// if: randomly chose as locked, lockCell and setInput for
			//      gameBoard and solution. Else, only set value for
			//      solution but don't lock
			while (failedRules) {
				randomInput = floor(dist(mt));
				if (enforceRules(row, col, randomInput, true)) {
					failedRules = false;
					if (randomizer <= (fillPercent / 100)) {
						solution[col][row].lockCell();
						solution[col][row].setInput(randomInput);
						gameBoard[col][row].setInput(randomInput);
					}
					else {
						solution[col][row].setInput(randomInput);
					}
				}
			}
		}
		printSolution();
	}
}

void Board::endGame() {
	// calculate scores, and store away score
}

vector< vector<Cell> > Board::getBoard() {
	return gameBoard;
}

vector< vector<Cell> > Board::getSolution() {
	return solution;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   enforceRules() will check for a matching value to the input in the same
*   row, column, or box. If a match is found the function will return false,
*   indicating that it is not a winning move (will remain unknown to player).
*   Otherwise, if a match is not found, it will return true.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::enforceRules(int row, int col, int input, bool init) {
	int rootRow;
	int rootCol;
	int currentInput;

	// forloop: loops through row (increments through column)
	//      skips column in which the input exists
	//      if: cell input is same as new input, return false
	for (int c = 0; c < gridSize; ++c) {
		currentInput = (init) ? solution[c][row].getInput() : gameBoard[c][row].getInput();
		if (c != col && currentInput == input) {
			return false;
		}
	}

	// forloop: loops through column (increments through row)
	//      skips row in which the new input exists
	//      if: cell input is same as new input, return false
	for (int r = 0; r < gridSize; ++r) {
		currentInput = (init) ? solution[col][r].getInput() : gameBoard[col][r].getInput();
		if (r != row && currentInput == input) {
			return false;
		}
	}

	// if: checks which set of rows input belongs to then looks for colRoots().
	//      Runs checkBox() for any matching values
	if (row >= 0 && row < 3) {
		rootRow = 0;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	} else if (row >= 3 && row < 6) {
		rootRow = 3;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	} else {
		rootRow = 6;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   rowCases() returns an index for the root column for the box containing the
*   specified column.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Board::colRoots(int col) {
	if (col >= 0 && col < 3) {
		return 0;
	}
	else if (col >= 3 && col < 6) {
		return 3;
	}
	else {
		return 6;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   checkBox() returns a boolean for enforceRules(). If cell input is same as
*   new input, return false. If none returned false, return true.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::checkBox(int rootRow, int rootCol, int row, int col, int input, bool init) {
	int currentInput;
	
	for (int rows = rootCol; rows < rootCol + 3; ++rows) {
		for (int cols = rootRow; cols < rootRow + 3; ++cols) {
			currentInput = (init) ? solution[cols][rows].getInput() : gameBoard[cols][rows].getInput();
			if (cols != col
				&& rows != row
				&& currentInput == input) {
				return false;
			}
		}
	}
	return true;
}

// This is only for testing purposes, printing to the console.
void Board::printSolution() {
	cout << "\nSolution Board: \n";
	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) {
			cout << solution[x][y].getInput() << " ";
		}
		cout << endl;
	}
	cout << "Game Board: \n";
	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) {
			cout << gameBoard[x][y].getInput() << " ";
		}
		cout << endl;
	}
}