#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"

uint8_t Window::nbWindow = 0;

Window::Window() {

}

Window::Window(LiquidCrystal_I2C *lcd, char* title) {
	_id = 0;
	_lcd = new LiquidCrystal_I2C(0x00,0,0);
	_lcd = lcd;

	_nextWindow = 0;
	_windowType = WINDOW;
	_windowState = INACTIVE;

	_title = title;

	Window::addWindow();

}

Window::Window(LiquidCrystal_I2C *lcd) {
	_id = 0;
	_lcd = new LiquidCrystal_I2C(0x00,0,0);
	_lcd = lcd;

	_nextWindow = 0;
	_windowType = WINDOW;
	_windowState = INACTIVE;

	Window::addWindow();
}

void Window::draw() const{
	_lcd->backlight();
	_lcd->clear();
	_lcd->print(_title);
}

bool Window::refresh(Event event) {
	if (event==EVENT_OK||event==EVENT_MORE||event==EVENT_RIGHT) {
			return true;
	}
	else {
		return false;
	}
}

void Window::setNextWindow(Window *window) {
	_nextWindow = new Window();
	_nextWindow = window;
	_nextWindowId = _nextWindow->getId();
}

void Window::setFatherWindow(Window* window) {
	_fatherWindow = new Window();
	_fatherWindow = window;
	_fatherWindowId = _fatherWindow->getId();
}