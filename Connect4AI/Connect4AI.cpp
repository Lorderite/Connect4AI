// Connect4AI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Connect4Game.h"
#include "Negamax.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>

using namespace std;
using namespace std::chrono;

void PlayerHuman(Connect4Game& game, int player) {
	//Get input
	string input;
	cout << "Player " << player << ", where do you want to drop the token? (1-7)\n";

	//While drop fails 
	while (1) {

		//Get input
		cin >> input; 

		if (_stricmp(input.c_str(), "quit") == 0) {
			cout << "Quitting\n";
			exit(0);
		}

		try {
			//Check conversion
			int col = std::stoi(input)-1;

			if (game.IsDroppable(col)) { //If conversion valid and is droppable allow drop
				game.Drop(col, player);
				cout << "\n";
				break;
			}

			cout << "Invalid drop, try again\n";
		}
		catch (const std::invalid_argument e) {
			cout << "Invalid input, try again\n";
		}
	}
}

void PlayerNegamax(NegaMaxSolver &nms, Connect4Game& game, int player) {
	auto start = high_resolution_clock::now();
	game.Drop(nms.SolveMove(game, player), player);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Nodes explored: " <<  nms.nodesExplored<<"\nTime taken: "<<duration.count()<<"ms\n\n";
}

int main(int argc, char* argv[])
{
	Connect4Game game;
	NegaMaxSolver nms;

	int player, startingPlayer = 1;
	
	//Ai Difficulty
	if (argc > 1) {
		nms = NegaMaxSolver(stoi(argv[1]));
	}
	else {
		nms = NegaMaxSolver();
	}

	cout << "Current Version: 1.0\n";
	cout << "Ai difficulty: " << nms.GetSearchDepth() << " Moves ahead\n\n";

	//gameplay loop
StartGame: 
	player = startingPlayer;
	while (true)
	{
		std::cout << "Turn: " << game.GetMoves()+1 << "\n";
		game.PrintBoard();

		if (player == 1) {
			PlayerHuman(game, player);
		}
		else {
			PlayerNegamax(nms, game, player);
		}

		//check if there is a winner
		if (game.CheckWin() != 0) {

			cout << "Player " << player << " has won!\n";
			game.PrintBoard();
			break;
		}//Draw
		else if (game.GetMoves() == (Connect4Game::WIDTH * Connect4Game::HEIGHT)){
			cout << "The game is a draw!\n";
			game.PrintBoard();
			break;
		}

		//Player switch
		player = (player == 1 ? 2 : 1);
	}

	//Play again?
	cout << "Play again? y/n\n";
	string in;
	cin >> in;
	if (in[0] == 'y' || in[0] == 'Y') {
		game.ResetGame();
		startingPlayer = (startingPlayer == 1 ? 2 : 1);
		goto StartGame;
	}


	cout << "Press enter to quit\n";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();

	return 0;
}