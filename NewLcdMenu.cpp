#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define _DEBUGMODE_
#define SERIALSPEED 9600

#include "NewLcdMenu.h"

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

LcdMenu::LcdMenu(uint8_t adress, uint8_t ROWS, uint8_t COLS)
{
	_lcdAdress = adress;
	_lcdROWS = ROWS;
	_lcdCOLS = COLS;
	lcd = LiquidCrystal_I2C(_lcdAdress, _lcdROWS, _lcdCOLS); 
	_isFirstWindow = true;
	_nbAddedWindows = 0;

	_currentWindow = new Window();
	_previousWindow = new Window();

	//char* _keys_table = NULL;
	//Event* _event_table = NULL;
}

void LcdMenu::begin()
{
	lcd.init();
	lcd.backlight();
	#if defined _DEBUGMODE_
		Serial.begin(SERIALSPEED);
		Serial.println("/---Debug mode activated, see here messages");	
	#endif 
}

void LcdMenu::begin(char* strBEGIN_table[])
{
	lcd.init();
	lcd.backlight();
	lcd.print(strBEGIN_table[0]);
	lcd.setCursor(0,1);
	lcd.print(strBEGIN_table[1]);
	lcd.setCursor(0,2);
	lcd.print(strBEGIN_table[2]);
	lcd.setCursor(0,3);
	lcd.print(strBEGIN_table[3]);

	#if defined _DEBUGMODE_
		Serial.begin(SERIALSPEED);
		Serial.println("Debug mode activated, see here messages");
		Serial.println("/---Begin Screen Strings---/" );
		Serial.println(strBEGIN_table[0]);
		Serial.println(strBEGIN_table[1]);
		Serial.println(strBEGIN_table[2]);
		Serial.println(strBEGIN_table[3]);
		Serial.println();
	#endif 

	delay(1000);
}

void LcdMenu::testCharLcd() 
{
	lcd.clear();
	uint8_t i = 0;
	for(int i=0; i<17;i++){
		lcd.clear();
		lcd.print("Codes 0x"); 
		lcd.print(16*i, HEX);
		lcd.print("-0x"); 
		lcd.print(16*(i+1), HEX);
		lcd.setCursor(0, 1);

		for (int j=0; j<16; j++) {
			lcd.printByte(16*i+j);
		}
		delay(2000);
	}
}

void LcdMenu::addWindowToUI(Window* window) 
{
	#if defined _DEBUGMODE_
		Serial.println("\n/---Adding window to UI---/");
	#endif

	if (_isFirstWindow) 
	{
		uint8_t nbWindow = Window::getNbWindow();
		_window_table = new Window*[nbWindow];
		_windowType_table = new WindowType[nbWindow];
		#if defined _DEBUGMODE_
			Serial.println("Window Array created with success !");
			Serial.print("Number of windows : ");
			Serial.println(nbWindow);
			Serial.print("Current is first window: ");
			Serial.println(_isFirstWindow);
		#endif

	_isFirstWindow = false ;
	}

	Window* _tempWindow;
	_tempWindow = new Window();
	_tempWindow = window;
	
	_window_table[_nbAddedWindows] = _tempWindow;
	_windowType_table[_nbAddedWindows] = _tempWindow->getWindowType();
	//voir enum pour les differents types de window.

	_tempWindow->setId(_nbAddedWindows);

	#if defined _DEBUGMODE_
		Serial.print("Window's title : ");
		Serial.println(_window_table[_nbAddedWindows]->getTitle());

		Serial.print("Window's type : ");
		Serial.println(_windowType_table[_nbAddedWindows]);
		
		Serial.print("Window's ID : ");
		//Serial.println(window->getId());
		Serial.println(_window_table[_nbAddedWindows]->getId());
		//Serial.println(_tempWindow->getId());
	#endif  

	_nbAddedWindows++;
}

void LcdMenu::setCurrentWindow(Window* window)
{

	#if defined _DEBUGMODE_
		Serial.println("\n/---Setting current window---/");
	#endif
	_previousWindow = _currentWindow;
	_previousWindow->setWindowState(INACTIVE);
	_currentWindow = new Window();
	_currentWindow = window;
	_currentWindow->setWindowState(ACTIVE);
	_currentWindow->setFatherWindow(_previousWindow);
	_currentWindow->draw();
	
	#if defined _DEBUGMODE_

		Serial.print(_previousWindow->getTitle());
		Serial.print(" with Id ");
		Serial.print(_previousWindow->getId());
		Serial.print(" is now ");
		Serial.println(_previousWindow->getWindowState());
		Serial.print(_currentWindow->getTitle());
		Serial.print(" with Id ");
		Serial.print(_currentWindow->getId());
		Serial.print(" is now ");
		Serial.println(_currentWindow->getWindowState());
	#endif
}

bool LcdMenu::loop(Event event) 
{
	
	if (event)//if an event happens, the event value is different from 0
	{
		#if defined _DEBUGMODE_
			Serial.print("/---LcdMenu Loop---/");
			Serial.print("on event : ");
			Serial.print(event);
		#endif

		WindowType currentWindowType = _currentWindow->getWindowType();
		if (_currentWindow->refresh(event)) //if the condition is true, we call the child window according to the type of the window currently opened.
		{
			switch (currentWindowType) 
			{
				case WINDOW :
					setCurrentWindow(_currentWindow->getNextWindow());
				break;
			}
		}
	}
}

Event LcdMenu::convertKeyToEvent(char key)
{
	if(key) 
	{
		for (uint8_t indice = 0; indice<_keypadCOLS*_keypadROWS; indice++) 
		{
			if(key==accessKeyTable(indice))
			{
				#if defined _DEBUGMODE_
					Serial.println("\n/---convert key to event---/");
					Serial.println(accessEventTable(indice));
				#endif
				return accessEventTable(indice);
				break;
			}
		}
	}
	else {
		return EVENT_NONE;
	}
}

void LcdMenu::createEventTable(uint8_t ROWS, uint8_t COLS, char* keys_table, Event* event_table)
{
	#if defined _DEBUGMODE_
		Serial.print("\n/---Creating Event Table---/");
	#endif
	_keypadROWS = ROWS;
	_keypadCOLS = COLS;

	_keys_table = keys_table;
	_event_table = event_table;

	#if defined _DEBUGMODE_
		Serial.print("\nChar key list : ");
		for (byte r=0; r<ROWS; r++) {
			for (byte c=0; c<COLS; c++) {
				Serial.print(accessKeyTable(r,c));
				Serial.print(", ");
			}
		}	
		Serial.print("\nEvent key list : ");
		for (byte r=0; r<ROWS; r++) {
			for (byte c=0; c<COLS; c++) {
				Serial.print(accessEventTable(r,c));
				Serial.print(", ");
			}
		}
		Serial.print("\n");
	#endif
}

