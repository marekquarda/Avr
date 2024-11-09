
/*
 * LCD.c
 *
 * Author : Hossam Elbahrawy
 */ 
#include "lcd.h"


void lcd_init(void)
{	
	DATA_DDR = (1<<LCD_VCC) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5)| (1<<LCD_D4);
	CTL_DDR = (1<<LCD_EN) | (1<<LCD_RS) | (1<<LCD_K);

	DATA_BUS = (0<<LCD_VCC) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	CTL_BUS = (0<<LCD_EN) | (0<<LCD_RS) | (1<<LCD_K);

	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(20);									/* LCD Power ON delay always >15ms */
	//lcd_send_command(LCD_CMD_CURSOR_HOME);		    /* send for 4 bit initialization of LCD  */
	lcd_send_command(LCD_FUNCTION_4BIT_2LINES);     /* Use 2 line and initialize 5*7 matrix in (4-bit mode)*/
	lcd_send_command(LCD_DISP_ON_CURSOR_BLINK);     /* Display on cursor off*/
	//lcd_send_command(LCD_MOVE_CURSOR_RIGHT);              	/* Increment cursor (shift cursor to right)*/
	//lcd_send_command(LCD_CMD_CLEAR_DISPLAY);      	/* Clear display screen*/
	_delay_ms(2);
	lcd_send_command(0x80);							/* Cursor 1st row 0th position */
}

void lcd_send_command (unsigned char command)
{
	// High value
	DATA_BUS=((DATA_BUS & 0xF0) | ((command & 0xF0)>>4)); 
	CTL_BUS &=~(1<<LCD_RS);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(2);
	//low value
	DATA_BUS=((DATA_BUS & 0xF0) | (command & 0x0F));
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(1);
}

void lcd_write_word(char* str)
{
	int i=0;
	while(str[i]!='\0')
	{
		lcd_write_character(str[i]);
		i++;
	}
}

void lcd_write_character(char character)
{
	// high value
	DATA_BUS=((DATA_BUS & 0xF0) | ((character & 0xF0)>>4)); 
	CTL_BUS |=~(1<<LCD_RS);	/* RS=1, data reg. */
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(2);
	// low value
	DATA_BUS=((DATA_BUS & 0xF0) | (character & 0x0F));
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(2);
}

void lcd_clear(void)
{
	lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(5);
}

void lcd_goto_xy (uint8_t line,uint8_t pos)				//line = 0 or 1
{
	if (line == 0 && pos<16)
		lcd_send_command((pos & 0x0F) | 0x80);	/* Command of first row and required position<16 */
	else if (line == 1 && pos<16)
		lcd_send_command((pos & 0x0F) | 0xC0);	/* Command of first row and required position<16 */
	//	LCD_String(str);		/* Call LCD string function */

	//	lcd_send_command((0x80|(line<<6))+pos);
	_delay_us (50);
}