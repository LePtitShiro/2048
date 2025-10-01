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
        int getRows() const;
        int getCols() const;
        int** getGrid() const;
        void setCell(int row, int col, int value);
        int getCell(int row, int col);

    private:
        int rows;
        int cols;
        int** grid;
        void initGrid();
};