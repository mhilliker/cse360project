#include "Cell.h"

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