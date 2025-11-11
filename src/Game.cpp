//
// Created by Lucas on 01/10/2025.
//

#include "Game.hpp"


Game::Game() {
    this->score = 0;
    this->bestScore = 0;
    this->board = new Board(4,4);
}
Game::~Game() {
    delete this->board;
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
const Board& Game::getBoard() {return *this->board;}
int Game::getBestScore() {return this->bestScore;}
int Game::getScore() {return this->score;}
void Game::setScore(int score) {this->score = score;}
void Game::setBestScore(int bestScore) {this->bestScore = bestScore;}
void Game::updateScore() {
    this->score += 1; //TODO: faire l'implémentation de méthode updateScore() correctement
}
void Game::updateBestScore() {
    int score = this->getScore();
    if (score > this->getBestScore()) {
        this->setBestScore(score);
    }
}


