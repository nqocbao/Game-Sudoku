#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdlib.h>

#include "Button.h"

namespace Sudoku
{
	class Cell : public Button
	{
        private:
            // xác định số có thể điền hay ko
            bool mEditable;
            char mCharNumber;
            char mCharSolution;

        public:
            Cell();
            void setNumber(const int number);
            char getNumber() const;

            void setSolution(const int solution);

            // cài khả năng đổi màu cho các số chỉnh sửa được
            void setEditable(const bool editable);
            bool isEditable() const;

            void handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[]);

            // đối chiếu kqua số nhập với answer's solution
            bool isCorrect() const;
	};

};
