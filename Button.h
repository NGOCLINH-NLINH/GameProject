#pragma once
#include <SDL.h>


enum class ButtonState
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER_MOTION = 1,
	BUTTON_MOUSE_DOWN = 2,
	BUTTON_MOUSE_UP = 3,
};

class Button {

	ButtonState currentState;

	SDL_Texture* texture;

	SDL_Rect buttonRect;
	SDL_Rect textureRect;

	bool selected;

protected:

	SDL_Color mouseOutColour;
	SDL_Color mouseOverMotionColour;
	SDL_Color mouseDownColour;
	SDL_Color mouseUpColour;

public:
	Button();

	void setTexture(SDL_Texture* texture);
	void setButtonRect(const SDL_Rect& rect);
	void setTextureRect(const SDL_Rect& rect);
	void centerTextureRect();
	void setSelected(const bool selected);
	bool isMouseInside(const int x, const int y);
	ButtonState getMouseEvent(const SDL_Event* event);
	void setMouseDownColour(const SDL_Color& colour);
	void setAllColor(const SDL_Color& colour);
	void renderButton(SDL_Renderer* renderer);
	void renderTexture(SDL_Renderer* renderer);
};
