#include "Button.h"


Button::Button()
	: currentState(ButtonState::BUTTON_MOUSE_OUT),
	  texture(NULL),
	  buttonRect({ 0, 0, 0, 0 }),
	  textureRect({0, 0, 0, 0}),
	  mouseOutColour({ 186,195,196,255 }), // light grey
	  mouseOverMotionColour({ 146,197,244,255 }),//blue
	  mouseDownColour({ 252,219,119,255 }), // yellow
	  mouseUpColour({ 146,197,244,255 }), // blue
	  selected(0)
{}


void Button::setButtonRect(const SDL_Rect& rect){
	buttonRect = rect;
}
void Button::setTextureRect(const SDL_Rect& rect){
	textureRect = rect;
}

void Button::setTexture(SDL_Texture* texture){
	this->texture = texture;
}
void Button::renderTexture(SDL_Renderer* renderer){
	SDL_RenderCopy(renderer, texture, NULL, &textureRect);
}

void Button::setMouseDownColour(const SDL_Color& colour){
	mouseDownColour = colour;
}
void Button::setAllColor(const SDL_Color& colour){
	mouseDownColour = colour;
	mouseOverMotionColour=colour;
	mouseOutColour=colour;
	mouseUpColour=colour;
}
void Button::setSelected(const bool selected){
	this->selected = selected;
}

void Button::centerTextureRect(){
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

	const int textureStartRow = buttonRect.y + 0.5 * (buttonRect.h - textureHeight);
	const int textureStartCol = buttonRect.x + 0.5 * (buttonRect.w - textureWidth);

	textureRect = { textureStartCol, textureStartRow, textureWidth, textureHeight };
}

bool Button::isMouseInside(const int x, const int y){
	return (x >= buttonRect.x &&
		x <= buttonRect.x + buttonRect.w &&
		y >= buttonRect.y &&
		y <= buttonRect.y + buttonRect.h
	);
}

ButtonState Button::getMouseEvent(const SDL_Event* event)
{
	//If mouse event happened
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP){

		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (!isMouseInside(x, y)){
			currentState = ButtonState::BUTTON_MOUSE_OUT;
		}
		else{
			switch (event->type){
				case SDL_MOUSEMOTION:
					currentState = ButtonState::BUTTON_MOUSE_OVER_MOTION;
					break;

				case SDL_MOUSEBUTTONDOWN:
					currentState = ButtonState::BUTTON_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					currentState = ButtonState::BUTTON_MOUSE_UP;
					break;
			}
		}
	}
	return currentState;
}

void Button::renderButton(SDL_Renderer* renderer){
	// If current button is selected
	if (selected){
		SDL_SetRenderDrawColor(renderer, mouseDownColour.r, mouseDownColour.g, mouseDownColour.b, mouseDownColour.a);
	}
	else{
		// Show current button State
		switch (currentState)
		{
		case ButtonState::BUTTON_MOUSE_OUT:
			SDL_SetRenderDrawColor(renderer, mouseOutColour.r, mouseOutColour.g, mouseOutColour.b, mouseOutColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_OVER_MOTION:
			SDL_SetRenderDrawColor(renderer, mouseOverMotionColour.r, mouseOverMotionColour.g, mouseOverMotionColour.b, mouseOverMotionColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_DOWN:
			SDL_SetRenderDrawColor(renderer, mouseDownColour.r, mouseDownColour.g, mouseDownColour.b, mouseDownColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_UP:
			SDL_SetRenderDrawColor(renderer, mouseUpColour.r, mouseUpColour.g, mouseUpColour.b, mouseUpColour.a);
			break;
		}
	}

	SDL_RenderFillRect(renderer, &buttonRect);
}

