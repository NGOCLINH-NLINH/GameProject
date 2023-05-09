#include "Sudoku.h"

Sudoku::Sudoku():
	  WINDOW_HEIGHT(880),
	  WINDOW_WIDTH(720),
	  GRID_HEIGHT(720),
	  GRID_WIDTH(720),
	  GRID_ROWS(9),
	  GRID_COLS(9),
	  window(NULL),
	  renderer(NULL),
	  TotalTextures(30),
	  TextureCache{ NULL },
	  ImageTextures{NULL},
	  font(NULL),
	  FontSize(GRID_WIDTH/18),
	  TotalCells(81)
{}

Sudoku::~Sudoku() {
	freeTextures();
	close();
}

int Sudoku::getIndex(int row, int col) const {
	return row * GRID_ROWS + col;
}

bool Sudoku::init(){

	if(SDL_Init(SDL_INIT_VIDEO) !=0){
		std::cout<<"SDL couldn't initialize: "<<SDL_GetError()<<std::endl;
		return 0;
	}
	if(TTF_Init()==-1){
		std::cout<<"SDL_ttf couldn't initialize!: "<<SDL_GetError()<<std::endl;
		return 0;
	}

	window = SDL_CreateWindow("SudokuM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (window == NULL){
		std::cout << "SDL could not create window! Error: " << SDL_GetError() << std::endl;
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		return 0;
	}

	font = TTF_OpenFont("fonts/octinsports.ttf", FontSize);
	if (font == nullptr){
		std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
		return 0;
	}

	SDL_SetRenderDrawColor(renderer, 11,2,59, 255);
	return 1;

}

void Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColor){

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, fontColor);
	if (textSurface == NULL){
		std::cout << "Could not create SDL_Surface! Error: " <<" \""<<text<<"\""<< TTF_GetError() << std::endl;
	}
	else{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL){
			std::cout << "Could not create texture from surface! Error: " <<" \""<<text<<"\"" << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);
	}
}

void Sudoku::loadFromRenderedText(SDL_Texture*& texture, std::string textureText, SDL_Color& fontColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), fontColor );
    if (textSurface == NULL){
		std::cout << "Could not create SDL_Surface! Error: " <<" \""<<textureText.c_str()<<"\""<< TTF_GetError() << std::endl;
	}
	else{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL){
			std::cout << "Could not create texture from surface! Error: " <<" \""<< textureText.c_str() <<"\"" << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);
	}
}

