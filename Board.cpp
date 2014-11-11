#include "Board.h"

Board::Board(int level, bool ai) {
	difficulty = level;
	enableAI = ai;
	gridSize = 9;

	switch (difficulty) {
	case 0: fillPercent = 43;   // easy
		break;
	case 1: fillPercent = 38;   // medium 
		break;
	case 2: fillPercent = 33;   // hard
		break;
	case 3: fillPercent = 41;   // evil
		gridSize = 16;
		break;
	default:
		break;
	}

	solution.resize(gridSize, vector<Cell>());
	gameBoard.resize(gridSize, vector<Cell>());
	for (int col = 0; col < gridSize; col++) {
		solution[col].resize(gridSize, Cell());
		gameBoard[col].resize(gridSize, Cell());
	}

	newBoard(); // generate a new board with user
}

//Board::~Board() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   newBoard() creates a new solution, called when a board is instantiated.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newBoard() {
	double randomizer;
	char randomInput;
	bool invalid, validInput;
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> gridRand(49, 58); // 9x9 random input 1 - 9
	uniform_real_distribution<double> gridXLRand(65, 81); // 16x16 random input A - P
	uniform_real_distribution<double> putRand(0, 1); // select lock status

	// for2x: Loops through every cell to generate a solution and gameBoard.
	//		each cell will generate a random input, then move on. If no more
	//		possible inputs, backtrack to the previous cell and try remaining
	//		unattempted inputs.
	for (int row = 0; row < gridSize; row++)
	for (int col = 0; col < gridSize; col++) {
		invalid = true;
		randomizer = putRand(mt); // decides whether locked cell or not.

		// while: input failed the rules (invalid), retry with new input. As
		//		a default setting, invalid is true for the first iteration.
		while (invalid) {
			validInput = true; // false when input was already attempted.
			randomInput = (gridSize == 9) ? floor(gridRand(mt)) : floor(gridXLRand(mt)); // randomizes an input value
			int attemptSize = solution[col][row].attempted.size();

			// for: for every previously attempted value in the current cell,
			//		check if new input has already been attempted. If a match
			//		is found in the array of attempted values, set valid to
			//		false, and break.
			for (int i = 0; i < attemptSize; i++) {
				if (solution[col][row].attempted[i] == randomInput) {
					validInput = false;
					break;
				}
			}

			// if: input is valid, check if rules don't fail and attempts are within
			//		range then continue to set values. Else, push the attempted random
			//		input to the cell's list of attempted inputs.
			//	else: if the input is not valid, check if number of attempts equal or
			//		exceeds possible number of inputs. If exceeded, clear current set
			//		of attempted values then move back one cell, send current input
			//		to the array of attempts, and reset the cell's contents.
			if (validInput) {

				// if: rules are enforced correctly and number of attempts don't exceed 9
				if (enforceRules(row, col, randomInput) && attemptSize < gridSize) {
					invalid = false;

					// if: random is within percent range, lockCell and setInput for
					//		gameBoard and solution. Else, only set value for solution
					//		but don't lock.
					if (randomizer <= (fillPercent / 100)) {
						solution[col][row].lockCell();
						solution[col][row].setInput(randomInput);
						gameBoard[col][row].lockCell();
						gameBoard[col][row].setInput(randomInput);
					} else {
						solution[col][row].setInput(randomInput);
					} // end if-else
				} else {
					solution[col][row].attempted.push_back(randomInput);
				} // end if-else
			} else if (attemptSize >= gridSize) {
				solution[col][row].attempted.clear();

				// if: column is at minimum, jump back a row, and set column to last index.
				//		Else, jump back a column.
				if (col == 0) {
					row--;
					col = (gridSize == 9) ? 8 : 15;
				} else {
					col--;
				} // end if-else
				solution[col][row].attempted.push_back(solution[col][row].getInput());
				solution[col][row].resetCell();
				gameBoard[col][row].resetCell();
			} // end if-elseif
		} // end while
	} // end double for
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	endGame() will send calculated score and store it in the user's profile.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void Board::endGame() {
//	// do stuff
//}

vector< vector<Cell> > Board::getBoard() { return gameBoard; }

