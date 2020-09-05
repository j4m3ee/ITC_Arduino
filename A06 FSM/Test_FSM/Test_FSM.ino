#define LED_W_R 4
#define LED_W_Y 5
#define LED_W_G 6
#define WEST_BUTTON_PIN 2
#define LED_S_R 8
#define LED_S_Y 9
#define LED_S_G 10
#define SOUTH_BUTTON_PIN 3
#define LED_Walk_R 11
#define LED_Walk_G 12
#define WALK_BUTTON_PIN 13

#define goS 0   
#define waitStW 1  
#define goW 2    
#define waitWtS 3   
#define goWalk 4  
#define wWalk1 5  
#define wWalk2 6  
#define wWalk3 7  
#define waitStWk 8
#define sWalk1 9  
#define sWalk2 10  
#define sWalk3 11  
#define waitWtWk 12

struct State {
  unsigned long ST_Out; // 8-bit pattern to street output
  unsigned long Time; // delay in ms units
  unsigned long Next[12];
}; // next state for inputs 0,1,2,3,4,5,6,7,8,9,10,11,12

typedef const struct State SType;
SType FSM[13] = {
  {0x86, 2000, {goS, waitStWk, goS, waitStWk, waitStW, waitStW, waitStW, waitStW}}, //goS 0
  {0x8A, 500, {goW, goW, goS, goW, goW, goW, goW, goWalk}}, //waitS 1
  {0x32, 2000, {goW, waitWtWk, waitWtS, waitWtWk, goW, waitWtWk, waitWtS, waitWtS}}, //goW 2
  {0x52, 500, {goS, goWalk, goS, goS, goS, goWalk, goS, goS}}, //waitW 3
  {0x91, 2000, {goWalk, goWalk, sWalk1, sWalk1, wWalk1, wWalk1, wWalk1, wWalk1}}, //goWalk 4
  {0x90, 300, {wWalk2, wWalk2, sWalk2, sWalk2, wWalk2, wWalk2, wWalk2, wWalk2}}, //wWalk1 5
  {0x91, 300, {wWalk3, wWalk3, sWalk3, sWalk3, wWalk3, wWalk3, wWalk3, wWalk3}}, //wWalk2 6
  {0x90, 300, {goW, goW, goS, goS, goW, goW, goW, goW}}, //wWalk3 7
  {0x8A, 500, {goWalk, goWalk, goS, goWalk, goW, goWalk, goW, goW}}, //waitStWk 8
  {0x90, 300, {sWalk2, sWalk2, sWalk2, sWalk2, wWalk2, wWalk2, wWalk2, wWalk2}}, //sWalk1 5
  {0x91, 300, {sWalk3, sWalk3, sWalk3, sWalk3, wWalk3, wWalk3, wWalk3, wWalk3}}, //sWalk2 6
  {0x90, 300, {goS, goS, goS, goS, goW, goW, goW, goW}}, //sWalk3 7
  {0x52, 500, {goWalk, goWalk, goS, goWalk, goW, goWalk, goW, goW}} //waitWtWk 12
};

unsigned long S = 0; // index to the current state
void setup() {
  Serial.begin(9600);
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT);
  pinMode(LED_S_R, OUTPUT);
  pinMode(LED_S_Y, OUTPUT);
  pinMode(LED_S_G, OUTPUT);
  pinMode(SOUTH_BUTTON_PIN, INPUT);
  pinMode(LED_Walk_R, OUTPUT);
  pinMode(LED_Walk_G, OUTPUT);
  pinMode(WALK_BUTTON_PIN, INPUT);
}

int input, input1, input2, input3;

void loop() {
  digitalWrite(LED_W_R, !(FSM[S].ST_Out & B10000000));
  digitalWrite(LED_W_Y, !(FSM[S].ST_Out & B01000000));
  digitalWrite(LED_W_G, !(FSM[S].ST_Out & B00100000));
  digitalWrite(LED_S_R, !(FSM[S].ST_Out & B00010000));
  digitalWrite(LED_S_Y, !(FSM[S].ST_Out & B00001000));
  digitalWrite(LED_S_G, !(FSM[S].ST_Out & B00000100));
  digitalWrite(LED_Walk_R, !(FSM[S].ST_Out & B00000010));
  digitalWrite(LED_Walk_G, !(FSM[S].ST_Out & B00000001));
  delay(FSM[S].Time);
  
  input1 = digitalRead(WEST_BUTTON_PIN);
  input2 = digitalRead(SOUTH_BUTTON_PIN);
  input3 = digitalRead(WALK_BUTTON_PIN);
  input = input1 * 4 + input2 * 2 + input3;
  S = FSM[S].Next[input];
  
  Serial.print(digitalRead(WEST_BUTTON_PIN));
  Serial.print(digitalRead(SOUTH_BUTTON_PIN));
  Serial.print(digitalRead(WALK_BUTTON_PIN));
  Serial.print("  = ");
  Serial.println(input);
}
