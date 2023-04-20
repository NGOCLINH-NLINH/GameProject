#pragma once
#include<iostream>
#include<SDL_ttf.h>
#include<SDL.h>
#include<SDL_image.h>
#include<stdlib.h>
#include<vector>
#include "Button.h"
#include "gridcell.h"
#include "SudokuGenerator.h"

class Sudoku{
	const int WINDOW_HEIGHT, WINDOW_WIDTH ;
	const int GRID_HEIGHT, GRID_WIDTH ;
	const int GRID_ROWS, GRID_COLS ;
	int totalTime;
//	int SCORE;
	int solution[81] = { };

	SDL_Window* window;
	SDL_Renderer* renderer;

	// Total textures required
	// and the array of those many textures
	int TotalTextures;
	SDL_Texture* TextureCache[30];
	SDL_Texture* ImageTextures[4];

	TTF_Font* font;
	int FontSize;

	const int TotalCells;

	// Define grid of cells (normally set to 9 x 9 = 81)
	Cell grid[81];

	// Define hint and pause and new buttons
	Button PauseButton;
	Button NewButton;
	Button HintButton;
	Button Timer;

	// Buttons of Menu
	Button PlayButton;
	Button InstrButton;
	Button ExitButton; // common button

	//Buttons of Level
	Button EasyButton;
	Button MediumButton;
	Button HardButton;

	Button TimeTakenButton;

	SDL_Color ClearColor;

	Generator G;


private:

	bool init();
	int getIndex(int row, int col) const;
	void loadTexture(SDL_Texture*& ,const char* , SDL_Color& fontColor);
	void preloadTextures();
	void createLayout();
	void freeTextures();
	//void loadImage(SDL_Texture*& ,const char* );
	SDL_Texture* loadImageTexture(const std::string path, SDL_Renderer* renderer);
	void instr(int&);
	bool showtillesc(int);
	void generateSudoku(int lvl);

public:
	Sudoku();
	~Sudoku();

	void menu();

	void chooseLevel(int &);

	void gameOver(bool &);

	void play(int &, int lvl);

	void close();

};

// class Option{
// 	int x,y;
// }
