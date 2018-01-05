#ifndef _NewLcdMenuh_
#define _NewLcdMenuh_

/*If you want to use the debug mode uncomment these two lines.
Please consider disabling it for normal use of the library as 
it will save both static and dynamic memory.*/
// #define _DEBUGMODE_
// #define SERIALSPEED 9600
//----------------------------------------------------------------

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Window.hpp"
#include "WindowText.hpp"
#include "WindowChoice.hpp"
#include "Choice.hpp"

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif


class LcdMenu {
public :

	Event convertKeyToEvent(char key);
	void createEventTable(uint8_t ROWS, uint8_t COLS, char* keys_table, Event* event_table);


	LcdMenu(uint8_t adress, uint8_t ROWS, uint8_t COLS);
	
	void begin();
	void begin(char* strBEGIN_table[]); //Screen that displays on boot, if necessary
	void testCharLcd();

	void addWindowToUI(Window* window);
	void setCurrentWindow(Window* window);

	bool loop(Event event);

	LiquidCrystal_I2C lcd =  LiquidCrystal_I2C(_lcdAdress,_lcdROWS,_lcdCOLS);

private :

	inline char accessKeyTable(uint8_t row, uint8_t col) {return _keys_table[row * _keypadCOLS + col];}
	inline char accessKeyTable(uint8_t indice) {return _keys_table[indice];}
	inline Event accessEventTable(uint8_t row, uint8_t col) {return _event_table[row * _keypadCOLS + col];}
	inline Event accessEventTable(uint8_t indice) {return _event_table[indice];}


// attributes for lcd
	uint8_t _lcdAdress;
	uint8_t _lcdROWS;
	uint8_t _lcdCOLS;

//attributes for Keypad
	uint8_t _keypadROWS;
	uint8_t _keypadCOLS;

	char* _keys_table;
	Event* _event_table;

//array of references to the differents windows
	bool _isFirstWindow; //used to initialise the the two array _windowType_table and _window_table.
	uint8_t _nbAddedWindows; //Counter that indicates the number of windows added. This is used to give the window a unique ID. 
	WindowType* _windowType_table; //Array containing the types (see WindowType enum in Window.hpp) of the windows
	Window** _window_table; //Array containing the references to the differents windows added to the UI with addWindowToUI(Window* window)

//going next scren
	Window* _currentWindow;
	Window* _previousWindow;
	bool _onStart;

};

#endif