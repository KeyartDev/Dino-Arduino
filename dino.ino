#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#define printByte(args)  write(args);
#define JUMPD 300
#define EMPTY 0
#define ROAD 1
#define SUSROAD 2
#define CACROAD 3
#define SUSJUMP 4
#define FUNC pow(millis()/100,1/3)
#define ARR 8


volatile LiquidCrystal_I2C lcd(0x27,20,4);

uint8_t road[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};
uint8_t sus_road[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x1f};
uint8_t sus_jump[8] = {0xe,0x1f,0x11,0x1f,0x1b,0x1b,0x0,0x0};
uint8_t cac_road[8] = {0x4,0x15,0x15,0x15,0x1f,0x4,0x4,0x1f};
uint8_t empty[8] = {};
uint8_t road_misc[8] = {};

void(* reset)(void) = 0;

volatile static uint32_t tmr;
volatile static uint32_t tmr1;
volatile static uint32_t tmr2;
volatile bool isJumped = false;
volatile bool isEnd = false;
volatile int arrSize = 0;
volatile int creationDelay = 0;
volatile bool isGameOver = false;
volatile int hearts = 3;
volatile int arrCounter = 0;
volatile int cacCount = 0;
volatile int lastCreationDelay = 0;

volatile int susPos = 2;
volatile int score = 0;

volatile int scores[3] = {0, 0, 0};

class Cactus;

void removeCac(Cactus* cac);

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

  void setEnded(bool flag) {
    this->isEnded = flag;
  }

  int getColumn() {
    return this->column;
  }

  int getRow() {
    return this->row;
  }

  void moveLeft() {
    if (isEnded) return;

    if (this->column <= 0) {
      this->setEnded(true);
      cacCount--;
      score+=3;
      lcd.setCursor(16,0);
      lcd.print("0000");
      lcd.setCursor(20-String(score).length(),0);
      lcd.print(String(score));
    }
    
    lcd.setCursor(getColumn(), getRow());
    lcd.printByte(ROAD);
  
    decColumn();
  
    lcd.setCursor(getColumn(), getRow());
    lcd.printByte(CACROAD);

    
  }
private:
  int column;
  int row;
  bool isEnded = false;
};


volatile Cactus* arr[ARR];

void addCac(Cactus* cac) {
  delete arr[arrCounter];
  arr[arrCounter] = cac;
  if (arrCounter == ARR-1) {arrCounter = 0;}
  else {arrCounter++;}
  arrSize = min(++arrSize,ARR);
  cacCount++;
}

void removeCac(Cactus* cac) {
  cacCount--;
  lcd.setCursor(0,2);
  lcd.printByte(ROAD);
  
  
}

void onClick() {
  Serial.println("Jump!");
  if (!isJumped) isJumped = true;
  if (isGameOver) {
    Serial.write('3');
    reset();
  }
}
void onReset() {
  if (!isGameOver) return;
  
  Serial.println("PRERIVANIE!!!");
  for (int i=0;i<3;i++) {
    scores[i]=0;
    EEPROM.write(i,0);
  }
  
  lcd.setCursor(5,1);
  lcd.print(scores[0] == 0 ? "1-st: --" : "1-st: " + String(scores[0]));
  lcd.setCursor(5,2);
  lcd.print(scores[1] == 0 ? "2-nd: --" : "2-nd: " + String(scores[1]));
  lcd.setCursor(5,3);
  lcd.print(scores[2] == 0 ? "3-rd: --" : "3-rd: " + String(scores[2]));
}

