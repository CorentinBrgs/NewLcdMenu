#ifndef _WindowText_
#define _WindowText_ 

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"

class WindowText : public Window
{
public :
	WindowText();

	void draw() const;
	void addText(char* text_table[]);

	//get methods
	inline WindowType getWindowType() const {return WINDOW_TEXT;}

protected :
	char* _text_table[3];

};

#endif 