#include "Tetris.h"
#include <iostream>

using namespace std;

Tetris::Tetris()
{
	clearBoard();
	curPiece = new Piece();

	curPiece->printPiece();
}

void Tetris::clearBoard()
{
	for (int x = 0; x < TETRIS_COLS; x++) {
		for (int y = 0; y < TETRIS_ROWS; y++) {
			//Nothing marked
			board[x][y] = EMPTY_SPACE;
		}
	}
}

int Tetris::highestValidCol()
{
	return TETRIS_COLS - curPiece->pieceWidth() - 1;
}

void Tetris::dropInColumn(int col)
{
	assert(col >= 0);
	assert(col <= highestValidCol());
	assert(col < TETRIS_COLS);

	int dropRow = -PIECESIZE; //Start above the board
	while (!collision(col, dropRow)) {
		cout << "drop: " << dropRow << endl;
		dropRow += 1;
	}
	dropRow -= 1;
	cout << dropRow << endl;
	placePiece(col, dropRow);
}

void Tetris::placePiece(int dropCol, int dropRow) {
	bool p[PIECESIZE][PIECESIZE];
	curPiece->copyPiece(p);

	for (int x = 0; x < PIECESIZE; x++) {
		for (int y = 0; y < PIECESIZE; y++) {
			if (p[x][y]) {
				//Check if the board has been lost
				if (y + dropRow < 0) {
					gameover = true;
				} else {
					board[x + dropCol][y + dropRow] = curPiece->getPieceID();
				}
			}
		}
	}

	curPiece = new Piece();
}

bool Tetris::collision(int dropCol, int dropRow)
{
	cout << "testing: " << dropCol << ", " << dropRow << endl;
	bool p[PIECESIZE][PIECESIZE];
	curPiece->copyPiece(p);

	for (int x = 0; x < PIECESIZE; x++) {
		for (int y = 0; y < PIECESIZE; y++) {
			//Only check when the piece is there and the piece is on the board
			if (p[x][y] && y + dropRow > 0) {
				//Check if piece is off the board
				if (x + dropCol >= TETRIS_COLS) {
					cout << "Off the board X" << endl;
					return true;
				}
				//Check if piece is falling through the bottom
				if (y + PIECESIZE + dropRow > TETRIS_ROWS + 1) {
					cout << "Off the board Y" << endl;
					return true;
				}
				//Check if piece is colliding with already placed pieces
				if (board[x + dropCol][y + dropRow] != EMPTY_SPACE) {
					cout << "COLLISION" << endl;
					return true;
				}
			}
		}
	}
	return false;
}

void Tetris::printBoard()
{
	for (int y = 0; y < TETRIS_ROWS; y++) {
		for (int x = 0; x < TETRIS_COLS; x++) {
			if (board[x][y] != EMPTY_SPACE) {
				printf("%c[%d;%d;%dm",27,1,37,40 + board[x][y]);
				cout << "  ";
				printf("%c[%dm", 0x1B, 0);
			} else {
				cout << " .";
			}
		}
		cout << endl;
	}
}

Piece* Tetris::currentPiece()
{
	int id = curPiece->getPieceID();
	cout << "id " << id << endl;
	return new Piece(id);
}

bool Tetris::isLost()
{
	return gameover;
}