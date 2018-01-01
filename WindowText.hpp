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
	void init(LiquidCrystal_I2C *lcd); 
	//used to redefine the _windowType with WINDOW_TEXT
	void addText(char* text_table[]);
protected :
	char* _text_table[3];

};

#endif 