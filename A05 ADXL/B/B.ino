#include <SPI.h>
char buffer[3];
volatile byte index;
volatile boolean status;

#include <LedControl.h>
int DIN = 7;
int CS =  6;
int CLK = 5;
LedControl lc = LedControl(DIN, CLK, CS, 4);

byte tetra[8] = {0x18, 0x3C, 0x66, 0x7E, 0x7E, 0x66, 0x3C, 0x18};
byte eq[8] = {0x00, 0x7E, 0x7E, 0x00, 0x7E, 0x7E, 0x00, 0x00};
byte one[8] = {0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C};
byte two[8] = {0x00, 0x3C, 0x04, 0x04, 0x3C, 0x20, 0x20, 0x3C};
byte three[8] = {0x00, 0x3C, 0x04, 0x04, 0x3C, 0x04, 0x04, 0x3C};
byte four[8] = {0x00, 0x24, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x04};
byte five[8] = {0x00, 0x3C, 0x20, 0x20, 0x3C, 0x04, 0x04, 0x3C};
byte six[8] = {0x00, 0x3C, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x3C};
byte seven[8] = {0x00, 0x3C, 0x24, 0x24, 0x04, 0x04, 0x04, 0x04};
byte eight[8] = {0x00, 0x3C, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x3C};
byte nine[8] = {0x00, 0x3C, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x04};
byte zero[8] = {0x00, 0x3C, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3C};
byte non[8] = {0x00, 0x3C, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3C};

void setup() {
  Serial.begin (9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  index = 0;
  status = false;


  for (int i = 0 ; i < 4 ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 15);
    lc.clearDisplay(i);
  }


}


ISR(SPI_STC_vect) {
  byte c = SPDR;
  if (index < sizeof buffer) {
    buffer[index] = c;
    index++;
    status = true;

  }
}

char dea;
char deb;
char deb2;

void loop() {
  dregree();
  //delay(600);

  delay(20);
  if ((buffer[1] == deb && buffer[0] == deb2 && buffer[2] == dea) && (buffer[0] != buffer[1]) && (buffer[1] != buffer[2])) {

    Serial.println(" OK!!! ");
  } else {
    Tdisplay(1, buffer[2]);
    Tdisplay(2, buffer[1]);
    Tdisplay(3, buffer[0]);
  }
  
  dea = buffer[2];
  deb = buffer[1];
  deb2 = buffer[0];


  Serial.print(buffer[0]);
  Serial.print(" ");
  Serial.print(buffer[1]);
  Serial.print(" ");
  Serial.println(buffer[2]);
  Serial.print(" ");
  if (status == 1) {
    index = 0;
    status = false;
  }

}

void dregree() {
  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 2, 1, true);
  lc.setLed(0, 2, 2, true);
}

void start(int j, byte x[]) {
  for (int i = 0 ; i < 8; i++) {
    lc.setRow(j, i, x[i]);
  }
}

void Tdisplay(int j, char chk) {
  if (chk == '1') start(j, one);
  if (chk == '2') start(j, two);
  if (chk == '3') start(j, three);
  if (chk == '4') start(j, four);
  if (chk == '5') start(j, five);
  if (chk == '6') start(j, six);
  if (chk == '7') start(j, seven);
  if (chk == '8') start(j, eight);
  if (chk == '9') start(j, nine);
  if (chk == '0') start(j, zero);
}
