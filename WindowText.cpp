#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"
#include "WindowText.hpp"

WindowText::WindowText()
{}

void WindowText::draw() const
{

	Window::draw();
	_lcd->setCursor(0,1);
	_lcd->print(_text_table[0]);
	_lcd->setCursor(0,2);
	_lcd->print(_text_table[1]);
	_lcd->setCursor(0,3);
	_lcd->print(_text_table[2]);
}

void WindowText::addText(char* text_table[3]) 
{
	for (uint8_t line = 0; line <3; line ++) {
		_text_table[line] = text_table[line];
	}

	#ifdef _DEBUGMODE_
		Serial.println(F("\n/---Adding text to WindowText---/"));
		for (uint8_t line = 0; line <3; line ++) {
			Serial.println(_text_table[line]);
		}
	#endif 
}