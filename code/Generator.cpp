#include "Generator.h"
using namespace std;

Sudoku::Generator::Generator()
	: mGrid(nullptr), mGridSolution(nullptr)
{

}

inline void Sudoku::Generator::setElement(const int row, const int col, const int num)
{
	mGrid[row * 9 + col] = num;
}

inline int Sudoku::Generator::getElement(const int row, const int col) const
{
	return mGrid[row * 9 + col];
}

void Sudoku::Generator::swapNumbers(const int index1, const int index2)
{
     swap(mGrid[index1],mGrid[index2]);
}

void Sudoku::Generator::swapRows(const int row1, const int row2)
{
	for (int col = 0; col < 9; col++)
	{
		swapNumbers(row1 * 9 + col, row2 * 9 + col);
	}
}
void Sudoku::Generator::swapCols(const int col1, const int col2)
{
	for (int row = 0; row < 9; row++)
	{
		swapNumbers(row * 9 + col1, row * 9 + col2);
	}
}

void Sudoku::Generator::swapRowBlocks(const int rowBlock1, const int rowBlock2)
{
	int startRow1 = rowBlock1 * 3;
	int startRow2 = rowBlock2 * 3;

	for (int i = 0; i < 3; i++)
	{
		swapRows(startRow1 + i, startRow2 + i);
	}
}

void Sudoku::Generator::swapColBlocks(const int colBlock1, const int colBlock2)
{
	int startCol1 = colBlock1 * 3;
	int startCol2 = colBlock2 * 3;

	for (int i = 0; i < 3; i++)
	{
		swapCols(startCol1 + i, startCol2 + i);
	}
}

void Sudoku::Generator::fillNextRow(const int previousRow, const int nextRow, const int shifts)
{
	for (int col = 0; col < (9 - shifts); col++)
	{
		setElement(nextRow, col, getElement(previousRow, col + shifts));
	}

	for (int col = (9 - shifts); col < 9; col++)
	{
		setElement(nextRow, col, getElement(previousRow, col - 9 + shifts));
	}
}

void Sudoku::Generator::copyGrid(int* grid) const
{
	for (int i = 0; i < 81; i++)
	{
		grid[i] = mGrid[i];
	}
}

void Sudoku::Generator::createCompletedSudoku()
{
	// Set random seed using time
	srand((unsigned int)time(NULL));

	// điền hàng 1 với 1->9
	for (int i = 0; i < 9; i++)
	{
		mGrid[i] = i + 1;
	}

	// xáo trộn hàng 1
	int swaps = 50;
	for (int i = 0; i < swaps; i++)
	{
		int randIndex1 = rand() % 9;
		int randIndex2 = rand() % 9;
		if (randIndex1 != randIndex2)
		{
			swapNumbers(randIndex1, randIndex2);
		}
	}

	// điền hàng 2 và 3 bằng hàng trước nma bị xáo trộn n số ptu đầu
	fillNextRow(0, 1, 3);
	fillNextRow(1, 2, 3);
	fillNextRow(2, 3, 1);
	fillNextRow(3, 4, 3);
	fillNextRow(4, 5, 3);
	fillNextRow(5, 6, 1);
	fillNextRow(6, 7, 3);
	fillNextRow(7, 8, 3);

	// xáo trộn các hàng trong 1 ô vuông 3x3
	int shuffles = 10;
	for (int rowBlock = 0; rowBlock < 3; rowBlock++)
	{
		for (int shuffle = 0; shuffle < shuffles; shuffle++)
		{
			int randRow1 = rowBlock * 3 + (rand() % 3);
			int randRow2 = rowBlock * 3 + (rand() % 3);
			if (randRow1 != randRow2)
			{
				swapRows(randRow1, randRow2);
			}
		}
	}

	// xáo trộn các cột trong 3x3
	for (int colBlock = 0; colBlock < 3; colBlock++)
	{
		for (int shuffle = 0; shuffle < shuffles; shuffle++)
		{
			int randCol1 = colBlock * 3 + (rand() % 3);
			int randCol2 = colBlock * 3 + (rand() % 3);
			if (randCol1 != randCol2)
			{
				swapCols(randCol1, randCol2);
			}
		}
	}

	// xáo trộn hàng
	for (int shuffle = 0; shuffle < shuffles; shuffle++)
	{
		int randRowBlock1 = rand() % 3;
		int randRowBlock2 = rand() % 3;
		if (randRowBlock1 != randRowBlock2)
		{
			swapRowBlocks(randRowBlock1, randRowBlock2);
		}
	}

	// xáo cột
	for (int shuffle = 0; shuffle < shuffles; shuffle++)
	{
		int randColBlock1 = rand() % 3;
		int randColBlock2 = rand() % 3;
		if (randColBlock1 != randColBlock2)
		{
			swapColBlocks(randColBlock1, randColBlock2);
		}
	}

	// lưu suduku đã giải
	copyGrid(mGridSolution);

}

void Sudoku::Generator::generate(int* grid, int* solutionGrid)
{
	mGrid = grid;
	mGridSolution = solutionGrid;
	createCompletedSudoku();
	srand((unsigned int)time(NULL));

	Solver SS;
	SS.setGenModifier(true);

	bool removed[81] = { };
	int duplicateGrid[81];
	int removingNumber = 0;
	int toRemove = 50;

	while (toRemove)
	{
		int randRow = rand() % 9;
		int randCol = rand() % 9;
		if (!removed[randRow * 9 + randCol])
		{
			removingNumber = getElement(randRow, randCol);
			copyGrid(duplicateGrid);
			duplicateGrid[randRow * 9 + randCol] = 0;
			Ignore numToIgnore = { removingNumber, randRow, randCol };
			SS.setGrid(duplicateGrid, numToIgnore);
			if (!SS.solve())
			{
				setElement(randRow, randCol, 0);
				removed[randRow * 9 + randCol] = true;
				toRemove--;
			}
		}
	}

}

void Sudoku::Generator::display() const
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cout << getElement(row, col) << ", ";
		}
		cout << endl;
	}
}