SDL_Texture* Sudoku::loadImageTexture(const std::string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
        std::cout << "Unable to load image" << path << " SDL_image Error: "
             << IMG_GetError() << std::endl;
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
            std::cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Sudoku::preloadTextures(){
	SDL_Color fontColourForButtons = { 255, 255, 255, 255 }; // white
	SDL_Color fontColourForCells = { 96, 94, 94, 255}; //grey
	SDL_Color fontColourWhenWrong = { 211, 70, 70, 255 }; //red
	SDL_Color fontColourWhenWin = {11,83,148,255}; //blue

	loadFromRenderedText(TextureCache[0], " ", fontColourForButtons);

	for (int num = 1; num < 10; num++){
		const char temp[] = { '0' + num, '\0'};
		loadTexture(TextureCache[num], temp, fontColourForCells);
	}

	loadTexture(TextureCache[10], "Pause", fontColourForButtons);
	loadTexture(TextureCache[11], "New", fontColourForButtons);
	loadTexture(TextureCache[12], "GAME OVER", fontColourWhenWrong);
	loadTexture(TextureCache[13], "Unpause", fontColourForButtons);
	loadTexture(TextureCache[14], "Hint", fontColourForButtons);
	loadTexture(TextureCache[15], "Play", fontColourForButtons);
	loadTexture(TextureCache[16], "Instr", fontColourForButtons);
	loadTexture(TextureCache[17], "Exit", fontColourForButtons);
	loadTexture(TextureCache[18], "Easy", fontColourForButtons);
	loadTexture(TextureCache[19], "Medium", fontColourForButtons);
	loadTexture(TextureCache[20], "Hard", fontColourForButtons);
	loadTexture(TextureCache[30], "Mistakes: 0/3", fontColourForButtons);
	loadTexture(TextureCache[31], "Mistakes: 1/3", fontColourForButtons);
	loadTexture(TextureCache[32], "Mistakes: 2/3", fontColourForButtons);
	loadTexture(TextureCache[33], "Mistakes: 3/3", fontColourWhenWrong);
	loadTexture(TextureCache[34], "Hints: 3/3", fontColourForButtons);
	loadTexture(TextureCache[35], "Hints: 2/3", fontColourForButtons);
	loadTexture(TextureCache[36], "Hints: 1/3", fontColourForButtons);
	loadTexture(TextureCache[37], "Hints: 0/3", fontColourForButtons);
	loadTexture(TextureCache[38], "YOU WIN", fontColourWhenWin);

	for (int num = 1; num < 10; num++) {
        const char temp[] = { '0' + num, '\0'};
        loadTexture(TextureCache[num + 20], temp, fontColourWhenWrong); //TextureCache with index from 21 to 29
	}

	ImageTextures[0] = loadImageTexture("images/Instruction.png", renderer);
	ImageTextures[1] = loadImageTexture("images/GameIcon.png", renderer);
	ImageTextures[2] = loadImageTexture("images/ChooseLevel.png", renderer);
}

void Sudoku::createLayout(){
    const int thinBorder =2;
	const int thickBorder = thinBorder + 6;

	int buttonStartRow=0, buttonWidth=0, buttonHeight=0;
	buttonHeight = (WINDOW_HEIGHT - 6 * thinBorder - 6 * thickBorder) / (GRID_ROWS+2);

	buttonWidth = (GRID_WIDTH - 6*thinBorder - 4*thickBorder)/GRID_COLS ;
	buttonStartRow = 0;

	for (int row = 0; row < GRID_ROWS; row++){
		if (row == 0) buttonStartRow += thickBorder;
		else if (row % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
		else buttonStartRow += buttonHeight + thinBorder;

		int buttonStartCol = 0;

		for (int col = 0; col < GRID_COLS; col++)
		{
			if (col == 0) buttonStartCol += thickBorder;
			else if (col % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
			else buttonStartCol += buttonWidth + thinBorder;

			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = getIndex(row, col);
			grid[index].setButtonRect(buttonRect);
		}
	}

	const int numberOfFixedButtons = 3;

	CountMistakes.setTexture(TextureCache[30]);
	CountHints.setTexture(TextureCache[34]);

	Button* fixedButton[numberOfFixedButtons] = {&Timer, &CountMistakes, &CountHints};

	buttonWidth = GRID_WIDTH / numberOfFixedButtons;

	buttonStartRow += (buttonHeight + thickBorder);

	int borderWidthTotal = 0;

	for (int button = 0; button < numberOfFixedButtons; button++){
		int buttonStartCol = button * buttonWidth;

		SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
		fixedButton[button]->setButtonRect(buttonRect);
		fixedButton[button]->centerTextureRect();
	}


    const int numberOfOtherButtons = 4;

    PauseButton.setTexture(TextureCache[10]);
	HintButton.setTexture(TextureCache[14]);
	NewButton.setTexture(TextureCache[11]);
	ExitButton.setTexture(TextureCache[17]);

	Button* otherButtons[numberOfOtherButtons] = {&PauseButton, &NewButton , &HintButton, &ExitButton};

	buttonWidth = (GRID_WIDTH - (numberOfOtherButtons+1) * thickBorder) / numberOfOtherButtons;

	buttonStartRow += (buttonHeight + thickBorder);

    borderWidthTotal = 0;

	for (int button = 0; button < numberOfOtherButtons; button++){

		if (button == 0) borderWidthTotal += thickBorder;
		else borderWidthTotal += thickBorder;
		int buttonStartCol = button * buttonWidth + borderWidthTotal;

		SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
		otherButtons[button]->setButtonRect(buttonRect);
		otherButtons[button]->centerTextureRect();
	}
}

void Sudoku::freeTextures(){
    for (int i = 0; i < TotalTextures; i++){
		if (TextureCache[i] != nullptr){
			SDL_DestroyTexture(TextureCache[i]);
			TextureCache[i] = nullptr;
		}
	}
    for (int i = 0; i < 3; i++){
		if (ImageTextures[i] != nullptr){
			SDL_DestroyTexture(ImageTextures[i]);
			ImageTextures[i] = NULL;
		}
	}
}

int Sudoku::showtillesc(int textureid){
    showing = true;
	SDL_Event e;
	while(showing){
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, ImageTextures[textureid], NULL, NULL);
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&e)){
			if(e.key.keysym.scancode==SDL_SCANCODE_ESCAPE){
				showing=0;
			}
			if(e.type==SDL_QUIT){
				showing=0;
				return 0;
			}
		}
	}
	return 1;
}

