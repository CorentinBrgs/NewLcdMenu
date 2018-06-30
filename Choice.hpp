#ifndef _Choiceh_
#define _Choiceh_

#include <Arduino.h>
#include "Window.hpp"

enum ChoiceType {
	CHOICE_WINDOW,
	CHOICE_VALUE
};

class Choice //abstract class
{
	public :
		Choice();

		inline virtual void init(char** label, uint8_t position) {_label = label; _position = position;}
		virtual ChoiceType getChoiceType() const =0;


	//get Methods
		inline char* getLabel() const {return *_label;}
		inline uint8_t getLabelSize() const {return strlen(*_label);}

		virtual inline uint8_t getValue() const {return 0;}
		virtual inline void changeValue(int newValue) {};
		virtual inline Window* getWindow() const {return NULL;}


	protected :
		char** _label; //string that will be displayed on the WindowChoice 
		uint8_t _position; //UNUSED : position of the choice in the list on WindowChoice.
};

class ChoiceWindow : public Choice
{
	public : 
		ChoiceWindow();
		inline void init(char** label, uint8_t position);
 		inline void addWindow(Window* window){_window = window;}

	//get Methods
 		inline ChoiceType getChoiceType() const {return CHOICE_WINDOW;}
 		inline Window* getWindow() const {return _window;}

 	protected :
 		Window* _window;

};

class ChoiceValue : public Choice
{
	public:
		ChoiceValue();
		inline void init(char** label, uint8_t position);
		inline void addValue(uint8_t* value){_value = value;}
		inline void changeValue(int value) {*_value = value;}

	//get Methods
		inline ChoiceType getChoiceType() const {return CHOICE_VALUE;}
		inline uint8_t getValue() const {return *_value;}

	protected :
		uint8_t* _value;
};

#endif 
