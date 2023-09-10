#include "Tetris.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include "Block.h"
#include <mmsystem.h>
#include <SFML/Audio.hpp>
#pragma comment(lib, "winmm.lib")

#define MAX_LEVEL 5
#define RECORDER_FILE "highscore_recorder.txt"

const int SPEED_NORMAL[MAX_LEVEL] = { 500, 300, 150, 100, 80 };                 // ms
const int SPEED_FAST = 30;

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize) {
    this->rows = rows;
    this->cols = cols;
    this->leftMargin = left;
    this->topMargin = top;
    this->blockSize = blockSize;

    for (int i = 0; i < rows; i++) {
        vector<int> mapRow;
        for (int j = 0; j < cols; j++) {
            mapRow.push_back(0);
        }
        map.push_back(mapRow);              // add the row into our map
    }
}

void Tetris::init() {

    mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
    delay = SPEED_NORMAL[0];

    srand(time(NULL));                      // generate a random seed

    initgraph(557, 765);                    // create game page

    loadimage(&imgBackground, "res/background.png", 557, 765, true);    // load background image

    loadimage(&imgWin, "res/win.png");
    loadimage(&imgOver, "res/over.png");

    char data[20][10]{};
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            map[i][j] = 0;
        }
    }

    score = 0;
    lineCount = 0;
    level = 1;

    ifstream file(RECORDER_FILE);
    if (!file.is_open()) {
        cout << RECORDER_FILE << "Failed to open" << endl;
        highScore = 0;
    } else {
        file >> highScore;
    }
    file.close();

    gameOver = false;
}

void Tetris::play() {
    OutputDebugString(_T("My output string."));
    init();

    nextBlock = new Block;
    curBlock = nextBlock;
    nextBlock = new Block;                      // create the next preview of the next block

    
    int timer = 0;
    while (true) {
        keyEvent();

        timer += getDelay();

        if (timer > delay) {
            timer = 0;
            drop();
            update = true;
        }

        if (update) {
            update = false;
            updateWindow();                     // update game page
        }

        clearLine();                            // update game data
        checkOver();

        if (gameOver) {
            saveScore();                            // save current score
            displayOver();                          // update game page

            system("pause");
            init();                                 // restart game
        }
    }
}
void Tetris::keyEvent() {
    unsigned char ch;                           // 0..255
    bool rotateFlag = false;
    int dx = 0;

    if (_kbhit()) {
        ch = _getch();
        // if arrow keys are pressed, automatically return two characters
        // if up arrow pressed, return to 224, 72, respectively
        // if down arrow pressed, return to 224, 80, respectively
        // if left arrow pressed, return to 224, 75, respectively
        // if right arrow pressed, return to 224, 77, respectively
        if (ch == 224) {
            ch = _getch();

            switch (ch) {
            case 72:
                rotateFlag = true;
                break;
            case 80:
                delay = SPEED_FAST;
                break;
            case 75:
                dx = -1;
                break;
            case 77:
                dx = 1;
                break;
            default:
                break;
            }
        }
    }
    
    if (rotateFlag) {
        rotate();
        update = true;
        
    }

    if (dx != 0) {                              // move left or right
        moveLeftRight(dx);
        update = true;                          // update immediately after moving left or right
    }
}

void Tetris::updateWindow() {
    putimage(0, 0, &imgBackground);             // draw background image

    Block blockInstance;
    IMAGE** imgs = blockInstance.getImages();
    BeginBatchDraw();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == 0) 
                continue;
            

            int x = j * blockSize + leftMargin;
            int y = i * blockSize + topMargin;
            putimage(x, y, imgs[map[i][j] - 1]);
        }
    }

    curBlock->draw(leftMargin, topMargin);
    nextBlock->draw(45, 107);                        // position of nextblock 

    drawScore();

    EndBatchDraw();
}

int Tetris::getDelay() {

    static unsigned long long lastTime = 0;

    unsigned long long currentTime = GetTickCount();

    if (lastTime == 0) {
        lastTime = currentTime;
        return 0;
    }
    else {
        int ret = currentTime - lastTime;
        lastTime = currentTime;
        return ret;
    }

    return 0;
}

