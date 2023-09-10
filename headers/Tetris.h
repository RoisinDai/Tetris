#pragma once
#include <graphics.h>
#include <vector>
#include "Block.h"
using namespace std;

class Tetris {
public:
    Tetris(int rows, int cols, int left, int top, int blockSize);
    void init();
    void play();

private:
    void keyEvent();
    void updateWindow();

    int getDelay();                  // retrieve time taken since last time function was called
    void drop();
    void clearLine();
    void moveLeftRight(int offset);
    void rotate(); 
    void drawScore();
    void checkOver();                // check if the game is over
    void saveScore();                // save current score
    void displayOver();              // draw game over page
    void drawStatus(bool win);

private:
    int delay;
    bool update;                    // check if updated

    //int map[20][10];
    // 0: no block
    // 5: type 5 block
    vector<vector<int>> map;
    int rows;
    int cols;
    int leftMargin;
    int topMargin;
    int blockSize;
    IMAGE imgBackground;

    Block* curBlock;
    Block* nextBlock;               // preview of the next block
    Block backBlock;                // while current block falls, used to backup the last appropriate block location

    int score;                      // current score
    int highScore;                  // current highscore 
    int level;                      // current level
    int lineCount;                  // current number of lines cleared
    bool gameOver;                  

    IMAGE imgOver;
    IMAGE imgWin;
};



