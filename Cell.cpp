#include "Cell.h"

Cell::Cell() {
	input = NULL;
	locked = false;
	validRow = true;
	validCol = true;
	validBox = true;
}

bool Cell::getLocked() {
	return locked;
}

void Cell::lockCell() {
	locked = true;
}

char Cell::getInput() {
	return input;
}

void Cell::setInput(char newInput) {
	input = newInput;
}

void Cell::resetCell() {
	input = NULL;
	locked = false;
	validRow = true;
	validCol = true;
	validBox = true;
}

void Cell::setRowValid(bool validR) {
	validRow = validR;
}

void Cell::setColValid(bool validC) {
	validCol = validC;
}

void Cell::setBoxValid(bool validB) {
	validBox = validB;
}

bool Cell::isValid() {
	return (validRow && validCol && validBox) ? true : false;
}