#include "Connect4Game.h"
#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

//BOARD[HEIGHT][WIDTH] (Row,Col)

Connect4Game::Connect4Game()
{
	moves = 0;

	////Allocate memory for board
	//board = new int* [HEIGHT];
	//for (size_t i = 0; i < HEIGHT; i++)
	//{
	//	board[i] = new int[WIDTH];
	//}

	//board = new int[HEIGHT][WIDTH]();

	//set all values to zero
	ResetBoard();
}

Connect4Game::Connect4Game(const Connect4Game &gameIn)
{
	memcpy(this->board, gameIn.board, sizeof(gameIn.board));
	//this->board = CopyBoardState(gameIn.board);
	this->moves = gameIn.moves;
	this->lastMove = gameIn.lastMove;
}

Connect4Game::~Connect4Game() 
{
	/*for (size_t i = 0; i < HEIGHT; i++)
	{
		delete[] board[i];
	}*/
	//delete[] board;
}

void Connect4Game::ResetBoard()
{
	for (size_t i = 0; i < HEIGHT; i++)
	{
		for (size_t j = 0; j < WIDTH; j++)
		{
			board[i][j] = 0;
		}
	}
}

void Connect4Game::ResetGame() {
	ResetBoard();
	moves = 0;
	lastMove = 0;
}

int Connect4Game::Drop(int column, int value)
{
	int lowest = 0;

	//Check lowest token
	for (int i = 0; i < HEIGHT; i++)
	{
		//if this token is empty
		if (board[i][column] == 0)
		{
			lowest = i;
			break;
		}
	}

	//Drop token
	board[lowest][column] = value;
	lastMove = column;
	moves++;

	return lowest;
}

void Connect4Game::PrintBoard() const
{
	string out;
	out = "";

	//Print header
	for (size_t i = 0; i < WIDTH; i++)
	{
		out += ("\"" + to_string(i+1) + "\" ");
	}
	out += "\n";

	//Brint board
	for (size_t i = HEIGHT; i > 0; i--)
	{
		for (size_t j = 0; j < WIDTH; j++)
		{
			out += "["; 
			out += (board[i - 1][j] == 0 ? "-" : (board[i - 1][j] == 1 ? "O" : "X")); 
			out +="] ";
		}
		out += "\n";
	}
	cout << out;
}

const bool Connect4Game::IsDroppable(int column) const
{
	//Check bounds
	if (column >= WIDTH || column < 0) { return false; }
	//Check empty top
	if (board[HEIGHT - 1][column] == 0) {return true;}
	return false;
}

//Returns value of winner else 0
const int Connect4Game::CheckWin() const
{
	//Multicheck
	for (int i = 0; i < HEIGHT; i++) {	//For each row

		for (int j = 0; j < WIDTH; j++) {	//For each Column

			if (board[i][j] != 0) { //Check for player
			
				//Verticle down check
				if (i < HEIGHT - 3
					&& board[i][j] == board[i+1][j]
					&& board[i][j] == board[i+2][j]
					&& board[i][j] == board[i+3][j]) {
					return(board[i][j]);
				}

				//Horizontal right check
				if (j < WIDTH - 3
					&& board[i][j] == board[i][j + 1]
					&& board[i][j] == board[i][j + 2]
					&& board[i][j] == board[i][j + 3]) {
					return(board[i][j]);
				}
				//Diagonal North-East check
				if (i < HEIGHT - 3 && j < WIDTH -3
					&& board[i][j] == board[i + 1][j + 1]
					&& board[i][j] == board[i + 2][j + 2]
					&& board[i][j] == board[i + 3][j + 3]) {
					return(board[i][j]);
				}
				//Diagonal South-East check
				if(i >= 3 && j < WIDTH-3
					&& board[i][j] == board[i - 1][j + 1]
					&& board[i][j] == board[i - 2][j + 2]
					&& board[i][j] == board[i - 3][j + 3]) {
					return(board[i][j]);
				}
			}
		}
	}
	return 0;
}

//const bool Connect4Game::IsWinningMove(int column, int player)const {
//	//Clone board
//	Connect4Game testGame(*this);
//	
//	//Test drop
//	testGame.Drop(column, player);
//
//	return(testGame.CheckWin() == player);
//}

//#pragma optimize( "", off )
const bool Connect4Game::IsWinningMove(int column, int player)const {
	
	//Clone board
	Connect4Game testGame(*this);

	//Drop and retrieve how deep the token fell
	int hi = testGame.Drop(column, player);//Height (up/down)
	int col = column;//Column (Left/right)

	auto board = testGame.board;

	//Evaluate if this move wins
	//Verticle down check
	if (hi > 2
		&& board[hi][col] == board[hi - 1][col]
		&& board[hi][col] == board[hi - 2][col]
		&& board[hi][col] == board[hi - 3][col]) {
		return true;
	}
	////Horizontal left check
	//if (col > 3
	//	&& board[hi][col] == board[hi][col - 1]
	//	&& board[hi][col] == board[hi][col - 2]
	//	&& board[hi][col] == board[hi][col - 3]) {
	//	return true;
	//}
	////Horizontal right check
	//if (col < WIDTH - 3
	//	&& board[hi][col] == board[hi][col + 1]
	//	&& board[hi][col] == board[hi][col + 2]
	//	&& board[hi][col] == board[hi][col + 3]) {
	//	return true;
	//}
	//Horizontal Checks
	for (int off = 0; off < 4; off++) {
		if (board[hi][col - off] == player) {
			if (col - off >= 0 && col - off < WIDTH - 3
				&& board[hi][col - off] == board[hi][col + 1 - off]
				&& board[hi][col - off] == board[hi][col + 2 - off]
				&& board[hi][col - off] == board[hi][col + 3 - off]) {
				return true;
			}
		}
	}
	//Diagonal Left check
	for (int off = 0; off < 4; off++) {
		if (board[hi - off][col + off] == player) {
			if (hi - off < HEIGHT - 3 && hi - off >= 0 && col + off > 2 && col + off < WIDTH
				&& board[hi - off][col + off] == board[hi + 1 - off][col - 1 + off]
				&& board[hi - off][col + off] == board[hi + 2 - off][col - 2 + off]
				&& board[hi - off][col + off] == board[hi + 3 - off][col - 3 + off]) {
				return true;
			}
		}
	}
	//Diagonal Right check
	for (int off = 0; off < 4; off++) {
		if (board[hi - off][col - off] == player) {
			if (hi - off < HEIGHT - 3 && hi - off >= 0 && col - off >= 0 && col - off < WIDTH - 3 
				&& board[hi - off][col - off] == board[hi + 1 - off][col + 1 - off]
				&& board[hi - off][col - off] == board[hi + 2 - off][col + 2 - off]
				&& board[hi - off][col - off] == board[hi + 3 - off][col + 3 - off]) {
				return true;
			}
		}
	}
	return false;
}
//#pragma optimize( "", off )