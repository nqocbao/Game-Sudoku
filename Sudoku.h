#pragma once
#include <SDL.h>
#include <iostream>
#include <time.h>

#include "Button.h"
#include "Cell.h"
#include "Generator.h"

namespace Sudoku
{
	class Sudoku
	{
        private:
            const int mWindowHeight;
            const int mWindowWidth;

            const int mGridHeight;
            const int mGridWidth;

            const int mGridRows;
            const int mGridCols;

            SDL_Window* mWindow;
            SDL_Renderer* mRenderer;

            int mTotalTextures;
            SDL_Texture* mTextureCache[20];

	    	TTF_Font* mFont;
            int mFontSize;

            const int mTotalCells;
            Cell mGrid[81];

            Button mCheckButton;
            Button mNewButton;
            Button mTimer;

            SDL_Color mClearColour;

            bool initialiseSDL();
            inline int getIndex(int row, int col) const;

            void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);
            void preloadTextures();
            void createInterfaceLayout();
            void generateSudoku();
            void freeTextures();
        public:
            Sudoku();
            ~Sudoku();

            void play();
            void close();
	};
};