vector< vector<Cell> > Board::getSolution() { return solution; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   enforceRules() will check for a matching value to the input in the same
*   row, column, or box. If a match is found the function will return false,
*   indicating that it is not a valid input (will remain unknown to player).
*   Otherwise, if a match is not found, it will return true.
*	A boolean init is passed in to indicate whether the input is by the
*	new board function or by the user. True means the function is set to
*	initializing a new board.
*
*	Update: will not check user input -- will only check for newBoard.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::enforceRules(int row, int col, char input) {
	int rootCol = (gridSize == 9) ? (col >= 0 && col < 3) ? 0 : (col >= 3 && col < 6) ? 3 : 6 :
									(col >= 0 && col < 4) ? 0 : (col >= 4 && col < 8) ? 4 : (col >= 8 && col < 12) ? 8 : 12;
	int rootRow = (gridSize == 9) ? (row >= 0 && row < 3) ? 0 : (row >= 3 && row < 6) ? 3 : 6 :
									(row >= 0 && row < 4) ? 0 : (row >= 4 && row < 8) ? 4 : (row >= 8 && row < 12) ? 8 : 12;
	int endCol = (gridSize == 9) ? rootCol + 3 : rootCol + 4;
	int endRow = (gridSize == 9) ? rootRow + 3 : rootRow + 4;

	// forloop: loops through row (increments columns of one row)
	//      skips column in which the input exists
	//	if: column is != input column, proceed. Else, increment to next column.
	for (int c = 0; c < gridSize; ++c) {
		if (c != col && input == solution[c][row].getInput())
			return false;
	}

	// forloop: loops through column (increments rows of one column)
	//      skips row in which the new input exists
	//	if: match found, return false 
	for (int r = 0; r < gridSize; ++r) {
		if (r != row && input == solution[col][r].getInput())
			return false;
	}

	for (int r = rootRow; r < endRow; r++)
	for (int c = rootCol; c < endCol; c++) {
		if (c != col && r != row && input == solution[c][r].getInput())
			return false;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	newInput() calls enforceRules with the new input details then sets the
*	input to the gameBoard.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newInput(int col, int row, char newInput) {
	char currentInput = gameBoard[col][row].getInput();

	// if: set input, then find first match within row, col, or box, and set valid
	//			status if applicable. When found and set, move on.
	//		else if: unset input, then count number of matches to currently set
	//			input. One match - set match valid to true. More than one
	//			match - leave matches alone. Then after either check, reset
	//			to unset cell.
	//		else if: change input, call check for unsetting input then call to
	//			set input to new value.
	if (currentInput == NULL && newInput != NULL) {
		gameBoard[col][row].setInput(newInput);
	} else if (currentInput != NULL && newInput == NULL) {
		gameBoard[col][row].resetCell();
	} else if (currentInput != NULL && newInput != NULL) {
		gameBoard[col][row].resetCell();
		gameBoard[col][row].setInput(newInput);
	}

	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (!gameBoard[x][y].getLocked() && gameBoard[x][y].getInput() != NULL) {
			check(x, y);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	check() makes sure each non-locked cell gets checked to update their valid
*	status for column, row, and box. It is called every time there is a new
*	input.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::check(int col, int row) {
	int rootCol = (gridSize == 9) ? (col >= 0 && col < 3) ? 0 : (col >= 3 && col < 6) ? 3 : 6 :
									(col >= 0 && col < 4) ? 0 : (col >= 4 && col < 8) ? 4 : (col >= 8 && col < 12) ? 8 : 12;
	int rootRow = (gridSize == 9) ? (row >= 0 && row < 3) ? 0 : (row >= 3 && row < 6) ? 3 : 6 :
									(row >= 0 && row < 4) ? 0 : (row >= 4 && row < 8) ? 4 : (row >= 8 && row < 12) ? 8 : 12;
	int endCol = (gridSize == 9) ? rootCol + 3 : rootCol + 4;
	int endRow = (gridSize == 9) ? rootRow + 3 : rootRow + 4;

	Cell * check = &gameBoard[col][row];
	bool vCol = true, vRow = true, vBox = true;

	// forloop: loops through row (increments columns of one row)
	//      skips column in which the input exists
	//	if: match found, vRow is false;
	for (int c = 0; c < gridSize; ++c) {
		if (c != col && check->getInput() == gameBoard[c][row].getInput()) {
			vRow = false;
			break;
		}
	}

	// forloop: loops through column (increments rows of one column)
	//      skips row in which the new input exists
	//	if: match found, vCol is false 
	for (int r = 0; r < gridSize; ++r) {
		if (r != row && check->getInput() == gameBoard[col][r].getInput()) {
			vCol = false;
			break;
		}
	}

	// forloop: loops through box, excluding current cell
	//	if: match found, vBox is false.
	for (int r = rootRow; r < endRow; r++)
	for (int c = rootCol; c < endCol; c++) {
		if (c != col && r != row && check->getInput() == gameBoard[c][r].getInput()) {
			vBox = false;
			r = endRow;
			break;
		}
	}

	if (vCol) check->setColValid(true);
	else check->setColValid(false);

	if (vRow) check->setRowValid(true);
	else check->setRowValid(false);
	
	if (vBox) check->setBoxValid(true);
	else check->setBoxValid(false);
}

void Board::oNote(int col, int row, char note, bool add) {
	Cell * cell = &gameBoard[col][row];

	if (add) {
		cell->oNotes.push_back(note);
	} else {
		for (int i = 0; i < cell->oNotes.size(); i++)
		if (cell->oNotes[i] == note) {
			cell->oNotes[i] = cell->oNotes.back();
			cell->oNotes.pop_back();
		}
	}
}

vector< char > Board::getoNote(int col, int row) {
	return gameBoard[col][row].oNotes;
}

void Board::xNote(int col, int row, char note, bool add) {
	Cell * cell = &gameBoard[col][row];

	if (add) {
		cell->xNotes.push_back(note);
	} else {
		for (int i = 0; i < cell->xNotes.size(); i++)
		if (cell->xNotes[i] == note) {
			cell->xNotes[i] = cell->xNotes.back();
			cell->xNotes.pop_back();
		}
	}
}

vector< char > Board::getxNote(int col, int row) {
	return gameBoard[col][row].xNotes;
}

void Board::playAI() {
	// TO DO: find a way to delay by few seconds // "Please wait while AI is 'thinking...'"
	// TO DO: decide how AI will make a move.
}