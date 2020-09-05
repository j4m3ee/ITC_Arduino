#include<SPI.h>
#define SS 10

double deltax[2][64];
int xpos[2] = {0, 0};
long Time;
double Calibrate = 0;
double angle;

void setup()
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  pinMode(8, INPUT);
  Serial.begin(9600);
}


void loop()
{
  double xaccel = analogRead(A0);
  int yaccel = analogRead(A1);
  int zaccel = analogRead(A2);
  int b = digitalRead(8);

  double x = calx(1, xaccel);
  double z = calx(0, zaccel);
  double inasin = (x - Calibrate) / 76;
  angle = asin(inasin) * (180 / PI);

  if (b == HIGH && millis() - Time > 250) {
    Serial.println(" Calibrate !!");
    Calibrate = x - 0.5;
    Time = millis();
  }

  if (angle < 0) {
    angle = angle;
  }

  if (isnan(angle)) {
    angle = 90;
  }

  if (z < 387) {
    inasin = (z - 387) / 77;
    angle = acos(inasin) * (180 / PI);

    if (isnan(angle)) {
      angle = 180;
    }
  }


  Serial.print(angle);
  Serial.print("  ");
  Serial.print(x);
  Serial.print("  ");
  Serial.print(z);
  Serial.print("  ");
  Serial.print(Calibrate);
  Serial.print("  ");
  Serial.print(xaccel);

  int abc = (int)angle; //calx(0,angle);
  Serial.println(abc);
  int f = abc / 100 + '0';
  int mmm = (abc / 10 % 10) + '0';
  int l = abc % 10 + '0';

  digitalWrite(SS, LOW);
  SPI.transfer(f);
  SPI.transfer(mmm);
  SPI.transfer(l);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}

double calx(int t, float xx) {
  deltax[t][xpos[t]] = xx;
  xpos[t] = (xpos[t] + 1) % 64;
  double sum = 0 ;
  for (int i = 0 ; i < 64 ; i++) {
    sum += deltax[t][i];
  }
  return (sum / 64);
}
