//
// Created by Lucas on 01/10/2025.
//

#include "Game.hpp"


Game::Game() {
    this->score = 0;
    this->bestScore = 0;
    this->board = new Board(4,4);
}

void Game::move(const Direction direction) {
    switch (direction) {
        case Direction::UP:
            moveUp();
            break;
        case Direction::DOWN:
            moveDown();
            break;
        case Direction::LEFT:
            moveLeft();
            break;
        case Direction::RIGHT:
            moveRight();
            break;
        default:
            break;
    }
}

void Game::moveDown() {this->board->make_action(Direction::DOWN);}
void Game::moveLeft() {this->board->make_action(Direction::LEFT);}
void Game::moveRight() {this->board->make_action(Direction::RIGHT);}
void Game::moveUp() {this->board->make_action(Direction::UP);}

