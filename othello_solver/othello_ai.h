#pragma once
#include <vector>
#include "othello_env.h"

namespace othello_ai {
	std::pair<int, int> selectRandom(Env& env);
	std::pair<int, int> maxBeneNow(Env& env);
	std::pair<int, int> minLossNextTurn(Env& env);
}