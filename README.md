# Tetris

## Overview

A classic Tetris game that allows users to control falling blocks using arrow keys. Points are earned when rows in the playing field are filled up and cleared. The game ends when all 5 levels are passed, or if the block reaches the top of the playing field.

## Demo

https://github.com/RoisinDai/Tetris/assets/125154836/1863b585-3e65-475d-b58d-2e87470491b6

## Technologies Used

This game is implemented using Visual Code CE in C++ with EasyX, Conio, and Mmsystem frameworks.

## Features

- **Level System**
  
  The game has 5 levels with increasing speeds. Every 100 points scored progresses the user to the next level. Users can win the game by passing all 5 levels.

- **High score**

  The high score is tracked locally across all games, and updated when it has been exceeded.

- **Random block generation**

  One out of the 7 possible tetris block configurations are generated at random. The next block to drop is displayed in a preview window.

- **Drop**

  Blocks will drop to the lowest possible position in the playing field. Users can press the down arrow key to increase the speed of descent. 

- **Move**

  Users can press the left and right arrow keys to move the block horizontally within the playing field.

- **Rotate**

  Users can press the up arrow key to change the orientation of the tetris blocks.

- **Clearline**

  Once an entire row in the playing field is filled, all blocks within the row are cleared. The player will receive 10 points for one row clearance and 30 points for clearing two rows simultaneously. 

- **Game Status**

  Game messages with corresponding sounds will appear according to win/lose scenarios. 
