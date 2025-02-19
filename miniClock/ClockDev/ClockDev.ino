#include <LedControl.h>
LedControl lc = LedControl(7, 5, 6, 4); //DIN CLK CS

#define ledPin 13
#define bot 8
#define bot2 9
int timer1_counter;
unsigned long t1 = 0, Time, Time2, timeb, timeb2, timer = 0;
bool sStopW;

const unsigned int X_PIN = A0;
const unsigned int LDR_PIN = A1;
const unsigned int Y_PIN = A2;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = {
  X_PIN, LDR_PIN, Y_PIN
};
const unsigned int BUFFER_SIZE = 32;
const unsigned int BAUD_RATE = 9600;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };

double get_axis(const int axis) { //average value
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  double sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}

byte num[10][5] =  { //Font Number
  {0x3E, 0x45, 0x49, 0x51, 0x3E}, // 0 - 9
  {0x00, 0x21, 0x7F, 0x1 , 0x0 },
  {0x21, 0x43, 0x45, 0x49, 0x31},
  {0x22, 0x41, 0x49, 0x49, 0x36},
  {0xC , 0x14, 0x24, 0x7F, 0x4},
  {0x72, 0x51, 0x51, 0x51, 0x4E},
  {0x3E, 0x49, 0x49, 0x49, 0x26},
  {0x40, 0x40, 0x4F, 0x50, 0x60},
  {0x36, 0x49, 0x49, 0x49, 0x36},
  {0x32, 0x49, 0x49, 0x49, 0x3E},
};

void setup() {
  Serial.begin(9600);
  //Serial.setTimeout(10);
  pinMode(bot, INPUT);
  pinMode(bot2, INPUT);
  pinMode(ledPin, OUTPUT);
  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_counter = 3036; // preload timer 65536-16MHz/256/1Hz
  TCNT1 = timer1_counter; // preload timer
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts(); // enable all interrupts

  for (int i = 0 ; i < 4 ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
}

ISR(TIMER1_OVF_vect) // interrupt service routine
{
  TCNT1 = timer1_counter; // preload timer
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  duodot(digitalRead(ledPin) ^ 1);
  Serial.println(t1++);
  if (sStopW == 1) Serial.println(timer++);
}

int minute, minute1, minute2, hour, hour1, hour2, day, sec;
int tminute, tminute1, tminute2, thour, thour1, thour2, tday, tday1, tday2, tsec, tsec1, tsec2;

void loop() {
  //------LDR-------
  for (int i = 0 ; i < 4 ; i++) {
    lc.setIntensity(i, map(get_axis(LDR_PIN), 700, 40, 1, 8));
  }

  //------SetTime------
  if (get_axis(X_PIN) < 420 && get_axis(X_PIN) > 340
      && get_axis(Y_PIN) < 420 && get_axis(Y_PIN) > 340 )
  {
    if (digitalRead(bot) == HIGH && millis() - timeb > 250) {
      Serial.println(" + 1m");
      t1 += 60;
      timeb = millis();
    }
    if (digitalRead(bot2) == HIGH && millis() - timeb2 > 250) {
      Serial.println(" + 1h");
      t1 += 3600;
      timeb2 = millis();
    }
  } else {
    if (digitalRead(bot) == HIGH && millis() - timeb > 250) {
      sStopW = 0;
      timer = 0;
      timeb = millis();
    }
    if (digitalRead(bot2) == HIGH && millis() - timeb2 > 250) {
      sStopW = 1;
      timeb2 = millis();
    }
  }


  //------CalTime------
  day = t1 / 86400;
  Time = t1 - (day * 86400);
  hour = Time / 3600;
  hour1 = (hour / 10) % 10;
  hour2 = hour % 10;
  minute = (Time - (3600 * (Time / 3600))) / 60;
  minute1 = (minute / 10) % 10;
  minute2 = minute % 10;
  sec = Time - ((3600 * hour) + (60 * minute));
  //------CalTimer------
  tday = timer / 86400;
  Time2 = timer - (tday * 86400);
  thour = Time2 / 3600;
  thour1 = (thour / 10) % 10;
  thour2 = thour % 10;
  tminute = (Time2 - (3600 * (Time2 / 3600))) / 60;
  tminute1 = (tminute / 10) % 10;
  tminute2 = tminute % 10;
  tsec = Time2 - ((3600 * thour) + (60 * tminute));
  tsec1 = (tsec / 10) % 10;
  tsec2 = tsec % 10;

  Serial.print(" X = ");
  Serial.print(get_axis(X_PIN));
  Serial.print(" Y = ");
  Serial.print(get_axis(Y_PIN));
  Serial.print(" LDR = ");
  Serial.print(get_axis(LDR_PIN));
  Serial.print(" Day = ");
  Serial.print(day);
  Serial.print(" Hour = ");
  Serial.print(hour);
  Serial.print(" Minute = ");
  Serial.print(minute);
  Serial.print(" ");
  Serial.print(" Sec = ");
  Serial.println(sec);
  Serial.print(" Day = ");
  Serial.print(tday);
  Serial.print(" Hour = ");
  Serial.print(thour);
  Serial.print(" Minute = ");
  Serial.print(tminute);
  Serial.print(" ");
  Serial.print(" Sec = ");
  Serial.println(tsec);



  //------Show-Number-------
  if (get_axis(X_PIN) < 420 && get_axis(X_PIN) > 340
      && get_axis(Y_PIN) < 420 && get_axis(Y_PIN) > 340 )
  {
    showNum(1, 3, num[hour1]);  //Show hr/Min
    showNum(1, 2, num[hour2]);
    showNum(2, 1, num[minute1]);
    showNum(2, 0, num[minute2]);
  } else {
    showNum(1, 3, num[tminute1]);   //Timer
    showNum(1, 2, num[tminute2]);
    showNum(2, 1, num[tsec1]);
    showNum(2, 0, num[tsec2]);
  }
}

void duodot(bool state) {
  lc.setLed(1, 1, 0, state); lc.setLed(1, 2, 0, state);
  lc.setLed(2, 1, 7, state); lc.setLed(2, 2, 7, state);
  lc.setLed(1, 5, 0, state); lc.setLed(1, 6, 0, state);
  lc.setLed(2, 5, 7, state); lc.setLed(2, 6, 7, state);
}

void showNum(int go, int add, byte x[]) {
  for (int i = 0 ; i < 5; i++)
    lc.setColumn(add, go + i, x[i]);
}
