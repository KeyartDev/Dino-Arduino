//ACHTUNG!!!
//Эта хуйня разрабатывается для аттестации!! Поэтому не трогать блять это!! За невыполнение табу будет взыматься плата!!!
//--------------------------------------------------------------------------------------------------------------------------

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define printByte(args)  write(args);

LiquidCrystal_I2C lcd(0x27,20,4);

uint8_t road[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};
uint8_t sus_road[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x1f};
uint8_t sus_jump[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x0};
uint8_t cac_road[8] = {0x4,0x15,0x15,0x15,0x1f,0x4,0x4,0x1f};
uint8_t road_misc[8] = {};



void setup() {
  Serial.begin(115200);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.createChar(1, road);
  lcd.createChar(2, sus_road);
  lcd.createChar(3, cac_road);
  lcd.createChar(4, sus_jump);
  lcd.home();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,2);
  lcd.printByte(2);
  for (int i=0;i<20;i++) {
    if (i==1) continue;
    lcd.setCursor(i,2);
    lcd.printByte(1);
  }
  lcd.setCursor(12,2);
  lcd.printByte(3);

  lcd.setCursor(13,1);
  lcd.printByte(4);
}

void loop() {
  
}
