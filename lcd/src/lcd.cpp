
/*
 * LCD.c
 *
 * Author : Hossam Elbahrawy
 */ 
#include "lcd.h"


void lcd_init(void)
{	
	
	DATA_DDR = (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5)| (1<<LCD_D4) | (1<<LCD_VCC);
	CTL_DDR |= (1<<LCD_EN)|(1<<LCD_RS)|(1<<LCD_K);

	DATA_BUS = (0<<LCD_D7)|(0<<LCD_D6)|(0<<LCD_D5)|(0<<LCD_D4)|(1<<LCD_VCC);
	CTL_BUS|= (1<<LCD_EN)|(0<<LCD_RS)|(1<<LCD_K);

	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(20);
	lcd_send_command(0x33);
	lcd_send_command(0x32);	/* Send for 4 bit initialization of LCD  */
	lcd_send_command(LCD_FUNCTION_4BIT_2LINES); /* 2 line, 5*7 matrix in 4-bit mode */
	lcd_send_command(LCD_DISP_ON_CURSOR_BLINK);	/* Display on cursor off */
	lcd_send_command(0x06);	/* Increment cursor (shift cursor to right) */
	lcd_send_command(0x01);	/* Clear display screen */

	// lcd_send_command(LCD_FUNCTION_4BIT_2LINES); 
	// _delay_ms(1);
	// lcd_send_command(LCD_DISP_ON_CURSOR_BLINK);
	// _delay_ms(100);
	// lcd_send_command(0x80);
}

void lcd_send_command (uint8_t command)
{
	// High value
	DATA_BUS=(DATA_BUS&0xF0) | ((command>>4)&0x0F); 
	CTL_BUS &=~(1<<LCD_RS);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(200);
	//low value
	DATA_BUS=(DATA_BUS&0xF0) | (command&0x0F);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(1);

	// LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);/* Sending upper nibble */
	// LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	// LCD_Port |= (1<<EN);		/* Enable pulse */
	// _delay_us(1);
	// LCD_Port &= ~ (1<<EN);
	// _delay_us(200);
	// LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);/* Sending lower nibble */
	// LCD_Port |= (1<<EN);
	// _delay_us(1);
	// LCD_Port &= ~ (1<<EN);
	// _delay_ms(2);
}

void lcd_write_word(char* word)
{
	int i=0;
	while(word[i]!='\0')
	{
		lcd_write_character(word[i]);
		i++;
	}
}

void lcd_write_character(char character)
{
	// high value
	DATA_BUS=(DATA_BUS&0xF0) | ((character>>4)&0x0F); 
	CTL_BUS &=~(1<<LCD_RS);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(200);
	// low value
	DATA_BUS=(DATA_BUS&0xF0) | (character&0x0F);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(2);
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
	lcd_send_command((0x80|(line<<6))+pos);
	_delay_us (50);
}