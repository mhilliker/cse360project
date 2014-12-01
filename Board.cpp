#include "Board.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   Board constructor creates a new board. It sets the difficulties and
*	AI settings.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Board::Board(int level, bool ai, bool test) {
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

	// initialize solution and gameboard Cell type vectors with gridSize input.
	solution.resize(gridSize, vector< Cell >());
	gameBoard.resize(gridSize, vector< Cell >());
	for (int col = 0; col < gridSize; col++) {
		solution[col].resize(gridSize, Cell());
		gameBoard[col].resize(gridSize, Cell());
	}

	newBoard(); // generate a new board
}

Board::~Board() {}

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
			int attemptSize = solution[col][row].attempted.size(); // number of different attempts

			// forif: for every previously attempted value in the current cell,
			//		check if new input has already been attempted. If a match
			//		is found in the array of attempted values, set valid to
			//		false, and break.
			for (int i = 0; i < attemptSize; i++)
			if (solution[col][row].attempted[i] == randomInput) {
				validInput = false;
				break;
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
				if (enforceRules(col, row, randomInput, false) && attemptSize < gridSize) {
					invalid = false;

					// if: random is within percent range, lockCell and setInput for
					//		gameBoard and solution.
					//	else: only set value for solution but don't lock.
					if (randomizer <= (fillPercent / 100)) {
						solution[col][row].lockCell();
						solution[col][row].setInput(randomInput);
						gameBoard[col][row].lockCell();
						gameBoard[col][row].setInput(randomInput);
					} else {
						solution[col][row].setInput(randomInput);
					}
				} else {
					solution[col][row].attempted.push_back(randomInput);
				}
			} else if (attemptSize >= gridSize) {
				solution[col][row].attempted.clear();

				// if: column is at minimum, jump back a row, and set column to last index.
				//	else: jump back a column.
				if (col == 0) {
					row--;
					col = (gridSize == 9) ? 8 : 15;
				} else {
					col--;
				}
				solution[col][row].attempted.push_back(solution[col][row].getInput());
				solution[col][row].resetCell();
				gameBoard[col][row].resetCell();
			}
		}
	}

	// for2xif: go through ever cell to clear attempted lists.
	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x)
	if (!gameBoard[x][y].getLocked() && !gameBoard[x][y].attempted.empty())
		gameBoard[x][y].attempted.clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	endGame() will return a score to be stored in th user profile.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//int Board::endGame() {
//	// do stuff
//}

vector< vector< Cell > > Board::getBoard() { return gameBoard; }

