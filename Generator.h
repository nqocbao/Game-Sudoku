#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Solver.h"

namespace Sudoku
{
	class Generator
	{
        private:
            // mảng 1 chiều kthuoc 81
            int* mGrid;
            int* mGridSolution;  // mảng 1 chiều sau khi giải

            inline void setElement(const int row, const int col, const int num);
            inline int getElement(const int row, const int col) const;

            void swapNumbers(const int index1, const int index2);
            void swapRows(const int row1, const int row2);
            void swapCols(const int col1, const int col2);
            void swapRowBlocks(const int rowBlock1, const int rowBlock2);
            void swapColBlocks(const int colBlock1, const int colBlock2);

            // thay các cột sau bằng cột trước đó
            void fillNextRow(const int previousRow, const int nextRow, const int shifts);

            void copyGrid(int* grid) const;

            void createCompletedSudoku();

        public:
            Generator();
            // tạo ra su đu ku từ sudoku hoàn chỉnh và trả cho nó con trỏ
            void generate(int* grid, int* solutionGrid);

            void display() const;

	};

};
