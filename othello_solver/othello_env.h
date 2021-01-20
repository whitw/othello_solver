#pragma once
#include <vector>

enum class piece : char {
    EMPTY,
    BLACK,
    WHITE,
    OBSTACLE,
    INVALID
};


class Env {
public:
    Env(bool blackLeftUp = true, bool blackFirst = true, int szboard = 8);
    ~Env();
    inline bool isBlackLeftUp() { return blackLeftUp; }
    inline bool isBlackFirst() { return blackFirst; }
    inline bool isBlackTurn() { return blackTurn; }
    inline bool isWhiteTurn() { return !blackTurn; }
    inline void setBlackTurn(bool blackTurn) { blackTurn = blackTurn; }
    inline void reverseTurn() { blackTurn = !blackTurn; }
    //return the size of board. The board will always be square, and the size of each side are always even.
    inline int getSize() { return szboard; }

    void init();
    void draw();
    void drawPlaceable();

    //return board[x][y]
    piece get(int x, int y);
    //return board[x][y]
    inline piece get(std::pair<int, int> x) { return get(x.first, x.second); }

    //board[x][y] = pc
    void set(int x, int y, piece pc);
    //board[x][y] = pc
    inline void set(std::pair<int, int> x, piece pc) { set(x.first, x.second, pc); }
    
    //board[x][y] = BLACK if black turn, WHITE else.
    void placeAt(int x, int y);
    //board[x][y] = BLACK if black turn, WHITE else.
    inline void placeAt(std::pair<int, int>x) { placeAt(x.first, x.second); }

    //true if you can place piece pc at board[x][y]
    bool isPlaceable(int x, int y, piece pc);
    //true if you can place piece pc at board[x][y]
    inline bool isPlaceable(std::pair<int, int> x, piece pc) { return isPlaceable(x.first, x.second, pc); }
    
    /*return all place that one can place this turn.
    call isBlackTurn() to check whose turn now is,
    call setBlackTurn(bool) to change the turn if you wish.*/
    std::vector<std::pair<int, int>> calcPlaceable();

    /*copy environment with deep copy-ing board[][]
    and return the pointer to new dynamically allocated Env*/
    Env* copy();

    //return num(BlackPiece)/(num(BlackPiece) + num(WhitePiece))
    float getBlackScore();
    inline float getWhiteScore() { return 1 - getBlackScore(); }

private:
    //will be used to implement the reversing method of othello game.
    const std::vector<std::pair<int, int>> dirList = { {1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1} };
    /**
    thinkabout) std::set<std::pair<int, int>> emptyList;
    
    stores all the empty points
    pros: fast calcPlaceable()(~~ 2x). calcPlaceable are called frequently.
    cons: slow init(), need managements, poor storage usage
    */
    //
    /*
    true if the initial state looks like:(black, white)(white, black)
    false if the initial state looks like:(white, black)(black, white)
    */
    bool blackLeftUp;
    //true if black play first
    bool blackFirst;
    //true if black can play now
    bool blackTurn;
    //size of the board==(szboard * szboard)
    int szboard;
    //dynamically allocated board: board[szboard][szboard]
    piece** board;
    //num of black pieces
    int numBlack;
    //num of white pieces
    int numWhite;
};
