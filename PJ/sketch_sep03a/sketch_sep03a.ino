#define button 12
void setup() {
}
  Serial.begin(9600);
  pinMode(button,INPUT_PULLUP);

void loop() {
  int ReadSwitch = digitalRead(button);

  if(ReadSwitch == 0)
  {
    Serial.println("Pressed Switch."); delay(500);
  }
}
