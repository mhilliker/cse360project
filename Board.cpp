#import <iostream>
#import <string>
#import <math.h>

using namespace std;

class Board {
private:
    int difficulty;
    int gridSize;
    int gameBoard[9][9];

    int colRoots( int col );
    bool checkBox( int rootRow, int rootCol, int row, int col, int input );

public:
    Board( int level ) {
        difficulty = level;
    };
    ~Board() {
        // anything requiring delete?
    };

    void newBoard( );
    bool enforceRules( int row, int col, int input );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   newBoard() reinitializes all the locked inputs, and replaces current array
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Board::newBoard( ) {
    int fillPercent;
    float randomize;

        // take the current score, and save.
        // ...

    switch ( difficulty ) {
        case 0:
            fillPercent = 40;   // easy
            gridSize = 9;
            break;
        case 1:
            fillPercent = 30;   // medium
            gridSize = 9;
            break;
        case 2:
            fillPercent = 20;   // hard
            gridSize = 9;
            break;
        case 3:
            fillPercent = 10;   // evil
            gridSize = 16;      // change board size?
            break;
        default:
            break;
    }

    for ( int row = 0; row < gridSize; ++row ) {
        for ( int col = 0; col < gridSize; ++col ) {
            randomize = ( float ) rand() / ( RAND_MAX / 1 );
            if ( randomize <= fillPercent/100 ) {
                // lock cell gameBoard[column][row]
                randomize = floor( rand() / ( RAND_MAX / 10 ) );
                if ( !enforceRules( row, col, randomize ) ) {
                    
                }
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   enforceRules() will check for a matching value to the input in the same row,
*   column, or box. If a match is found the function will return false,
*   indicating that it is not a winning move (will remain unknown to player).
*   Otherwise, if a match is not found, it will return true.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::enforceRules( int row, int col, int input ) {
    int rootRow;
    int rootCol;

    // forloop: loops through row = incrementing column
    // skips column in which the input exists
    for ( int c = 0; c < gridSize; ++c ) {
        if ( c != col && gameBoard[c][row] == input ) {
            return false;
        }
    }

    // forloop: loops through column = incrementing row
    // skips row in which the input exists
    for ( int r = 0; r < gridSize; ++r ) {
        if ( r != row && gameBoard[col][r] == input ) {
            return false;
        }
    }

    // ifstatement: checks which set of rows input belongs to
    // then looks for column. runs checkbox for any matching values
    if ( row >= 0 && row < 3 ) {
        rootRow = 0;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    } else if ( row >= 3 && row < 6 ) {
        rootRow = 3;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    } else if ( row >= 6 && row < 9 ) {
        rootRow = 6;
        rootCol = colRoots( col );
        return checkBox( rootRow, rootCol, row, col, input );
    } else {
        // error
        return false;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   rowCases() returns an index for the root column for the box containing the
*   specified column
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Board::colRoots( int col ) {
    if ( col >= 0 && col < 3 ) {
        return 0;
    } else if ( col >= 3 && col < 6 ) {
        return 3;
    } else if ( col >= 6 && col < 6 ) {
        return 6;
    } else {
        // error
        return -1;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   checkBox() returns a boolean for enforceRules()
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Board::checkBox( int rootRow, int rootCol, int row, int col, int input ) {
    for ( int cols = rootRow; cols < rootRow + 3; ++cols ) {
        for ( int rows = rootCol; rows < rootCol + 3; ++cols ) {
            if ( cols != col &&
                 rows != row &&
                 gameBoard[cols][rows] == input ) {
                return false;
            }
        }
    }
    return true;
}