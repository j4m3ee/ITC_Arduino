#include "LedControl.h"
#include "FontLEDClock.h"

LedControl lc = LedControl(11, 13, 10, 4);              // DIN,CLK,CS,Number of LED Module
uint8_t intensity = 8;
uint8_t hour  = 23, minute = 59, second = 50 ; //Variable Clock mode
int cd_hour = 0, cd_minute = 0, cd_second = 0;
int day = 31, month = 12, year = 19;
int mode = 6;
bool stop_time;
void setup()
{
  int devices = lc.getDeviceCount();                    /* find no of LED modules */
  for (int address = 0; address < devices; address++)   /* set up each device         */
  { /* set up each device         */

    lc.shutdown(address, false);                        /* turn off power saving mode */
    lc.setIntensity(address, 2);                        /* set intensity=2 of 8       */
    lc.clearDisplay(address);                           /* clear screen               */
  }                                                     /* set up each device         */
  Serial.begin(9600);
  noInterrupts();           /* disable all interrupts             */
  TCCR1A  = 0;
  TCCR1B  = 0;
  TCNT1   = 3036;           /* preload timer 65536-16MHz/256/1Hz  */
  TCCR1B |= (1 << CS12);    /* 256 prescaler                      */
  TIMSK1 |= (1 << TOIE1);   /* enable timer overflow interrupt    */
  interrupts();             /* enable all interrupts              */


}

void clocks()
{
  print_tiny_char( 2, 1,  hour / 10 + '0'); //print hour
  print_tiny_char( 6, 1,  hour % 10 + '0');
  print_tiny_char(12, 1, minute / 10 + '0'); //print minute
  print_tiny_char(16, 1, minute % 10 + '0');
  print_tiny_char(22, 1, second / 10 + '0'); //print second
  print_tiny_char(26, 1, second % 10 + '0');

  // blink : for each minute
  if (second == 0)
  {
    plot(10, 2, 0);
    plot(10, 4, 0);
    plot(20, 2, 0);
    plot(20, 4, 0);
  }
  else
  {
    plot(10, 2, 1);
    plot(10, 4, 1);
    plot(20, 2, 1);
    plot(20, 4, 1);
  }

  // display second count line
  if (second < 30)
  {
    plot(second + 1, 7, 1);
  }
  else
  {
    plot(60 - second, 7, 0);
  }
}

void countdown()
{
  print_tiny_char( 2, 1,  cd_hour / 10 + '0'); //print hour
  print_tiny_char( 6, 1,  cd_hour % 10 + '0');
  print_tiny_char(12, 1, cd_minute / 10 + '0'); //print minute
  print_tiny_char(16, 1, cd_minute % 10 + '0');
  print_tiny_char(22, 1, cd_second / 10 + '0'); //print second
  print_tiny_char(26, 1, cd_second % 10 + '0');

  // blink : for each minute
  if (second == 0)
  {
    plot(10, 2, 0);
    plot(10, 4, 0);
    plot(20, 2, 0);
    plot(20, 4, 0);
  }
  else
  {
    plot(10, 2, 1);
    plot(10, 4, 1);
    plot(20, 2, 1);
    plot(20, 4, 1);
  }

}

void date()
{
  print_tiny_char( 2, 1,  day / 10 + '0'); //print hour
  print_tiny_char( 6, 1,  day % 10 + '0');
  print_tiny_char(12, 1, month / 10 + '0'); //print minute
  print_tiny_char(16, 1, month % 10 + '0');
  print_tiny_char(22, 1, year / 10 + '0'); //print second
  print_tiny_char(26, 1, year % 10 + '0');

  // blink : for each minute
  if (second == 0)
  {
    plot(10, 2, 0);
    plot(10, 4, 0);
    plot(20, 2, 0);
    plot(20, 4, 0);
  }
  else
  {
    plot(11, 2, 1);
    plot(10, 3, 1);
    plot(9, 4, 1);
    plot(21, 2, 1);
    plot(20, 3, 1);
    plot(19, 4, 1);
  }

}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  TCNT1 = 3036;             /* preload timern */
  second++;
  if (second > 59) { second = 0; minute++; }
  if (minute > 59) { minute = 0; hour++; }
  if (hour > 23) { hour = 0; day++; }
  if ( month == 1 || month == 3 || month == 5 || month == 7 || 
  month == 8 || month == 10 || month == 12) {
    if (day > 31) { day = 1; month++; }
  }
  if (month == 2) {
    if (day > 28) { day = 1; month++; }
  }
  if (month == 4 || month == 6 || month == 9 || month == 11 ) {
    if (day > 30) { day = 1; month++; }
  }
  if (month > 12) { month = 1; year++; }
  if (mode == 3) { cd_second--; }
  if (cd_second < 0) { cd_second = 59; cd_minute--; }
  if (cd_minute < 0) { cd_minute = 59; cd_hour--; }
  if (cd_hour < 0) {
    cd_hour = 0; cd_minute = 0; cd_second = 0;
  }
  if (stop_time == true) {
    if (mode == 5) {
      cd_second++;
      if (second > 59) { cd_second = 0; minute++; }
      if (minute > 59) { cd_minute = 0; hour++; }
      if (hour > 23) { cd_hour = 0; }
    }
  }
}

