#import <iostream>

class Cell {
private:
    int input;
    bool locked;

public:
    Cell( );
    void lockCell( );
    void setInput( int input );
    int getInput( );
};

Cell( ) {
    // default input = 0 ?
    locked = false;
};