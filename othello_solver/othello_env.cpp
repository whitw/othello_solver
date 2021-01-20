#include "othello_env.h"
#include <string>
#include <vector>
#include <iostream>

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U>& a, const std::pair<T, U>& b) {
    return { a.first + b.first, a.second + b.second };
}


Env::Env(bool blackLeftUp, bool blackFirst, int szboard) :
    blackLeftUp(blackLeftUp), blackFirst(blackFirst), szboard(szboard), blackTurn(blackFirst), numBlack(0),numWhite(0)
{
    board = new piece* [szboard];
    for (int i = 0; i < szboard; i++) {
        board[i] = new piece[szboard];
    }
    init();
}

Env::~Env()
{
    for (int i = 0; i < szboard; i++) {
        delete[] board[i];
    }
    delete[] board;
}

void Env::init()
{
    if (szboard % 2 == 1) {
        std::string exceptStr = "Size of the board is not even.(size=" + std::to_string(szboard) + ")";
        throw std::domain_error(exceptStr);
    }
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            board[i][j] = piece::EMPTY;
        }
    }
    piece leftUpPc = blackLeftUp ? piece::BLACK : piece::WHITE;
    piece rightUpPc = blackLeftUp ? piece::WHITE : piece::BLACK;
     
    set(szboard / 2 - 1, szboard / 2 - 1,leftUpPc);
    set(szboard / 2    , szboard / 2    , leftUpPc);
    set(szboard / 2 - 1, szboard / 2    , rightUpPc);
    set(szboard / 2    , szboard / 2 - 1, rightUpPc);
}


piece Env::get(int x, int y)
{
    if (x < 0 || y < 0 || x >= szboard || y >= szboard) {
        return piece::INVALID;
    }
    return board[x][y];
}

void Env::set(int x, int y, piece pc)
{
    if (x < 0 || y < 0 || x >= szboard || y >= szboard || board[x][y] != piece::EMPTY) {
        return;
    }

    //place it anyway.
    board[x][y] = pc;
    if (pc == piece::BLACK) numBlack++;
    else if (pc == piece::WHITE) numWhite++;

    //if the piece was BLACK or WHITE piece, flip pieces by the rule of the othello.
    if (pc == piece::BLACK || pc == piece::WHITE) {

        std::vector<std::pair<int, int>> toFlip;
        piece mine = pc;
        piece yours = (pc == piece::BLACK ? piece::WHITE : piece::BLACK);

        for (auto dir : dirList) {
            toFlip.clear();
            std::pair<int, int> currPos = std::make_pair(x, y) + dir;
            while (get(currPos) == yours) {
                toFlip.push_back(currPos);
                currPos = currPos + dir;
            }
            if (get(currPos) == mine) {
                if (pc == piece::BLACK) {
                    numBlack += (int)toFlip.size();
                    numWhite -= (int)toFlip.size();
                }
                else {
                    numBlack -= (int)toFlip.size();
                    numWhite += (int)toFlip.size();
                }
                for (auto flips : toFlip) {
                    board[flips.first][flips.second] = mine;
                }
            }
        }
        blackTurn = !blackTurn;
    }
}

void Env::placeAt(int x, int y)
{
    piece mine = isBlackTurn() ? piece::BLACK : piece::WHITE;
    set(x, y, mine);
}

bool Env::isPlaceable(int x, int y, piece pc)
{
    if (pc == piece::BLACK || pc == piece::WHITE) {
        if (get(x, y) == piece::EMPTY) {
            for (auto dir : dirList) {
                std::pair<int, int> currPos = std::make_pair(x, y) + dir;
                //black turn:pattern W^nB
                //white turn:pattern B^nW
                piece mine = blackTurn ? piece::BLACK : piece::WHITE;
                piece your = blackTurn ? piece::WHITE : piece::BLACK;
                if (get(currPos) == your) {
                    do { currPos = currPos + dir; } while (get(currPos) == your);
                    if (get(currPos) == mine) {
                        return true;
                    }
                }
            }
        }
    }
    else {
        if (get(x, y) == piece::EMPTY) {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> Env::calcPlaceable()
{
    piece mine = isBlackTurn()?piece::BLACK : piece::WHITE;
    std::vector<std::pair<int, int>> retval;
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            if (isPlaceable(i, j, mine)) {
                retval.push_back(std::make_pair(i, j));
            }
        }
    }
    return retval;
}

Env* Env::copy()
{
    Env* retval = new Env(blackLeftUp, blackFirst, szboard);
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            retval->board[i][j] = board[i][j];
        }
    }
    return retval;
}

float Env::getBlackScore()
{
    if (isBlackTurn()) {
        return numBlack / (float)(numBlack + numWhite);
    }
    else {
        return numWhite / (float)(numBlack + numWhite);
    }
}