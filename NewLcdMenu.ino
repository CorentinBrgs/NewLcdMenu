#include "NewLcdMenu.h"
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

//-----creating the LCD MENU UI-----/
	LcdMenu lcdMenu = LcdMenu(0x3F, 20, 4);

//-----Keypad declaration-----/
	const uint8_t ROWS = 2;
	const uint8_t COLS = 2;

	const char keys[ROWS][COLS] = {
		{'0','1'},
		{'2','3'}
	};
	const Event events[ROWS][COLS] = {
		{EVENT_OK, EVENT_CANCEL},
		{EVENT_MORE, EVENT_LESS}
	};

	const uint8_t rowPins[ROWS] = {3, 2};
	const uint8_t colPins[COLS] = {5, 4};

	Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
	char key = 0;	

//-----For LcdMenu UI -----/
	const char* const strDEMARRAGE_table[] PROGMEM = {
		"NewLcdMenu",
		"    by cocoBibi",
		"v0.1",
		""
	};

	Window menuPcp = Window(&lcdMenu.lcd);
	Window menuScd = Window(&lcdMenu.lcd, "Menu Secondaire");
	Window text1 = Window(&lcdMenu.lcd, "coucou :D");

void setup(){
	lcdMenu.begin(strDEMARRAGE_table);
	menuPcp.setTitle("Menu Principal");
	
	menuPcp.setNextWindow(&menuScd);
	menuScd.setNextWindow(&menuPcp);

	lcdMenu.addWindowToUI(&menuScd);
	lcdMenu.addWindowToUI(&menuPcp);

	lcdMenu.createEventTable(ROWS, COLS,(char*)keys, (Event*)events);

	lcdMenu.setCurrentWindow(&menuPcp);
	delay(1000);

}

void loop()
{
	key = kpd.getKey();
	lcdMenu.loop(lcdMenu.convertKeyToEvent(key));
}
