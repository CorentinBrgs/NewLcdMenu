#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"
#include "Choice.hpp"
#include "WindowChoice.hpp"

#ifndef _DEBUGMODE_
#define _DEBUGMODE_
#endif 

WindowChoice::WindowChoice()
{

}

void WindowChoice::init(LiquidCrystal_I2C *lcd) 
{
	Window::init(lcd);
	nbChoice = 0;
	for (int i=0;i<3;i++)
	{
		_choice_table[i] = NULL;
	}
	_onStart = true;
}

void WindowChoice::draw() const
{

	Window::draw();

	for (int line=0; line<this->nbChoice; line++)
	{
		_lcd->setCursor(1,line+1);
		_lcd->print(_choice_table[line]->getLabel());
	if (_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_VALUE)
		{
			_lcd->print(" : ");
			_lcd->print(_choice_table[line]->getValue());
		}
		this->refreshScreen();
	}
}

bool WindowChoice::refresh(Event event) {
	#ifdef _DEBUGMODE_
		Serial.println(F("\n/---Refreshing WindowChoice---/"));
	#endif

	//List of actions depending on the event value
	switch (event) {
		case EVENT_MORE :
			Serial.print(F("EVENT MORE Detected\n"));
			if(	this->_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_VALUE && _aChoiceIsSelected)
			{
				#ifdef _DEBUGMODE_
					Serial.print(F("Old value = "));
					Serial.println(_choice_table[_currentSelectedChoice]->getValue());
				#endif

				_choice_table[_currentSelectedChoice]->changeValue(_choice_table[_currentSelectedChoice]->getValue()+1);
			
				#ifdef _DEBUGMODE_
					Serial.print(F("New value = "));
					Serial.println(_choice_table[_currentSelectedChoice]->getValue());
				#endif
			}

			else //if (_currentSelectedChoice > 0 ) 
			{
				_previousSelectedChoice = _currentSelectedChoice;
				_currentSelectedChoice++;
				_currentSelectedChoice = _currentSelectedChoice % nbChoice ;
				#ifdef _DEBUGMODE_
					Serial.print(F("Currently selected choice is "));
					Serial.println(_choice_table[_currentSelectedChoice]->getLabel());
				#endif
			}
			this->refreshScreen();
			return true;
		break;

		case EVENT_LESS :
			Serial.print(F("EVENT LESS Detected\n"));
			if(	this->_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_VALUE && _aChoiceIsSelected)
			{
				#ifdef _DEBUGMODE_
					Serial.print(F("Old value = "));
					Serial.println(_choice_table[_currentSelectedChoice]->getValue());
				#endif

				_choice_table[_currentSelectedChoice]->changeValue(_choice_table[_currentSelectedChoice]->getValue()-1);
			
				#ifdef _DEBUGMODE_
					Serial.print(F("New value = "));
					Serial.println(_choice_table[_currentSelectedChoice]->getValue());
				#endif
			}

			else //if (_currentSelectedChoice > 0 ) 
			{
				_previousSelectedChoice = _currentSelectedChoice;
				_currentSelectedChoice--;
				_currentSelectedChoice = _currentSelectedChoice % nbChoice ;
				#ifdef _DEBUGMODE_
					Serial.print(F("Currently selected choice is "));
					Serial.println(_choice_table[_currentSelectedChoice]->getLabel());
				#endif
			}
			this->refreshScreen();
			return true;
		break;

		case EVENT_OK :
			#ifdef _DEBUGMODE_
				Serial.print(F("Currently selected choice is "));
				Serial.println(_choice_table[_currentSelectedChoice]->getLabel());
			#endif

			if(	this->_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_VALUE)//only in the choice is a CHOICE_VALUE type.
			{
				_aChoiceIsSelected = true;
				this->refreshScreen();
				return true;
			}
			else {return false;} //case when the choice is a window choice, it can't be selected. A press on OK will lead to next window. See goNextWindow for behaviour.
		break;

		case EVENT_CANCEL :

			if(_aChoiceIsSelected) {
				_aChoiceIsSelected = false;
				this->refreshScreen();
				return true;
			}
			else {
				//this->refreshScreen();
				return false;
			}
		break;
		default :
			return false;
		break;
	}
}

bool WindowChoice::goNextWindow(Event event)
{
	if(this->_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_WINDOW) 
	{
		switch(event) {
			case EVENT_OK :
				setChildWindow(this->_choice_table[_currentSelectedChoice]->getWindow()); //it is mandatory to set it as child window. If this operation is not done, the father will be set to this window by the UI 
				setNextWindow(this->getChildWindow());
				return true;
			break;

			//default :
			//	return false;
			//break;
		}
	}
	if (!_aChoiceIsSelected)
	{
		switch(event) {
			case EVENT_CANCEL :
				setNextWindow(this->getFatherWindow());;
				return true;
			break;
			default :
				return false;
			break;
		}
	}
}

void WindowChoice::addChoice(Choice* choice)
{	 
	_choice_table[nbChoice] = choice;
	nbChoice++;
	if (_onStart) 
	{
		_currentSelectedChoice = 0;
		_previousSelectedChoice = 0;
		_onStart = false;
	}

	#ifdef _DEBUGMODE_
		Serial.println(F("\n/---Adding choice---/"));
		Serial.print(F("Choice Label : "));
		Serial.println(_choice_table[nbChoice-1]->getLabel());
		Serial.print(F("Choice Type : "));
		Serial.println(_choice_table[nbChoice-1]->getChoiceType());
	#endif
}

void WindowChoice::refreshScreen() const
{
	if(!_aChoiceIsSelected)
	{
		_lcd->setCursor(0,_previousSelectedChoice+1);
		_lcd->print(" ");
		_lcd->setCursor(0,_currentSelectedChoice+1);
		_lcd->print(">");
	}

	else
	{	
		_lcd->setCursor(0,_currentSelectedChoice+1);
		_lcd->printByte(0);//printing the custom char full_arrow

		if (_choice_table[_currentSelectedChoice]->getChoiceType() == CHOICE_VALUE)
		{
			uint8_t tempCursor = _choice_table[_currentSelectedChoice]->getLabelSize() + 4;
			_lcd->setCursor(tempCursor, _currentSelectedChoice +1);
			
			for (uint8_t loop = tempCursor; loop < 20; loop++)//cleaning the value place
			{
				_lcd->setCursor(loop, _currentSelectedChoice+1);
				_lcd->print(" ");
			}
			_lcd->setCursor(tempCursor, _currentSelectedChoice+1);
			_lcd->print(_choice_table[_currentSelectedChoice]->getValue());
		}
	}
}

//TO BE ADDED : storeOldValue method should be added to choice so that the user can cancel the changes made. A confirmation screen should follow the cancel to propose to save changes or delete them. 
