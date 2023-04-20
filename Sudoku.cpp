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
	  FontSize(GRID_WIDTH/15),
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
    // Initalise SDL video subsystem
	if(SDL_Init(SDL_INIT_VIDEO) !=0){
		std::cout<<"SDL couldn't initialize: "<<SDL_GetError()<<std::endl;
		return 0;
	}
    // Initialise SDL_ttf
	if(TTF_Init()==-1){
		std::cout<<"SDL_ttf couldn't initialize!: "<<SDL_GetError()<<std::endl;
		return 0;
	}
    // Create window
	window = SDL_CreateWindow("SudokuM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (window == NULL){
		std::cout << "SDL could not create window! Error: " << SDL_GetError() << std::endl;
		return 0;
	}
    // Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		return 0;
	}
    // Load font
	font = TTF_OpenFont("fonts/octinsports.ttf", FontSize);
	if (font == nullptr){
		std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
		return 0;
	}

	SDL_SetRenderDrawColor(renderer, 11,2,59, 255);
	return 1;

}

void Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColor){
	// Create text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, fontColor);
	if (textSurface == NULL){
		std::cout << "Could not create SDL_Surface! Error: " <<" \""<<text<<"\""<< TTF_GetError() << std::endl;
	}
	else{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL){
			std::cout << "Could not create texture from surface! Error: " <<" \""<<text<<"\"" << SDL_GetError() << std::endl;
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
	// Choose colour of font
	SDL_Color fontColourForButtons = { 255, 255, 255, 255 }; // white
	SDL_Color fontColourForCells = { 96, 94, 94, 255}; //grey
	SDL_Color fontColourWhenWrong = { 211, 70, 70, 255 }; //red

	// Load texture for empty space
	loadTexture(TextureCache[0], " ", fontColourForButtons);

	// Load textures for numbers from 1 to 9
	for (int num = 1; num < 10; num++){
		const char temp[] = { '0' + num, '\0'};
		loadTexture(TextureCache[num], temp, fontColourForCells);
	}

	// Load texture for "Check", "Menu", and "New" buttons
	loadTexture(TextureCache[10], "Pause", fontColourForButtons);
	loadTexture(TextureCache[11], "New", fontColourForButtons);
	loadTexture(TextureCache[12], "Time taken: ", fontColourForButtons);
	loadTexture(TextureCache[13], "Unpause", fontColourForButtons);
	loadTexture(TextureCache[14], "Hint", fontColourForButtons);
	loadTexture(TextureCache[15], "Play", fontColourForButtons); // DON'T CHANGE TextureCache index of play, instr, exit
	loadTexture(TextureCache[16], "Instr", fontColourForButtons);
	loadTexture(TextureCache[17], "Exit", fontColourForButtons);
	loadTexture(TextureCache[18], "Easy", fontColourForButtons);
	loadTexture(TextureCache[19], "Medium", fontColourForButtons);
	loadTexture(TextureCache[20], "Hard", fontColourForButtons);

	//loadTexture(TextureCache[30], "Mistakes 0/3", fontColourForButtons)

	for (int num = 1; num < 10; num++) {
        const char temp[] = { '0' + num, '\0'};
        loadTexture(TextureCache[num + 20], temp, fontColourWhenWrong); //TextureCache with index from 21 to 29
	}

	// Load textures for instr image
	ImageTextures[0] = loadImageTexture("images/Instruction.jpg", renderer);
	ImageTextures[1] = loadImageTexture("images/GameIcon.png", renderer);
	ImageTextures[2] = loadImageTexture("images/ChooseLevel.png", renderer);
	ImageTextures[3] = loadImageTexture("images/GameOver.png", renderer);
}

void Sudoku::createLayout(){
	// TODO: change 3's and 6's in this function

	const int thinBorder =2;
	const int thickBorder = thinBorder + 6;

	int buttonStartRow=0, buttonWidth=0, buttonHeight=0;
	buttonHeight = (WINDOW_HEIGHT - 6 * thinBorder - 6 * thickBorder) / (GRID_ROWS+2);

	buttonWidth = (GRID_WIDTH - 6*thinBorder - 4*thickBorder)/GRID_COLS ;
	buttonStartRow = 0;

	for (int row = 0; row < GRID_ROWS; row++){
		// Add to starting row
		if (row == 0) buttonStartRow += thickBorder;
		else if (row % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
		else buttonStartRow += buttonHeight + thinBorder;

		// Reset starting column
		int buttonStartCol = 0;

		for (int col = 0; col < GRID_COLS; col++)
		{
			// Add to starting column
			if (col == 0) buttonStartCol += thickBorder;
			else if (col % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
			else buttonStartCol += buttonWidth + thinBorder;

			// Set button position and dimensions
			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = getIndex(row, col);
			grid[index].setButtonRect(buttonRect);
		}
	}

	buttonStartRow += (buttonHeight + thickBorder);

	// About Watch

	SDL_Rect buttonRect = {thickBorder, buttonStartRow, (GRID_WIDTH - 2 * thickBorder), buttonHeight };
	Timer.setButtonRect(buttonRect);


	// Watch ends


	const int numberOfOtherButtons = 4;
	PauseButton.setTexture(TextureCache[10]);
	HintButton.setTexture(TextureCache[14]);
	NewButton.setTexture(TextureCache[11]);
	ExitButton.setTexture(TextureCache[17]);
	Button* otherButtons[numberOfOtherButtons] = { &PauseButton, &NewButton , &HintButton, &ExitButton};

	buttonWidth = (GRID_WIDTH - (numberOfOtherButtons+1) * thickBorder) / numberOfOtherButtons;

	buttonStartRow += (buttonHeight + thickBorder);

	// Reset starting column
	int borderWidthTotal = 0;

	// Set check, menu, and new buttons (last row)
	for (int button = 0; button < numberOfOtherButtons; button++){
		// Add border width to total
		if (button == 0) borderWidthTotal += thickBorder;
		else borderWidthTotal += thickBorder;
		int buttonStartCol = button * buttonWidth + borderWidthTotal;

		// Set button position and dimensions
		SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
		otherButtons[button]->setButtonRect(buttonRect);
		otherButtons[button]->centerTextureRect();
	}

	// printf("Layout Created!\n");

}

void Sudoku::freeTextures(){

	for (int i = 0; i < TotalTextures; i++){
		// Free texture if it exists
		if (TextureCache[i] != nullptr){
			SDL_DestroyTexture(TextureCache[i]);
			TextureCache[i] = nullptr;
		}
	}

	for (int i = 0; i < 4; i++){
		if (ImageTextures[i] != nullptr){
			SDL_DestroyTexture(ImageTextures[i]);
			ImageTextures[i] = NULL;
		}
	}

}

bool Sudoku::showtillesc(int textureid){
	bool showing =1;
	SDL_Event e;
	while(showing){
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, ImageTextures[textureid], NULL, NULL);
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&e)){
			if(e.key.keysym.scancode==SDL_SCANCODE_ESCAPE){
				showing=0;

				std::cout << "tf";
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
	if(showtillesc(0) == 0) running = 0;

}

void Sudoku::menu(){
	// Initialise SDL
	if (!init()){
		close();
	}

	// Preload textures for menu, grid, buttons and images
	preloadTextures();

	// int temp=1;
	// play(temp);   // FOR DEBUGGING PURPOSE
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

	// listen to events
	SDL_Event event;
	int menuRunning=1;
	while(menuRunning){
		// play(menuRunning);
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
	// Free button textures
	freeTextures();

	// Destroy and quit
	close();
}

void Sudoku::chooseLevel(int &chooseLevelRunning) {

	// Preload textures for menu, grid, buttons and images
	preloadTextures();

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
	int levelRunning=1;
	int const level1=1;
	int const level2=2;
	int const level3=3;
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
                break;
            }

            if (EasyButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level1);
                //while(levelRunning == 2) play(levelRunning = 1);
            }

            if (MediumButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level2);
                //while(levelRunning == 2) play(levelRunning = 1);
            }

            if (HardButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                play(levelRunning, level3);
                //while(levelRunning == 2) play(levelRunning = 1);
            }
        }

	}
	// Free button textures
	freeTextures();

	// Destroy and quit
	close();

}

