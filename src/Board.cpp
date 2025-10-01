//
// Created by Lucas on 01/10/2025.
//

#include "Board.hpp"


/**
 * Constructeur de la classe Board représentant la grille de jeu
 *
 * @param rows Nombre de lignes
 * @param cols Nombre de colonnes
 */
Board::Board(const int rows, const int cols) {
    this->rows = rows;
    this->cols = cols;
    initGrid();


}


/**
 * Destructeur de la classe Board, libère la mémoire allouée pour la grille
 *
 */
Board::~Board() {
    for (int i = 0; i < rows; i++) {
        this->grid[i] = nullptr; // chaque
    }
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

//TODO : initiliaser la méthode setCell(int row, int col, int value)
//TODO : initialiser la méthode getCell(int row, int col)




