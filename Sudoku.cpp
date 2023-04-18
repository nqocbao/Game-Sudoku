#include "Sudoku.h"
using namespace std;
/*..........................Khởi tạo..........................*/
Sudoku::Sudoku::Sudoku()
	: mWindowHeight(500), mWindowWidth(900),
	  mGridHeight(500), mGridWidth(500),
	  mGridRows(9), mGridCols(9),
	  mWindow(NULL), mRenderer(NULL),
	  mTotalTextures(14), mTextureCache{ NULL },
	  mFont(NULL), mFontSize(mGridHeight/9),
	  mTotalCells(81),
	  mClearColour({ 0, 0, 0, SDL_ALPHA_OPAQUE })
{

}

Sudoku::Sudoku::~Sudoku()
{
	freeTextures();
	close();
}

/* .........................Bắt đầu..........................*/
bool Sudoku::Sudoku::initialiseSDL()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout<<"SDL could not intialise! Error: "<<SDL_GetError()<<endl;
		success = false;
	}
	if (TTF_Init() == -1)
	{
		cout<<"SDL_ttf could not initialise! Error: "<<TTF_GetError()<<endl;
		success = false;
	}
	mWindow = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN);
	if (mWindow == NULL)
	{
		cout<<"SDL could not create window! Error: "<<SDL_GetError()<<endl;
		success = false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (mRenderer == NULL)
	{
		cout<<"SDL could not create renderer! Error: "<<SDL_GetError()<<endl;
		success = false;
	}
	mFont = TTF_OpenFont("gfx/octin sports free.ttf", mFontSize);
	if (mFont == NULL)
	{
		cout<<"Failed to load lazy font! Error: "<<TTF_GetError()<<endl;
		success = false;
	}
	return success;
}

inline int Sudoku::Sudoku::getIndex(int row, int col) const
{
	return row * mGridRows + col;
}

void Sudoku::Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text, fontColour);
	if (textSurface == NULL)
	{
		cout<<"Could not create TTF SDL_Surface! Error: "<<TTF_GetError()<<endl;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (texture == NULL)
		{
			cout<<"Could not create texture from surface! Error: "<<SDL_GetError()<<endl;
		}
		SDL_FreeSurface(textSurface);
	}
}

void Sudoku::Sudoku::preloadTextures()
{
	SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black
	loadTexture(mTextureCache[0], " ", fontColour);
	for (int num = 1; num < 10; num++)
	{
		const char temp[] = { '0' + num, '\0' };
		loadTexture(mTextureCache[num], temp, fontColour);
	}
	loadTexture(mTextureCache[10], "Check", fontColour);
	loadTexture(mTextureCache[11], "New", fontColour);
	loadTexture(mTextureCache[12], "Wrong!", fontColour);
	loadTexture(mTextureCache[13], "Right!", fontColour);
}

