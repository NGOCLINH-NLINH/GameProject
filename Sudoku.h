#pragma once
#include<iostream>
#include<SDL_ttf.h>
#include<SDL.h>
#include<SDL_image.h>
#include<stdlib.h>
#include<vector>
#include "Button.h"
#include "gridcell.h"
#include "puzzle.h"

class Sudoku{
	const int WINDOW_HEIGHT, WINDOW_WIDTH ;
	const int GRID_HEIGHT, GRID_WIDTH ;
	const int GRID_ROWS, GRID_COLS ;
	int SCORE;

	SDL_Window* window;
	SDL_Renderer* renderer;

	// Total textures required
	// and the array of those many textures
	int TotalTextures;
	SDL_Texture* TextureCache[22];
	SDL_Texture* ImageTextures[2];

	TTF_Font* font;
	int FontSize;

	const int TotalCells;

	// Define grid of cells (normally set to 9 x 9 = 81)
	Cell grid[81];

	// Define check and new buttons
	Button CheckButton;
	Button NewButton;
	Button MenuButton;
	Button Timer;

	// Buttons of Menu
	Button PlayButton;
	Button InstrButton;
	Button ExitButton;

	//Buttons of Level
	Button EasyButton;
	Button MediumButton;
	Button HardButton;

	SDL_Color ClearColor;

	puzzle P;


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
	void generateSudoku();

public:
	Sudoku();
	~Sudoku();

	void menu();

	void chooseLevel(int &);

	void play(int &);

	void close();

};

// class Option{
// 	int x,y;
// }
