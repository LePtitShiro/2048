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


void Board::slide_cells_col(int col, Direction dir) {
    if (dir == UP) {
        for (int i = 0; i < rows - 1; i++) {
            if (this->grid[i][col] == 0) {
                for (int j = i + 1; j < rows; j++) {
                    if (this->grid[j][col] != 0) {
                        this->grid[i][col] = this->grid[j][col];
                        this->grid[j][col] = 0;
                        break;
                    }
                }
            }
        }
    } else if (dir == DOWN) {
        for (int i = rows - 1; i > 0; i--) {
            if (this->grid[i][col] == 0) {
                for (int j = i - 1; j >= 0; j--) {
                    if (this->grid[j][col] != 0) {
                        this->grid[i][col] = this->grid[j][col];
                        this->grid[j][col] = 0;
                        break;
                    }
                }
            }
        }
    }


}
void Board::merge_cells_col(int col, Direction dir) {
    if (dir == UP) {
        for (int i = 0; i< cols - 1; i++) {
            if (this->grid[i][col]!= 0 && this->grid[i][col] == this->grid[i + 1][col]){
                this->grid[i][col] *= 2;
                this->grid[i+1][col] = 0;
                i++;
            }
        }
    }else if (dir == DOWN) {
        for (int i = cols - 1; i > 0; i--) {
            if (this->grid[i][col] != 0 && this->grid[i][col] == this->grid[i - 1][col]) {
                this->grid[i][col] *= 2;
                this->grid[i - 1][col] = 0;
                i--;
            }
        }

    }
}

void Board::slide_cells_line(int *row, Direction dir) {
    if (dir == LEFT) {
        for (int i = 0; i < cols - 1; i++) {
            if (row[i] == 0) {
                for (int j = i + 1; j < cols; j++) {
                    if (row[j] != 0) {
                        row[i] = row[j];
                        row[j] = 0;
                        break;
                    }
                }
            }
        }
    }else if (dir == RIGHT) {
        for (int i = cols - 1; i > 0; i--) {
            if (row[i] == 0) {
                for (int j = i - 1; j >= 0; j--) {
                    if (row[j] != 0) {
                        row[i] = row[j];
                        row[j] = 0;
                        break;
                    }
                }
            }
        }
    }
}
void Board::merge_cells_line(int *row, Direction dir) {
    if (dir == LEFT) {
        for (int i = 0; i < cols - 1; i++) {
            if (row[i] != 0 && row[i] == row[i + 1]) {
                row[i] *= 2;
                row[i + 1] = 0;
                i++; // on passe la tuile suivante pour éviter un double mélange
            }
        }
    } else if (dir == RIGHT) {
        for (int i = cols - 1; i > 0; i--) {
            if (row[i] != 0 && row[i] == row[i - 1]) {
                row[i] *= 2;
                row[i - 1] = 0;
                i--; // on passe la tuile suivante pour éviter un double mélange
            }
        }
    }
}

void Board::slide_and_merge_line(int *row, Direction dir) {
    slide_cells_line(row, dir);
    merge_cells_line(row, dir);
    slide_cells_line(row, dir);
}
void Board::slide_and_merge_col(int col, Direction dir) {
    slide_cells_col(col, dir);
    merge_cells_col(col, dir);
    slide_cells_col(col, dir);
}

void Board::slide_and_merge_cell(int row, int col, Direction dir) {
    if (dir == LEFT || dir == RIGHT) {
        slide_and_merge_line(this->grid[row], dir);
    } else if (dir == UP || dir == DOWN) {
        slide_and_merge_col(col, dir);
    }
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
    generateStartGrid(this->rows,this->cols,this->grid,this->PROBA_4);
}

void Board::generateStartGrid(int rows,int cols, int** grid,const float PROBA_4) {
    for (int k = 0; k < 2; k++) {
        int i = rand() % rows;
        int j = rand() % cols;
        double p = rand() % 1;
        while (grid[i][j] != 0) {
            i = rand() % rows;
            j = rand() % cols;
        }
        p <= PROBA_4 ? grid[i][j] = 4 : grid[i][j] = 2;
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






