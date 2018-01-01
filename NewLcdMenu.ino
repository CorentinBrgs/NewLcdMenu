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

	const char* const strText1_table[] PROGMEM = {
		"      coucou !      ",
		"Gege ptite mere :D  ",
		" WESH le frerot     "};

	Window menuPcp = Window();
	Window menuScd = Window();
	WindowText text1 = WindowText();

void setup()
{
	lcdMenu.begin(strDEMARRAGE_table);

	menuPcp.init(&lcdMenu.lcd);
	menuPcp.setTitle("Menu Principal");

	menuScd.init(&lcdMenu.lcd);
	menuScd.setTitle("Menu Secondaire");

	text1.init(&lcdMenu.lcd);
	text1.setTitle("Blablabla");
	text1.addText(strText1_table);
	
	menuPcp.setChildWindow(&menuScd);
	menuScd.setChildWindow(&text1);
	text1.setChildWindow(&text1);

	//please add first the root window. This window will have an _id = 0.
	//that means it will have no father window. 

	lcdMenu.addWindowToUI(&menuPcp);
	lcdMenu.addWindowToUI(&menuScd);
	lcdMenu.addWindowToUI(&text1);

	lcdMenu.createEventTable(ROWS, COLS,(char*)keys, (Event*)events);

	lcdMenu.setCurrentWindow(&menuPcp);
	delay(1000);

}

void loop()
{
	key = kpd.getKey();
	lcdMenu.loop(lcdMenu.convertKeyToEvent(key));
}
