#ifndef LCD_CONTROL_GLA
#define LCD_CONTROL_GLA

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

void update_lcd(const char* name, int line_no, float temp, float set_temp, float ramped_temp, bool isOn, bool powered, int error);

void lcd_begin(void);

void set_name_len(int len);

#endif //LCD_CONTROL_GLA