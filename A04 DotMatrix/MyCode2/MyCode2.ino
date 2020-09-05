#include "LedControl.h"

LedControl lc = LedControl(11, 13, 10, 4); // DIN,CLK,CS,Number of LED Module
int left_button = 5;
int joystick_button = 8;
int joystick_axis_x = A0;
int shipbaseX, shipbaseY = 7, XFirst = 2, XLast = XFirst + 2; //Ship
int EShipX, EShipY, EFirst;
int bulletX, bulletY = 7, address = 1;
int bombX , bombY , BAddress = 2;
int score = 0, life = 6;
unsigned long int timeMove1, timeMove2, timeShoot1, timeShoot2, ShootMove1, ShootMove2;
unsigned long int EMove1, EMove2, BombMove1, BombMove2;
bool bulletstate = false, bombstate = false, bomb;

byte num[11][8] =  { //Font Number
  {0x0, 0x0, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x0},
  {0x0, 0x0, 0x0, 0x1, 0x7F, 0x21 , 0x00 , 0x0},
  {0x0, 0x0, 0x31, 0x49, 0x45, 0x43, 0x21, 0x0},
  {0x0, 0x0, 0x36, 0x41, 0x49, 0x49, 0x22, 0x0},
  {0x0, 0x0, 0x4 , 0x7F, 0x24, 0x14, 0xC, 0x0},
  {0x0, 0x0, 0x4E, 0x51, 0x51, 0x51, 0x72, 0x0},
  {0x0, 0x0, 0x26, 0x49, 0x49, 0x49, 0x3E, 0x0},
  {0x0, 0x0, 0x60, 0x50, 0x4F, 0x40, 0x40, 0x0},
  {0x0, 0x0, 0x36, 0x49, 0x49, 0x49, 0x36, 0x0},
  {0x0, 0x0, 0x3E, 0x49, 0x49, 0x49, 0x32, 0x0},
  {0, 0, 0, 0, 0},
};
byte text[7][8] = { //Font Char
  {0x0, 0x0, 0x7f, 0x02, 0x04, 0x02, 0x7f, 0x0}, // W
  {0x0, 0x0, 0x00, 0x41, 0x7f, 0x41, 0x00, 0x0}, // I
  {0x0, 0x0, 0x7f, 0x04, 0x08, 0x10, 0x7f, 0x0}, // N
  {0x0, 0x0, 0x01, 0x01, 0x01, 0x01, 0x7f, 0x0}, // L
  {0x0, 0x0, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x0}, // O
  {0x0, 0x0, 0x46, 0x49, 0x49, 0x49, 0x31, 0x0}, // S
  {0x0, 0x0, 0x41, 0x49, 0x49, 0x49, 0x7f, 0x0}, // E
};

void setup() {
  randomSeed(5);
  Serial.begin(9600);
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, 1);
    lc.clearDisplay(address);
  }
  for (shipbaseX = XFirst; shipbaseX <= XLast; shipbaseX++) {
    lc.setLed(1, shipbaseX, shipbaseY, true);
    lc.setLed(1, XFirst + 1, shipbaseY - 1, true);
  }
  start(0, num[life]);
  start(3, num[score]);
}

