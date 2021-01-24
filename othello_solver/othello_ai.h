#pragma once
#include <vector>
#include "othello_env.h"

namespace othello_ai {
	std::pair<int, int> selectRandom(Env& env);
	std::pair<int, int> maxBeneNow(Env& env);
	std::pair<int, int> minLossNextTurn(Env& env);
	//std::pair<int, int> minLoss4thTurn(Env& env);
	//std::pair<int, int> minLossNextTurnBiased(Env& env);
	//std::pair<int, int> minLoss4thTurnBiased(Env& env);
}