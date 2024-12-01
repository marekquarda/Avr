#include "micromenu.h"
#include "lcd.h"
#include "clock.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Time menu item select callback function */
static void Time_Select(void)
{
    LCDclr();
    PCF_DateTime datetime;
    LCDstring((uint8_t*)"Time:", 5);
    LCDshiftRight(6);
    while (1)
    {
       	uint8_t err = PCF_GetDateTime(&datetime);
 		char ho[4];
 		char min[4];
 		char sec[4];
		//itoa(datetime.day, str, 1);
		//lcd_write_word(sprintf(print_buffer,"\r20%.2f/%.2f/%.2f", datetime.year, datetime.month, datetime.day));
		
		//lcd_write_word("Time: ");
		//LCDstring((uint8_t*)(itoa(datetime.hour, ho, 10), 2);
		LCDstring((uint8_t*)(itoa(datetime.hour, ho, 10)), 2);
		LCDsendChar(':');
		//LCDstring((uint8_t*)':',);
		LCDstring((uint8_t*)(itoa(datetime.minute, min, 10)), 2);
		LCDsendChar(':');
		LCDstring((uint8_t*)(itoa(datetime.second, sec, 10)), 2);
		//lcd_write_character(' ');
                /* code */
    }

}

static void Time_Enter(void)
{
    LCDclr();
    LCDstring("Time Ent");
}


/** Date menu item select callback function */
static void Date_Select(void)
{
    LCDclr();
    LCDstring("Date Sel");
}

static void Date_Enter(void)
{
    LCDclr();
    LCDstring("Date Ent");
}

/** Voltage menu item select callback function */
static void Voltage_Select(void)
{
    LCDclr();
    LCDstring("Voltage Sel");
}

static void Voltage_Enter(void)
{
    LCDclr();
    LCDstring("Voltage Ent");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Current_Select(void)
{
    LCDclr();
    LCDstring("Current Sel");
}

static void Current_Enter(void)
{
    LCDclr();
    LCDstring("Current Ent");
}


// Menu initialization
MENU_ITEM(Menu_1, Menu_2, Menu_4, NULL_MENU, NULL_MENU, Time_Select, Time_Enter, "1");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU, Date_Select, Date_Enter, "2");
MENU_ITEM(Menu_3, Menu_4, Menu_2, NULL_MENU, NULL_MENU, Voltage_Select, Voltage_Enter, "3");
MENU_ITEM(Menu_4, Menu_1, Menu_3, NULL_MENU, NULL_MENU, Current_Select,Current_Enter, "4");

// SubMenu initialization
MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");

void InitMenu() {
    // Init menu
	Menu_Navigate(&Menu_1);
}