void Sudoku::instr(int& running){
    running = showtillesc(0);
}

void Sudoku::menu(){

	if (!init()){
		close();
	}

	preloadTextures();

    const int OptionSpace = 100;
    const int OptionWidth = 220, OptionHeight = 80;
	const int totalOptions = 3;
	const int OptionPosition_x = WINDOW_WIDTH/2 - OptionWidth/2;
    int OptionPosition_y = WINDOW_HEIGHT/2 + OptionHeight/2 - OptionSpace * 2 + 100;


	PlayButton.setTexture(TextureCache[15]);
	InstrButton.setTexture(TextureCache[16]);
	ExitButton.setTexture(TextureCache[17]);

	Button* menuButtons[totalOptions] = { &PlayButton, &InstrButton , &ExitButton};

	for (int button = 0; button < totalOptions; button++) {
        SDL_Rect buttonRect = {OptionPosition_x, OptionPosition_y, OptionWidth, OptionHeight};
        OptionPosition_y = OptionPosition_y + OptionSpace;
        menuButtons[button]->setButtonRect(buttonRect);
        menuButtons[button]->centerTextureRect();
	}

	SDL_Event event;
	menuRunning = 1;
	while(menuRunning){

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);


		SDL_Rect backgroundRect;
		SDL_QueryTexture(ImageTextures[1], NULL, NULL, &backgroundRect.w, &backgroundRect.h);
		backgroundRect.w = backgroundRect.w / 1.5;
        backgroundRect.h = backgroundRect.h / 1.5;
		backgroundRect.x = WINDOW_WIDTH / 2 - backgroundRect.w / 2;
        backgroundRect.y = WINDOW_HEIGHT / 2 - 350;
		SDL_RenderCopy(renderer, ImageTextures[1], NULL, &backgroundRect);

        PlayButton.renderButton(renderer);
        PlayButton.renderTexture(renderer);
        InstrButton.renderButton(renderer);
        InstrButton.renderTexture(renderer);
        ExitButton.renderButton(renderer);
        ExitButton.renderTexture(renderer);

        SDL_RenderPresent(renderer);
		if(SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    menuRunning = 0;
                    break;
                }

                if (PlayButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
                    chooseLevel(menuRunning);
                }

                if (InstrButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
                    instr(menuRunning);

                }

                if (ExitButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
                    menuRunning = 0;
                }
		}
    }

	freeTextures();
	close();
}