void loop() {
  //-------------MoveGunLeft-------------//
  if (analogRead(joystick_axis_x) <= 450 && XFirst <= 5) { //Left
    timeMove1 = millis();
    for (shipbaseX = XFirst; shipbaseX <= XLast; shipbaseX++) {
      lc.setLed(1, shipbaseX, shipbaseY, false);
      lc.setLed(1, XFirst + 1, shipbaseY - 1, false);
    }
    if (timeMove1 - timeMove2 > 100) {
      XFirst++; XLast++;
      timeMove2 = timeMove1;
    }
  }
  if (analogRead(joystick_axis_x) >= 600 && XFirst >= 0) { //Right
    timeMove1 = millis();
    for (shipbaseX = XFirst; shipbaseX <= XLast; shipbaseX++) {
      lc.setLed(1, shipbaseX, shipbaseY, false);
      lc.setLed(1, XFirst + 1, shipbaseY - 1, false);
    }
    if (timeMove1 - timeMove2 > 100) {
      XFirst--; XLast--;
      timeMove2 = timeMove1;
    }
  }
  for (shipbaseX = XFirst; shipbaseX <= XLast; shipbaseX++) {
    lc.setLed(1, shipbaseX, shipbaseY, true);
    lc.setLed(1, XFirst + 1, shipbaseY - 1, true);
  }
  //-------------Enermy-Ship-------------//
  EMove1 = millis();
  if (EMove1 - EMove2 > 500) {
    for (EShipX = EFirst; EShipX <= EFirst + 2; EShipX++) {
      lc.setLed(2, EShipX, EShipY, false);
      EMove2 = EMove1;
    }
    EShipX = random(0, 6); EFirst = EShipX;
    EShipY = random(0, 2);

    if (bombstate == false) {
      bombstate = true;
      bombX = EFirst + 1;
      bombY = EShipY + 1;
    }
  }
  for (EShipX = EFirst; EShipX <= EFirst + 2; EShipX++) {
    lc.setLed(2, EShipX, EShipY, true);
  }
  //-------------Drop_Bomb-------------//
  if (bombstate == true) {
    BombMove1 = millis();
    if (BAddress > 0) {
      lc.setLed(BAddress, bombX, bombY, true);
      if (BombMove1 - BombMove2 > 50) {
        lc.setLed(BAddress, bombX, bombY, false);
        bombY++;
        BombMove2 = BombMove1;
      }
    }
  }
  if (bombY == 8) {
    bombY = 0;
    BAddress--;
    if (BAddress == 0) {
      bombstate = false;
      BAddress = 2;
      if (life > 0) start(0, num[--life]);
    }
  }

  //-------------Shoot-------------//
  if (digitalRead(left_button) == 0) {
    timeShoot1 = millis();
    if (timeShoot1 - timeShoot2 > 200) {
      if (bulletstate == false) {
        bulletstate = true;
        bulletX = XFirst + 1;
      }
      timeShoot2 = timeShoot1;
    }
  }
  if (bulletstate == true) {
    ShootMove1 = millis();
    if (address < 3) {
      lc.setLed(address, bulletX, bulletY, true);
      if (ShootMove1 - ShootMove2 > 50) {
        lc.setLed(address, bulletX, bulletY, false);
        bulletY--;
        ShootMove2 = ShootMove1;
      }
    }
  }
  if (bulletY == -1) {
    bulletY = 7;
    address++;
    if (address == 3) {
      bulletstate = false;
      address = 1;
    }
  }

  //-------------Check-Crash-------------//
  if (bombX == bulletX && bombY == bulletY && BAddress == address) {
    lc.setLed(BAddress, bombX, bombY, false);
    lc.setLed(address, bulletX, bulletY, false);
    bombstate = false;
    BAddress = 2;
    bulletstate = false;
    address = 1;
    bombX = -1; bombY = -1; bulletX = -1; bulletY = 7;
    if (score < 10) start(3, num[++score]);
  }

  //-------------Game-End-------------//
  if (score == 9) { //Win
    clr();
    bombstate = false; BAddress = 2;
    bulletstate = false; address = 1;
    lc.clearDisplay(address);
    start(3, text[0]); //Print Win
    start(2, text[1]); 
    start(1, text[2]); 
    start(0, num[10]);
    delay(3000);
    clr();
    delay(500);
    score = 0; start(3, num[score]);
    life = 6; start(0, num[life]);
  }
  if (life == 0) { //Lose
    clr();
    bombstate = false; BAddress = 2;
    bulletstate = false; address = 1;
    lc.clearDisplay(address);
    start(3, text[3]); //Print Lose
    start(2, text[4]); 
    start(1, text[5]); 
    start(0, text[6]);
    delay(3000);
    clr();
    delay(500);
    score = 0; start(3, num[score]);
    life = 6; start(0, num[life]);
  }
}

//--------------------------Loop-End--------------------------//

void start(int add , byte test[]) { //Print Char
  for (int i = 0; i < 8; i++) {
    lc.setRow(add, i, test[i]);
  }
}

void clr() { //Clear Screen
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++)
      for (int k = 0; k < 8; k++)
      {
        lc.setLed(i, j, k, false);
        delay(5);
      }
}
