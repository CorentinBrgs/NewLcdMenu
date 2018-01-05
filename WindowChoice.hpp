#ifndef _WindowChoice_
#define _WindowChoice_ 

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"
#include "Choice.hpp"

class WindowChoice : public Window
{
public :
	WindowChoice();
	void draw() const;
	bool refresh(Event event);
	void init(LiquidCrystal_I2C *lcd);

	void addChoice(Choice* choice);

	//get methods
	inline WindowType getWindowType() const {return WINDOW_TEXT;}
	inline uint8_t getNbChoice() const {return nbChoice;}
protected :
	void refreshScreen() const;

	bool _onStart;
	uint8_t nbChoice;
	Choice* _choice_table[3];
	uint8_t _currentSelectedChoice;
	uint8_t _previousSelectedChoice;
	bool _aChoiceIsSelected;
};

#endif 