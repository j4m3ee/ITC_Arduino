#include "ST7735_TEE.h"

const byte PIN_A = 2;
const byte PIN_B = 3;
const byte PIN_C = 4;
const byte PIN_D = 5;
const byte PIN_E = 6;
const byte PIN_F = 7;
const byte PIN_K = 8;

uint8_t x = 35;
uint8_t y = 20;

char chour[3];
char cminute[3];
char csecond[3];
char cday[3];
char cmonth[3];
char cyear[5];


int hour = 23 ,minute = 59 ,second = 50;
int ehour,eminute,esecond;
int day = 31,month = 12,year = 2019;
char input;
String num;
int input_number;
int Mode = 0;
int Emode = 0;


TEE_ST7735 lcd(9, 10, 11, 12, 13); //Arduino  csk,sda,A0,rst,cs

void setup() {
    Serial.begin(9600);
  //lcd.init(lcd.HORIZONTAL);
    lcd.init(lcd.VERTICAL);
    lcd.fillScreen(BLACK);
    
     for (int i = 0; i < 3; i++)
    {
        lcd.drawRect(i * 5, i * 5, lcd.width - i * 10, lcd.height - i * 10, GREEN);
    }

     noInterrupts();           /* disable all interrupts             */
    TCCR1A  = 0;
    TCCR1B  = 0;
    TCNT1   = 3036;           /* preload timer 65536-16MHz/256/1Hz  */
    TCCR1B |= (1 << CS12);    /* 256 prescaler                      */
    TIMSK1 |= (1 << TOIE1);   /* enable timer overflow interrupt    */
    interrupts();             /* enable all interrupts              */

  pinMode(PIN_A, INPUT);
  digitalWrite(PIN_A, HIGH);

  pinMode(PIN_B, INPUT);
  digitalWrite(PIN_B, HIGH);

  pinMode(PIN_C, INPUT);
  digitalWrite(PIN_C, HIGH);

  pinMode(PIN_D, INPUT);
  digitalWrite(PIN_D, HIGH);

  pinMode(PIN_E, INPUT);
  digitalWrite(PIN_E, HIGH);
 
  pinMode(PIN_F, INPUT);
  digitalWrite(PIN_F, HIGH);
 
  pinMode(PIN_K, INPUT);
  digitalWrite(PIN_K, HIGH);
    
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  TCNT1 = 3036;             /* preload timern */
  //Serial.println(second++);
  second++;
  if (second > 59) { second = 0; minute++; }
  if (minute > 59) { minute = 0; hour++; }
  if (hour > 23) { hour = 0; day++; }
  if( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 
  || month == 10 || month == 12){
    if(day > 31){ day = 1; month++; }
  }
  if(month == 2){
    if(day > 28){ day = 1; month++; }
  }
  if(month == 4 || month == 6 || month == 9 || month == 11 ){
    if(day > 30){ day = 1; month++; }
  }
  if(month > 12){ month = 1; year++; }
}

  int hour2;
  int second2;
  int minute2;
  int day2,month2,year2;

void loop() {
  
  Serial.print("B:");
  Serial.print(digitalRead(PIN_B));
  Serial.print(" ");
  Serial.print(second);
  Serial.print("     ");
  Serial.print(second2);
  Serial.print("     ");
  
  if(Emode == 0){
    if(hour != hour2) lcd.drawString( x, y, (char*) chour , BLACK, 5);
    if(minute != minute2) lcd.drawString( x, y+40, (char*) cminute , BLACK, 5);
    if(second != second2) lcd.drawString( x, y+80, (char*)  csecond, BLACK, 4);
  }
  if(Emode == 1){
    if(day != day2) lcd.drawString( x, y, (char*) cday , BLACK, 5);
    if(month != month2) lcd.drawString( x, y+40, (char*) cmonth , BLACK, 5);
    if(year != year2) lcd.drawString( x-20, y+80, (char*)  cyear, BLACK, 4);
  }
  
  if(hour/10==0)chour[0] = '0';
  else chour[0] = '0'+(hour/10);
  chour[1] = '0'+(hour%10);

  if(minute/10==0)cminute[0] = '0';
  else cminute[0] = '0'+(minute/10);
  cminute[1] = '0'+(minute%10);

  if(second/10==0)csecond[0] = '0';
  else csecond[0] = '0'+(second/10);
  csecond[1] = '0'+(second%10);

  if(day/10==0)cday[0] = '0';
  else cday[0] = '0'+(day/10);
  cday[1] = '0'+(day%10);

  if(month/10==0)cmonth[0] = '0';
  else cmonth[0] = '0'+(month/10);
  cmonth[1] = '0'+(month%10);

  if(year/1000==0)cyear[0] = '0';
  else cyear[0] = '0'+(year/1000);
  if((year/100)%10==0)cyear[1] = '0';
  else cyear[1] = '0'+(year/100)%10;
  if((year/10)%100==0)cyear[2] = '0'; 
  else cyear[2] = '0'+(year/10)%100;
  cyear[3] = '0'+(year%10);
  
//////////////////////////////////////////////MODE///////////////////////////////////////

  if(Emode == 0){
    lcd.drawString( x, y, (char*) chour , WHITE, 5);  
    lcd.drawString( x, y+40, (char*)  cminute, WHITE, 5);  
    lcd.drawString( x, y+80, (char*) csecond , WHITE, 4);
    if(Mode == 1) lcd.drawString( x, y, (char*) chour , BLACK, 5);
    if(Mode == 2) lcd.drawString( x, y+40, (char*)  cminute, BLACK, 5);
    if(Mode == 3) lcd.drawString( x, y+80, (char*) csecond , BLACK, 4);
  }
  if(Emode == 1){
    lcd.drawString( x, y, (char*) cday , WHITE, 5);  
    lcd.drawString( x, y+40, (char*)  cmonth, WHITE, 5);  
    lcd.drawString( x-20, y+80, (char*) cyear , WHITE, 4);
    if(Mode == 1) lcd.drawString( x, y, (char*) cday , BLACK, 5);
    if(Mode == 2) lcd.drawString( x, y+40, (char*)  cmonth, BLACK, 5);
    if(Mode == 3) lcd.drawString( x-20, y+80, (char*) cyear , BLACK, 4);
  }
  
/////////////////////////////////////////////////////////////////////////////////////////

  hour2 = hour;
  second2 = second;
  minute2 = minute;

  day2 = day;
  month2 = month;
  year2 = year;

   while (Serial.available()) {
         input = (char)Serial.read();
         num += input;
     }
     if(num != ""){
      input_number = num.toInt();
      if(Emode == 0){
        if(Mode == 1)      hour = input_number;
        if(Mode == 2)      minute = input_number;
        if(Mode == 3)      second = input_number;
      }
      if(Emode == 1){
        if(Mode == 1)      day = input_number;
        if(Mode == 2)      month = input_number;
        if(Mode == 3)      year = input_number;
      }
     }
     num = "";
     Serial.println (input_number);

  if(digitalRead(PIN_B) == 0) Mode++;
  if(Mode > 3) Mode = 0;  

  if(digitalRead(PIN_E) == 0){
    Emode++; lcd.fillScreen(BLACK); Mode = 0; 
    
    if(Emode == 1) for (int i = 0; i < 3; i++)
    {
        lcd.drawRect(i * 5, i * 5, lcd.width - i * 10, lcd.height - i * 10, GREEN);
    }else for (int i = 0; i < 3; i++)
    {
        lcd.drawRect(i * 5, i * 5, lcd.width - i * 10, lcd.height - i * 10, BLUE);
    }
  }
  if(Emode > 1) Emode = 0;
}
