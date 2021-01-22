#include <vector>
#include <random>
#include "othello_ai.h"

std::pair<int, int> othello_ai::selectRandom(Env& env)
{
	std::vector<std::pair<int, int>> placeAble = env.calcPlaceable();
	std::random_device rnd;
	std::mt19937 gen(rnd());
	std::uniform_int_distribution<int> dist(0, placeAble.size() - 1);
	int which = dist(gen);
	return placeAble[which];
}
std::pair<int, int> othello_ai::maxBeneNow(Env& env)
{
	bool blackTurn = env.isBlackTurn();
	std::vector<std::pair<int, int>> placeAble = env.calcPlaceable();
	float maxScore = -10000.0f;
	std::pair<int, int> maxPlace;
	for (int i = 0; i < placeAble.size(); i++) {
		Env* nenv = env.new_copy();
		nenv->placeAt(placeAble[i]);
		if (maxScore < nenv->getBlackScore()) {
			maxPlace = placeAble[i];
			maxScore = nenv->getBlackScore();
		}
		delete nenv;
	}
	return maxPlace;
}
std::pair<int, int> othello_ai::minLossNextTurn(Env& env)
{
	return othello_ai::selectRandom(env);
}