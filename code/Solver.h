#pragma once
#include <iostream>

namespace Sudoku
{
	struct Ignore
	{
		int num = 0;
		int row = 0;
		int col = 0;
	};

	// giải các ô số
	class Solver
	{
        private:
            // đưa các ô suduku thành con trỏ 1 chiều
            int* mGrid;

            // bỏ qua số vừa giải (for the Sudoku generator class)
            bool mGenModifier;
            Ignore mNumToIgnore;

            inline void setElement(const int row, const int col, const int num);
            inline int getElement(const int row, const int col) const;

            // kiểm tra phần tử có hợp lệ hay không
            bool checkValid(const int inputRow, const int inputCol, const int num) const;

        public:
            Solver();

            // cài bộ điều chỉnh khởi tạo gêm
            void setGenModifier(const bool input);

            // cài mảng sudoku
            void setGrid(int* grid, const Ignore& ignoreInput = { 0, 0, 0 });

            // giải bằng thuật toán quay lui của đệ quy (backtracking)
            bool solve();

            // in ra answers
            void display() const;

	};

};
