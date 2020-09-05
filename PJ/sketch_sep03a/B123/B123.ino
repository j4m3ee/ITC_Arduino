#define B1 12
#define B2 11
#define B3 10
#define L1 6
#define L2 5
#define L3 4
#define H 1
#define L 0
long t1, t2, t3;

long lastMil1 = 0;
long lastMil2 = 0;
long lastMil2s = 0;
long lastMil3 = 0;
bool r = false, y = false, g = false;

void setup() {
  Serial.begin(9600);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
}

void loop() {
  int ReadSwitch1 = digitalRead(B1);
  int ReadSwitch2 = digitalRead(B2);
  int ReadSwitch3 = digitalRead(B3);

  ////////////////////////////////////////////////-----RED-----////////////////////////////////////////////////

  if (ReadSwitch1 == H && millis() - t1 > 300)
  {
    t1 = millis();
      
    if (r == false)
    {
      lastMil1 = millis();
      digitalWrite(L1, H); Serial.println("1");
      r = true;
    }
    else if (r == true)
    {
      digitalWrite(L1, L);
      r = false;
    }
  }

  if (millis() >= lastMil1 + 3000)
  {
    digitalWrite(L1, L);
    r = false;
  }

  ////////////////////////////////////////////////-----YELLOW-----////////////////////////////////////////////////
  if (digitalRead(L1) == L && digitalRead(L3) == L)
  {
    if (ReadSwitch2 == L)
    {
      lastMil2 = millis();
      lastMil2s = millis();
      y = true;
    }

  } 
      if (y == true) {
      if (millis() < lastMil2 + 500)
      {
        digitalWrite(L2, H); Serial.println("Y");
      }
      else if (millis() >= lastMil2 + 500)
      {
        digitalWrite(L2, L);
      }

      if (millis() >= lastMil2 + 1000)
      {
        lastMil2 = millis();
        digitalWrite(L2, L);
      }
    }
  
      if (millis() >= lastMil2s + 2000)
    {
      y = false;
      digitalWrite(L2, L);
      lastMil2s = 0;
    }

  ////////////////////////////////////////////////-----GREEN-----////////////////////////////////////////////////
  if (digitalRead(L1) == L)
  {
    if (ReadSwitch3 == L && millis() - t3 > 300)
    {
      t3 = millis();
      
      if (g == false)
      {
        lastMil3 = millis();
        digitalWrite(L3, H); Serial.println("3");
        g = true;
      }
      else if (g == true)
      {
        digitalWrite(L3, L);
        g = false;
      }

    }
    if (millis() >= lastMil3 + 3000)
    {
      digitalWrite(L3, L);
      g = false;
    }
  }

}
