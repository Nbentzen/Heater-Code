#include "LCDControl.h"

hd44780_I2Cexp lcd(0x27); // declare lcd object: auto locate & auto config expander chip

#define LCD_COLS 20
#define LCD_ROWS 4

void lcd_begin()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
}

static int longest_name;
void set_name_len(int len)
{
  if(len > longest_name) {longest_name = len;}
}

void update_lcd(const char* name, int line_no, float temp, float set_temp, float ramped_temp, bool isOn, bool powered, int error)
{
  if(line_no >= LCD_ROWS || line_no < 0){return;}

  lcd.setCursor(0, line_no);
  lcd.print(name);

  int hPos = longest_name;

  lcd.setCursor(longest_name, line_no);
  if(powered)
  {
    hPos += lcd.print(" # ");
  }
  else
  {
    hPos += lcd.print(" _ ");
  }

  if(error == 0 && isOn)
  {
    hPos += lcd.print((int)temp);
    hPos += lcd.print("/");
    hPos += lcd.print((int)ramped_temp);
    hPos += lcd.print("/");
    hPos += lcd.print((int)set_temp);
  }
  if(error != 0)
  {
    if(error == 1)
    {
      hPos += lcd.print("open t/c");
    }
    else if(error == 4)
    {
      hPos += lcd.print("comm err");
    }
    else
    {
      hPos += lcd.print("error ");
      hPos += lcd.print(error);
    }
  }
  if(error == 0 && !isOn)
  {
    hPos += lcd.print("off / ");
    hPos += lcd.print((int)temp);
  }
  while(hPos < LCD_COLS)
  {
    hPos += lcd.print(" ");
  }

}

