#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

enum class piece : char{
    EMPTY,
    BLACK,
    WHITE,
    OBSTACLE,
    INVALID
};

bool blackLeftUp = true;
bool blackFirst = true;
const int szboard = 8;
const vector<pair<int, int>> dirList = { {1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1} };

void init(piece** board, bool blackLeftUp = true, int sz = szboard) {
    if (sz % 2 == 1) {
        string exceptStr = "Size of the board is not even.(size=" + to_string(sz) + ")";
        throw domain_error(exceptStr);
    }
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            board[i][j] = piece::EMPTY;
        }
    }
    board[sz / 2 - 1][sz / 2 - 1] = blackLeftUp ? piece::BLACK : piece::WHITE;
    board[sz / 2    ][sz / 2    ] = blackLeftUp ? piece::BLACK : piece::WHITE;
    board[sz / 2 - 1][sz / 2    ] = blackLeftUp ? piece::WHITE : piece::BLACK;
    board[sz / 2    ][sz / 2 - 1] = blackLeftUp ? piece::WHITE : piece::BLACK;
}

void drawBoard(piece** board, int sz = szboard) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            switch (board[i][j]) {
            case piece::EMPTY:
                printf("¡à");
                break;
            case piece::BLACK:
                printf("¡Û");
                break;
            case piece::WHITE:
                printf("¡Ü");
                break;
            case piece::OBSTACLE:
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

vector<pair<int, int>> calcPlaceable(piece** board, bool blackTurn, int sz);
void drawPlaceAble(piece** board, bool blackTurn, int sz = szboard) {
    vector<pair<int, int>> placeable = calcPlaceable(board, blackTurn, sz);
    vector<pair<int, int>>::iterator iPlaceable = placeable.begin();
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            switch (board[i][j]) {
            case piece::EMPTY:
                if (iPlaceable != placeable.end() && make_pair(i, j) == *iPlaceable) {
                    if (blackTurn) printf("¡Þ");
                    else printf("¡ß");
                    iPlaceable++;
                    break;
                }
                else{
                    printf("¡à");
                    break;
                }

                case piece::BLACK:
                printf("¡Û");
                break;
            case piece::WHITE:
                printf("¡Ü");
                break;
            case piece::OBSTACLE:
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
        return piece::INVALID;
    }
    return board[x][y];
 }

piece boardAt(piece** board, pair<int, int> loc, int sz = szboard) {
    return boardAt(board, loc.first, loc.second, sz);
}

void placeAt(piece** board, int x, int y, piece pc, int sz = szboard) {
    if (x < 0 || y < 0 || x >= szboard || y >= szboard || board[x][y] != piece::EMPTY) {
        return;
    }
    board[x][y] = pc;
}

vector<pair<int, int>> calcPlaceable(piece** board, bool blackTurn, int sz=szboard) {
    vector<pair<int, int>> result;
    for (int i = 0; i < sz;i++) {
        for (int j = 0; j < sz; j++) {

            //check for board[i][j]
            if (board[i][j] == piece::EMPTY) {
                for (auto dir:dirList) {
                    pair<int, int> currPos = make_pair(i,j) + dir;
                    //black turn:pattern W^nB
                    //white turn:pattern B^nW
                    piece mine = blackTurn ? piece::BLACK : piece::WHITE;
                    piece your = blackTurn ? piece::WHITE : piece::BLACK;
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
        drawPlaceAble(board, blackTurn);
        cout << "Turn:" << (blackTurn ? "BLACK" : "WHITE") << endl;
        cout << "placable:";
        placeAble = calcPlaceable(board, blackTurn);
        for (auto i:placeAble) {
            cout << "(" << i.first << "," << i.second << ") ";
        }
        while(true) {
            cout << endl << "place at(input two 0-based integer):";
            cin >> a >> b;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(256, '\n');
                continue;
            }
            //if (a,b) not in placeAble continue;
            bool _valid = false;
            for (auto i : placeAble) {
                if (make_pair(a, b) == i) {
                    _valid = true;
                    break;
                }
            }
            if(_valid)
                break;
        }
        placeAt(board, a, b, (blackTurn ? piece::BLACK : piece::WHITE));
        blackTurn = !blackTurn;
    }
    for (int i = 0; i < szboard; i++) {
        delete[] board[i];
    }
    delete[] board;
    return 0;
}