//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_BOARD_HPP
#define C_PROJETS_BOARD_HPP

#include "Direction.hpp"

class Board {
public:

    Board(int rows, int cols);
    ~Board();
    [[nodiscard]] int getRows() const;
    [[nodiscard]] int getCols() const;
    [[nodiscard]] int** getGrid() const;
    void setCell(int row, int col, int value);
    [[nodiscard]] int getCell(int row, int col) const;
    int make_action(Direction dir);

private:
    const float PROBA_4 = 0.1; // 10 % de chance que un 4 apparait
    int rows;
    int cols;
    int value_of_action = 0;
    int** grid{};



    void generateStartGrid();
    void add_new_cell();
    [[nodiscard]] int countEmpty() const;
    void initBoard();
    void initGrid();
    void resetValueOfAction();


    void slide_and_merge_line(int* row,Direction dir);
    void slide_cells_line(int* row,Direction dir);
    void merge_cells_line(int* row,Direction dir);

    void slide_and_merge_col(int col,Direction dir);
    void slide_cells_col(int row,Direction dir);
    void merge_cells_col(int row,Direction dir);

    void slide_and_merge_cell(int row,int col,Direction dir);



};
#endif //C_PROJETS_BOARD_HPP