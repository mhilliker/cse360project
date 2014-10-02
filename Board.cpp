Board::Board( int level ) {
    difficulty = level;
    gridSize = 9;
}

Board::~Board( ) {
    // delete stuff
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   newBoard() reinitializes board, and replaces current array
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newBoard( ) {
    int fillPercent;
    float randomizer;
    int randomInput;
    bool failedRules = true;

    switch ( difficulty ) {
        case 0:
            fillPercent = 40;   // easy
            break;
        case 1:
            fillPercent = 30;   // medium
            break;
        case 2:
            fillPercent = 20;   // hard
            break;
        // case 3:                 // case 3 will require a change in code for enforceRules() & checkBox() !!!!!!
        //     fillPercent = 20;   // evil
        //     gridSize = 16;
        //     break;
        default:
            break;
    }

    // forloop: loops through every cell to create a solution and gameBoard.
    for ( int row = 0; row < gridSize; ++row ) {
        for ( int col = 0; col < gridSize; ++col ) {
            randomizer = ( float ) rand( ) / ( RAND_MAX / 1 );
            randomInput = floor( rand( ) / ( RAND_MAX / 10 ) );

            // while: if the input failed the rules, redo the input.
            // if: rules don't fail, continue to next if, else randomize.
            // if: randomly chose as locked, lockCell and setInput for
            //      gameBoard and solution. Else, only set value for
            //      solution but don't lock
            while ( failedRules ) {
                if ( enforceRules( row, col, randomInput ) ) {
                    failedRules = false;
                    if ( randomizer <= fillPercent / 100 ) {
                        solution[col][row].lockCell( );
                        solution[col][row].setInput( randomInput );
                        gameBoard[col][row] = solution[col][row];               // need to test: will this work? else manually lock and set for gameBoard
                    } else {
                        solution[col][row].setInput( randomInput );
                    }   
                } else {
                    randomInput = floor( rand( ) / ( RAND_MAX / 10 ) );
                }
            }
        }
    }
}

void Board::endGame( ) {
    // calculate scores, and store away score
}

vector< vector<Cell> > Board::getBoard( ) {
    return gameBoard;
}

vector< vector<Cell> > Board::getSolution( ) {
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
bool Board::enforceRules( int row, int col, int input ) {
    int rootRow;
    int rootCol;

    // forloop: loops through row (increments through column)
    //      skips column in which the input exists
    //      if: cell input is same as new input, return false
    for ( int c = 0; c < gridSize; ++c ) {
        if ( c != col && gameBoard[c][row].getInput( ) == input ) {
            return false;
        }
    }

    // forloop: loops through column (increments through row)
    //      skips row in which the new input exists
    //      if: cell input is same as new input, return false
    for ( int r = 0; r < gridSize; ++r ) {
        if ( r != row && gameBoard[col][r].getInput( ) == input ) {
            return false;
        }
    }

    // if: checks which set of rows input belongs to then looks for colRoots().
    //      Runs checkBox() for any matching values
    if ( row >= 0 && row < 3 ) {
        rootRow = 0;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    } else if ( row >= 3 && row < 6 ) {
        rootRow = 3;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    } else {
        rootRow = 6;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   rowCases() returns an index for the root column for the box containing the
*   specified column.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Board::colRoots( int col ) {
    if ( col >= 0 && col < 3 ) {
        return 0;
    } else if ( col >= 3 && col < 6 ) {
        return 3;
    } else {
        return 6;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   checkBox() returns a boolean for enforceRules(). If cell input is same as
*   new input, return false. If none returned false, return true.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::checkBox( int rootRow, int rootCol, int row, int col, int input ) {
    for ( int cols = rootRow; cols < rootRow + 3; ++cols ) {
        for ( int rows = rootCol; rows < rootCol + 3; ++cols ) {
            if ( cols != col
                 && rows != row
                 && gameBoard[cols][rows].getInput( ) == input ) {
                return false;
            }
        }
    }
    return true;
}