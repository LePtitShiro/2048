//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_GAME_H
#define C_PROJETS_GAME_H
#include "Board.hpp"
#include "Direction.hpp"



class Game {
public:
    Game();
    ~Game();
    int getScore() const;
    int getBestScore() const;
    void setScore(int score);
    void setBestScore(int bestScore);
    const Board& getBoard() const;
    void move(Direction direction);
private:
    int score;
    int bestScore;
    Board *board;
    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    void updateScore(int value);
    void updateBestScore();
};


#endif //C_PROJETS_GAME_H