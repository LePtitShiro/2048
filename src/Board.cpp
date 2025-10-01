//
// Created by Lucas on 01/10/2025.
//

#include "Board.hpp"
#include <cstdlib>

/**
 * Constructeur de la classe Board représentant la grille de jeu
 *
 * @param rows Nombre de lignes
 * @param cols Nombre de colonnes
 */
Board::Board(const int rows, const int cols) {
    this->rows = rows;
    this->cols = cols;
    initBoard();
}

void Board::slide_and_merge_left(int* row) {
    return;
}


/**
 * Destructeur de la classe Board, libère la mémoire allouée pour la grille
 *
 */
Board::~Board() {
    for (int i = 0; i < rows; i++) this->grid[i] = nullptr; // chaque
    this->grid = nullptr;
}


/**
 * Getter pour le nombre de lignes (méthode const car elle ne modifie pas l'objet)
 *
 * @return le nombre de lignes
 */
int Board::getRows() const {
    return this->rows;
}

/**
 * Getter pour le nombre de colonnes (méthode const car elle ne modifie pas l'objet)
 *
 * @return le nombre de colonnes
 */
int Board::getCols() const {
    return this->cols;
}

int** Board::getGrid() const {
    return this->grid;
}

void Board::initGrid() {
    this->grid = new int*[rows];
    for (int i = 0; i < rows; i++) {
        this->grid[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            this->grid[i][j] = 0; // lorsque la tuile est à vide on met 0;
        }
    }
}

void Board::initBoard() {
    initGrid();
    generateStartGrid();
}
//TODO : création d'un
void Board::generateStartGrid() {
    int i,j;
    double p;
    for (int k = 0; k < 2; k++) {
        i = rand() % this->rows;
        j = rand() % this->cols;
        p = rand() % 1;
        while (this->grid[i][j] != 0) {
            i = rand() % this->rows;
            j = rand() % this->cols;
        }
        p <= PROBA_4 ? this->grid[i][j] = 4 : this->grid[i][j] = 2;
    }
}

/**
 * Setter pour assigner une valeur à une cellule spécifique de la grille
 *
 * @param row Coordonnée de la ligne pour la cellule
 * @param col Coordonnée de la colonne pour la cellule
 * @param value Valeur à assigner à la cellule
 */
void Board::setCell(const int row,const  int col,const int value) {
    this->grid[row][col] = value;
}

int Board::getCell(const int row, const int col) const {
    return this->grid[row][col];
}






