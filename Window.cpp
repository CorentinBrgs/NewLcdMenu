#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"

uint8_t Window::nbWindow = 0;

Window::Window() 
{

}


void Window::draw() const
{
	_lcd->backlight();
	_lcd->clear();
	_lcd->print(_title);
}

bool Window::goNextWindow(Event event) 
{
	if (event==EVENT_OK) 
	{
		setNextWindow(this->getChildWindow());
		return true;
	}
	else if (event==EVENT_CANCEL) 
	{
		setNextWindow(this->getFatherWindow());;
		return true;
	}
	else {return false;}
}

bool Window::refresh(Event event)
{
	return false;
}

void Window::setNextWindow(Window *window) 
{
	_nextWindow = new Window();
	_nextWindow = window;
	_nextWindowId = _nextWindow->getId();
}

void Window::setChildWindow(Window *window) 
{
	_childWindow = new Window();
	_childWindow = window;
	_childWindowId = _childWindow->getId();
}

void Window::setFatherWindow(Window* window) 
{
	_fatherWindow = new Window();
	_fatherWindow = window;
	_fatherWindowId = _fatherWindow->getId();
}

void Window::init(LiquidCrystal_I2C *lcd) 
{
	_lcd = new LiquidCrystal_I2C(0x00,0,0);
	_lcd = lcd;
	_id = 0;
	_nextWindow = 0;
	_windowState = INACTIVE;

	Window::addWindow(); //increments the nbWindow static attribute
}