void Sudoku::chooseLevel(int &chooseLevelRunning) {

    const int LevelSpace = 100;
    const int LevelWidth = 220, LevelHeight = 80;
	const int totalLevels = 3;
	const int LevelPosition_x = WINDOW_WIDTH/2 - LevelWidth/2;
    int LevelPosition_y = WINDOW_HEIGHT/2 + LevelHeight/2 - LevelSpace * 2 + 100;

	EasyButton.setTexture(TextureCache[18]);
	MediumButton.setTexture(TextureCache[19]);
	HardButton.setTexture(TextureCache[20]);

	Button* LevelButtons[totalLevels] = { &EasyButton, &MediumButton , &HardButton};

	for (int button = 0; button < totalLevels; button++) {
        SDL_Rect buttonRect = {LevelPosition_x, LevelPosition_y, LevelWidth, LevelHeight};
        LevelPosition_y = LevelPosition_y + LevelSpace;
        LevelButtons[button]->setButtonRect(buttonRect);
        LevelButtons[button]->centerTextureRect();
	}

	SDL_Event event;

    levelRunning = chooseLevelRunning;
	while (levelRunning) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Rect backgroundRect;
		SDL_QueryTexture(ImageTextures[2], NULL, NULL, &backgroundRect.w, &backgroundRect.h);
		backgroundRect.w = backgroundRect.w / 1.4;
        backgroundRect.h = backgroundRect.h / 1.4;
		backgroundRect.x = WINDOW_WIDTH / 2 - backgroundRect.w / 2;
        backgroundRect.y = WINDOW_HEIGHT / 2 - 400;
		SDL_RenderCopy(renderer, ImageTextures[2], NULL, &backgroundRect);

        EasyButton.renderButton(renderer);
        EasyButton.renderTexture(renderer);
        MediumButton.renderButton(renderer);
        MediumButton.renderTexture(renderer);
        HardButton.renderButton(renderer);
        HardButton.renderTexture(renderer);

        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                levelRunning = 0;
                chooseLevelRunning = 0;
                break;
            }

            if (EasyButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level1);
            }

            if (MediumButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level2);
            }

            if (HardButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level3);
            }
        }

	}
	freeTextures();

	close();

}

void Sudoku::generateSudoku(int lvl, int& emptyCells){
	int generatedGrid[81] = { };

	G.generate(generatedGrid, solution, lvl);

	int tmp = 0;
	for (int i = 0; i < 81; i++){
		grid[i].setNumber(generatedGrid[i]);
		if (generatedGrid[i] == 0){
			grid[i].setEditable(true);
			tmp++;
		}
		else{
			grid[i].setEditable(false);
		}
		grid[i].setTexture(TextureCache[generatedGrid[i]]);
		grid[i].centerTextureRect();
	}
	emptyCells = tmp;
}