void Sudoku::generateSudoku(int lvl){

	// Create empty an empty grid to store generated Sudoku
	int generatedGrid[81] = { };


	// Instantiate a Sudoku generator object and generate Sudoku with the empty grids

	G.generate(generatedGrid, solution, lvl);

	for (int i = 0; i < 81; i++){
		// Set number and solution
		grid[i].setNumber(generatedGrid[i]);

		// Set editability
		if (generatedGrid[i] == 0){
			// This is a cell that can be editable
			grid[i].setEditable(true);
		}
		else{
			// This cell is fixed and cannot be edited
			grid[i].setEditable(false);
		}

		// Set texture (0 = ' ', 1 to 9 = '1' '2'... '9')
		grid[i].setTexture(TextureCache[generatedGrid[i]]);

		// Center texture onto button
		grid[i].centerTextureRect();
	}
}

void Sudoku::gameOver(bool &gameOverRunning) {
    //preloadTextures();
    const int GameOverButtonSpace = 100;
    const int GameOverButtonWidth = 220, GameOverButtonHeight = 80;
	const int totalGameOverButtons = 3;
	const int GameOverButtonPosition_x = WINDOW_WIDTH/2 - GameOverButtonWidth/2;
    int GameOverButtonPosition_y = WINDOW_HEIGHT/2 + GameOverButtonHeight/2 - GameOverButtonSpace * 2 + 100;


	TimeTakenButton.setTexture(TextureCache[12]);
	NewButton.setTexture(TextureCache[11]);
	ExitButton.setTexture(TextureCache[17]);

	Button* GameOverButtons[totalGameOverButtons] = {&TimeTakenButton, &NewButton , &ExitButton};

	for (int button = 0; button < totalGameOverButtons; button++) {
        SDL_Rect buttonRect = {GameOverButtonPosition_x, GameOverButtonPosition_y, GameOverButtonWidth, GameOverButtonHeight};
        GameOverButtonPosition_y = GameOverButtonPosition_y + GameOverButtonSpace;
        GameOverButtons[button]->setButtonRect(buttonRect);
        GameOverButtons[button]->centerTextureRect();
	}

	SDL_Event event;
	int looseRunning = 1;
	while (gameOverRunning) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Rect gameOverRect;
		SDL_QueryTexture(ImageTextures[3], NULL, NULL, &gameOverRect.w, &gameOverRect.h);
		gameOverRect.w = gameOverRect.w / 1.4;
        gameOverRect.h = gameOverRect.h / 1.4;
		gameOverRect.x = WINDOW_WIDTH / 2 - gameOverRect.w / 2;
        gameOverRect.y = WINDOW_HEIGHT / 2 - 400;
		SDL_RenderCopy(renderer, ImageTextures[3], NULL, &gameOverRect);

        TimeTakenButton.renderButton(renderer);
        TimeTakenButton.renderTexture(renderer);
        NewButton.renderButton(renderer);
        NewButton.renderTexture(renderer);
        ExitButton.renderButton(renderer);
        ExitButton.renderTexture(renderer);

        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT || ExitButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN ) {
                //looseRunning = 0;
                gameOverRunning = false;
                std::cout << "w" << std::endl;
                //gameOverRunning = 0;
                break;
                //return;
            }

            if (NewButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                chooseLevel(looseRunning);
            }

        }

	}
    // Free button textures
	freeTextures();

	// Destroy and quit
	close();

}

