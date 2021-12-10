#pragma once
#include "relearn.hpp"
#include "Connect4Game.h"
#include <random>
#include <chrono>

using namespace std;

class RLNN {

	using state = relearn::state<int**>;
	using action = relearn::action<int>;

	static relearn::policy<state, action> policies;

	RLNN() {

	}

	template<typename S, typename A>
	deque<relearn::link<S, A>> explore(const Connect4Game& game, mt19937 &gen) {
		//Explore until reward
		bool stop = false;

		deque<relearn::link<S, A>> episode;

		//Set up root start
		Connect4Game current = game;
		auto state_now = state(current.board, current);

		//Explore while reward isn't positive or negative
		while (!stop) {
			auto result = 
		}
	}

	void Run(Connect4Game game) {
		std::mt19937 gen(static_cast<std::size_t>(
			std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count()));

		for (;;) {
			auto episode = explore<state, action>(game, gen);
		}

	}

};