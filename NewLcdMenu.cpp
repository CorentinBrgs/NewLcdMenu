#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

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

	_currentWindow = NULL; //initilizing pointers to NULL
	_previousWindow = NULL; //initilizing pointers to NULL

	_onStart = true;
}

void LcdMenu::begin()
{
	lcd.init();
	lcd.backlight();
	#if defined _DEBUGMODE_
		Serial.begin(SERIALSPEED);
		Serial.println(F("/---Debug mode activated, see here messages"));	
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
		Serial.println(F("Debug mode activated, see here messages"));
		Serial.println(F("/---Begin Screen Strings---/"));
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
		lcd.print(F("Codes 0x")); 
		lcd.print(16*i, HEX);
		lcd.print(F("-0x")); 
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
		Serial.println(F("\n/---Adding window to UI---/"));
	#endif

	if (_isFirstWindow) 
	{
		uint8_t nbWindow = Window::getNbWindow();
		_window_table = new Window*[nbWindow];
		_windowType_table = new WindowType[nbWindow];
		#if defined _DEBUGMODE_
			Serial.println(F("Window Array created with success !"));
			Serial.print(F("Number of windows : "));
			Serial.println(nbWindow);
			Serial.print(F("Current is first window: "));
			Serial.println(_isFirstWindow);
		#endif

	_isFirstWindow = false ;
	}

	Window* _tempWindow;
	_tempWindow = NULL;
	_tempWindow = window;
	
	_window_table[_nbAddedWindows] = _tempWindow;
	_windowType_table[_nbAddedWindows] = _tempWindow->getWindowType();
	//See enum for the different types of windows.

	_tempWindow->setId(_nbAddedWindows);

	#if defined _DEBUGMODE_
		Serial.print(F("Window's title : "));
		Serial.println(_window_table[_nbAddedWindows]->getTitle());

		Serial.print(F("Window's type : "));
		Serial.println(_windowType_table[_nbAddedWindows]);
		
		Serial.print(F("Window's ID : "));
		//Serial.println(window->getId());
		Serial.println(_window_table[_nbAddedWindows]->getId());
		//Serial.println(_tempWindow->getId());
	#endif  

	_nbAddedWindows++;
}

void LcdMenu::setCurrentWindow(Window* window)
{

	#ifdef _DEBUGMODE_
		Serial.println("\n/---Setting current window---/");
	#endif

	if (!_onStart) 
	{
		_previousWindow = _currentWindow;
		_previousWindow->setWindowState(INACTIVE);
	}

	_currentWindow = window;
	_currentWindow->setWindowState(ACTIVE);
	_currentWindow->draw();

	if ((_currentWindow->getId() != _previousWindow->getId()) && (_currentWindow->getChildWindow()->getId() != _previousWindow->getId() ) && (_currentWindow->getId()!=0)) 
		/*two conditions must be true to assignate the current Window a new fatherWindow :
		- the window must have a child window
		- the previous Window is not the child of the current.
		- the current window's Id!=0. That means it's not the root window*/ 
	{
		_currentWindow->setFatherWindow(_previousWindow);
	}	

	if (_onStart) {
	 	_onStart = false;
	 	_previousWindow = _currentWindow;
	 	_currentWindow->setFatherWindow(_currentWindow);
	 	if (_currentWindow->getId() != 0)
	 	{
	 		// if the currentWindow Id is not 0 then the window is not the root window. That will lead to further problems. 
	 	#if defined _DEBUGMODE_ 
	 		Serial.println(F("!!!Please add the root window with addWindowToUI() method first!!!"));
	 		this->lcd.setCursor(0,1);
	 		this->lcd.print(F("!!Add root window !!"));
	 		this->lcd.setCursor(0,2);
	 		this->lcd.print(F("!!  first to UI   !!"));
	 	#endif 
	 	}
	}
	
	#ifdef _DEBUGMODE_
		if (!_onStart) 
			{
				Serial.print(_previousWindow->getTitle());
				Serial.print(F(" with Id "));
				Serial.print(_previousWindow->getId());
				Serial.print(F(" is now "));
				Serial.println(_previousWindow->getWindowState());
			}
			Serial.print(_currentWindow->getTitle());
			Serial.print(F(" with Id "));
			Serial.print(_currentWindow->getId());
			Serial.print(F(" is now "));
			Serial.println(_currentWindow->getWindowState());
	#endif
}

bool LcdMenu::loop(Event event) 
{
	
	if (event)//if an event happens, the event value is different from 0
	{
		#if defined _DEBUGMODE_
			Serial.println(F("/---LcdMenu Loop---/"));
			Serial.print(F("On event : "));
			Serial.print(event);
		#endif

		WindowType currentWindowType = _currentWindow->getWindowType();
		if (_currentWindow->goNextWindow(event)) //if the condition is true, we call the child window according to the type of the window currently opened.
		{
			switch (currentWindowType) 
			{
				case WINDOW :
					setCurrentWindow(_currentWindow->getNextWindow());
				break;

				case WINDOW_TEXT :
					setCurrentWindow(_currentWindow->getNextWindow());
				break;

				/*TO BE MODIFIED : two methods instead of one :
				- refresh() to refresh the screen when somethings changes in the window : value, selection,...
				- goToNext() if the window calls the next window in getNextWindow()*/

			}
		}
	}
}

Event LcdMenu::convertKeyToEvent(char key) //converts key received from the keypad to event understandable by the LcdMenu UI.
{
	if(key) //if a valid key is received
	{
		for (uint8_t indice = 0; indice<_keypadCOLS*_keypadROWS; indice++) 
		{
			if(key==accessKeyTable(indice))
			{
				#if defined _DEBUGMODE_
					Serial.println(F("\n/---convert key to event---/"));
					Serial.println(accessEventTable(indice));
				#endif
				return accessEventTable(indice);
				break;
			}
		}
	}
	else {//if no valid key is received, no event is comming from the keypad.
		return EVENT_NONE;
	}
}

void LcdMenu::createEventTable(uint8_t ROWS, uint8_t COLS, char* keys_table, Event* event_table)
{
	#if defined _DEBUGMODE_
		Serial.print(F("\n/---Creating Event Table---/"));
	#endif
	_keypadROWS = ROWS;
	_keypadCOLS = COLS;

	_keys_table = keys_table;
	_event_table = event_table;

	#if defined _DEBUGMODE_
		Serial.print(F("\nChar key list : "));
		for (byte r=0; r<ROWS; r++) {
			for (byte c=0; c<COLS; c++) {
				Serial.print(accessKeyTable(r,c));
				Serial.print(", ");
			}
		}	
		Serial.print(F("\nEvent key list : "));
		for (byte r=0; r<ROWS; r++) {
			for (byte c=0; c<COLS; c++) {
				Serial.print(accessEventTable(r,c));
				Serial.print(", ");
			}
		}
		Serial.print("\n");
	#endif
}

