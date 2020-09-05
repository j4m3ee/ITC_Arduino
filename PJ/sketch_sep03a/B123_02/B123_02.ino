#define B1 12
#define B2 11
#define B3 10
#define L1 6
#define L2 5
#define L3 4
#define H HIGH
#define L LOW
unsigned long t1, t2, t3;
long i1 = 0, i2 = 0, i3 = 0;
unsigned long rt1, rt2, rt2s, rt3;
long v1 = 0, v2 = 0, v3 = 0;
bool r = false, y = false, g = false;
int contrlV(int, int, int);

void setup() {
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
}

void loop() {
  int ReadSwitch1 = digitalRead(B1);
  int ReadSwitch2 = digitalRead(B2);
  int ReadSwitch3 = digitalRead(B3);

  //----------------------------RED----------------------------//

  if (ReadSwitch1 == 1 && millis() > rt1 + 200)
  {
    t1 = millis();
    rt1 = millis();
    i1 = 1;
    Serial.println("Red 3 Sec");
  } else {
    i1 = 0;
  }

  if (v1 == 1) //timer
  {
    if (millis() > t1 + 3000) v1 = 0;
  }


  //---------------------------- YELLOW ----------------------------//

  if (ReadSwitch2 == 0 && millis() > rt2 + 200 && v1 == 0 && v3 == 0)
  {
    i2 = 1;
    t2 = millis();
    rt2 = millis();
    Serial.println("Yellow Blink 2 Sec");
  } else {
    i2 = 0;
  }

  if (v2 == 1) //timer
  {
    if (millis() >= t2 + 2000) v2 = 0;
  }


  //---------------------------- GREEN ----------------------------//

  if (ReadSwitch3 == 0 && millis() > rt3 + 200 && v1 == 0)
  {
    rt3 = millis();
    i3 = 1;
    t3 = millis();
    Serial.println("Green 3 Sec");
  } else {
    i3 = 0;
  }

  if (v3 == 1) //timer
  {
    if (millis() > t3 + 3000) v3 = 0;
  }

  //---------------------------- FUNC On-Off ----------------------------//

  v1 = contrlV(i1, v1, t1);
  v2 = contrlV(i2, v2, t2);
  v3 = contrlV(i3, v3, t3);
  lightOnOff(v1, v2, v3);
}

int contrlV(int bot, int v, int t)
{
  if (bot == 1)
  {
    if (v == 1) v = 0;
    else v = 1;
  }
  return v;
}

void lightOnOff(int x, int y, int z) {
  if (x == 1) digitalWrite(L1, H);
  if (x == 0) digitalWrite(L1, L);
  if (y == 1)
  {
    if (((millis() - t2) / 500) % 2 == 0)digitalWrite(L2, H);
    else digitalWrite(L2, L);  //BLINK 0.5 Sec
  }
  if (y == 0) digitalWrite(L2, L);
  if (z == 1) digitalWrite(L3, H);
  if (z == 0) digitalWrite(L3, L);
}
