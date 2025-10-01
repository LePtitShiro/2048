//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_BOARD_HPP
#define C_PROJETS_BOARD_HPP

#endif //C_PROJETS_BOARD_HPP

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    [[nodiscard]] int getRows() const;
    [[nodiscard]] int getCols() const;
    [[nodiscard]] int** getGrid() const;
    void setCell(int row, int col, int value);
    [[nodiscard]] int getCell(int row, int col) const;


protected:
    const float PROBA_4 = 0.1; // 10 % de chance que un 4 apparait
    int rows;
    int cols;
    int** grid{};

    void generateStartGrid();
    void initBoard();
    void initGrid();
    void slide_and_merge_left(int* row);
};