#include "Cell.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	Constructor
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Cell::Cell() {
	input = NULL;
	locked = false;
	validRow = true;
	validCol = true;
	validBox = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	returns a boolean of whether the cell is locked.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Cell::getLocked() {
	return locked;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	locks the cell.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::lockCell() {
	locked = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	returns the input character.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char Cell::getInput() {
	return input;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	sets the input of the cell.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::setInput(char newInput) {
	input = newInput;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	returns a boolean of whether the cell's input was made by the AI.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Cell::getAIMove() {
	return aiMove;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	clears all current values of the cell to default values.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::resetCell() {
	input = NULL;
	locked = false;
	validRow = true;
	validCol = true;
	validBox = true;
	aiMove = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	sets the validity of the cell in relation to the row of cells in whch it
*	is contained.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::setRowValid(bool validR) {
	validRow = validR;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	sets the validity of the cell in relation to the column of cells in which
*	it is contained.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::setColValid(bool validC) {
	validCol = validC;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	sets the validity of the cell in relation to the box of cells in which it
*	is contained.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Cell::setBoxValid(bool validB) {
	validBox = validB;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	returns a true if the cell is valid in its row, column, and box. False
*	means that the cell has at least one false, and the cell cannot be valid.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Cell::isValid() {
	return (validRow && validCol && validBox) ? true : false;
}