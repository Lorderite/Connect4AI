#pragma once

#include "Connect4Game.h"
#include <iostream>
#include <future>
#include <vector>

const static int columnOrder[] = { 3, 4, 2, 5, 1, 6, 0 };

using namespace std;

class NegaMaxSolver {

public:
	NegaMaxSolver(int searchDepth=17) {
		this->searchDepth = searchDepth;
		nodesExplored = 0;
	}

private:
	int searchDepth;

public:

	unsigned nodesExplored;
	static const int INVALID_MOVE = -1000;

	const int GetSearchDepth()const { return this->searchDepth; }
	

	int SolveMove(Connect4Game game, int player) {

		future<int> threads[Connect4Game::WIDTH];

		int bestScore = INVALID_MOVE;
		int scores[7]{};

		cout << "Thinking...\n";
		
		//Evaluate all potential initial move scores (0-6)
		for (size_t i = 0; i < Connect4Game::WIDTH; i++)
		{
			//Evaluate score
			if (game.IsDroppable(columnOrder[i])) { //Evaluate potential
				if (game.IsWinningMove(columnOrder[i], player)) { scores[columnOrder[i]] = 1000; } //If immediate win
				else {
					Connect4Game testGame(game);
					testGame.Drop(columnOrder[i], player);

					//Multithread
					//cout << "Thread \"" << i << "\" started\n";
					threads[columnOrder[i]] = async(launch::async, [this, testGame, &player] {return NegamaxSolve(testGame, (player == 1 ? 2 : 1)); });
				}
			}
			else { scores[columnOrder[i]] = INVALID_MOVE; }
		}

		//Score retreival and move selection
		vector<int> bestMoves;
		for (int i = 0; i < Connect4Game::WIDTH; i++)
		{
			//Get thread Score
			if (threads[columnOrder[i]].valid()) {
				scores[columnOrder[i]] = -threads[columnOrder[i]].get();
			}

			//Best scores
			if (scores[columnOrder[i]] > bestScore) { //If score greater
				//Assign new best score
				bestScore = scores[columnOrder[i]];
				//Clear vector and add move
				bestMoves.clear();
				bestMoves.push_back(columnOrder[i]);
			}
			else if (scores[columnOrder[i]] == bestScore) {
				bestMoves.push_back(columnOrder[i]);
			}
		}


		//Print Scores
		cout << "Move values:\n";
		for (size_t i = 0; i < Connect4Game::WIDTH; i++)
		{
			std::cout << "[" << scores[i] << "]";
		}
		std::cout << "\n";

		//Moves that are being considered
		cout << "Moves considered: ";
		for (int var : bestMoves)
		{
			cout << var+1 << ", ";
		}
		cout << "\n";

		//Return the move to be made (randomized if equal values)
		srand((unsigned int)time(NULL));
		return bestMoves[rand() % bestMoves.size()];
	}

	//Solve the move potential
	int NegamaxSolve(Connect4Game game, int player) {

		//Check win in one move
		for (int i = 0; i < Connect4Game::WIDTH; i++)
		{
			if (game.IsDroppable(i) && game.CheckWin() == player) {
				return(10000); //This is immediate win!
			}
		}

		//If no immediate win then explore potential deeper
		//Set up variables
		nodesExplored = 0;
		int depth = 0;

		//Run algorithm
		return Negamax(game, -Connect4Game::WIDTH * Connect4Game::HEIGHT/2, Connect4Game::WIDTH * Connect4Game::HEIGHT/2, player, depth);
	}

	//===Negamax===
	int Negamax(Connect4Game &game, int alpha, int beta, int player, int depth) {
		//Count this node
		nodesExplored++;

		//If draw or beyond max depth
		if (game.GetMoves() == (Connect4Game::WIDTH * Connect4Game::HEIGHT) || depth > searchDepth) {return 0;}

		//Check for winning move
		for (int i = 0; i < Connect4Game::WIDTH; i++){
			if (game.IsDroppable(i) && game.IsWinningMove(i, player)) {
				return(Connect4Game::WIDTH * Connect4Game::HEIGHT + 1 - game.GetMoves());// / 2; //Favours less moves
			}
		}

		//handle max
		int max = (Connect4Game::WIDTH * Connect4Game::HEIGHT - 1 - game.GetMoves());// / 2;
		if (beta > max) {
			beta = max; //Cap beta to max
			if (alpha >= beta) { 
				return beta; 
			} //prune 
		}

		//Compute all next possible moves and keep best
		for (int i = 0; i < Connect4Game::WIDTH; i++){
			if (game.IsDroppable(columnOrder[i])) {
				Connect4Game testGame(game);
				testGame.Drop(columnOrder[i], player); //Current player tries this column
				//Opponent Negamax
				int score = -Negamax(testGame, -beta, -alpha, (player == 1 ? 2 : 1), depth+1);

				//If score is superior
				if (score >= beta) {
					return score;
				}
				//Reduce window in search for better than what we have
				if (score > alpha) { 
					alpha = score; 
				}
			}
		}

		return alpha;
	}
};
