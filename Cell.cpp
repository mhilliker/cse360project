#include <iostream>

class Cell {
private:
    int input;
    bool locked;

public:
    Cell( );
    bool getLocked( );
    void lockCell( );
    int getInput( );
    void setInput( int newInput );
};

Cell::Cell( ) {
    // default input = 0 ?
    locked = false;
}

bool Cell::getLocked( ) {
    return locked;
}

void Cell::lockCell( ) {
    locked = true;
}

int Cell::getInput( ) {
    return input;
}

void Cell::setInput( int newInput ) {
    input = newInput;
}