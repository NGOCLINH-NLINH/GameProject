#pragma once
#include<iostream>
#include<SDL_ttf.h>
#include<SDL.h>
#include<SDL_image.h>
#include<stdlib.h>
#include<vector>
#include <sstream>
#include "Button.h"
#include "gridcell.h"
#include "SudokuGenerator2.h"
#include "TimeTicks.h"

class Sudoku{
	const int WINDOW_HEIGHT, WINDOW_WIDTH ;
	const int GRID_HEIGHT, GRID_WIDTH ;
	const int GRID_ROWS, GRID_COLS ;
	int totalTime;
	int solution[81] = { };

	SDL_Window* window;
	SDL_Renderer* renderer;

	int TotalTextures;
	SDL_Texture* TextureCache[39];
	SDL_Texture* ImageTextures[3];

	TTF_Font* font;
	int FontSize;

	const int TotalCells;

	Cell grid[81];

	Button NewButton;
	Button HintButton;
	Button PauseButton;
	Button Timer;
	Button CountMistakes;
	Button CountHints;

	Button PlayButton;
	Button InstrButton;
	Button ExitButton;

	//Buttons of Level
	Button EasyButton;
	Button MediumButton;
	Button HardButton;


	SDL_Color ClearColor;

	Generator G;


private:

	bool init();
	int getIndex(int row, int col) const;
	void loadTexture(SDL_Texture*& ,const char* , SDL_Color& fontColor);
	void loadFromRenderedText(SDL_Texture*&, std::string , SDL_Color& fontColor);
	void preloadTextures();
	void createLayout();
	void freeTextures();

	SDL_Texture* loadImageTexture(const std::string path, SDL_Renderer* renderer);
	void instr(int&);
	int showtillesc(int);
	void generateSudoku(int lvl, int& emptyCells);


    bool pause;
//    int time;
    TimeTicks timer;

    bool showing = false;
	int menuRunning=0;
	int levelRunning=0;
	int gameRunning = 0;

	int level1=1;
	int level2=2;
	int level3=3;



public:
	Sudoku();
	~Sudoku();

	void menu();

	void chooseLevel(int &);

	void gameOver(bool &);

	void play(int &, int lvl);

	void close();

};

