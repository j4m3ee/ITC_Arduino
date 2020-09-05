#define a 2
#define b 3
#define c 4
#define d 5
#define e 6
#define f 7
#define g 8
#define DP 9 

#define B1 13
#define B2 12
#define buzzer A0

void setup() {
  Serial.begin(9600);
  pinMode(B1, INPUT); //Switch
  pinMode(B2, INPUT);
  pinMode(a, OUTPUT); //7-Segment
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(buzzer, OUTPUT); //Buzzer

  digitalWrite(a , HIGH);
  digitalWrite(b , HIGH);
  digitalWrite(c , HIGH);
  digitalWrite(d , HIGH);
  digitalWrite(e , HIGH);
  digitalWrite(g , HIGH);
  digitalWrite(f , HIGH);
  digitalWrite(DP , HIGH);

  pinMode(10, OUTPUT); //LED
  pinMode(11, OUTPUT);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
}

int num = 0 , ran , pran = 0;
int n[8] = { 192, 249, 164, 176, 153, 146, 130, 170}; //(0,1,2,3,4,5,6,LED GREEN Blink)

long t1, t2; //debounce time for swich 1 and 2

void loop() {
  int ReadSwitch1 = digitalRead(B1);
  int ReadSwitch2 = digitalRead(B2);

  if (millis() < 1000) {
    for (int i = 0 ; i <= 30 ; i++) { //Effect num
      int r = random(0, 7);
      handle_guess_button(r);
      beep(10);
      delay(30);
    }
    handle_guess_button(num); //Show 0 on 7-Segment
    Serial.println("Ready to play.");
  }

  if((millis()/1000)%2 == 1) digitalWrite(DP , 0); //DP Blink 1 Sec
  else digitalWrite(DP , 1);

  if (ReadSwitch1 == 1 && millis() - t1 > 300)
  {
    t1 = millis();
    Serial.print("Number = ");
    beep(50);
    num++;
    if (num == 7) {
      num = 1;
    }
    Serial.println(num);
    digitalWrite(10, 1);
    digitalWrite(11, 1);
    handle_guess_button(num);
  }

  if (ReadSwitch2 == 1 && millis() - t2 > 300 && num != 0)
  {
    t2 = millis();
    Serial.println("Random Num...");
    digitalWrite(10, 1);
    digitalWrite(11, 1);
    beep(50);
    handle_start_button(num);
  }
}

void handle_guess_button(int num) {
  for (int i = 0; i <= 7; i++) {
    bool x = bitRead(n[num], i);
    digitalWrite(2 + i, x);
  }
}

void handle_start_button(int numx) {
  int ran = random(1, 7); //Random Number
  while (pran == ran) {
    if (pran == ran) {  //Check last number and random number
      ran = random(1, 7); 
    } else break;
  }

  for (int i = 0 ; i <= 20 ; i++) { //Effect num
    int r = random(0, 7);
    handle_guess_button(r);
    beep(20);
    delay(50);
  }
  handle_guess_button(ran);
  Serial.print("Your Number = ");
  Serial.println(numx); //Show choosing number
  Serial.print("Random Number = ");
  Serial.println(ran);  //Show random number

  if (ran == numx) {
    digitalWrite(10, 0);
    digitalWrite(11, 1);
    Serial.println("Result -> Win");
    for (int i = 0; i <= 7; i++) {
      bool x = bitRead(n[7], i);
      digitalWrite(buzzer, !x);
      digitalWrite(10, x);
      delay(100);
    }
    digitalWrite(10, 0);
    delay(2500);
    //digitalWrite(10, 1);
    handle_guess_button(num);
  } else {
    digitalWrite(10, 1);
    Serial.println("Result -> Lose");
    for (int i = 1; i <= 2; i++) {
      digitalWrite(buzzer, 1);
      digitalWrite(11, 0);
      delay(100);
      digitalWrite(buzzer, 0);
      digitalWrite(11, 1);
      delay(100);
    }
    digitalWrite(11, 0);
    delay(3000);
    //digitalWrite(11, 1);
    handle_guess_button(num);
  }
  pran = ran; 
  ran = 0;
}

int beep(int t){
  digitalWrite(buzzer, 1);
  delay(t);
  digitalWrite(buzzer, 0);
  delay(t);
}
