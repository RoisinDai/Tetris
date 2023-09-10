#include "Block.h"
#include "Tetris.h"
#include <stdlib.h>
#include <vector>
using namespace std;

IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 31;

Block::Block() {

    if (imgs[0] == NULL) {
        IMAGE imgTmp;
        loadimage(&imgTmp, "res/tiles.png", 217, 31, true);

        SetWorkingImage(&imgTmp);
        for (int i = 0; i < 7; i++) {
            imgs[i] = new IMAGE;
            getimage(imgs[i], i * size, 0, size, size);      //image, x coordinate, and y coordinate
        }
        SetWorkingImage();
    }
    int blocks[7][4] = {
        1, 3, 5, 7, // I
        2, 4, 5, 7, // Z first type
        3, 5, 4, 6, // Z second type
        3, 5, 4, 7, // T 
        2, 3, 5, 7, // L
        3, 5, 7, 6, // J
        2, 3, 4, 5, // square
    };

    blockType = 1 + rand() % 7;                 // randomly generate a block type from 1 to 7    

    for (int i = 0; i < 4; i++) {               // retrieve location for each small block within a tetris block
        int value = blocks[blockType - 1][i];
        smallBlocks[i].row = value / 2;
        smallBlocks[i].col = value % 2;
    }

    img = imgs[blockType - 1];
}

void Block::drop() {
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].row++;

  //for (auto &block : smallBlocks) {
  //block.row++;
  //}
    }
}

void Block::moveLeftRight(int offset) {
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].col += offset;
    }
}

void Block::rotate() {                                      
    Point p = smallBlocks[1];                       

    for (int i = 0; i < 4; i++) {
        Point temp = smallBlocks[i];
        smallBlocks[i].col = p.col - temp.row + p.row;
        smallBlocks[i].row = p.row + temp.col - p.col;
    }
}

void Block::draw(int leftMargin, int topMargin) {
    for (int i = 0; i < 4; i++) {
        int x = leftMargin + smallBlocks[i].col * size;
        int y = topMargin + smallBlocks[i].row * size;
        putimage(x, y, img);
    }
}

IMAGE** Block::getImages() {

    return imgs;
}

Block& Block::operator=(const Block& other) {
    if (this == &other)
        return *this;
    this->blockType = other.blockType;

    for (int i = 0; i < 4; i++) {
        this->smallBlocks[i] = other.smallBlocks[i];
    }
    return *this;
}

bool Block::blockInMap(const vector<vector<int>>& map) {            // check if the block is within the map

    int rows = map.size();
    int cols = map[0].size();

    for (int i = 0; i < 4; i++) {
        if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
            smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
            map[smallBlocks[i].row][smallBlocks[i].col]) {
            return false;
        }
    }
    return true;
}

void Block::solidify(vector<vector<int>>& map) {

    for (int i = 0; i < 4; i++) {
        map[smallBlocks[i].row][smallBlocks[i].col] = blockType;   // create marker, solidify to destined location

    }
}

int Block::getBlockType() {
    return blockType;
}

