// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
int enB = 13;
int in3 = 11;
int in4 = 12;
void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
void loop()
{
  // this function will run the motors in both directions at a fixed speed
  // turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 255 out of possible range 0~255
  analogWrite(enA, 255); // Speed Control 200
  analogWrite(enB, 255); // Speed Control 180
  delay(1000);
  
  analogWrite(enB, 30); // Speed Control
  delay(500);
  
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  
}
