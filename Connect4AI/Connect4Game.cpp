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

	//Allocate memory for board
	board = new int* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; i++)
	{
		board[i] = new int[WIDTH];
	}

	//set all values to zero
	ResetBoard();
}

Connect4Game::Connect4Game(const Connect4Game &gameIn)
{
	//memcpy(this->board, gameIn.board, sizeof(gameIn.board));
	this->board = CopyBoardState(gameIn.board);
	this->moves = gameIn.moves;
	this->lastMove = gameIn.lastMove;
}

Connect4Game::~Connect4Game() 
{
	for (size_t i = 0; i < HEIGHT; i++)
	{
		delete[] board[i];
	}
	delete[] board;
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

void Connect4Game::Drop(int column, int value)
{
	int lowest = -1;

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
			
				//Verticle check
				if (i < HEIGHT - 3
					&& board[i][j] == board[i+1][j]
					&& board[i][j] == board[i+2][j]
					&& board[i][j] == board[i+3][j]) {
					return(board[i][j]);
				}

				//Horizontal check
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


	////Check horizontal
	////for each row
	//for (size_t i = 0; i < HEIGHT; i++)
	//{
	//	//for each column -3
	//	for (size_t j = 0; j < WIDTH-3; j++)
	//	{
	//		if (board[i][j] != 0
	//			&& board[i][j] == board[i][j + 1]
	//			&& board[i][j] == board[i][j + 2]
	//			&& board[i][j] == board[i][j + 3]) {
	//			return(board[i][j]);
	//		}
	//	}
	//}

	////Check Verticle
	//for (size_t i = 0; i < HEIGHT - 3; i++)
	//{
	//	//for each column -3
	//	for (size_t j = 0; j < WIDTH; j++)
	//	{
	//		if (board[i][j] != 0
	//			&& board[i][j] == board[i+1][j]
	//			&& board[i][j] == board[i+2][j]
	//			&& board[i][j] == board[i+3][j]) {
	//			return(board[i][j]);
	//		}
	//	}
	//}

	////Check Diagonal North East
	//for (size_t i = 0; i < HEIGHT - 3; i++)
	//{
	//	//for each column -3
	//	for (size_t j = 0; j < WIDTH - 3; j++)
	//	{
	//		if (board[i][j] != 0
	//			&& board[i][j] == board[i + 1][j + 1]
	//			&& board[i][j] == board[i + 2][j + 2]
	//			&& board[i][j] == board[i + 3][j + 3]) {
	//			return(board[i][j]);
	//		}
	//	}
	//}

	////Check Diagonal South East
	//for (size_t i = 3; i < HEIGHT; i++)
	//{
	//	//for each column -3
	//	for (size_t j = 0; j < WIDTH-3; j++)
	//	{
	//		if (board[i][j] != 0
	//			&& board[i][j] == board[i - 1][j + 1]
	//			&& board[i][j] == board[i - 2][j + 2]
	//			&& board[i][j] == board[i - 3][j + 3]) {
	//			return(board[i][j]);
	//		}
	//	}
	//}

	return 0;
}

const bool Connect4Game::IsWinningMove(int column, int player)const {
	//Clone board
	Connect4Game testGame(*this);
	
	//Test drop
	testGame.Drop(column, player);

	return(testGame.CheckWin() == player);
}

int** Connect4Game::CopyBoardState(int** const boardIn) {
	int** boardOut;

	//Allocate memory for board
	boardOut = new int* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; i++)
	{
		boardOut[i] = new int[WIDTH];

		//Copy
		for (size_t j = 0; j < WIDTH; j++)
		{
			boardOut[i][j] = boardIn[i][j];
		}
	}
	return boardOut;
}