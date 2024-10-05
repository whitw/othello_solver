#include <vector>
#include <random>
#include "othello_ai.h"

float getBiasedBlackScore(Env& env, vector<vector<float>>& bias) {
	if (bias == vector<vector<float>>()) {
		return env.getBlackScore();
	}

	float total = 0.0f;
	float black = 0.0f;
	for (int i = 0; i < env.getSize(); i++) {
		for (int j = 0; j < env.getSize(); j++) {
			if (env.get(i, j) == piece::BLACK) {
				total += bias[i][j];
				black += bias[i][j];
			}
			else if (env.get(i, j) == piece::WHITE) {
				total += bias[i][j];
			}
		}
	}
	return black / total;
}

float getBiasedWhiteScore(Env& env, vector<vector<float>>& bias) {
	return 1.0f - getBiasedBlackScore(env, bias);
}

float getBiasedMyScore(Env& env, vector<vector<float>>& bias) {
	if (env.isBlackTurn()) {
		return getBiasedBlackScore(env, bias);
	}
	else return getBiasedWhiteScore(env, bias);
}
float getBiasedOtherScore(Env& env, vector<vector<float>>& bias) {
	return 1.0f - getBiasedMyScore(env, bias);
}

std::pair<int, int> othello_ai::selectRandom(Env& env, vector<vector<float>> bias)
{
	std::vector<std::pair<int, int>> placeAble = env.calcPlaceable();
	std::random_device rnd;
	std::mt19937 gen(rnd());
	std::uniform_int_distribution<int> dist(0, placeAble.size() - 1);
	int which = dist(gen);
	return placeAble[which];
}
std::pair<int, int> othello_ai::maxBeneNow(Env& env, vector<vector<float>> bias)
{
	std::vector<std::pair<int, int>> placeAble = env.calcPlaceable();
	float maxScore = -10000.0f;
	std::pair<int, int> maxPlace;
	for (int i = 0; i < placeAble.size(); i++) {
		Env* nenv = env.new_copy();
		nenv->placeAt(placeAble[i]);
		if (maxScore < getBiasedOtherScore(*nenv, bias)) {
			maxPlace = placeAble[i];
			maxScore = getBiasedOtherScore(*nenv, bias);
		}
		delete nenv;
	}
	return maxPlace;
}

std::pair<int, int> othello_ai::minLossNextTurn(Env& env, vector < vector < float >> bias)
{
	bool blackTurn = env.isBlackTurn();
	std::vector<std::pair<int, int>> placeAble = env.calcPlaceable();
	float best = -10000.0f;
	std::pair<int, int> maxPlace;
	for (auto i : placeAble){
		Env* nenv = env.new_copy();
		nenv->placeAt(i);
		nenv->placeAt(maxBeneNow(*nenv));//use bias or not?
		float localMinScore = getBiasedMyScore(*nenv, bias);
		if (best < localMinScore) {
			maxPlace = i;
			best = localMinScore;
		}
		delete nenv;
	}
	return maxPlace;
}

std::pair<int, int> othello_ai::minLoss4thTurn(Env& env, vector<vector<float>> bias)
{
	return std::pair<int, int>();
}