vector< vector< Cell > > Board::getSolution() { return solution; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   enforceRules() will check for a matching value to the input in the same
*   row, column, or box. If a match is found the function will return false,
*   indicating that it is not a valid input. Otherwise, if a match is not
*	found, it will return true. Only checks for newBoard() for the solution.
*	aiCheck parameter is used to make checks when AI is deciding a move. It
*	makes checks to the gameBoard.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::enforceRules(int col, int row, char input, bool aiCheck) {
	int inputCheck;
	int rootCol = (gridSize == 9) ? (col >= 0 && col < 3) ? 0 : (col >= 3 && col < 6) ? 3 : 6 :
									(col >= 0 && col < 4) ? 0 : (col >= 4 && col < 8) ? 4 : (col >= 8 && col < 12) ? 8 : 12;
	int rootRow = (gridSize == 9) ? (row >= 0 && row < 3) ? 0 : (row >= 3 && row < 6) ? 3 : 6 :
									(row >= 0 && row < 4) ? 0 : (row >= 4 && row < 8) ? 4 : (row >= 8 && row < 12) ? 8 : 12;
	int endCol = (gridSize == 9) ? rootCol + 3 : rootCol + 4;
	int endRow = (gridSize == 9) ? rootRow + 3 : rootRow + 4;

	// for: loops through row (increments columns of one row)
	//      skips column in which the input exists
	//	if: column is != input column, proceed. Else, increment to next column.
	for (int c = 0; c < gridSize; ++c) {
		inputCheck = (aiCheck) ? gameBoard[c][row].getInput() : solution[c][row].getInput();
		if (c != col && input == inputCheck)
			return false;
	}

	// for: loops through column (increments rows of one column)
	//      skips row in which the new input exists
	//	if: match found, return false 
	for (int r = 0; r < gridSize; ++r) {
		inputCheck = (aiCheck) ? gameBoard[col][r].getInput() : solution[col][r].getInput();
		if (r != row && input == inputCheck)
			return false;
	}

	// for2x: loops through box, excluding cells in the same row or column (to
	//		avoid unnecessary rechecking of cells.
	//	if: match found, rturn false
	for (int r = rootRow; r < endRow; r++)
	for (int c = rootCol; c < endCol; c++) {
		inputCheck = (aiCheck) ? gameBoard[c][r].getInput() : solution[c][r].getInput();
		if (c != col && r != row && input == inputCheck)
			return false;
	}

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	newInput() calls enforceRules with the new input details then sets the
*	input to the gameBoard. It will return true only when every non-locked
*	cell input is determined to be filled and valid.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::newInput(int col, int row, char newInput) {
	Cell * cell = &gameBoard[col][row];
	char currentInput = cell->getInput();
	bool valid = true, full = true;

	// if: set input, then find first match within row, col, or box, and set valid
	//			status if applicable. When found and set, move on.
	//		else if: unset input, then count number of matches to currently set
	//			input. One match - set match valid to true. More than one
	//			match - leave matches alone. Then after either check, reset
	//			to unset cell.
	//		else if: change input, call check for unsetting input then call to
	//			set input to new value.
	if (currentInput == NULL && newInput != NULL) {
		cell->setInput(newInput);
	} else if (currentInput != NULL && newInput == NULL) {
		cell->resetCell();
	} else if (currentInput != NULL && newInput != NULL) {
		cell->resetCell();
		cell->setInput(newInput);
	}

	// for2x: loops through whole board, looking for every non-locked cell. Checks
	//		for if they each have a valid row, column, or box.
	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (!gameBoard[x][y].getLocked()) {
			if (gameBoard[x][y].getInput() != NULL) check(x, y);
			else full = false;
			if (!gameBoard[x][y].isValid()) valid = false;
		}
	}

	// if: all cells are full & valid, return true
	//	else: return false
	return (full && valid) ? true : false;
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

	// ifelse3x: take results of each check, and set validities. 
	if (vCol) check->setColValid(true);
	else check->setColValid(false);

	if (vRow) check->setRowValid(true);
	else check->setRowValid(false);
	
	if (vBox) check->setBoxValid(true);
	else check->setBoxValid(false);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	oNote() can add or delete a character from a specified cell's list of
*	noted values. Arguments are passed in for the coordinates, the note
*	value, and a boolean indicating whether to add or remove the note.
*	oNotes are used for the user to better see what cells need what values.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::oNote(int col, int row, char note, bool add) {
	Cell * cell = &gameBoard[col][row];
	int x = note - '0';
	if (add) {
		cell->oNotes[x]=true;
	} else {
		cell->oNotes[x]=false;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	getoNotes() returns a vector list of noted possible values that may work
*	for the cell.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool* Board::getoNote(int col, int row) {
	return gameBoard[col][row].oNotes;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	xNote() can add or delete a character from a specified cell's list of
*	noted values. Arguments are passed in for the coordinates, the note
*	value, and a boolean indicating whether to add or remove the note.
*	xNotes are used for the user to better see what cells don't need what
*	values.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::xNote(int col, int row, char note, bool add) {
	Cell * cell = &gameBoard[col][row];
	int x = note - '0';
	if (add)
		cell->xNotes[x]=true;
	
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	getxNotes() returns a vector list of noted values in which a cell can not
*	use or else it violates the sudoku rules.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool* Board::getxNote(int col, int row) {
	return gameBoard[col][row].xNotes;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	playAI() will act as the computer's move. Every open cell will be
*	scanned, checking for every possible input, and storing those
*	possibilities in the list of attempted. After checking, the cell with
*	the least number of possibilities will get a new input within the list
*	of attempted inputs.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::playAI() {
	char testInput;
	int minAttempts = gridSize, min_x = NULL, min_y = NULL;
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> gridRand(49, 58);

	// DO: before calling playAI, delay AI move by few seconds
	//		"Please wait while AI is 'thinking...'"
	// TO DO: AI will scan all non-locked cells. It will test each cell with all
	//		possible inputs. If pass, add to attempted. AI will play a move in
	//		an empty cell with least possible moves. AI may or may not choose the
	//		correct input. (no? yes?)

	// for2x: loop through every cell, and check if cell is empty.
	//	if: empty cell, try all inputs.
	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x)
	if (gameBoard[x][y].getInput() == NULL) {

		// for: every possible input is tested, and any valid inputs are pushed to the
		//		attempted list
		for (int i = 0; i < gridSize; ++i) {
			testInput = (gridSize == 9) ? i + 49 : i + 65;
			if (enforceRules(x, y, testInput, true)) {
				gameBoard[x][y].attempted.push_back(testInput);
			}
		}

		// if: total attempts for current cell is smaller than previous, then hold
		//		coordinates and set as current minimum number of attempts.
		if (gameBoard[x][y].attempted.size() != 0 && gameBoard[x][y].attempted.size() < minAttempts) {
			minAttempts = gameBoard[x][y].attempted.size();
			min_x = x;
			min_y = y;
		}
	}

	// if: minAttempts only has one possible input, make it a new input for the cell.
	//		could be wrong, if user's inputs are incorrect.
	//	elseif: more than one possible inputs, use solution's input. (commented out below)
	//		CHANGE TO(?): if more than one possible inputs, pick one at random.
	if (minAttempts == 1)
		newInput(min_x, min_y, gameBoard[min_x][min_y].attempted[0]);
	else if (minAttempts > 1)
		//newInput(min_x, min_y, solution[min_x][min_y].getInput());
		newInput(min_x, min_y, gameBoard[min_x][min_y].attempted[floor((rand() * minAttempts) / RAND_MAX)]);
}
