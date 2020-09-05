#define B1 12 
#define B2 11
#define B3 10
#define L1 6
#define L2 5
#define L3 4 
#define H HIGH
#define L LOW

void setup(){
  pinMode(B1,INPUT_PULLUP);
  pinMode(B2,INPUT_PULLUP);
  pinMode(B3,INPUT_PULLUP);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
}

void loop(){
  int ReadSwitchA = digitalRead(B1);
  int ReadSwitchB = digitalRead(B2);
  int ReadSwitchC = digitalRead(B3);

  if(ReadSwitchA == L)
  {
    digitalWrite(L1,L);
  }
  else digitalWrite(L1,H);

  if(ReadSwitchB == L)
  {
    digitalWrite(L2,H);
    digitalWrite(L2,L);
  }
  
  if(ReadSwitchC == L)
  {
    digitalWrite(L3,H);
    digitalWrite(L3,L);
  }
  
}
