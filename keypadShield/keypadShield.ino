/*
  Arduino 2x16 LCD - Detect Buttons
  modified on 18 Feb 2019
  by Saeed Hosseini @ Electropeak
  https://electropeak.com/learn/
*/
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

// Make custom characters:
byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte Bell[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
byte Alien[] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B11011,
  B00000
};
byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};
byte Speaker[] = {
  B00001,
  B00011,
  B01111,
  B01111,
  B01111,
  B00011,
  B00001,
  B00000
};
byte Sound[] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
};
byte Skull[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};
byte Lock[] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};
void setup() {
  // Specify the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Create a new characters:
  lcd.createChar(0, Heart);
  lcd.createChar(1, Bell);
  lcd.createChar(2, Alien);
  lcd.createChar(3, Check);
  lcd.createChar(4, Speaker);
  lcd.createChar(5, Sound);
  lcd.createChar(6, Skull);
  lcd.createChar(7, Lock);
  // Clears the LCD screen:
  lcd.clear();
  // Print a message to the lcd:
  lcd.print("Custom");
  lcd.cursor();
  lcd.blink();
  /*
  lcd.setCursor(16, 1);
  lcd.autoscroll();
  */
}

void loop2() {
}
void loop3() {
  // Print all the custom characters:
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(2, 1);
  lcd.write(byte(1));
  lcd.setCursor(4, 1);
  lcd.write(byte(2));
  lcd.setCursor(6, 1);
  lcd.write(byte(3));
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(5));
  lcd.setCursor(12, 1);
  lcd.write(byte(6));
  lcd.setCursor(14, 1);
  lcd.write(byte(7));
}


void loop() {
  int x;
  x = analogRead (0);
  if (x < 60) {
    lcd.write(byte(0));
  }
  else if (x < 200) {
    lcd.write(byte(1));
  }
  else if (x < 400) {
    lcd.write(byte(2));
  }
  else if (x < 600) {
    lcd.write(byte(3));
  }
  else if (x < 800) {
    lcd.write(byte(4));
  }
  delay(300);
  lcd.cursor();
}
