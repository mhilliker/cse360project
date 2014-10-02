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