////////////////    main      ////////////////
void loop() {

  Serial.print("Pin5 : ");
  Serial.print(digitalRead(5));
  Serial.print("   ");
  Serial.print("Pin6 : ");
  Serial.print(digitalRead(6));
  Serial.print("   ");
  Serial.print("Pin7 : ");
  Serial.print(digitalRead(7));
  Serial.print("   ");
  Serial.print("Mode : ");
  Serial.print(mode);
  Serial.print("   ");
  Serial.print("   ");

  if (digitalRead(5) == 1) {
    mode++;
    clear_display();
    if (mode == 2) {
      cd_hour = 0, cd_minute = 0, cd_second = 0;
    }
    if (mode == 4) {
      cd_hour = 0, cd_minute = 0, cd_second = 0;
    }
    if (stop_time == true) {
      stop_time = false;
    }
  }
  
  intensity = map(analogRead(A0), 490, 32, 0, 8);
  for (uint8_t address = 0; address < 4; address++)
    lc.setIntensity(address, intensity);

  if (mode == 0) { clocks(); }
  if (mode == 1) {
    clocks();
    if (digitalRead(8) == 1) { hour++; }
    if (digitalRead(7) == 1) { minute++; }
    if (digitalRead(6) == 1) { second++; }
  }
  if (mode == 2) {
    countdown();
    if (digitalRead(8) == 1) { cd_hour++; }
    if (digitalRead(7) == 1) { cd_minute++; }
    if (digitalRead(6) == 1) { cd_second++; }
  }
  if (mode == 3) { countdown(); }
  if (mode == 4) {
    countdown();
    if (digitalRead(8) == 1) { cd_hour++; }
    if (digitalRead(7) == 1) { cd_minute++; }
    if (digitalRead(6) == 1) { cd_second++; }
  }
  if (mode == 5) {
    countdown();
    if (digitalRead(8) == 1) {
      cd_hour = 0, cd_minute = 0, cd_second = 0; stop_time = false;
    }
    if (digitalRead(7) == 1) {
      stop_time = false;
    }
    if (digitalRead(6) == 1) {
      stop_time = true;
    }
  }
  if (mode == 6) {
    date();
    if (digitalRead(7) == 1) { month++; }
    if (digitalRead(8) == 1) { day++; }
    if (digitalRead(6) == 1) { year++; }
  }
  if (mode == 0) plot(31, 0, 1); else plot(31, 0, 0);
  if (mode == 1) plot(31, 1, 1); else plot(31, 1, 0);
  if (mode == 2) plot(31, 2, 1); else plot(31, 2, 0);
  if (mode == 3) plot(31, 3, 1); else plot(31, 3, 0);
  if (mode == 4) plot(31, 4, 1); else plot(31, 4, 0);
  if (mode == 5) plot(31, 5, 1); else plot(31, 5, 0);
  if (mode == 6) plot(31, 6, 1); else plot(31, 6, 0);
  delay(100);

  if (mode > 6) {
    mode = 0;
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clear_display()
{
  for (uint8_t address = 0; address < 4; address++)
    lc.clearDisplay(address);
}

void fade_down()
{
  for (uint8_t i = intensity; i > 0; i--) //fade from global intensity to 1
  {
    for (uint8_t address = 0; address < 4; address++)
      lc.setIntensity(address, i);
    delay(30); //change this to change fade down speed
  }
}

void fade_up()
{
  for (uint8_t i = 1; i < intensity; i++) //fade from global intensity to 1
  {
    for (uint8_t address = 0; address < 4; address++)
      lc.setIntensity(address, i);
    delay(30); //change this to change fade down speed
  }
  for (uint8_t address = 0; address < 4; address++)
    lc.setIntensity(address, intensity);  //reset intentsity to global value
}

void plot (uint8_t x, uint8_t y, bool value)
{
  uint8_t address;                                      /* select which matrix depending on the x coord  */
  if (x >= 0  && x <= 7)  {
    address = 3;
  }
  if (x >= 8  && x <= 15) {
    address = 2;
    x = x - 8;
  }
  if (x >= 16 && x <= 23) {
    address = 1;
    x = x - 16;
  }
  if (x >= 24 && x <= 31) {
    address = 0;
    x = x - 24;
  }

  if (value == 1)
    lc.setLed(address, y, x, true);
  else
    lc.setLed(address, y, x, false);
}

void print_tiny_char(uint8_t x, uint8_t y, char c)
{
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') )  c &= 0x1F;              /* A-Z maps to 1-26  */
  else if (c >= '0' && c <= '9')                        c  = (c - '0') + 32;
  else if (c == ' ' )                                   c  =  0;                /* space             */
  else if (c == '.' )                                   c  = 27;                /* full stop         */
  else if (c == ':' )                                   c  = 28;                /* colon             */
  else if (c == '\'')                                   c  = 29;                /* single quote mark */
  else if (c == '!' )                                   c  = 30;                /* single quote mark */
  else if (c == '?' )                                   c  = 31;                /* question mark     */

  for (uint8_t col = 0; col < 3; col++)
  {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (uint8_t row = 0; row < 5; row++)
    {
      if (dots & (16 >> row))     plot(x + col, y + row, 1);
      else                        plot(x + col, y + row, 0);
    }
  }
}
