#include<SPI.h>
#define SS 10
const unsigned int X_PIN = 0;
const unsigned int Y_PIN = 1;
const unsigned int Z_PIN = 2;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = {
  X_PIN, Y_PIN, Z_PIN
};
const unsigned int BUFFER_SIZE = 32;
const unsigned int BAUD_RATE = 9600;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };

double Calibrate, angle;
long Time;

void setup() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  Serial.begin(9600);
  pinMode(8, INPUT);
}

double get_axis(const int axis) { //average value
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  double sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}

double get_x() {
  return get_axis(0);
}
double get_y() {
  return get_axis(1);
}
double get_z() {
  return get_axis(2);
}

void loop() {
  //---------- Calibrate ----------
  int c = digitalRead(8);
  if (c == HIGH && millis() - Time > 250 || millis() < 1000 ) {
    Serial.println(" Calibrate !!");
    Calibrate = get_x();
    Time = millis();
  }

  //---------- Map-Func ----------
  if (get_z() < 363 && get_x() >= 359) 
  angle = map(get_x(), Calibrate, Calibrate + 70, 0, 90);
  else if (get_z() >= 363) 
  angle = map(get_x(), Calibrate + 70, Calibrate, 90, 180);
  else if (get_x() < 359)
  angle = map(get_x(), Calibrate, Calibrate - 70, 359, 270);

  Serial.print(" X =");
  Serial.print(get_x());
  Serial.print(" Y =");
  Serial.print(get_y());
  Serial.print(" Z =");
  Serial.print(get_z());
  Serial.print(" angle = ");
  Serial.print(angle);

  //---------- Sent-Data ----------
  int sent = (int)angle;
  Serial.print(" Sent = ");
  Serial.println(sent);
  digitalWrite(SS, LOW);
  SPI.transfer(sent / 100 + '0');
  SPI.transfer((sent / 10) % 10 + '0');
  SPI.transfer(sent % 10 + '0');
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}
