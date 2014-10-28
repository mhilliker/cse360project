#include "Board.h"

// TO DO: enforce rules check for user inputs.

Board::Board(int level) {
	difficulty = level;
	gridSize = 9;

	switch (difficulty) {
	case 0: fillPercent = 50;   // easy
		break;
	case 1: fillPercent = 45;   // medium 
		break;
	case 2: fillPercent = 40;   // hard
		break;
	case 3: fillPercent = 35;   // evil
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
	bool valid;
	random_device randDev;
	mt19937 mt(randDev());
	uniform_real_distribution<double> gridRand(1, 10);
	uniform_real_distribution<double> putRand(0, 1);

	// for: loops through every cell to generate a solution and gameBoard.
	for (int row = 0; row < gridSize; row++) {
		for (int col = 0; col < gridSize; col++) {
			failedRules = true;
			randomizer = putRand(mt);

			// while: input failed the rules, retry with new input.
			while (failedRules) {
				valid = true;
				randomInput = floor(gridRand(mt));
				int attemptSize = solution[col][row].attempted.size();

				// for: for every previously attempted value for the current cell,
				//		check if new input has already been attempted. If a match
				//		is found in the array of attempted values, set valid to
				//		false, and break.
				for (int i = 0; i < solution[col][row].attempted.size(); i++) {
					if (solution[col][row].attempted[i] == randomInput) {
						valid = false;
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
				if (valid) {
					if (enforceRules(row, col, randomInput, true) && solution[col][row].attempted.size() != 9) {
						failedRules = false;

						// if: random is within percent range, lockCell and setInput for
						//		gameBoard and solution. Else, only set value for solution
						//		but don't lock.
						if (randomizer <= (fillPercent / 100)) {
							solution[col][row].lockCell();
							solution[col][row].setInput(randomInput);
							gameBoard[col][row].setInput(randomInput);
						}
						else {
							solution[col][row].setInput(randomInput);
						} // end if-else
					}
					else {
						solution[col][row].attempted.push_back(randomInput);
					} // end if-else
				}
				else if (solution[col][row].attempted.size() >= 9) {
					solution[col][row].attempted.clear();

					// if: column is at minimum, jump back a row, and set column to last index.
					//		Else, jump back a column.
					if (col == 0) {
						row--;
						col = 8;
					}
					else {
						col--;
					} // end if-else
					solution[col][row].attempted.push_back(solution[col][row].getInput());
					solution[col][row].resetCell();
					gameBoard[col][row].resetCell();
				} // end if-elseif
			} // end while
		} // end col for
	} // end row for
}

void Board::endGame() {
	// calculate scores, and store away score
}

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
*	Needs cell input before change. (!)
*	So when user inputs, only setInput AFTER checking rules. (!)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::enforceRules(int row, int col, int input, bool init) {
	int rootRow;
	int rootCol;
	int checkInput;
	bool validRow = true;
	bool validCol = true;
	bool validBox = true;

	// forloop: loops through row (increments columns of one row)
	//      skips column in which the input exists
	for (int c = 0; c < gridSize; ++c) {
		checkInput = (init) ? solution[c][row].getInput() : gameBoard[c][row].getInput();

		// if: cell's input data is same as new input, return false & label
		//		matching cells as bad input.
		if (c != col) {
			if (init && checkInput == input) {
				return false;
			}
			else if (!init) {
				if (checkInput == input) {
					gameBoard[col][row].setRowValid(false);
					gameBoard[c][row].setRowValid(false);
					validRow = false;
				}
				else if (gameBoard[c][row].getRowValid() == false) {
					if (gameBoard[col][row].getInput() != 0 && checkInput == gameBoard[col][row].getInput()) {
						gameBoard[c][row].setRowValid(true);
					}
				}
			}
		}
	}

	// forloop: loops through column (increments rows of one column)
	//      skips row in which the new input exists
	for (int r = 0; r < gridSize; ++r) {
		checkInput = (init) ? solution[col][r].getInput() : gameBoard[col][r].getInput();

		// if: cell input is same as new input, return false
		//		else if cell is already bad input not equal to, 
		if (init && r != row && checkInput == input) {
			return false;
		}
		else if (!init) {
			if (r != row && checkInput == input) {
				gameBoard[col][row].setColValid(false);
				gameBoard[col][r].setColValid(false);
				validCol = false;
			}
			else if (gameBoard[col][r].getColValid() == false) {
				// if: bad input because previous input ( == previous)...
				//		else do nothing???
				if (checkInput == gameBoard[col][row].getInput()) {

				}
			}
		}
	}

	// if: checks which set of rows input belongs to then looks for colRoots().
	//      Runs checkBox() for any matching values within the box.
	if (row >= 0 && row < 3) {
		rootRow = 0;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	}
	else if (row >= 3 && row < 6) {
		rootRow = 3;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	}
	else {
		rootRow = 6;
		rootCol = colRoots(col);
		return checkBox(rootRow, rootCol, row, col, input, init);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   rowCases() returns an index for the root column for the box containing the
*   specified column. Separated by (0 <= col < 3), (3 <= col < 6), and
*	(6 <= col < 9).
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Board::colRoots(int col) {
	if (col >= 0 && col < 3) return 0;
	else if (col >= 3 && col < 6) return 3;
	else return 6;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   checkBox() returns a boolean for enforceRules(). If cell input is same
*	as the new input, return false. If none returned false, return true to
*	indicate that there were no repeated values in the box.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::checkBox(int rootRow, int rootCol, int row, int col, int input, bool init) {
	int checkInput;
	for (int rows = rootRow; rows < rootRow + 3; rows++)
	for (int cols = rootCol; cols < rootCol + 3; cols++)
	if (cols != col && rows != row) {
		checkInput = (init) ? solution[cols][rows].getInput() : gameBoard[cols][rows].getInput();
		if (checkInput == input) return false;
	}
	return true;
}

// This is only for testing purposes, printing to the console.
void Board::printSolution() {
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
	cout << endl;
}