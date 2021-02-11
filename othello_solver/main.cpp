#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <vector>
#include <random>
#include "othello_env.h"
#include "othello_ai.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

int szboard = 8;
bool blackLeftUp = true;
bool blackFirst = true;
int cntObstacle = 5;

bool customObstacle = false;
bool blackUserInput = false;
bool whiteUserInput = false;
//selectRandom, maxBeneNow, minLossNextTurn
pair<int, int> (*blackPlaceRule)(Env& env, vector<vector<float>> bias) = othello_ai::selectRandom;
pair<int, int>(*whitePlaceRule)(Env& env, vector<vector<float>> bias) = othello_ai::minLossNextTurn;
vector<vector<float>> bias;
bool drawOnCMD = false; //draw every step.
bool drawResult = false; //draw the result of the game. once per game.
int repeatCnt = 500;
bool drawRuntime = true; //draw the ellapsed time.
bool repeatWithReversedOrder = true;
int blackWinCnt = 0;
int whiteWinCnt = 0;
bool drawBlackWinRate = true;

float getBiasRate(int atk, int def) {
    return atk * 1.f / (def * 5.f + .01f);
}

/*
calculateBias()
uses locations of piece::OBSTACLE to calculate
the preference of each place at board.
if the preference is high, we may say that taking the position
may lead to higher win rate.
*/
vector<vector<float>> calculateBias(const Env& env) {
    vector<vector<float>> bias;
    bias.resize(env.getSize());
    /*
        localAtkDir
            number of direction that can be used to attack
            the higher the better
        localDefDir
            number of directions that can be attacked
            the lower the better
        attackable
            true if the next two points are
            not obstacle, nor out of board.
        could be attacked
            true if some direction and the opposite directions are
            both not obstacle nor out of board.
        preference
            (attackable)/(could be attacked + 1). quite arbitrary.
    */
    int localAtkDir = 0, localDefDir = 0;
    for (int i = 0; i < env.getSize(); i++) {
        bias[i] = vector<float>();
        bias[i].resize(env.getSize());
        for (int j = 0; j < env.getSize(); j++) {
            localAtkDir = localDefDir = 0;
            if (env.get(i, j) == piece::OBSTACLE) {
                bias[i][j] = 0;
                continue;
            }
            int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
            for (int k = 0; k < 4;k++) {
                if (env.get(i+dx[k], j+dy[k]) != piece::OBSTACLE && env.get(i+dx[k], j+dy[k]) != piece::INVALID) {
                    if (env.get(i - dx[k], j - dy[k]) != piece::OBSTACLE && env.get(i + dx[k], j + dy[k]) != piece::INVALID)
                        localDefDir++;
                    if (env.get(i + 2 * dx[k], j + 2 * dy[k]) != piece::OBSTACLE && env.get(i + dx[k], j + 2 * dy[k]) != piece::INVALID)
                        localAtkDir++;
                }
            }
            for(int k = 4; k < 8;k++)
                if (env.get(i + dx[k], j + dy[k]) != piece::OBSTACLE && env.get(i + dx[k], j + dy[k]) != piece::INVALID) {
                    if (env.get(i + 2 * dx[k], j + 2 * dy[k]) != piece::OBSTACLE && env.get(i + dx[k], j + 2 * dy[k]) != piece::INVALID)
                        localAtkDir++;
                }
            bias[i][j] = getBiasRate(localAtkDir, localDefDir);
        }
    }
    return bias;
}

void placeObstacle(Env& env, int count)
{
    int a, b;
    if (count > env.getSize() * env.getSize() - 4) {
        for (int i = 0; i < env.getSize(); i++) {
            for (int j = 0; j < env.getSize(); j++) {
                env.set(i, j, piece::OBSTACLE);
            }
        }
        return;
    }
    if (customObstacle) {
        for (int i = 0; i < count;) {
            env.draw();
            if(drawOnCMD)
                cout << "select the position to place obstacles(" << i + 1 << "/" << count << "):";
            while (true) {
                cin >> a >> b;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(256, '\n');
                    continue;
                }
                if (!env.isPlaceable(a, b, piece::OBSTACLE)) {
                    if (drawOnCMD)
                        cout << "You can't place it there." << endl;
                    break;
                }
                else {
                    i++;
                    break;
                }
            }
            env.set(a, b, piece::OBSTACLE);
        }
    }
    else {
        std::random_device rnd;
        std::mt19937 gen(rnd());
        std::uniform_int_distribution<int> dist(0, env.getSize() - 1);
        for (int i = 0; i < count;) {
            a = dist(gen);
            b = dist(gen);
            while (true) {
                if (!env.isPlaceable(a, b, piece::OBSTACLE)) {
                    break;
                }
                else {
                    i++;
                    break;
                }
            }
            env.set(a, b, piece::OBSTACLE);
        }
    }
}

