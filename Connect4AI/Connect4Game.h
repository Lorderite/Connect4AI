#pragma once

class Connect4Game
{
public:
	static const int WIDTH = 7;
	static const int HEIGHT = 6;

private:
	int moves;
	int lastMove;
	int board[HEIGHT][WIDTH];

public:
	Connect4Game();

	Connect4Game(const Connect4Game &gameIn);

	~Connect4Game();

	void ResetBoard();

	void ResetGame();

	int Drop(int column, int value);

	void PrintBoard() const;

	const bool IsDroppable(int column) const;

	const int CheckWin() const;

	const bool IsWinningMove(int column, int player)const;

	//Game states

	//int** CopyBoardState(int** const boardIn);
	
	//Other Gets

	inline int GetMoves() const {
		return moves;
	}

	const inline int GetLastMove() {
		return lastMove;
	}
};

