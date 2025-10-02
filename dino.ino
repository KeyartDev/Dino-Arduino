#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define printByte(args)  write(args);
#define JUMPD 300
#define EMPTY 0
#define ROAD 1
#define SUSROAD 2
#define CACROAD 3
#define SUSJUMP 4


volatile LiquidCrystal_I2C lcd(0x27,20,4);

uint8_t road[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};
uint8_t sus_road[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x1f};
uint8_t sus_jump[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x0};
uint8_t cac_road[8] = {0x4,0x15,0x15,0x15,0x1f,0x4,0x4,0x1f};
uint8_t empty[8] = {};
uint8_t road_misc[8] = {};


volatile static uint32_t tmr;
volatile static uint32_t tmr1;
volatile static uint32_t tmr2;
volatile bool isJumped = false;
volatile int arrSize = 0;
volatile int creationDelay = 0;
volatile bool isGameOver = false;
volatile int hearts = 3;

volatile int susPos = 2;

void removeCac(int pos);

class Cactus {
public:
  Cactus(int column, int row) {
    this->column = column;
    this->row = row;
  }

  void incColumn() {
    this->column++;
  }

  void decColumn() {
    if (this->column != 0) {
      this->column--;
    }
  }

  int getColumn() {
    return this->column;
  }

  int getRow() {
    return this->row;
  }

  void moveLeft() {
    lcd.setCursor(getColumn(), getRow());
    lcd.printByte(ROAD);
  
    decColumn();
  
    lcd.setCursor(getColumn(), getRow());
    lcd.printByte(CACROAD);

    if (this->column == 0) {
      removeCac(this->pos);
      lcd.setCursor(0,2);
      lcd.printByte(ROAD);
    }
  }

  void setPos(int pos) {
    this->pos = pos;
  }

  void decPos() {
    this->pos = this->pos == 0 ? 0 : this->pos-1;
  }
private:
  int column;
  int row;
  int pos;
};


volatile Cactus* arr[3];

void addCac(Cactus* cac) {
  cac->setPos(arrSize);
  arr[arrSize] = cac;
  arrSize++;
}

void removeCac(int pos) {
  for (int i=pos;i<2;i++) {
    arr[i]=arr[i+1];
    arr[i]->decPos();
  }
  arrSize--;

  lcd.setCursor(0,2);
  lcd.printByte(1);
}

void onClick() {
  Serial.println("Jump!");
  if (!isJumped) isJumped = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(2,INPUT_PULLUP);
  pinMode(3 , OUTPUT);
  pinMode(4 , OUTPUT);
  pinMode(5 , OUTPUT);

  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.createChar(ROAD, road);
  lcd.createChar(SUSROAD, sus_road);
  lcd.createChar(CACROAD, cac_road);
  lcd.createChar(SUSJUMP, sus_jump);
  lcd.createChar(EMPTY, empty);
  lcd.home();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,2);
  lcd.printByte(SUSROAD);
  for (int i=0;i<20;i++) {
    if (i==3) continue;
    lcd.setCursor(i,2);
    lcd.printByte(ROAD);
  }

  attachInterrupt(digitalPinToInterrupt(2), onClick, RISING);
  
  //delay(2000);
  Cactus* cac = new Cactus(12,2);
  addCac(cac);
}

void loop() {
  if (isJumped) {
    lcd.setCursor(3,2);
    lcd.printByte(ROAD);
  
    lcd.setCursor(3,1);
    lcd.printByte(SUSJUMP);
    susPos--;

    delay(JUMPD);
    
    lcd.setCursor(3,1);
    lcd.printByte(EMPTY);
  
    lcd.setCursor(3,0);
    lcd.printByte(SUSJUMP);
    susPos--;

    delay(JUMPD+200);
    //----------------------------

    lcd.setCursor(3,0);
    lcd.printByte(EMPTY);

    lcd.setCursor(3,1);
    lcd.printByte(SUSJUMP);
    susPos++;

    delay(JUMPD);

    lcd.setCursor(3,1);
    lcd.printByte(EMPTY);

    lcd.setCursor(3,2);
    lcd.printByte(SUSROAD);
    susPos++;

    delay(JUMPD);
    
    isJumped = false;
  }
  
  if (arrSize > 0 && millis()-tmr >= 300) {
    for (int i=0;i<arrSize;i++) {
      arr[i]->moveLeft();
    }
    tmr = millis();
  }

  if (millis()-tmr1 >= creationDelay && arrSize < 3) {
    Cactus* cac = new Cactus(19, 2);
    if (arrSize != 3) addCac(cac);
    
    
    randomSeed(analogRead(A0));
    creationDelay = random(1500,2500);
    tmr1 = millis();
  }

  for (int i=0;i<arrSize;i++) {
    if (susPos == 2 && arr[i]->getColumn() == 3 && millis()-tmr2 >= 300) {
      digitalWrite(hearts+2,LOW);
      hearts--;
      tmr2 = millis();
      if (hearts == 0) {
        isGameOver = true;
      }
      break;
    }
  }

  if (isGameOver) {
    lcd.print("GAME OVER!");
    for (int r=0;r<4;r++) {
      for (int c=0;c<20;c++) {
        lcd.setCursor(c,r);
        lcd.printByte(EMPTY);
      }
    }
    lcd.setCursor(5,1);
    lcd.print("GAME OVER!");
    while (true) {}
  }

  if (susPos == 2) {
    lcd.setCursor(3,2);
    lcd.printByte(SUSROAD);
  }
}

void yield() {
  if (arrSize > 0 && millis()-tmr >= 300) {
    for (int i=0;i<arrSize;i++) {
      arr[i]->moveLeft();
    }
    tmr = millis();
  }

  if (millis()-tmr1 >= creationDelay && arrSize < 3) {
    Cactus* cac = new Cactus(19, 2);
    if (arrSize != 3) addCac(cac);
    
    
    randomSeed(analogRead(A0));
    creationDelay = random(1500,2500);
    tmr1 = millis();
  }

  for (int i=0;i<arrSize;i++) {
    if (susPos == 2 && arr[i]->getColumn() == 3 && millis()-tmr2 >= 300) {
      digitalWrite(hearts+2,LOW);
      hearts--;
      tmr2 = millis();
      if (hearts == 0) {
        isGameOver = true;
      }
      lcd.setCursor(3,2);
      lcd.printByte(SUSROAD);
      break;
    }
  }

                            
}