void Sudoku::Sudoku::createInterfaceLayout()
{
	const int thinBorder = 2;
	const int thickBorder = thinBorder + 6;

	//                     ĐỒNG HỒ TÍNH GIỜ
	int buttonWidth = mWindowWidth- (mGridWidth + 2 * thickBorder);
	int buttonHeight = (mWindowHeight - 4 * thickBorder)/3 ;

	int buttonStartRow = thickBorder;
	int buttonStartCol = mGridWidth+thickBorder/2;
	SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
	mTimer.setButtonRect(buttonRect);

	//                      CÁC NÚT 9X9
	buttonWidth = (mGridWidth - 6 * thinBorder - 4 * thickBorder) / mGridCols;
    buttonHeight= (mGridHeight- 6* thinBorder-4* thickBorder)/mGridRows;
	// tiếp tục từ hàng trước đó
	buttonStartRow =0;

	// đặt vị trí và kích thước của các nút ô
	for (int gridRow = 0; gridRow < mGridRows; gridRow++)
	{
		// thêm vô hàng bắt đầu
		if (gridRow == 0) buttonStartRow += thickBorder;
		else if (gridRow % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
            else buttonStartRow += buttonHeight + thinBorder;

		int buttonStartCol = 0;
		for (int gridCol = 0; gridCol < mGridCols; gridCol++)
		{
			// thêm vô cột bắt đầu
			if (gridCol == 0) buttonStartCol += thickBorder;
			else if (gridCol % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
                else buttonStartCol += buttonWidth + thinBorder;

			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = getIndex(gridRow, gridCol);
			mGrid[index].setButtonRect(buttonRect);
		}
	}

	//                  TẠO NÚT CHECK VỚI NÚT NEW
	const int numberOfOtherButtons = 2;
	mCheckButton.setTexture(mTextureCache[10]);
	mNewButton.setTexture(mTextureCache[11]);
	Button* otherButtons[numberOfOtherButtons] = { &mCheckButton, &mNewButton };
    //                  NÚT CHECK
	buttonWidth = (mWindowWidth- (mGridWidth + 2 * thickBorder))/2;
    buttonHeight = (mWindowHeight - 4 * thickBorder)/3 ;
	buttonStartRow =2*thickBorder+ 2*buttonHeight;
    buttonStartCol= mGridWidth+thinBorder;
    buttonRect= {buttonStartCol,buttonStartRow,buttonWidth,buttonHeight};
    otherButtons[0]->setButtonRect(buttonRect);
    //                  NÚT NEW
    buttonStartCol += buttonWidth+thickBorder;
    buttonRect= {buttonStartCol,buttonStartRow,buttonWidth,buttonHeight};
    otherButtons[1]->setButtonRect(buttonRect);
}

void Sudoku::Sudoku::generateSudoku()
{
	int generatedGrid[81] = {};
	int solution[81] = {};

	Generator G;
	G.generate(generatedGrid, solution);

	for (int i = 0; i < 81; i++)
	{
		mGrid[i].setNumber(generatedGrid[i]);
		mGrid[i].setSolution(solution[i]);

		if (generatedGrid[i] == 0) mGrid[i].setEditable(true);
		else mGrid[i].setEditable(false);

		mGrid[i].setTexture(mTextureCache[generatedGrid[i]]);
		mGrid[i].centerTextureRect();

	}
}

void Sudoku::Sudoku::freeTextures()
{
	for (int i = 0; i < mTotalTextures; i++)
	{
		if (mTextureCache[i] != NULL)
		{
			SDL_DestroyTexture(mTextureCache[i]);
			mTextureCache[i] = NULL;
		}
	}
}

void Sudoku::Sudoku::play()
{
	if (!initialiseSDL())
	{
		close();
	}
	preloadTextures();
	createInterfaceLayout();

	// khởi tạo suduku, cài texture với các ô có thể điền số
	generateSudoku();

	Cell* currentCellSelected = &mGrid[0];
	for (int cell = 0; cell < mTotalCells; cell++)
	{
		if (mGrid[cell].isEditable())
		{
			currentCellSelected = &mGrid[cell];
			currentCellSelected->setSelected(true);
			break;
		}
	}

	SDL_StartTextInput();
	SDL_Event event;
	bool stop = false;
	bool completed = false;
	bool generateNewSudoku = false;
	bool checkSolution = false;

	// Timing for check button
	bool measureTimeForCheckButton = false;
	time_t startTimeForCheckButton;

	// Timer
	Uint32 startTime = 0;
    Uint32 currentTime = 0;
    Uint32 deltaTime = 0;
    Uint32 totalSeconds = 0;

	while (!stop)
	{

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				stop = true;
			}
			if (mCheckButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				checkSolution = true;
			}
			if (mNewButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				generateNewSudoku = true;
			}
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				if (mGrid[cell].isEditable())
				{

					if (mGrid[cell].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
					{
						currentCellSelected->setSelected(false);
						currentCellSelected = &mGrid[cell];
						currentCellSelected->setSelected(true);
					}
				}
			}
			currentCellSelected->handleKeyboardEvent(&event, mTextureCache);
		}
		if (generateNewSudoku)
		{
			generateSudoku();
			currentCellSelected->setSelected(false);
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				if (mGrid[cell].isEditable())
				{
					currentCellSelected = &mGrid[cell];
					currentCellSelected->setSelected(true);
					break;
				}
			}
			generateNewSudoku = false;
			completed = false;
			// Reset timer
            Uint32 startTime = 0;
            Uint32 currentTime = 0;
            Uint32 deltaTime = 0;
            Uint32 totalSeconds = 0;
		}
		else
        {
            if (checkSolution)
		{
			// Check if complete
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				if (!mGrid[cell].isCorrect())
				{
					completed = false;
					break;
				}
				completed = true;
			}

			// đặt cờ đo thời gian và thời gian bắt đầu
			measureTimeForCheckButton = true;
			time(&startTimeForCheckButton);

			checkSolution = false;
		}
		if (measureTimeForCheckButton)
		{
			int seconds = 2;
			if (difftime(time(NULL), startTimeForCheckButton) < seconds && completed)
			{
				// Set colour -> green
				SDL_Color colour = { 91, 191, 116, SDL_ALPHA_OPAQUE };
				SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);
				mCheckButton.setTexture(mTextureCache[13]);
				mCheckButton.setMouseDownColour(colour);
			}
			else if (difftime(time(NULL), startTimeForCheckButton) < seconds && !completed)
                 {
                    // Set colour -> red
                     SDL_Color colour = { 200, 73, 46, SDL_ALPHA_OPAQUE };
                     SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);
                     mCheckButton.setTexture(mTextureCache[12]);
                     mCheckButton.setMouseDownColour(colour);
                 }
                 else measureTimeForCheckButton = false;
		}
		else
		{
			mCheckButton.setTexture(mTextureCache[10]);
			SDL_SetRenderDrawColor(mRenderer, mClearColour.r, mClearColour.g, mClearColour.b, mClearColour.a);
		}
		SDL_RenderClear(mRenderer);

        //j kết xuất các nút với kết cấu của từng ô thành bộ đệm nền
		for (int cell = 0; cell < mTotalCells; cell++)
		{
			mGrid[cell].renderButton(mRenderer);
			// căn chỉnh lại chính giữa từng ô vì sized của số có thể khác
			mGrid[cell].centerTextureRect();
			mGrid[cell].renderTexture(mRenderer);
		}

		// render check button
		mCheckButton.renderButton(mRenderer);
		mCheckButton.centerTextureRect();
		mCheckButton.renderTexture(mRenderer);

		// render new button
		mNewButton.renderButton(mRenderer);
		mNewButton.centerTextureRect();
		mNewButton.renderTexture(mRenderer);

		// tinhs giờ
        /*startTimer=SDL_GetTicks();
		time_t difference = time(NULL)-startTimer;
		tm formattedTime;
		localtime_s(&formattedTime, &difference);
		char timer[100];
		strftime(timer, sizeof(timer), "Time: %H:%M:%S", &formattedTime);*/

		startTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - startTime;
        totalSeconds += deltaTime;

        // Tính toán thời gian hiện tại
        int hours = totalSeconds / 3600000;
        int minutes = (totalSeconds % 3600000) / 60000;
        int seconds = ((totalSeconds % 3600000) % 60000) / 1000;

        // Định dạng chuỗi thời gian
        char timer[100];
        sprintf(timer, "time: %02d:%02d:%02d", hours, minutes, seconds);

		SDL_Texture* timerTexture = NULL;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE };
		loadTexture(timerTexture, timer, fontColour);
		mTimer.setTexture(timerTexture);
		mTimer.renderButton(mRenderer);
		mTimer.centerTextureRect();
		mTimer.renderTexture(mRenderer);
		SDL_DestroyTexture(timerTexture);
		timerTexture = NULL;

		SDL_RenderPresent(mRenderer);
		SDL_Delay(10);
        }
	}
	SDL_StopTextInput();
	freeTextures();
	close();
}

void Sudoku::Sudoku::close()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mRenderer = NULL;
	mWindow = NULL;
	TTF_CloseFont(mFont);
	mFont = NULL;
	SDL_Quit();
	TTF_Quit();
}
