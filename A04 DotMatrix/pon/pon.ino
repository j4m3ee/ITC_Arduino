#include "LedControl.h"

LedControl lc = LedControl(11, 13, 10, 4); // CLK,DIN,CS,Number of LED Module

short bulletstate[5] = {}, bulletrow[5] = {}, bulletcol[5] = {7, 7, 7, 7, 7}, address[5] = {}, addressbomb[5] = {2, 2, 2, 2, 2}, baserow = 2, basecol = 7, gunrow = 3, guncol = 6, rowstart = 2, rowend = rowstart + 2, check = 0;
short spacecraftrow = 0, spacecraftcol = 0 , bombrow[5] = {}, bombcol[5] = {}, startspacecraft = 0, endspacecraft = 0;
unsigned long int  currentmillis[5] = {}, previousmillis[5] = {}, currentdebouncemillis = 0, previousdebouncemillis = 0, currentgun = 0, previousgun = 0, currentbomb = 0, previousbomb = 0;
unsigned long int currentbombdrop[5] = {}, previousbombdrop[5] = {};

void setup() {
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, 1); lc.clearDisplay(address);
  }
  Serial.begin(9600);
  for (baserow = rowstart; baserow <= rowend; baserow++) {
    lc.setLed(0, baserow, basecol, true);
    lc.setLed(0, gunrow, guncol, true);
    startspacecraft = 0;
    spacecraftcol = 3;
  }
}



void loop() {
  randomSeed(analogRead(2));
  //movethegun
  if (analogRead(A0) > 600 && rowstart >= 0) {

    currentgun = millis();
    for (baserow = rowstart; baserow <= rowend; baserow++) {
      lc.setLed(0, baserow, basecol, false);
    }
    lc.setLed(0, gunrow, guncol, false);
    if (currentgun - previousgun > 50) {
      gunrow = gunrow - 1;
      rowstart = rowstart - 1;
      rowend = rowend - 1;
      previousgun = currentgun;
    }
  }
  if (analogRead(A0) < 450 && rowstart <= 5) {
    currentgun = millis();
    for (baserow = rowstart; baserow <= rowend; baserow++) {
      lc.setLed(0, baserow, basecol, false);
    }
    lc.setLed(0, gunrow, guncol, false);
    if (currentgun - previousgun > 50) {
      gunrow = gunrow + 1;
      rowstart = rowstart + 1;
      rowend = rowend + 1;
      previousgun = currentgun;
    }
  }
  for (baserow = rowstart; baserow <= rowend; baserow++) {
    lc.setLed(0, baserow, basecol, true);
  }
  lc.setLed(0, gunrow, guncol, true);
  //randomthespacecraft
  currentbomb = millis();
  for (spacecraftrow = startspacecraft; spacecraftrow <= startspacecraft + 2; spacecraftrow++) {
    lc.setLed(2, spacecraftrow, spacecraftcol, true);

  }

  //dropbomb

  if (currentbomb - previousbomb > 500) {
    for (spacecraftrow = startspacecraft; spacecraftrow <= startspacecraft + 2; spacecraftrow++) {
      lc.setLed(2, spacecraftrow, spacecraftcol, false);
    }
    previousbomb = currentbomb;
    startspacecraft = random(0, 6);
    spacecraftcol = random(0, 6);

  }

  Serial.print(startspacecraft);

  Serial.print("   ");

  Serial.println(spacecraftcol);

  //gunshootbullet
  if (digitalRead(2) == 0)
  {
    currentdebouncemillis = millis();
    if (currentdebouncemillis - previousdebouncemillis > 300) {
      for (int i = 0; i <= 4; i++) {
        if ( bulletstate[i] == 0) {
          bulletstate[i] = 1;
          bulletrow[i] = gunrow;
          break;
        }
      }
      previousdebouncemillis = currentdebouncemillis;
    }
  }
  for (int i = 0; i <= 4; i++) {
    if (bulletstate[i] == 1) {
      currentmillis[i] = millis();
      if (address[i] < 3) {
        lc.setLed(address[i], bulletrow[i], bulletcol[i], true);
        if (currentmillis[i] - previousmillis[i] > 50) {
          lc.setLed(address[i], bulletrow[i], bulletcol[i], false);
          bulletcol[i] = bulletcol[i] - 1;
          previousmillis[i] = currentmillis[i];
        }
      }
    }
    if (bulletcol[i] < 0) {
      address[i] = address[i] + 1;
      bulletcol[i] = 7;
      if (address[i] > 2) {
        bulletstate[i] = 0;
        address[i] = 0;
      }
    }
  }

}