void play(int _szboard, bool _blackLeftUp, bool _blackFirst,
          bool _customObstacle, int _cntObstacle, bool _customBlack, bool _customWhite)
{
    int a, b;
    Env env = Env(_blackLeftUp, _blackFirst, _szboard);
    placeObstacle(env, _cntObstacle);
    bias = calculateBias(env);
    vector<pair<int, int>> placeAble;
    while (true) {
        //or env.drawPlaceable();
        placeAble = env.calcPlaceable();
        if (drawOnCMD) {
            env.draw();
            float blackScore = env.getBlackScore();
            cout << "Portion:" << blackScore << " : " << 1 - blackScore << endl;
            cout << "numStones:" << env.getCountsPiece().first << ":" << env.getCountsPiece().second << endl;
            cout << "Turn:" << (env.isBlackTurn() ? "BLACK" : "WHITE") << endl;
            cout << "placable:";
            for (auto i : placeAble) {
                cout << "(" << i.first << "," << i.second << ") ";
            }
            cout << endl;
        }
        
        if (placeAble.size() == 0) {

            if (drawOnCMD) cout << "nowhere to place!" << endl;
            env.reverseTurn();
            placeAble = env.calcPlaceable();
            if (placeAble.size() == 0) {
                if (drawOnCMD) cout << "Game ended!" << endl;
                break;
            }
            continue;
        }
        while (true) {
            if ((env.isBlackTurn() && _customBlack) || (env.isWhiteTurn() && _customWhite)) {
                if (drawOnCMD)
                    cout << endl << "place at(input two 0-based integer):" << endl;
                cin >> a >> b;
            }
            else {
                pair<int, int> toPlace;
                if (env.isBlackTurn()) {
                    toPlace = blackPlaceRule(env, bias);
                }
                else {
                    toPlace = whitePlaceRule(env, bias);
                }
                a = toPlace.first;
                b = toPlace.second;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(256, '\n');
                continue;
            }
            //if (a,b) not in placeAble continue;
            bool _valid = false;
            for (auto i : placeAble) {
                if (std::make_pair(a, b) == i) {
                    _valid = true;
                    break;
                }
            }
            if (_valid)
                break;
        }
        env.placeAt(a, b);
    }
    float blackScore = env.getBlackScore();
    if (drawResult) cout << "final score:" << blackScore << ":" << 1 - blackScore << endl;
    if (blackScore > 0.5001) {
        blackWinCnt++;
        if (drawResult) cout << "Black Win!" << endl;
    }
    else if (blackScore < 0.4999) {
        whiteWinCnt++;
        if (drawResult) cout << "White Win!" << endl;
    }
    else {
        if (drawResult) cout << "draw!" << endl;
    }
    if (drawResult) env.draw();
}

int main()
{
    time_t begin = clock();
    for (int i = 0; i < repeatCnt; i++) {
        play(szboard, blackLeftUp, blackFirst, customObstacle, cntObstacle, blackUserInput, whiteUserInput);
    }
    time_t end = clock();
    if (drawRuntime)
        cout << "ellapsed " << (1.0f) * (end - begin) / CLOCKS_PER_SEC << "sec" << endl;
    if (drawBlackWinRate) {
        cout << (blackFirst ? "BLACK" : "WHITE") << " First" << endl;
        cout << "blackWin:" << (1.0f) * blackWinCnt / repeatCnt * 100 << "%" << endl;
        cout << "whiteWin:" << (1.0f) * whiteWinCnt / repeatCnt * 100 << "%" << endl;
        cout << "draw:" << (1.0f) * (repeatCnt - whiteWinCnt - blackWinCnt) / repeatCnt * 100 << "%" << endl;
    }
    if (repeatWithReversedOrder) {
        begin = clock();
        blackWinCnt = 0, whiteWinCnt = 0;
        for (int i = 0; i < repeatCnt; i++) {
            play(szboard, blackLeftUp, !blackFirst, customObstacle, cntObstacle, blackUserInput, whiteUserInput);
        }
        end = clock();
        if (drawRuntime)
            cout << "ellapsed " << (1.0f) * (end - begin) / CLOCKS_PER_SEC << "sec" << endl;
        if (drawBlackWinRate) {
            cout << (!blackFirst ? "BLACK" : "WHITE") << " First" << endl;
            cout << "blackWin:" << (1.0f) * blackWinCnt / repeatCnt * 100 << "%" << endl;
            cout << "whiteWin:" << (1.0f) * whiteWinCnt / repeatCnt * 100 << "%" << endl;
            cout << "draw:" << (1.0f) * (repeatCnt - whiteWinCnt - blackWinCnt) / repeatCnt * 100 << "%" << endl;
        }
    }
    return 0;
}