void Tetris::drop() {
    backBlock = *curBlock;
    curBlock->drop();

    if (!curBlock->blockInMap(map)) {                       // this indicates the block has hit the bottom
        backBlock.solidify(map);
        delete curBlock;
        curBlock = nextBlock;
        nextBlock = new Block;
    }

    delay = SPEED_NORMAL[level - 1];                                   
}

void Tetris::clearLine() {
    int lines = 0;
    int k = rows - 1;                                       // k represents the pointer to store info
    for (int i = rows - 1; i >= 0; i--) {                   // check if the ith row is full
        int count = 0;
        for (int j = 0; j < cols; j++) {
            if (map[i][j]) {
                count++;
            }
            map[k][j] = map[i][j];                          // scan and store
        }

        if (count < cols) {                                 // if count != cols, the row isnt full. Store the info
            k--;
        } else {                                            // if the row is full, check the next line
            lines++;    
        }
    }

    if (lines > 0) {                                        // indicates lines has been cleared. Calculate points
        int addScore[4] = { 10, 30, 60, 80 };                  // more points are gained for clearing more lines
        score += addScore[lines - 1];

        mciSendString("stop res/bg.mp3", 0, 0, 0);
        mciSendString("play res /clearline.wav", 0, 0, 0);
        mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
        update = true;

        // every 100 points is one level. For example, level 1 is 0-100
        this->level = (score + 99) / 100;
        if (level > MAX_LEVEL) {
            gameOver = true;
        }
        lineCount += lines;

    }
}

void Tetris::moveLeftRight(int offset) {
    backBlock = *curBlock;                                  // create backup copy first
    curBlock->moveLeftRight(offset);

    if (!curBlock->blockInMap(map)) {
        *curBlock = backBlock;
    }
}

void Tetris::rotate() {
    if (curBlock->getBlockType() == 7)                     // if we have the square blocktype, no need to rotate
        return;
    backBlock = *curBlock;
    curBlock->rotate();

    if (!curBlock->blockInMap(map)) {
        *curBlock = backBlock;
    }

}

void Tetris::drawScore() {
    char scoreText[32];
    sprintf_s(scoreText, sizeof(scoreText), "%d", score);

    setcolor(RGB(219,154,160));

    LOGFONT f;
    gettextstyle(&f);                                     // obtain current font
    f.lfHeight = 26;
    f.lfWidth = 13;
    f.lfQuality = ANTIALIASED_QUALITY;
    strcpy(f.lfFaceName, _T("Arial Bold"));
    settextstyle(&f);

    setbkmode(TRANSPARENT);                               // set font background as transparent

    outtextxy(450, 580, scoreText);

    sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);       // number of lines cleared
    gettextstyle(&f);
    //int xPos = 224 - f.lfWidth * strlen(scoreText);
    outtextxy(90, 657, scoreText);

    sprintf_s(scoreText, sizeof(scoreText), "%d", level);           // current level
    outtextxy(90, 580, scoreText);

    sprintf_s(scoreText, sizeof(scoreText), "%d", highScore);       // current highscore
    outtextxy(450, 657, scoreText);

}

void Tetris::checkOver() {
    gameOver = (curBlock->blockInMap(map) == false);
}

void Tetris::saveScore() {
    if (score > highScore) {
        highScore = score;

        ofstream file(RECORDER_FILE);
        file << highScore;
        file.close();
    }
}

void Tetris::displayOver() {
    updateWindow();
    mciSendString("stop res/bg.mp3", 0, 0, 0);
    if (level <= MAX_LEVEL) {
        drawStatus(false);
        mciSendString("play res/over.wav", 0, 0, 0);
    } else {
        drawStatus(true);
        mciSendString("play res/win.wav", 0, 0, 0);
    }
}

void Tetris::drawStatus(bool win) {
    LPCTSTR text = "YOU WIN!";
    if (!win) {
        text = "GAME OVER";
    }
    setfillcolor(BLACK);
    solidroundrect(190,330, 370, 380, 10, 10);
    settextcolor(RGB(247, 227, 173));
    settextstyle(40, 15, "Arial Bold", 0, 0, 900, 0, 0, 0);
    RECT r = { 190, 337, 370, 387 };
    drawtext(text, &r, DT_CENTER);
}
