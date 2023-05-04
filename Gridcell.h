#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Button.h"

class Cell : public Button
{
	char CharNumber;

public:
	bool isEditable;
	Cell();

	void setNumber(const int number);
	int getNumber() const;

	void setEditable(const bool editable);
	void handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[], int correctNum, int& mistakeFlag);

};
