#define BPM 136

#define LAD 220.00
#define SIDB 233.08
#define SID 246.96
#define DO 261.63
#define DOS 277.18
#define RE 293.66
#define RES 311.13
#define MI 329.63
#define FA 349.23
#define FAS 369.99
#define SOL 392.00
#define SOLS 415.30
#define LA 440.00
#define SIB 466.16
#define SI 493.88
#define DOUP 523.25
#define DOUPS 554.36
#define REUP 587.32
#define REUPS 622.26
#define MIUP 659.26
#define FAUP 698.46
#define FAUPS 739.98
#define SOLUP 784.00
#define SOLUPS 830.60
#define LAUP 880.00
#define SIUPB 932.32
#define SIUP 987.75
#define DOUU 1046.50
#define DOUUS 1108.70
#define REUU 1174.60
#define REUUS 1244.50

#define REP for(int x=0;x<2;x++) 

void(* reset)(void) = 0;

const long MINUTE = 60000000;
const long double TAKT = (MINUTE/BPM)*4;
const long HALF = TAKT/2;
const long FOUR = TAKT/4;
const long EIGHT = TAKT/8;
const long SIXTEEN = TAKT/16;

volatile bool isGameOver = true;

#define FOUR_PAUSE delayMicroseconds(FOUR);

void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(115200);
}

void playTone(float v, long t, bool withPoint, int port=10) {
  if (!isGameOver) {
    t = withPoint ? t+t/2 : t;
    const long T = 1000000 / v;
    long d = T/2;
    int count = t / T;
    Serial.println("V: " + String(v) + "   |   point: " + String(withPoint) + "   |   length: " + String(t));
    for (int i=0;i<count;i++) {
      digitalWrite(port, HIGH);
      delayMicroseconds(d);
      digitalWrite(port, LOW);
      delayMicroseconds(d);
    }
    delay(10);
  }
}

void rest(long t) {
  Serial.println("----------------DELAY: " + String(t/1000) + "----------------");
  delay(t/1000);
}

void onGameOver() {
  delay(500);
  
  playTone(LAUP,FOUR,false);
  playTone(DOUP,FOUR,false);
  playTone(REUPS,FOUR,false);
  playTone(REUP,FOUR,false);
  playTone(DOUP,FOUR,false);
  playTone(LA,FOUR,false);
  playTone(LA,HALF,false);

  isGameOver = true;
}

void onDamage() {
  for (int i=0;i<10;i++) {
    playTone(275,SIXTEEN/16,false);
    playTone(225,SIXTEEN/16,false);
  }
}

void onStart() {
  isGameOver = false;
}

void loop() {
  Serial.println("---------------------BEGIN---------------------");

  if (!isGameOver) {
    //1
    playTone(LA,FOUR,false);
    playTone(FAUPS,FOUR,false);
    playTone(SIUP,HALF,false);
  
    rest(FOUR);
    playTone(LAUP,FOUR,false);
    playTone(LAUP,FOUR,false);
    playTone(LAUP,FOUR,false);
  
    //2
    playTone(SOLUPS,FOUR,false);
    playTone(SOLUPS,FOUR,false);
    playTone(MIUP,HALF,false);
  
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(FAUPS,FOUR,false);
    playTone(SI,FOUR,false);
  
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(FAUPS,FOUR,false);
    playTone(SI,FOUR,false);
  
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(FAUPS,EIGHT,false);
    playTone(SI,FOUR,false);
  
    //3
    rest(FOUR);
    playTone(SI,EIGHT,false);
    playTone(SI,EIGHT,false);
    playTone(SI,FOUR,false);
    playTone(REUP,FOUR,false);
  
    rest(EIGHT);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(REUP,EIGHT,false);
    playTone(MIUP,FOUR,false);
    playTone(SI,FOUR,false);
  
    rest(EIGHT);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(REUP,EIGHT,false);
    playTone(MIUP,FOUR,false);
    playTone(SI,FOUR,false);
  
    rest(EIGHT);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,FOUR,false);
    playTone(SOLUPS,FOUR,false);
  
    //4
    rest(EIGHT);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,FOUR,false);
    playTone(FAUPS,FOUR,false);
  
    rest(EIGHT);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(REUP,EIGHT,false);
    playTone(MIUP,FOUR,false);
    playTone(SI,FOUR,false);
  
    rest(EIGHT);
    playTone(FAUPS,EIGHT,false);
    playTone(MIUP,EIGHT,false);
    playTone(REUP,EIGHT,false);
    playTone(MIUP,FOUR,false);
    playTone(SI,FOUR,false);
  
    rest(EIGHT);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,EIGHT,false);
    playTone(LAUP,FOUR,false);
    playTone(SOLUPS,FOUR,false);
  
    rest(EIGHT);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,EIGHT,false);
    playTone(SOLUPS,FOUR,false);
    playTone(FAUPS,FOUR,false);
  
    Serial.println("----------------------END----------------------");
  
    
  } 

  while (Serial.available()) {
    char n = Serial.read();
    if (n == '1') {
      isGameOver = false;
    } else if (n == '0') {
      onGameOver();
    } else if (n == '2') {
      onDamage();
    } else if (n == '3') {
      reset();
    }
    Serial.println(n);
  }
}

void yield() {
  while (Serial.available()) {
    char n = Serial.read();
    if (n == '1') {
      isGameOver = false;
    } else if (n == '0') {
      onGameOver();
    } else if (n == '2') {
      onDamage();
    } else if (n == '3') {
      reset();
    }
    Serial.println(n);
  }
}
