#pragma once
#include <vector>
#include "othello_env.h"
using std::vector;

namespace othello_ai {
	std::pair<int, int> selectRandom(Env& env,
		vector<vector<float>> bias=vector<vector<float>>());
	std::pair<int, int> maxBeneNow(Env& env,
		vector<vector<float>> bias = vector<vector<float>>());
	std::pair<int, int> minLossNextTurn(Env& env,
		vector<vector<float>> bias = vector<vector<float>>());
	std::pair<int, int> minLoss4thTurn(Env& env,
		vector<vector<float>> bias = vector<vector<float>>());
}