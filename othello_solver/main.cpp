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
bool blackFirst = false;
int cntObstacle = 1;

bool customObstacle = false;
bool blackUserInput = false;
bool whiteUserInput = false;
//selectRandom, maxBeneNow, minLossNextTurn
pair<int, int> (*blackPlaceRule)(Env& env) = othello_ai::selectRandom;
pair<int, int>(*whitePlaceRule)(Env& env) = othello_ai::selectRandom;

bool drawOnCMD = false; //draw help.
bool drawResult = false; //draw the result of the game. once per game.
int repeatCnt = 10000;
bool drawRuntime = false; //draw every step. once per turn.
bool repeatWithReversedOrder = false;
int blackWinCnt = 0;
int whiteWinCnt = 0;
bool drawBlackWinRate = true;

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

    vector<pair<int, int>> placeAble;
    while (true) {
        //or env.drawPlaceable();
        placeAble = env.calcPlaceable();
        if (drawOnCMD) {
            env.draw();
            float blackScore = env.getBlackScore();
            cout << "Portion:" << blackScore << " : " << 1 - blackScore << endl;
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
                    toPlace = blackPlaceRule(env);
                }
                else {
                    toPlace = whitePlaceRule(env);
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
    if (repeatWithReversedOrder) {
        for (int i = 0; i < repeatCnt; i++) {
            play(szboard, blackLeftUp, !blackFirst, customObstacle, cntObstacle, blackUserInput, whiteUserInput);
        }
    }
    time_t end = clock();
    if (drawRuntime)
        cout << "ellapsed " << (1.0f) * (end - begin) / CLOCKS_PER_SEC << "sec" << endl;
    if (drawBlackWinRate) {
        if (repeatWithReversedOrder) repeatCnt *= 2;
        cout << "blackWin:" << (1.0f) * blackWinCnt / repeatCnt * 100 << "%" << endl;
        cout << "whiteWin:" << (1.0f) * whiteWinCnt / repeatCnt * 100 << "%" << endl;
        cout << "draw:" << (1.0f) * (repeatCnt - whiteWinCnt - blackWinCnt) / repeatCnt * 100 << "%" << endl;
    }
    return 0;
}