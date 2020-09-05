#include <LedControl.h>
#include <math.h>

int DIN = 11;
int CS =  10;
int CLK = 13;

int down_button = 4;
int joystick_axis_x = A0;

LedControl lc = LedControl(DIN, CLK, CS, 0);

///////////////////////////////////////////////////////

byte test[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte full[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
byte gamecon[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte textY[8] = {0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x00};
byte textO[8] = {0x00, 0x7E, 0x7E, 0x66, 0x66, 0x7E, 0x7E, 0x00};
byte textU[8] = {0x00, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E, 0x00};
byte textL[8] = {0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00};
byte textS[8] = {0x00, 0x3C, 0x62, 0x38, 0x0C, 0x66, 0x3C, 0x00};
byte textE[8] = {0x00, 0x7E, 0x40, 0x7E, 0x7E, 0x40, 0x7E, 0x00};
byte textW[8] = {0x00, 0x42, 0x42, 0x5A, 0x5A, 0x5A, 0x7E, 0x00};
byte textI[8] = {0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00};
byte textN[8] = {0x00, 0x46, 0x66, 0x76, 0x6E, 0x66, 0x62, 0x00};

int ReadbuttonDown;
int Readjoy;

///////////////////////////////////////////////////////////
bool esready;
bool shoot;
int x;
unsigned long deltaTime;
unsigned long deltaTimeShoot ;
unsigned long deltaTimeBullet ;
unsigned long deltaTimelife ;
int Bx = -2;
int By;
int life;
///////////////////////////////////////////////////////////////

int ex;
bool eshoot;
int erandom;
unsigned long edeltaTime;
unsigned long edeltaTimeShoot ;
unsigned long edeltaTimeBullet ;
unsigned long edeltaTimelife;
int eBx = -1;
int eBy;
int elife;
////////////////////////////////////////////////////// void setup

void setup() {
  // for(int i=0 ; i< 4 ; i++){
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  // }


  //control
  pinMode(down_button, INPUT);
  digitalWrite(down_button, LOW);

  Serial.begin(9600);

  //start(test);
  //////////////////
  life = 4;
  x = 4;
  shoot = false;
  draw(x);
  lifedisplay(life);
  deltaTime = 0 ;
  deltaTimelife = 0;
  /////////////////
  esready = true;
  elife = 4;
  ex = random(2, 7);
  eshoot = false;
  edraw(ex);
  elifedisplay(elife);
  edeltaTime = 0 ;
  edeltaTimelife = 0 ;
}



////////////////////////////////////////////// void loop //////////////////////////

void loop() {

  /*****************************if check***********************/


  /********************************************** check keyborad *****************************************/

  ReadbuttonDown = digitalRead(down_button);
  Readjoy = (map(analogRead(joystick_axis_x), 0, 1000, -1, 1));


///////////////////////////////// shoot /////////////////////////////////////////////////
//if ((ex == x) && !eshoot && millis()-edeltaTimeShoot > 1000){
 // esready = true;
 // edeltaTimeShoot = millis();
//}

  if (ReadbuttonDown == 0 && !shoot && millis()-deltaTimeShoot > 500 ) {
    deltaTimeBullet = millis();
    deltaTimeShoot = millis();
    shoot = true;
    Bx = x;
    By = 6;
  }

/////////////////////////////////////////// shoot ////////////////////////////
  if (shoot) {
    if ((millis() - deltaTimeBullet) % 50 == 0 && shoot) {
      By--;
      bullet(Bx, By);
      draw(x);
      Serial.println("sdf");
    }
    if (By < -1) {
      //deltaTimeShoot = 0;
      deltaTimeBullet = 0;
      shoot = false;
      Bx = 20;
      //drawB(x,shoot);
    }
  }
///////////




 if ((ex == x) && !eshoot && millis()-edeltaTimeShoot > 500 ) {
    edeltaTimeBullet = millis();
    edeltaTimeShoot = millis();
    eshoot = true;
    esready = false;
    eBx = ex;
    eBy = 1;
}

  ///////////////////////////////// e shoot /////////////////////////////////////////////////

  if (eshoot) {
    if ((millis() - edeltaTimeBullet) % 50 == 0 && eshoot) {
      eBy++;
      ebullet(eBx, eBy);
      edraw(ex);
      Serial.println("sdf");
    }
    if (eBy > 8) {
      //edeltaTimeShoot = 0;
      edeltaTimeBullet = 0;
      eBx = 20;
      eshoot = false;
      //edrawB(ex,eshoot);
    }
  }


////////////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////


 

  ///////////////////////////////////////////////////// เลื่อนซ้ายขวา /////////////////////////////////


  
  //draw(x);
  if (Readjoy == 1 && x < 6 && millis() - deltaTime > 200) {
    de(x - 1, x);
    x++;
    draw(x);
    //drawB(x, shoot);
    deltaTime = millis();
    Serial.println("Right");

  }

  if (Readjoy == -1 && x > 2 && millis() - deltaTime > 200) {
    de(x + 1, x);
    x--;
    draw(x);
    //drawB(x, shoot);
    deltaTime = millis();
    Serial.println("Left");
  }



  /////////////////////////////////////////////////////////////////////////// e เลื่อซ้ายขวา
  // if (millis() % 50 == 0) {
  //   erandom = random(0, 5);
  // }


if (Bx == ex || Bx == ex - 1 || Bx == ex + 1) {

    if (abs(ex - 0) > abs(ex - 7) &&  millis() - edeltaTime > 200 ) {

      erandom = 0;
      //left
      
    }else if (abs(ex - 0) < abs(ex - 7) &&  millis() - edeltaTime > 200 ) {

      erandom = 1;
    //right
    }


  }else if(ex != x-1 || ex != x+1) {
  if(x < ex &&  millis() - edeltaTime > 200){
    erandom = 0;
  }else if (x > ex &&  millis() - edeltaTime > 200){
    erandom = 1;
  }
}else if (ex == x-1 || ex == x+1 &&  millis() - edeltaTime > 200){
  erandom = 2;
} 


  if (erandom == 1 && ex < 6 && millis() - edeltaTime > 200) {
    ede(ex - 1, ex);
    ex++;
    edraw(ex);
    //edrawB(ex, eshoot);
    edeltaTime = millis();
    Serial.println("ERight");
  }

  if (erandom == 0  && ex > 2 && millis() - edeltaTime > 200) {
    ede(ex + 1, ex);
    ex--;
    edraw(ex);
    //edrawB(ex, eshoot);
    edeltaTime = millis();
    Serial.println("ELeft");
  }


/////////////////////////// chk

 if (eBx == Bx) {

    if (By == eBy) {
      deletehit(Bx, By);
    }
    if (By + 1 == eBy) {
      deletehit(Bx, By);
    }
    if (By == eBy + 1) {

      deletehit(Bx, By);
    }
  }

  /////////////////////////////////// lose
  if (eBy == 6 && eBx == x && millis() - deltaTimelife > 250) {

    life = life - 1;
    lifedisplay(life);
    eBy = 20 ;
    draw(x);
    deltaTimelife = millis();

  }
  else if (eBy == 7 && (eBx == x + 1 || eBx == x - 1) && millis() - deltaTimelife > 250) {

    life = life - 1;
    lifedisplay(life);
    eBy = 20 ;
    draw(x);
    deltaTimelife = millis();
  }

  ///////////////////////////////////// win
  if (By == 1 && Bx == ex && millis() - edeltaTimelife > 250) {
    elife = elife - 1;
    elifedisplay(elife);
    By = -20 ;
    edraw(ex);
    edeltaTimelife = millis();
  }
  else if (By == 0 && (Bx == ex + 1 || Bx == ex - 1) && millis() - edeltaTimelife > 250 ) {
    elife = elife - 1;
    elifedisplay(elife);
    By = -20 ;
    edraw(ex);
    edeltaTimelife = millis();
  }

  ////////////////////////////////////////


  if (life == 0) {

    //lose
    eBy = 9;
    By = -1;
    start(textL);
    delay(300);
    start(textO);
    delay(300);
    start(textS);
    delay(300);
    start(textE);
    delay(300);
    start(test);
    delay(2000);
    ///////////////////////
    start(gamecon);
    life = 4;
    lifedisplay(life);
    elife = 4;
    elifedisplay(elife);
    x = random(2, 7);
    draw(x);
    ex = random(2, 7);
    edraw(ex);

  }

  if (elife == 0) {

    //win
    eBy = 9;
    By = -1;
    start(textW);
    delay(300);
    start(textI);
    delay(300);
    start(textN);
    delay(300);
    start(full);
    delay(2000);
    ///////////////////////
    start(gamecon);
    life = 4;
    lifedisplay(life);
    elife = 4;
    elifedisplay(elife);
    x = random(2, 7);
    draw(x);
    ex = random(2, 7);
    edraw(ex);


  }

}

///////////////////////////////////////////////////////////end of loop //////////////////////////////



void drawB(int dx, bool shoot) {
  if (!shoot) {
    lc.setLed(0, 6, dx, true);
  } else {
    lc.setLed(0, 6, dx, false);
  }
}


void de(int dx, int x) {
  lc.setLed(0, 6, x, false);
  lc.setLed(0, 7, dx, false);
}


void draw(int dx) {
  lc.setLed(0, 6, dx, true);
  lc.setLed(0, 7, dx, true);
  lc.setLed(0, 7, dx + 1, true);
  lc.setLed(0, 7, dx - 1, true);

}

//////////////////////////////////////////

void bullet(int dx, int dy) {
  deletebullet(dx, dy + 1);
  lc.setLed(0, dy , dx, true);




}


void deletebullet(int dx, int dy) {
  lc.setLed(0, dy , dx, false);
}

///////////////////////////////////////////////


void edrawB(int dx, bool shoot) {
  if (!shoot) {
    lc.setLed(0, 1, dx, true);
  } else {
    lc.setLed(0, 1, dx, false);
  }
}


void ede(int dx, int x) {
  lc.setLed(0, 1, x, false);
  lc.setLed(0, 0, dx, false);
}


void edraw(int dx) {
  lc.setLed(0, 1, dx, true);
  lc.setLed(0, 0, dx, true);
  lc.setLed(0, 0, dx + 1, true);
  lc.setLed(0, 0, dx - 1, true);

}


void ebullet(int dx, int dy) {
  edeletebullet(dx, dy - 1);
  lc.setLed(0, dy , dx, true);




}


void edeletebullet(int dx, int dy) {
  lc.setLed(0, dy , dx, false);
}


void deletehit(int dx, int dy) {
  By = -2;
  eBy = 9;
  deletebullet(dx, dy);
  deletebullet(dx, dy + 1);
  deletebullet(dx, dy - 1);
}

//////////////////////////////////////////////// life

void lifedisplay(int life) {
  for (int j = 4; j < 8 ; j++) {
    lc.setLed(0, j , 0 , false);
  }
  for (int i = 8 - life ; i < 8 ; i++) {
    lc.setLed(0, i , 0 , true);

  }
}

void elifedisplay(int elife) {
  for (int j = 0; j < 4 ; j++) {
    lc.setLed(0, j , 0 , false);
  }
  for (int i = elife - 1 ; i >= 0 ; i--) {
    lc.setLed(0, i , 0 , true);

  }
}


//////////////////////////////////////////////
void start(byte test[]) {
  int i = 0;

  for (i = 0; i < 8; i++) {

    lc.setRow(0, i, test[i]);
  }

}
