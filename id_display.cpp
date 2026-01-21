#include "id_display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void testdrawchar(void) {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  //delay(2000);
}

void displayNumber(uint16_t num)
{
  display.clearDisplay();

  display.setTextSize(3);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  int buf[20];
  int buf_i = 0;
  while (num > 9)
  {
    buf[buf_i] = num % 10;
    buf_i++;
    
    num /= 10;
  }
  buf[buf_i] = num;
  for (int i = buf_i;i >= 0;i--)
  {
    display.write(buf[i] + '0');  
  }

  display.display();
}

void clearDisplay(void)
{
  display.clearDisplay();
  display.display();
}

int8_t display_init(void)
{
  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
    return -1;
  }

  display.clearDisplay();
}

