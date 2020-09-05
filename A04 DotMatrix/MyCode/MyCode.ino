#include "LedControl.h"

LedControl lc = LedControl(11, 13, 10, 4); // DIN,CLK,CS,Number of LED Module
int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int joystick_button = 8;
int joystick_axis_x = A0;
int shipbaseX, shipbaseY = 7, XFirst = 2, XLast = XFirst + 2; //Ship
int EShipX, EShipY, EFirst;
int bulletX[5] = {}, bulletY[5] = {7, 7, 7, 7, 7}, address[5] = {1, 1, 1, 1, 1};
int bombX[5] = {}, bombY[5] = {}, BAddress[5] = {2, 2, 2, 2, 2};
int score = 0, life = 9;
unsigned long int timeMove1, timeMove2, timeShoot1, timeShoot2, ShootMove1[5], ShootMove2[5];
unsigned long int EMove1, EMove2, BombMove1[5], BombMove2[5];
bool bulletstate[5] = {}, bombstate[5] = {}, bomb, game;
byte num[11][8] =  {
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
byte text[7][8] = {
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

    for (int i = 0; i <= 4; i++) {
      if (bombstate[i] == false) {
        bombstate[i] = true;
        bombX[i] = EFirst + 1;
        bombY[i] = EShipY + 1;
        break;
      }
    }
  }
  for (EShipX = EFirst; EShipX <= EFirst + 2; EShipX++) {
    lc.setLed(2, EShipX, EShipY, true);
  }
  //-------------Drop_Bomb-------------//
  for (int i = 0; i <= 4; i++) {
    if (bombstate[i] == true) {
      BombMove1[i] = millis();
      if (BAddress[i] > 0) {
        lc.setLed(BAddress[i], bombX[i], bombY[i], true);
        if (BombMove1[i] - BombMove2[i] > 100) {
          lc.setLed(BAddress[i], bombX[i], bombY[i], false);
          bombY[i]++;
          BombMove2[i] = BombMove1[i];
        }
      }
    }
    if (bombY[i] == 8) {
      bombY[i] = 0;
      BAddress[i]--;
      if (BAddress[i] == 0) {
        bombstate[i] = false;
        BAddress[i] = 2;
        if (life > 0) start(0, num[--life]);
        Serial.println(life);
      }
    }
  }

  //-------------Shoot-------------//
  if (digitalRead(left_button) == 0) {
    timeShoot1 = millis();
    if (timeShoot1 - timeShoot2 > 200) {
      for (int i = 0; i <= 4; i++) {
        if (bulletstate[i] == false) {
          bulletstate[i] = true;
          bulletX[i] = XFirst + 1;
          break;
        }
      }
      timeShoot2 = timeShoot1;
    }
  }
  for (int i = 0; i <= 4; i++) {
    if (bulletstate[i] == 1) {
      ShootMove1[i] = millis();
      if (address[i] < 3) {
        lc.setLed(address[i], bulletX[i], bulletY[i], true);
        if (ShootMove1[i] - ShootMove2[i] >100) {
          lc.setLed(address[i], bulletX[i], bulletY[i], false);
          bulletY[i]--;
          ShootMove2[i] = ShootMove1[i];
        }
      }
    }
    if (bulletY[i] == -1) {
      bulletY[i] = 7;
      address[i]++;
      if (address[i] == 3) {
        bulletstate[i] = false;
        address[i] = 1;
      }
    }
  }

  //-------------Check-Crash-------------//
  for (int z = 0; z <= 4; z++)
    for (int i = 0; i <= 4; i++)
      for (int j = 0; j <= 4; j++)
        if (bombX[i] == bulletX[j] && bombY[i] == bulletY[j] && BAddress[i] == address[j]) {
          lc.setLed(BAddress[z], bombX[z], bombY[z], false);
          lc.setLed(address[z], bulletX[z], bulletY[z], false);
          bombstate[z] = false;
          BAddress[z] = 2;
          bulletstate[z] = false;
          address[z] = 1;
          bombX[z] = -1; bombY[z] = -1; bulletX[z] = -1; bulletY[z] = 7;
          if (score < 10) start(3, num[++score]);
          Serial.println(score);
        }
  //-------------Game-End-------------//
  if (score == 9) {   
    clr();
    lc.clearDisplay(address);
    start(3, text[0]); delay(500);  //Print Win
    start(2, text[1]); delay(500);
    start(1, text[2]); delay(500);
    start(0, num[10]);
    delay(3000);
    clr();
    score = 0; start(3, num[score]);
    life = 9; start(0, num[life]);
  }
  if (life == 0) {
    clr();
    lc.clearDisplay(address);
    start(3, text[3]); delay(500);  //Print Lose
    start(2, text[4]); delay(500);
    start(1, text[5]); delay(500);
    start(0, text[6]);
    delay(3000);
    clr();
    score = 0; start(3, num[score]);
    life = 9; start(0, num[life]);
  }
}
//--------------------------Loop-End--------------------------//

void start(int add , byte test[]) {
  for (int i = 1; i < 9; i++) {
    lc.setRow(add, i, test[i - 1]);
  }
}

void clr() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++)
      for (int k = 0; k < 8; k++)
      {
        lc.setLed(i, j, k, false);
      }
}
