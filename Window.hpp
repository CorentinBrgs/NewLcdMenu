#ifndef _Window_
#define _Window_ 

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum WindowType {
	WINDOW,
	WINDOW_CHOICE,
	WINDOW_CONFIRM,
	WINDOW_INT,
	WINDOW_TEXT
};

enum WindowState {
	ACTIVE,
	ON_PAUSE,
	INACTIVE
};

enum Event {
	EVENT_NONE,
	EVENT_MORE,
	EVENT_LESS,
	EVENT_RIGHT,
	EVENT_LEFT,
	EVENT_OK,
	EVENT_CANCEL,
	EVENT_INTERRUPT
};

class Window {

public :

	Window();
	Window(LiquidCrystal_I2C *lcd, char* title);
	Window(LiquidCrystal_I2C *lcd);
	
	void draw() const;	//draw the window on the first call
	bool refresh(Event event); //actualizes the windows on an event
	//returns true if the next Window set bu setNextWindow must be called by the UI.
	void setNextWindow(Window *window);
	void setFatherWindow(Window *window);
	

	//set methods
	inline void setTitle(char* title) {_title = title;}
	inline void setId(uint8_t id) {_id = id;}
	inline void setWindowState(WindowState windowState) {_windowState = windowState;}

	//get methods
	inline WindowType getWindowType() const {return _windowType;}
	inline Window* getNextWindow() const {return this->_nextWindow;}
	inline char* getTitle() const {return this->_title;}
	//returns title of the current window
	inline uint16_t getId() const {return this->_id;} //returns id of the current window
	inline WindowState getWindowState() const {return this->_windowState;}

	inline static void addWindow() {nbWindow++;}
	inline static uint8_t getNbWindow() {return nbWindow;}


protected :
	uint16_t _id;
	char* _title;

	LiquidCrystal_I2C* _lcd;
	Window* _nextWindow;
	Window* _fatherWindow;
	uint16_t _nextWindowId;
	uint16_t _fatherWindowId;

	WindowType _windowType;
	WindowState _windowState;

	static uint8_t nbWindow;

};

#endif 