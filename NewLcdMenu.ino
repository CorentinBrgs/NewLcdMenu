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
		{EVENT_OK, EVENT_MORE},
		{EVENT_CANCEL, EVENT_LESS}
	};

	const uint8_t rowPins[ROWS] = {4, 3};
	const uint8_t colPins[COLS] = {6, 5};

	Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
	char key = 0;	

//-----For LcdMenu UI -----/
	const char* const strDEMARRAGE_table[] PROGMEM = {
		"NewLcdMenu",
		"    by corentinB",
		"v0.4",
		""
	};

	const char* const strText1_table[] PROGMEM = {
		"      coucou !      ",
		" Gege ptite mere :D ",
		"  WESH le frerot    "};

	Window menuPcp = Window();
		WindowChoice wndChoice1 = WindowChoice();
			WindowText wndText1 = WindowText();
			WindowChoice wndChoice2 = WindowChoice();

	char* str_choiceWin1_1 = "window 1";
	ChoiceWindow choiceWin1_1 = ChoiceWindow();

	char* str_choiceWin1_2 = "window 2";
	ChoiceWindow choiceWin1_2 = ChoiceWindow();

	uint8_t val_choiceVal2_1 = 4;
	char* str_choiceVal2_1 = "choix 1";
	ChoiceValue choiceVal2_1 = ChoiceValue();

	uint8_t val_choiceVal2_2 = 4;
	char* str_choiceVal2_2 = "choix 2";
	ChoiceValue choiceVal2_2 = ChoiceValue();

void setup()
{
	lcdMenu.begin(strDEMARRAGE_table);
	
	choiceWin1_1.init(&str_choiceWin1_1, 0);
	choiceWin1_1.addWindow(&wndChoice2);

	choiceWin1_2.init(&str_choiceWin1_2, 1);
	choiceWin1_2.addWindow(&wndText1);

	choiceVal2_1.init(&str_choiceVal2_1, 0);
	choiceVal2_1.addValue(&val_choiceVal2_1);

	choiceVal2_2.init(&str_choiceVal2_2, 1);
	choiceVal2_2.addValue(&val_choiceVal2_2);

	menuPcp.init(&lcdMenu.lcd);
	menuPcp.setTitle("Menu Principal");

	wndChoice1.init(&lcdMenu.lcd);
	wndChoice1.setTitle("Je suis WndChoix1 :D");
	wndChoice1.addChoice(&choiceWin1_1);
	wndChoice1.addChoice(&choiceWin1_2);

	wndText1.init(&lcdMenu.lcd);
	wndText1.setTitle("Blablabla");
	wndText1.addText(strText1_table);

	wndChoice2.init(&lcdMenu.lcd);
	wndChoice2.setTitle("Je suis WndChoix2 :D");
	wndChoice2.addChoice(&choiceVal2_1);
	wndChoice2.addChoice(&choiceVal2_2);

	// menuPcp.setChildWindow(&wndText1);
	// //wndChoice2.setChildWindow(&wndChoice1);
	// wndText1.setChildWindow(&wndChoice2);
	// wndChoice2.setChildWindow(&wndChoice2);

	menuPcp.setChildWindow(&wndChoice1);
	//wndChoice2.setChildWindow(&wndChoice1);
	wndChoice1.setChildWindow(&wndChoice2);
	wndText1.setChildWindow(&wndText1);
	wndChoice2.setChildWindow(&wndChoice2);

	//please add first the root window. This window will have an _id = 0.
	//that means it will have no father window. 

	lcdMenu.addWindowToUI(&menuPcp);
	lcdMenu.addWindowToUI(&wndChoice1);
	lcdMenu.addWindowToUI(&wndText1);	
	lcdMenu.addWindowToUI(&wndChoice2);

	lcdMenu.createEventTable(ROWS, COLS,(char*)keys, (Event*)events);
	lcdMenu.setCurrentWindow(&menuPcp);

}

void loop()
{

//Uncomment this and comment the rest to have normal working for LcdMenu. 
	key = kpd.getKey();
	lcdMenu.loop(lcdMenu.convertKeyToEvent(key));

	//lcdMenu.testCharLcd();

}
