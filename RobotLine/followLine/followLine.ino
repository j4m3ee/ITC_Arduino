
// motor one
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
const unsigned int NUM_AXES = 5;
const unsigned int PINS[NUM_AXES] = {
  s1, s2, s3, s4, s5
};
const unsigned int BUFFER_SIZE = 32;
const unsigned int BAUD_RATE = 9600;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };


bool sensor[5]; int error = 0;
int motorSpeed,aSpeed,bSpeed,range = 60;
int aBaseSpeed = 75,aMaxSpeed = aBaseSpeed + range,aMinSpeed = aBaseSpeed - range;
int bBaseSpeed = 75,bMaxSpeed = bBaseSpeed + range,bMinSpeed = bBaseSpeed - range;
int Kp = 20,calibrate,state;

double get_axis(const int axis) { //average value
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  double sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
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
    calibrate = ((analogRead(2))+analogRead(0))/2;
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
  if(error>=-4 && error<=4) Forward(error);
  /*else if(error == 5){
    error = Error();
    if(error<=0){
      CurveLeft();
    }else if(error>0){
      CurveRight();
    }
  }*/else if(error == -5) {
      Back();
      delay(700);
    switch(state){
      case 1:
      Forward(0);
      state=2;
      delay(500);
      break;
      case 2:
      Forward(-4);
      state=3;
      
      delay(300);
      break;
      case 3:
      Forward(4);
      state=1;
      //delay(300);
      break;
      default:
      state = 1;
      delay(300);
      break;
      }
    }
    else if(error == 5){
      Forward(0);
      delay(300);
    
     /*error = Error();
    if(error<=0){
      CurveLeft();
    }else if(error>0){
      CurveRight();*/
    }
    /*Back();
    delay(500);
    Stop();*/
  //if(sensor[0]&&sensor[1]&&sensor[2]&&sensor[3]&&sensor[4]) {error=5;}
  //else {error=-5;
  
  Serial.println(error);

  //delay(1);
  }
}

void CurveLeft(){
  digitalWrite(in1, LOW); //Forward
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //Forward
  digitalWrite(in4, LOW);
  
  do{
    error = Error();
    analogWrite(enA, 30); // Speed Control
    analogWrite(enB, 70); // Speed Control
  }while(error > 0);
  
}

void CurveRight(){
  digitalWrite(in1, LOW); //Forward
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //Forward
  digitalWrite(in4, LOW);
  
  do{
    error = Error();
    analogWrite(enA, 70); // Speed Control
    analogWrite(enB, 30); // Speed Control
  }while(error < 0);
  
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
  if(error == 4 || error == -4) delay(400);
}

void Back(){
  digitalWrite(in1, HIGH); //back
  digitalWrite(in2, LOW);
  // turn on motor B
  digitalWrite(in3, LOW); //Back
  digitalWrite(in4, HIGH);

  analogWrite(enA, 80); // Speed Control
  analogWrite(enB, 80); // Speed Control
}

void Stop(){
  digitalWrite(in1, LOW); //Stop
  digitalWrite(in2, LOW);
  // turn on motor B
  digitalWrite(in3, LOW); //Stop
  digitalWrite(in4, LOW);
  analogWrite(enA, 0); // Speed Control
  analogWrite(enB, 0); // Speed Control
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
  else if(sensor[1]) {return 2;}
  else if(sensor[2]&&sensor[3]) {return -1;}
  else if(sensor[2]) {return 0;}
  else if(sensor[3]) {return -2;}
  else {return -5;}
  
}