void setup() {
  Serial.begin(115200);

  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4 , OUTPUT);
  pinMode(5 , OUTPUT);
  pinMode(6 , OUTPUT);
  pinMode(9,OUTPUT);

  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  
  
  lcd.init();
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

  attachInterrupt(digitalPinToInterrupt(2), onClick, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), onReset, FALLING);
  Serial.write('1');
  lcd.setCursor(16,0);
  lcd.print("0000");
  isEnd = false;

  for (int i=0;i<3;i++) {
    if (EEPROM.read(i) != 255) continue;
    EEPROM.write(i,0);
  }

  for (int i=0;i<3;i++) {
    scores[i]=EEPROM.read(i);
  }
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
  
  if (arrSize > 0 && millis()-tmr >= 350-FUNC && !isGameOver) {
    for (int i=0;i<arrSize;i++) {
      arr[i]->moveLeft();
      lcd.setCursor(0,2);
      lcd.printByte(ROAD);
    }
    tmr = millis();
  }

  if (millis()-tmr1 >= creationDelay && cacCount < ARR && !isGameOver) {
    Cactus* cac = new Cactus(19, 2);
    addCac(cac);
    randomSeed(millis());
    if (lastCreationDelay <= 5) lastCreationDelay = random(6,8);
    else lastCreationDelay = random(4,8);
    creationDelay = (350-FUNC)*lastCreationDelay;
    tmr1 = millis();
  }

  for (int i=0;i<arrSize;i++) {
    if (susPos == 2 && arr[i]->getColumn() == 3 && millis()-tmr2 >= 350-FUNC) {
      digitalWrite(hearts+3,LOW);
      if (hearts > 1) Serial.write('2');
      hearts--;
      tmr2 = millis();
      if (hearts == 0) {
        isGameOver = true;
      }
      break;
    }
  }

  if (isGameOver && !isEnd) {
    Serial.write('0');
    lcd.print("GAME OVER!");
    for (int r=0;r<4;r++) {
      for (int h=0;h<20;h++) {
        lcd.setCursor(h,r);
        lcd.printByte(EMPTY);
      }
    }
    lcd.setCursor(5,0);
    lcd.print("GAME OVER!");
    if (score > scores[0]) {
      scores[2]=scores[1];
      scores[1]=scores[0];
      scores[0]=score;
    } else if (score > scores[1]) {
      scores[2]=scores[1];
      scores[1]=score;
    } else if (score > scores[2]) {
      scores[2]=score;
    }
      
    for (int i=0;i<3;i++){
        EEPROM.write(i,scores[i]);
    }
    
    lcd.setCursor(5,1);
    lcd.print(scores[0] == 0 ? "1-st: --" : "1-st: " + String(scores[0]));
    lcd.setCursor(5,2);
    lcd.print(scores[1] == 0 ? "2-nd: --" : "2-nd: " + String(scores[1]));
    lcd.setCursor(5,3);
    lcd.print(scores[2] == 0 ? "3-rd: --" : "3-rd: " + String(scores[2]));
    isEnd = true;
  }

  if (susPos == 2 && !isGameOver) {
    lcd.setCursor(3,2);
    lcd.printByte(SUSROAD);
  }
}

void yield() {
  if (arrSize > 0 && millis()-tmr >= 350-FUNC && !isGameOver) {
    for (int i=0;i<arrSize;i++) {
      arr[i]->moveLeft();
      lcd.setCursor(0,2);
      lcd.printByte(ROAD);
    }
    tmr = millis();
  }

  if (millis()-tmr1 >= creationDelay && cacCount < ARR && !isGameOver) {
    Cactus* cac = new Cactus(19, 2);
    addCac(cac);
    randomSeed(millis());
    if (lastCreationDelay <= 4) lastCreationDelay = random(6,8);
    else lastCreationDelay = random(4,8);
    creationDelay = (350-FUNC)*lastCreationDelay;
    tmr1 = millis();
  } 
 
  for (int i=0;i<arrSize;i++) {
    if (susPos == 2 && arr[i]->getColumn() == 3 && millis()-tmr2 >= 350-FUNC) {
      digitalWrite(hearts+3,LOW);
      if (hearts > 1) Serial.write('2');
      hearts--;
      tmr2 = millis();
      if (hearts == 0) {
        isGameOver = true;
      }
      break;
    }
  }                         
}
