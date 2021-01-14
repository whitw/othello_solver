#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
using namespace std;

enum piece {
    EMPTY,
    BLACK,
    WHITE,
    OBSTACLE,
    INVALID
};

bool blackLeftUp = true;
bool blackFirst = true;
const int szboard = 8;

void init(piece** board, bool blackLeftUp = true, int sz = szboard) {
    if (sz % 2 == 1) {
        string exceptStr = "Size of the board is not even.(size=" + to_string(sz) + ")";
        throw domain_error(exceptStr);
    }
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            board[i][j] = EMPTY;
        }
    }
    board[sz / 2 - 1][sz / 2 - 1] = blackLeftUp ? BLACK : WHITE;
    board[sz / 2    ][sz / 2    ] = blackLeftUp ? BLACK : WHITE;
    board[sz / 2 - 1][sz / 2    ] = blackLeftUp ? WHITE : BLACK;
    board[sz / 2    ][sz / 2 - 1] = blackLeftUp ? WHITE : BLACK;
}

void drawBoard(piece** board, int sz = szboard) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            switch (board[i][j]) {
            case EMPTY:
                printf("¡à");
                break;
            case BLACK:
                printf("¡Û");
                break;
            case WHITE:
                printf("¡Ü");
                break;
            case OBSTACLE:
                printf("¡Ø");
                break;
            default:
                printf("  ");
                break;

            }
        }
        printf("\n");
    }
}
template <typename T, typename U>
pair<T, U> operator+(const pair<T, U>& a, const pair<T, U>& b) {
    return {a.first + b.first, a.second + b.second};
}

piece boardAt(piece** board, int x, int y, int sz=szboard) {
    if (x < 0 || y < 0 || x >= szboard || y >= szboard) {
        return INVALID;
    }
    return board[x][y];
 }

piece boardAt(piece** board, pair<int, int> loc, int sz = szboard) {
    return boardAt(board, loc.first, loc.second, sz);
}

void placeAt(piece** board, int x, int y, piece pc, int sz = szboard) {
    if (x < 0 || y < 0 || x >= szboard || y >= szboard) {
        return;
    }
    board[x][y] = pc;
}

vector<pair<int, int>> calcPlaceable(piece** board, bool blackTurn, int sz=szboard) {
    vector<pair<int, int>> dirList = { {1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1} };
    vector<pair<int, int>> result;
    for (int i = 0; i < sz;i++) {
        for (int j = 0; j < sz; j++) {

            //check for board[i][j]
            if (board[i][j] == EMPTY) {
                for (auto dir:dirList) {
                    pair<int, int> currPos = make_pair(i,j) + dir;
                    //black turn:pattern W^nB
                    //white turn:pattern B^nW
                    piece mine = blackTurn ? BLACK : WHITE;
                    piece your = blackTurn ? WHITE : BLACK;
                    if (boardAt(board, currPos) == your) {
                        do { currPos = currPos + dir; } while (boardAt(board, currPos) == your);
                        if (boardAt(board, currPos) == mine) {
                            result.push_back(make_pair(i,j));
                            //board[i][j] is definitely placable; no need to calculate other direction anymore.
                            break;
                        }
                    }
                }
            }

        }
    }
    return result;
}

int evaluate(piece** board, int row, int col, int turn, int sz=szboard) {
    return 0;
}

int main() {
    piece** board;
    board = new piece*[szboard];
    int a, b;
    for (int i = 0; i < szboard; i++) {
        board[i] = new piece[szboard];
    }
    init(board);
    bool blackTurn = blackFirst;
    vector<pair<int, int>> placeAble;
    while(true){
        drawBoard(board);
        cout << "Turn:" << (blackTurn ? "BLACK" : "WHITE") << endl;
        cout << "placable:";
        placeAble = calcPlaceable(board, blackTurn);
        for (auto i:placeAble) {
            cout << "(" << i.first << "," << i.second << ") ";
        }
        cout << endl << "place at(input two 0-based integer):";
        cin >> a >> b;
        placeAt(board, a, b, (blackTurn ? BLACK: WHITE));
        blackTurn = !blackTurn;
    }
    for (int i = 0; i < szboard; i++) {
        delete[] board[i];
    }
    delete[] board;
    return 0;
}