/*void LcdMenu::setKeypad(uint8_t *rowPins, uint8_t *colPins, uint8_t ROWS, uint8_t COLS)
{
	_keypadROWS = ROWS;
	_keypadCOLS = COLS;

	_keypadRowPins = new uint8_t[_keypadROWS];
	_keypadColPins = new uint8_t[_keypadCOLS];

	_keypadRowPins = rowPins;
	_keypadColPins = colPins;

	//_keypadArray = new int[2][2];

	for (int i=0; i<_keypadROWS; i++) 
	{
		for (int j=0; j<_keypadCOLS; j++) 
		{
			_keypadArray[i][j] = (char) 2*i + j;
			Serial.print("Assignement de _keypadArray[");
			Serial.print(i);
			Serial.print("][");
			Serial.print(j);
			Serial.print("] = ");
			Serial.println(_keypadArray[i][j]);
		}
	}
	for (int i=0; i<_keypadROWS; i++) 
	{
		Serial.print("Lecture de _keypadRowPins[");
		Serial.print(i);
		Serial.print("] = ");
		Serial.println(_keypadRowPins[i]);
	}
	for (int j=0; j<_keypadCOLS; j++) 
	{
		Serial.print("Lecture de _keypadColPins[");
		Serial.print(j);
		Serial.print("] = ");
		Serial.println(_keypadColPins[j]);
	}


	keypad = new Keypad(makeKeymap(_keypadArray), _keypadRowPins, _keypadColPins, _keypadROWS, _keypadCOLS); 
	_keypadKey = 'a';
}*/