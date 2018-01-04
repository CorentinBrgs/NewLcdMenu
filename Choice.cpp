#include <Arduino.h>
#include "Choice.hpp"

Choice::Choice() {
}

ChoiceWindow::ChoiceWindow() {
}

void ChoiceWindow::init(char** label, uint8_t position) 
{
	Choice::init(label, position);
	 _window = NULL;
}

ChoiceValue::ChoiceValue() {
}

void ChoiceValue::init(char** label, uint8_t position) 
{
	Choice::init(label, position);
	_value = NULL;
}