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

	Window(); //only one constructor allowed, to avoid conflicts with subclasses.
	
	virtual void draw() const;	//draw the window on the first call
	virtual bool goNextWindow(Event event); 
	//returns true if the loop must call the window with getNextWindow()
	virtual bool refresh(Event event);
	//actualizes the windows on an event
	//returns true if the window has been refreshed
	//by default this method is empty
	virtual void setNextWindow(Window *window);
	virtual void setChildWindow(Window *window);
	virtual void setFatherWindow(Window *window);
	

	//set methods
	void init(LiquidCrystal_I2C *lcd);

	inline void setTitle(char* title) {_title = title;}
	inline void setId(uint8_t id) {_id = id;}
	inline void setWindowState(WindowState windowState) {_windowState = windowState;}

	//get methods
	inline WindowType getWindowType() const {return WINDOW;}
	inline Window* getNextWindow() const {return this->_nextWindow;}
	inline char* getTitle() const {return this->_title;}//returns title of the current window
	inline uint16_t getId() const {return this->_id;} //returns id of the current window
	inline WindowState getWindowState() const {return this->_windowState;}

	inline Window* getChildWindow() const {return this->_childWindow;}
	inline Window* getFatherWindow() const {return this->_fatherWindow;}

	//static methods : used to count number of created windows, which is a static attribute fof the class Window
	inline static void addWindow() {nbWindow++;}
	inline static uint8_t getNbWindow() {return nbWindow;}

protected :
//common to all windows
	LiquidCrystal_I2C* _lcd;
	uint16_t _id;
	char* _title;
	WindowType _windowType;
	WindowState _windowState;
	
//used for simple transition when no Choice (only Next and Previous)
	Window* _fatherWindow;
	Window* _childWindow;
	uint16_t _fatherWindowId;
	uint16_t _childWindowId;


//for window transition
	Window* _nextWindow;
	uint16_t _nextWindowId;

	static uint8_t nbWindow;

};

#endif 