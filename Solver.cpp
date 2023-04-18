#include "Solver.h"
using namespace std;

Sudoku::Solver::Solver()
	: mGrid(nullptr), mGenModifier(false), mNumToIgnore({ 0, 0, 0 })
{

}

inline void Sudoku::Solver::setElement(const int row, const int col, const int num)
{
	mGrid[row * 9 + col] = num;
}

inline int Sudoku::Solver::getElement(const int row, const int col) const
{
	return mGrid[row * 9 + col];
}

bool Sudoku::Solver::checkValid(const int inputRow, const int inputCol, const int num) const
{
	// check hafng
	for (int col = 0; col < 9; col++)
	{
		if (getElement(inputRow, col) == num)
		{
			return false;
		}
	}
	// check cột
	for (int row = 0; row < 9; row++)
	{
		if (getElement(row, inputCol) == num)
		{
			return false;
		}
	}
	// check khối 3x3
	const int startRow = (int)(inputRow / 3) * 3;
	const int startCol = (int)(inputCol / 3) * 3;

	const int endRow = startRow + 3;
	const int endCol = startCol + 3;

	for (int row = startRow; row < endRow; row++)
	{
		for (int col = startCol; col < endCol; col++)
		{
			if (getElement(row, col) == num)
			{
				return false;
			}
		}
	}
	return true;
}

void Sudoku::Solver::setGenModifier(const bool input)
{
	mGenModifier = input;
}

void Sudoku::Solver::setGrid(int* grid, const Ignore& ignoreInput)
{
	mNumToIgnore = ignoreInput;
	mGrid = grid;
}

bool Sudoku::Solver::solve()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (getElement(row, col) == 0)
			{
				for (int num = 1; num <= 9; num++)
				{
				    // nếu bộ điều chỉnh khởi tạo được set true và cột,hàng số khớp nhau thì đến bước lặp tiếp
					if (!(mGenModifier && mNumToIgnore.row == row && mNumToIgnore.col == col && mNumToIgnore.num == num))
					{
						// check hợp lệ
						if (checkValid(row, col, num))
						{
							setElement(row, col, num);
							// đệ quy
							if (solve())
							{
								return true;
							}
							// gán ptu grid[row,col] =0 nếu ko thể đệ quy nữa
							setElement(row, col, 0);
						}
					}
				}
				// back track to previous element
				return false;
			}
		}
	}
	return true;
}

void Sudoku::Solver::display() const
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cout<<mGrid[row * 9 + col]<< ", ";
		}
		cout<<endl;
	}
	cout<<"----------------------------\n";
}
