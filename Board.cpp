#include "Board.h"

// TO DO: enforce rules check for user inputs (!init).

Board::Board(int level) {
	difficulty = level;
	gridSize = 9;

	switch (difficulty) {
	case 0: fillPercent = 45;   // easy
		break;
	case 1: fillPercent = 41;   // medium 
		break;
	case 2: fillPercent = 37;   // hard
		break;
	case 3: fillPercent = 43;   // evil
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
}

//Board::~Board() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   newBoard() reinitializes board, and replaces current vector
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newBoard() {
	double randomizer;
	int randomInput;
	bool invalid;
	bool validInput;
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> gridRand(1, 10);
	uniform_real_distribution<double> putRand(0, 1);

	// for: loops through every cell to generate a solution and gameBoard.
	for (int row = 0; row < gridSize; row++)
	for (int col = 0; col < gridSize; col++) {
		invalid = true;
		randomizer = putRand(mt);

		// while: input failed the rules, retry with new input.
		while (invalid) {
			validInput = true;
			randomInput = floor(gridRand(mt));
			int attemptSize = solution[col][row].attempted.size();

			// for: for every previously attempted value for the current cell,
			//		check if new input has already been attempted. If a match
			//		is found in the array of attempted values, set valid to
			//		false, and break.
			for (int i = 0; i < solution[col][row].attempted.size(); i++) {
				if (solution[col][row].attempted[i] == randomInput) {
					validInput = false;
					break;
				}
			}

			// if: valid input,  check if rules don't fail and attempts are in range
			//		then continue to set values. Else, push the attempted random input
			//		to the cell's array of attempted inputs.
			//		If the input is not valid, check if number of attempts equal or
			//		exceeds possible number of inputs. If exceeded, clear current set
			//		of attempted values then move back one cell, send current input
			//		to the array of attempts, and reset the cell's contents.
			if (validInput) {

				// if: rules are enforced correctly and number of attempts don't exceed 9
				if (enforceRules(row, col, randomInput) && solution[col][row].attempted.size() != 9) {
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
			} else if (solution[col][row].attempted.size() >= 9) {
				solution[col][row].attempted.clear();

				// if: column is at minimum, jump back a row, and set column to last index.
				//		Else, jump back a column.
				if (col == 0) {
					row--;
					col = 8;
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
*	endGame() will send calculate scores then the player will see their
*	score on the next screen.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void Board::endGame() {
//	// calculate scores, and store away to user
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
bool Board::enforceRules(int row, int col, int input) {
	int rootCol = (col >= 0 && col < 3) ? 0 : (col >= 3 && col < 6) ? 3 : 6;
	int rootRow = (row >= 0 && row < 3) ? 0 : (row >= 3 && row < 6) ? 3 : 6;

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

	for (int r = rootRow; r < rootRow + 3; r++)
	for (int c = rootCol; c < rootCol + 3; c++) {
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
void Board::newInput(int row, int col, int newInput) {
	int currentInput = gameBoard[col][row].getInput();

	// if: set input, then find first match within row, col, or box, and set valid
	//			status if applicable. When found and set, move on.
	//		else if: unset input, then count number of matches to currently set
	//			input. One match - set match valid to true. More than one
	//			match - leave matches alone. Then after either check, reset
	//			to unset cell.
	//		else if: change input, call check for unsetting input then call to
	//			set input to new value.
	if (currentInput == 0 && newInput != 0) {
		gameBoard[col][row].setInput(newInput);
	} else if (currentInput != 0 && newInput == 0) {
		gameBoard[col][row].resetCell();
	} else if (currentInput != 0 && newInput != 0) {
		gameBoard[col][row].resetCell();
		gameBoard[col][row].setInput(newInput);
	}

	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) {
			if (!gameBoard[x][y].getLocked() && gameBoard[x][y].getInput() != 0) {
				check(x, y);
			}
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
	int rootCol = (col >= 0 && col < 3) ? 0 : (col >= 3 && col < 6) ? 3 : 6;
	int rootRow = (row >= 0 && row < 3) ? 0 : (row >= 3 && row < 6) ? 3 : 6;
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
	for (int r = rootRow; r < rootRow + 3; r++)
	for (int c = rootCol; c < rootCol + 3; c++) {
		if (c != col && r != row && check->getInput() == gameBoard[c][r].getInput()) {
			vBox = false;
			r = rootRow + 3;
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

// only for testing output, printing boards to the console.
void Board::printSolution() {
	bool incomplete;
	bool invalid = false;
	int checks = 0;

	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (gameBoard[x][y].getInput() == 0) {
			y = gridSize;
			break;
		}
		checks++;
	}
	
	incomplete = (checks != 81) ? true : false;

	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (incomplete || !gameBoard[x][y].isValid()) {
			invalid = true;
			y = gridSize;
			break;
		}
	}

	cout << "\nSolution Board: \n";
	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) cout << solution[x][y].getInput() << " ";
		cout << endl;
	}
	cout << "\nGame Board: \n";
	for (int y = 0; y < gridSize; ++y) {
		for (int x = 0; x < gridSize; ++x) cout << gameBoard[x][y].getInput() << " ";
		cout << endl;
	}

	if (incomplete) cout << "\nIncompleted board.\n";
	else if (invalid) cout << "\nIncorrect inputs.\n";
	else cout << "\nComplete and valid!\n";

	cout << endl;
}

// for testing - makes random incorrect/correct moves
void Board::playBoardRndm() {
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> gridRand(1, 10);
	int input;

	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (!gameBoard[x][y].getLocked()) {
			input = floor(gridRand(mt));
			newInput(y, x, input);
			printSolution();
		}
	}

	printSolution();
}

// for testing - inputs all correct solutions from solution board
void Board::playBoardReal() {
	int input;
	Cell * temp = new Cell();

	for (int y = 0; y < gridSize; ++y)
	for (int x = 0; x < gridSize; ++x) {
		if (!gameBoard[x][y].getLocked()) {
			temp = &gameBoard[x][y];
			input = solution[x][y].getInput();
			newInput(y, x, input);
			printSolution();
		}
	}
}