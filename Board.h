#import <iostream>
#import <vector>
#import <string>
#import <math.h>
#include "Cell.h"

using namespace std;

class Board {
private:
    int startTime;
    int difficulty;
    int gridSize;
    int score;
    vector< vector<Cell> > solution;
    vector< vector<Cell> > gameBoard;
    int colRoots( int col );
    bool enforceRules( int row, int col, int input );
    bool checkBox( int rootRow, int rootCol, int row, int col, int input );

public:
    Board( int level );
    ~Board( );
    void newBoard( );
    void endGame( );
    vector< vector<Cell> > getSolution( );
    vector< vector<Cell> > getBoard( );
};