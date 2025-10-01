//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_BOARD_HPP
#define C_PROJETS_BOARD_HPP

#endif //C_PROJETS_BOARD_HPP

extern enum Direction {LEFT, RIGHT, UP, DOWN};

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    [[nodiscard]] int getRows() const;
    [[nodiscard]] int getCols() const;
    [[nodiscard]] int** getGrid() const;
    void setCell(int row, int col, int value);
    [[nodiscard]] int getCell(int row, int col) const;


private:
    const float PROBA_4 = 0.1; // 10 % de chance que un 4 apparait
    int rows;
    int cols;
    int** grid{};

    static void generateStartGrid(int rows, int cols, int** grid, float PROBA_4);
    void initBoard();
    void initGrid();


    void slide_and_merge_line(int* row,Direction dir);
    void slide_cells_line(int* row,Direction dir);
    void merge_cells_line(int* row,Direction dir);

    void slide_and_merge_col(int col,Direction dir);
    void slide_cells_col(int row,Direction dir);
    void merge_cells_col(int row,Direction dir);

    void slide_and_merge_cell(int row,int col,Direction dir);


};