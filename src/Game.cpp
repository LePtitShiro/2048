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

void Game::moveDown() {int value = this->board->make_action(Direction::DOWN); this->updateScore(value);}
void Game::moveLeft() {int value = this->board->make_action(Direction::LEFT); this->updateScore(value);}
void Game::moveRight() {int value = this->board->make_action(Direction::RIGHT); this->updateScore(value);}
void Game::moveUp() {int value = this->board->make_action(Direction::UP); this->updateScore(value);}
const Board& Game::getBoard() const {return *this->board;}
int Game::getBestScore() const {return this->bestScore;}
int Game::getScore() const {return this->score;}
void Game::setScore(int score) {this->score = score;}
void Game::setBestScore(int bestScore) {this->bestScore = bestScore;}
void Game::updateScore(int value) {
    this->score += value;
}
void Game::updateBestScore() {
    int score = this->getScore();
    if (score > this->getBestScore()) {
        this->setBestScore(score);
    }
}


