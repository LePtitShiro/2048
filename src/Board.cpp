//
// Created by Lucas on 01/10/2025.
//

#include "Board.hpp"
#include <cstdlib>
#include <vector>

/**
 * Constructeur de la classe Board représentant la grille de jeu
 *
 * @param rows Nombre de lignes
 * @param cols Nombre de colonnes
 */
Board::Board(const int rows, const int cols) {
    this->rows = rows;
    this->cols = cols;
    this->value_of_action = 0;
    initBoard();
}


void Board::slide_cells_col(int col, Direction dir) {
    if (dir == Direction::UP) {
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
    } else if (dir == Direction::DOWN) {
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
    resetValueOfAction();
    if (dir == Direction::UP) {
        for (int i = 0; i< rows - 1; i++) {
            if (this->grid[i][col]!= 0 && this->grid[i][col] == this->grid[i + 1][col]){
                this->grid[i][col] *= 2;
                value_of_action+= this->grid[i][col];
                this->grid[i+1][col] = 0;
                i++;
            }
        }
    }else if (dir == Direction::DOWN) {
        for (int i = rows - 1; i > 0; i--) {
            if (this->grid[i][col] != 0 && this->grid[i][col] == this->grid[i - 1][col]) {
                this->grid[i][col] *= 2;
                value_of_action+= this->grid[i][col];
                this->grid[i - 1][col] = 0;
                i--;
            }
        }

    }
}

void Board::slide_cells_line(int *row, Direction dir) {
    if (dir == Direction::LEFT) {
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
    }else if (dir == Direction::RIGHT) {
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
    resetValueOfAction();
    if (dir == Direction::LEFT) {
        for (int i = 0; i < cols - 1; i++) {
            if (row[i] != 0 && row[i] == row[i + 1]) {
                row[i] *= 2;
                value_of_action += row[i];
                row[i + 1] = 0;
                i++; // on passe la tuile suivante pour éviter un double mélange
            }
        }
    } else if (dir == Direction::RIGHT) {
        for (int i = cols - 1; i > 0; i--) {
            if (row[i] != 0 && row[i] == row[i - 1]) {
                row[i] *= 2;
                value_of_action += row[i];
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


int Board::make_action(Direction dir) {
    switch (dir) {
        case Direction::LEFT :
            for (int i = 0; i < rows; i++) {
                slide_and_merge_line(this->grid[i], dir);
            }
            break;
        case Direction::RIGHT :
            for (int i = 0; i < rows; i++) {
                slide_and_merge_line(this->grid[i], dir);
            }
            break;
        case Direction::UP :
            for (int i = 0; i < cols; i++) {
                slide_and_merge_col(i, dir);
            }
            break;
        case Direction::DOWN :
            for (int i = 0; i < cols; i++) {
                slide_and_merge_col(i, dir);
            }
            break;
        default :
            break;
    }
    if (countEmpty() >= 1) {
        add_new_cell();
    }
    return value_of_action;


}







/**
 * Destructeur de la classe Board, libère la mémoire allouée pour la grille
 *
 */
Board::~Board() {
    for (int i = 0; i < rows; i++) delete[] this->grid[i]; // chaque
    delete[] this->grid;
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

int Board::countEmpty() const {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (this->grid[i][j] == 0) count++;
        }
    }
    return count;
}

void Board::add_new_cell() {
    // on récupère les pos vides
    std::vector<std::pair<int, int>> empty_positions;
    for (int i = 0; i< rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 0) {
                empty_positions.emplace_back(i, j);
            }
        }
    }
    if (empty_positions.empty()) return; // si il n'y a pas de place on ne fait rien

    //maintenant on choisit aléatoirement une position et lui donne 2 ou 4
    auto [pos_x,pos_y] = empty_positions[rand() % empty_positions.size()];
    double p = (double)rand() / RAND_MAX;
    p <= PROBA_4 ? this->grid[pos_x][pos_y] = 4 : this->grid[pos_x][pos_y] = 2;
}


void Board::generateStartGrid() {
    for (int k = 0; k < 2; k++) {
        add_new_cell();
    }
}

void Board::resetValueOfAction() {
    this->value_of_action = 0;
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






