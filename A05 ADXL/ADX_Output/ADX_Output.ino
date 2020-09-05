#include <SPI.h>
char buffer[3];
volatile byte index;
volatile boolean status;
#include <LedControl.h>
LedControl lc = LedControl(7, 5, 6, 4); //DIN CLK CS

byte num[10][8] =  { //Font Number
  {0x00, 0x3C, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3C},
  {0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C},
  {0x00, 0x3C, 0x04, 0x04, 0x3C, 0x20, 0x20, 0x3C},
  {0x00, 0x3C, 0x04, 0x04, 0x3C, 0x04, 0x04, 0x3C},
  {0x00, 0x24, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x04},
  {0x00, 0x3C, 0x20, 0x20, 0x3C, 0x04, 0x04, 0x3C},
  {0x00, 0x3C, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x3C},
  {0x00, 0x3C, 0x24, 0x24, 0x04, 0x04, 0x04, 0x04},
  {0x00, 0x3C, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x3C},
  {0x00, 0x3C, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x04},
};

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  index = 0;
  status = false;

  for (int i = 0 ; i < 4 ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 10);
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

char f;
char m;
char l;

void loop() {
  dregree();
  delay(20);

  if ((buffer[0] == l && buffer[1] == m && buffer[2] == f) 
  && (buffer[0] != buffer[1] && buffer[1] != buffer[2])) 
  {
    Serial.println(" Work !!! ");
  } else {
    if (m == 0 && l == 0) { //check digit of value
      showNum(1, num[f]);
      lc.clearDisplay(2);
      lc.clearDisplay(3);
    } else if (l == 0) {
      showNum(1, num[f]);
      showNum(2, num[m]);
      lc.clearDisplay(3);
    } else {
      showNum(1, num[f]);
      showNum(2, num[m]);
      showNum(3, num[l]);
    }
  }

  f = buffer[2] - '0';
  m = buffer[1] - '0';
  l = buffer[0] - '0';
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
  lc.setLed(0, 0, 1, true);
  lc.setLed(0, 0, 2, true);
  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 1, 2, true);
}

void showNum(int add, byte x[]) {
  for (int i = 0 ; i < 8; i++) {
    lc.setRow(add, i, x[i]);
  }
}