void Sudoku::play(int &playRunning, int lvl){

    int hints = 3;
    int mistakescnt = 0;
    int emptyNums = 0;
    timer.start();

	createLayout();

	generateSudoku(lvl, emptyNums);

	SDL_StartTextInput();

	Cell* currentCellSelected;
	int selectedCellId=0;
	bool init_selection=0;
	for (int cell = 0; cell < TotalCells; cell++){
		if (!init_selection && grid[cell].isEditable){
			currentCellSelected = &grid[cell];
			currentCellSelected->setSelected(true);
			init_selection=1;
			selectedCellId=cell;
		}
		else{
			grid[cell].setSelected(0);
		}
	}

	SDL_Event event;
	int pause_tmp = 0;
	pause = 0;

    bool LOOSE = 0;
    bool WIN = 0;

    stringstream timeText;

    gameRunning = 1;
	while(gameRunning){

		while (SDL_PollEvent(&event) != 0){
			if (event.type == SDL_QUIT || ExitButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
				gameRunning = 0;
				playRunning = 0;
				menuRunning = 0;

				break;
			}

			if (PauseButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                pause_tmp++;
                if (pause_tmp%2 != 0) {
                    PauseButton.setTexture(TextureCache[13]);
                    timer.pause();
                    pause = true;
                }
                else {
                    PauseButton.setTexture(TextureCache[10]);
                    timer.unpause();
                    pause = false;
                }
			}

            if (hints > 0 && pause == false) {
                if (HintButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                    std::vector<int> tamj;
                    for (int i = 0; i < 81; i++) {
                        if (grid[i].getNumber() == 0) tamj.push_back(i);
                    }
                    int rantamj = rand() % (tamj.size() - 1);
                    grid[tamj[rantamj]].setNumber(solution[tamj[rantamj]]);
                    grid[tamj[rantamj]].setTexture(TextureCache[solution[tamj[rantamj]]]);
                    grid[tamj[rantamj]].centerTextureRect();
                    hints--;
                    emptyNums--;
                    CountHints.setTexture(TextureCache[37-hints]);
                }
            }

			if (NewButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
				playRunning=1;
				return;
			}
            if (pause ==  false) {
			for (int cell = 0; cell < TotalCells; cell++){
				if (grid[cell].isEditable){

					if (grid[cell].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){

						currentCellSelected->setSelected(false);

						currentCellSelected = &grid[cell];
						currentCellSelected->setSelected(true);
						selectedCellId=cell;
					}
				}
			}
            }
			int wrongcnt = 0;
            currentCellSelected->handleKeyboardEvent(&event, TextureCache, solution[selectedCellId], wrongcnt);
            G.setElement(selectedCellId/9, selectedCellId%9, grid[selectedCellId].getNumber());
            int cnt = 0;
            for (int i = 0; i < 81; i++) {

                if (grid[i].getNumber() != 0) cnt++;
            }
            if (cnt == 81) WIN = true;
            if (wrongcnt == 1) {
                ++mistakescnt;
                CountMistakes.setTexture(TextureCache[30 + mistakescnt]);
            }
            if (mistakescnt == 3) LOOSE = true;

        }


        if(pause==0){
            timeText.str( "" );
            if(int( timer.getTicks() / 1000.f)/60 < 10){
                timeText << "0" << (int( timer.getTicks() / 1000.f)/60 ) ;
            }
            else{
                timeText << (int( timer.getTicks() / 1000.f)/60 ) ;
            }
            timeText << ":";
            if(int( timer.getTicks() / 1000.f)%60 < 10){
                timeText << "0" << (int( timer.getTicks() / 1000.f)%60 ) ;
            }
            else{
                timeText << (int( timer.getTicks() / 1000.f)%60 ) ;
            }
		}


		if(LOOSE || WIN){
            timer.pause();
            for(int cell=0;cell<TotalCells;cell++){
                grid[cell].isEditable=0;
                grid[cell].setSelected(0);
            }
			if (LOOSE) CountMistakes.setTexture(TextureCache[12]);
			if (WIN) CountMistakes.setTexture(TextureCache[38]);
		}



        if (LOOSE) SDL_SetRenderDrawColor(renderer,  125, 0, 0, 255);
        else if (WIN) SDL_SetRenderDrawColor(renderer, 11,83,148,255);
        else SDL_SetRenderDrawColor(renderer, 0,0,0, 255);


		SDL_RenderClear(renderer);

		for (int cell = 0; cell < TotalCells; cell++){
			grid[cell].renderButton(renderer);
			grid[cell].centerTextureRect();
			grid[cell].renderTexture(renderer);
		}

        SDL_Texture* timerTexture = NULL;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE };
        loadFromRenderedText(timerTexture, timeText.str().c_str(), fontColour);
		Timer.setTexture(timerTexture);
		Timer.renderButton(renderer);
		Timer.centerTextureRect();
		Timer.renderTexture(renderer);
		SDL_DestroyTexture(timerTexture);
		timerTexture = NULL;

		CountHints.renderButton(renderer);
		CountHints.renderTexture(renderer);

		CountMistakes.renderButton(renderer);
		CountMistakes.renderTexture(renderer);

		PauseButton.renderButton(renderer);
		PauseButton.renderTexture(renderer);

		NewButton.renderButton(renderer);
		NewButton.renderTexture(renderer);

		ExitButton.renderButton(renderer);
		ExitButton.renderTexture(renderer);

		HintButton.renderButton(renderer);
		HintButton.renderTexture(renderer);


		SDL_RenderPresent(renderer);
    }

	SDL_StopTextInput();
}

void Sudoku::close(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	TTF_CloseFont(font);
	font = nullptr;

	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}
