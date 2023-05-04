#include "Gridcell.h"

Cell::Cell():

	CharNumber('0'),
	isEditable(1)
{}

void Cell::setNumber(const int number){

	CharNumber = '0' + number;

}

int Cell::getNumber() const{
	return int(CharNumber-'0');
}

void Cell::setEditable(const bool editable){
	isEditable = editable;
	if (isEditable){
		mouseOutColour = { 255, 255, 255, SDL_ALPHA_OPAQUE }; // light purple
		mouseOverMotionColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
		mouseDownColour = { 255,229,153, SDL_ALPHA_OPAQUE }; // yellow
		mouseUpColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
	}
	else{
		mouseOutColour = { 159,197,232, SDL_ALPHA_OPAQUE }; // blue
		mouseOverMotionColour = { 159,197,232, SDL_ALPHA_OPAQUE }; // blue
		mouseDownColour = { 159,197,232, SDL_ALPHA_OPAQUE }; // blue
		mouseUpColour = { 159,197,232, SDL_ALPHA_OPAQUE }; // blue
	}
}


void Cell::handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[], int correctNum, int& mistakeFlag){
	int cnt = 0;
	if (event->key.keysym.sym == SDLK_BACKSPACE && CharNumber != ' '){
		CharNumber = ' ';
		setTexture(textureCache[0]);
	}

	else if (event->type == SDL_TEXTINPUT){
		if (atoi(event->text.text)){
			CharNumber = *(event->text.text);
			if (CharNumber-'0' == correctNum) {
                setTexture(textureCache[CharNumber-'0']);
			}
			else {
                setTexture(textureCache[CharNumber-'0'+20]);
                cnt++;
			}
        }

	}
	mistakeFlag = cnt;
}