void Sudoku::play(int &running, int lvl){

	// Create interface layout
	createLayout();

	generateSudoku(lvl);

	// Enable text input
	SDL_StartTextInput();

	// Set first current cell selected
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

	// Loop variables
	SDL_Event event;
	bool gameRunning = 1;
	//bool checkSolution=0;
	bool completed=0;
	bool CompletionFlag=0;
	time_t startTimer= time(NULL);
	int mistakescnt = 0;
//	long long checkLightTime=1e10;
	char timer[10];
    int hints = 3;
	while(gameRunning){

		while (SDL_PollEvent(&event) != 0){
			if (event.type == SDL_QUIT || ExitButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
				gameRunning = 0;
				running=0;
				break;
			}

			// Handle mouse event for "Check" button
//			if (CheckButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
//				checkSolution = true;
//			}
            if (PauseButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN) {
                PauseButton.setTexture(TextureCache[13]);
            }

            //Handle mouse event for "Hint" button
//            while(hints>0){
            if (hints > 0) {
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

                }
            }

			// Handle mouse event for "New" button
			if (NewButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
				running=2;																	// TODO: Convert this to emum
				return;
			}

			for (int cell = 0; cell < TotalCells; cell++){
				if (grid[cell].isEditable){
					// Set button state and return if mouse pressed on cell
					if (grid[cell].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN){
						// Set current cell selected to false
						currentCellSelected->setSelected(false);
						// Set new cell selected to true
						currentCellSelected = &grid[cell];
						currentCellSelected->setSelected(true);
						selectedCellId=cell;
					}
				}
			}
			// Handle keyboard events for current cell selected
			int cnt = 0;
            currentCellSelected->handleKeyboardEvent(&event, TextureCache, solution[selectedCellId], cnt);
            if (cnt == 1){
                mistakescnt++;

                if (mistakescnt == 81 ) {
                    gameOver(gameRunning);
                    std::cout << "help";
                    break;

                }
            }
//            if (mistakescnt == 3) {
//                gameOver(gameRunning);
//                running = 0;
//                completed = 1;
//                return;
//                std::cout << "help";
//            }
            G.setElement(selectedCellId/9, selectedCellId%9, grid[selectedCellId].getNumber());

		}



		// If "Check" button was clicked
//		if (checkSolution){
//			// Check if complete
//			checkLightTime = min(checkLightTime, (long long)(difftime(time(NULL) , startTimer)+1ll));
//			if(P.checker(0,0,1))
//				completed = true;
//			else{
//				// Set texture to "Wrong!"
//			CheckButton.setTexture(TextureCache[13]);
//			}
//
//		}

		if(completed && CompletionFlag==0){
			CompletionFlag=1;
			std::string temp_timer= "Time Taken: " + std::to_string(totalTime);
            int length= temp_timer.size();
            for(int pos=0;pos<length;pos++){
                timer[pos]=temp_timer[pos];
            }
            timer[length]='\0';
			for(int cell=0;cell<TotalCells;cell++){
				grid[cell].isEditable=0;
				grid[cell].setSelected(0);
			}
			// Set texture to "Right!"
//			CheckButton.setTexture(TextureCache[14]);
		}

//		if(completed)
//			SDL_SetRenderDrawColor(renderer, 0, 125, 19, 255);
//		else if(checkSolution)
//			SDL_SetRenderDrawColor(renderer,  125, 0, 0, 255);
//		else
        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);

		SDL_RenderClear(renderer);
		for (int cell = 0; cell < TotalCells; cell++){
			grid[cell].renderButton(renderer);
			grid[cell].centerTextureRect();
			grid[cell].renderTexture(renderer);
		}

		if(!completed){
			long long seconds =difftime(time(NULL) , startTimer);
			int hr=seconds/3600;
			int mi=(seconds%3600)/60;
			int sec= (seconds%3600)%60;
			totalTime = hr*10000 + mi*100 + sec ;
			std::string temp_timer= std::to_string(hr)+":"+std::to_string(mi)+":"+std::to_string(sec);
			int length= temp_timer.size();
			for(int pos=0;pos<length;pos++){
				timer[pos]=temp_timer[pos];
			}
			timer[length]='\0';
//			if(checkSolution){

//			if(difftime(time(NULL) , startTimer) >= checkLightTime){
//					checkLightTime=1e10;
//					checkSolution = false;
//                    CheckButton.setTexture(TextureCache[10]);
//				}
//          }
		}

//  COPY FROM HERE
        SDL_Texture* timerTexture = NULL;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black
		loadTexture(timerTexture, timer, fontColour);
		Timer.setTexture(timerTexture);
		Timer.renderButton(renderer);
		Timer.centerTextureRect();
		Timer.renderTexture(renderer);
		SDL_DestroyTexture(timerTexture);
		timerTexture = NULL;

        PauseButton.renderButton(renderer);
		PauseButton.renderTexture(renderer);

		// Render new button
		NewButton.renderButton(renderer);
		NewButton.renderTexture(renderer);

		ExitButton.renderButton(renderer);
		ExitButton.renderTexture(renderer);

		HintButton.renderButton(renderer);
		HintButton.renderTexture(renderer);

		SDL_RenderPresent(renderer);


	}
    // Disable text input
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
