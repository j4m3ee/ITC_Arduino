// motor
int enB = 5;
int in4 = 9;
int in3 = 8;
int enA = 6;
int in2 = 11;
int in1 = 12;
int led[5] ={13,10,7,4,3};

const unsigned int s1 = 0;
const unsigned int s2 = 1;
const unsigned int s3 = 2;
const unsigned int s4 = 3;
const unsigned int s5 = 4;

bool sensor[5]; int error = 0,lerror = 0;
int motorSpeed,aSpeed,bSpeed,range = 60;
int aBaseSpeed = 80,aMaxSpeed = aBaseSpeed + range,aMinSpeed = aBaseSpeed - range;
int bBaseSpeed = 80,bMaxSpeed = bBaseSpeed + range,bMinSpeed = bBaseSpeed - range;
int Kp = 20,calibrate,state = 0;

int Calibrate()
{
  int mid=analogRead(2),edge1=analogRead(0),edge2=analogRead(4);
  return((mid+((edge1+edge2)/2))/2);
}

void setup()
{
  // set all the motor control pins to
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  for(int i=0;i<5;i++){
    pinMode(led[i], OUTPUT);
  }
}

void loop()
{
  if(millis()<1000){
    calibrate = Calibrate();
    // turn on motor A
    digitalWrite(in1, LOW); //Stop
    digitalWrite(in2, LOW);
    // turn on motor B
    digitalWrite(in3, LOW); //Stop
    digitalWrite(in4, LOW);
  }
  else{
  Serial.print(analogRead(0)); Serial.print(" ");
  Serial.print(analogRead(1)); Serial.print(" ");
  Serial.print(analogRead(2)); Serial.print(" ");
  Serial.print(analogRead(3)); Serial.print(" ");
  Serial.print(analogRead(4)); Serial.print(" Error : ");

  error = Error();
  if(error>=-4 && error<=4)
  {
    lerror = error;
    Forward(error);
    state = 0;
  }
    else if(error == -5) {
      if(lerror < 0){
         Forward(-4);
      }
      if(lerror > 0){
        Forward(4);
      }
      if(lerror == 0){
         Forward(4);
      }
    }
    else if(error == 5){
      Forward(0);
      delay(300);
    }
  Serial.println(error);
  }
}

int Forward(int error){
  digitalWrite(in1, LOW); //Forward
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //Forward
  digitalWrite(in4, LOW);
  motorSpeed = Kp*error;
  aSpeed = aBaseSpeed + motorSpeed;
  bSpeed = bBaseSpeed - motorSpeed;

  if(aSpeed > aMaxSpeed) aSpeed = aMaxSpeed;
  if(bSpeed > bMaxSpeed) bSpeed = bMaxSpeed;

  if(aSpeed < aMinSpeed) aSpeed = aMinSpeed;
  if(bSpeed < bMinSpeed) bSpeed = bMinSpeed;
  if(error == 1) aSpeed += 40;
  if(error == -1) bSpeed += 40;
  analogWrite(enA, aSpeed); // Speed Control
  analogWrite(enB, bSpeed); // Speed Control
  if(error == 4) {
    analogWrite(enB, 0);
    delay(400);
  }
  if(error == -4)
  {
    analogWrite(enA, 0);
    delay(400);
  }
}

int Error(){
  for(int i = 0;i<5;i++){
    if(analogRead(i)>calibrate) //If Detect White floor
    sensor[i] = true;
    else sensor[i] = false;
    digitalWrite(led[i], sensor[i]);
  }
  
  if(sensor[0]&&sensor[1]&&sensor[2]&&sensor[3]&&sensor[4]) {return 5;}
  else if(sensor[4]) {return -4;}
  else if(sensor[0]) {return 4;}
  else if(sensor[3]&&sensor[4]) {return -3;}
  else if(sensor[0]&&sensor[1]) {return 3;}
  else if(sensor[1]&&sensor[2]) {return 1;}
  else if(sensor[2]&&sensor[3]) {return -1;}
  else if(sensor[1]) {return 2;}
  else if(sensor[3]) {return -2;}
  else if(sensor[2]) {return 0;}
  else {return -5;}
}
