#include "Cell.h"

Sudoku::Cell::Cell()
	: mEditable(false),
	  mCharNumber(' '),
	  mCharSolution(' ')
{

}

void Sudoku::Cell::setNumber(const int number)
{
	if (number == 0)
	{
		mCharNumber = ' ';
	}
	else
	{
		mCharNumber = '0' + number;
	}
}

char Sudoku::Cell::getNumber() const
{
	return mCharNumber;
}

void Sudoku::Cell::setSolution(const int solution)
{
	if (solution == 0)
	{
		mCharSolution =' ';
	}
	else
	{
		mCharSolution = '0' + solution;
	}
}

void Sudoku::Cell::setEditable(const bool editable)
{
	mEditable = editable;
	if (mEditable)
	{
		mMouseOutColour = { 255, 255, 255, SDL_ALPHA_OPAQUE };
		mMouseOverMotionColour = { 95, 89, 191, SDL_ALPHA_OPAQUE };
		mMouseDownColour = { 91, 191, 116, SDL_ALPHA_OPAQUE };
		mMouseUpColour = { 95, 89, 191, SDL_ALPHA_OPAQUE };
	}
	else
	{
		mMouseOutColour = { 128, 128, 128, SDL_ALPHA_OPAQUE };
		mMouseOverMotionColour = { 128, 128, 128, SDL_ALPHA_OPAQUE };
		mMouseDownColour = { 128, 128, 128, SDL_ALPHA_OPAQUE };
		mMouseUpColour = { 128, 128, 128, SDL_ALPHA_OPAQUE };
	}
}

bool Sudoku::Cell::isEditable() const
{
	return mEditable;
}

void Sudoku::Cell::handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[])
{
	if (event->key.keysym.sym == SDLK_BACKSPACE && mCharNumber != ' ')
	{
		mCharNumber = ' ';
		setTexture(textureCache[0]);
	}
	else if (event->type == SDL_TEXTINPUT)
	{
		if (atoi(event->text.text))
		{
			mCharNumber = *(event->text.text);
            // hiện thị số mới nhập
			setTexture(textureCache[atoi(event->text.text)]);
		}
	}
}

bool Sudoku::Cell::isCorrect() const
{
	return (mCharNumber == mCharSolution||mCharNumber!=' ');
}
