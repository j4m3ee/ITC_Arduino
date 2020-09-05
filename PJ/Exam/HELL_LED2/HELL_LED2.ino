#define buttonA 4
#define buttonB 3
#define off INPUT
#define on OUTPUT


void setup() {
  Serial.begin(9600);
  pinMode(buttonA, off);
  pinMode(buttonB, off);
  pinMode(12, on);//บนบน
  pinMode(11, on);//บนขวา
  pinMode(10 , on);//บนซ้าย
  pinMode(9, on);//กลาง
  pinMode(8, on);//ล่างขวา
  pinMode(7 , on);//ล่างซ้าย
  pinMode(6 , on);//ล่างล่าง
  pinMode(A0, on);
  //pinMode(A1, on);
  //pinMode(A2, on);

  digitalWrite(7 , HIGH);//แทนล่างซ้าย
  digitalWrite(6 , HIGH);//แทนล่างล่าง
  digitalWrite(8 , HIGH);
  digitalWrite(9 , HIGH);
  digitalWrite(10 , HIGH);
  digitalWrite(11 , HIGH);
  digitalWrite(12 , HIGH);

  digitalWrite(A0 , LOW);
 // digitalWrite(A1 , LOW);
 // digitalWrite(A2 , LOW);


}

int num = 1;
//long int screen = 0;
int n[12] = {0, 36, 107, 109, 60, 93, 95, 100, 255, 125, 119, 8};

int pran;

void loop() {

  if (millis() < 1000) {

    handle_guess_button(num);

  }

  int switchA = digitalRead(buttonA);
  int switchB = digitalRead(buttonB);


  if (switchA == HIGH) {
    //screen = millis();
    num++;
    if (num == 7) {
      num = 1;
    }
    handle_guess_button(num);
    delay(200);



  }

  if (switchB == HIGH) {
    // screen = millis();
    handle_start_button(num);
    delay(100);
  }

  /*/
    if (millis()-screen > 10000){
    while(millis() > screen){
            if (switchA == HIGH ) {
          screen = millis();
          break;
        }
      handle_guess_button(10);
      handle_guess_button(11);

    }

    }
    /*/

  digitalWrite(A0 , LOW);
  digitalWrite(A1 , LOW);
  digitalWrite(A2 , LOW);


}

//1=off ,0 = on
void handle_guess_button(int num) {

  for (int i = 0 ; i <= 6 ; i++) {
    if (bitRead(n[num], i) == 1) {
      digitalWrite(6 + i, 0);
    } else {
      digitalWrite(6 + i, 1);
    }

  }

}


void handle_start_button(int numx) {

 int ran = random(1, 7);
  //int ran = 1;
  while (pran == ran) {
    if (pran == ran) {
      ran = random(1, 7);
    } else {
      break;
    }
  }

  num = 1;

  for (int i = 1 ; i <= 36 ; i++) {
    int d = random(1, 12);
    //if(d>=6){d=1;}
    handle_guess_button(d);
    delay(50);
  }

  for (int i = 1 ; i < 6 ; i++) {
    handle_guess_button(0);
    delay(200);
    handle_guess_button(ran);
    delay(200);
  }



  Serial.println(numx);
  Serial.println(ran);


  if (ran == (numx)) {
    digitalWrite(A0 , HIGH);
    //digitalWrite(A1 , HIGH);
    //digitalWrite(A2 , HIGH);
    handle_guess_button(ran);
    Serial.println("youwin");
    delay(5000);
    handle_guess_button(num);

    //delay(100);

  } else {
    Serial.println("youlose");
    delay(2500);
    handle_guess_button(num);
    

  }

  pran = ran;
  ran = 0;
}
