#include "Cell.h"

Cell::Cell() {
	input = NULL;
	locked = false;
}

bool Cell::getLocked() {
	return locked;
}

void Cell::lockCell() {
	locked = true;
}

int Cell::getInput() {
	return input;
}

void Cell::setInput(int newInput) {
	input = newInput;
}