void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  digitalWrite (2, LOW);
  digitalWrite (3, LOW);
  digitalWrite (4, LOW);
}
int redt = 0, redin = 0 , redl;
int yelt = 0, yelin = 0 , yell;
int gret = 0, grein = 0, grel;
int cont(int, int, int) ;
long bounce = 200;
void loop() {
  if (digitalRead(11) == HIGH && millis()-redt>bounce) {
    redt = millis();
    redin = 1;
  }
  else {
    redin = 0;
  }
  if (digitalRead(12) == LOW && (millis()-yelt)>bounce && redl==0 && grel==0) {
    yelt = millis();
    yelin = 1;
  }
   else {
    yelin = 0;
  }
  if ((digitalRead(13) == LOW && (millis()-gret)>bounce && redl==0) ) {
    gret = millis();
    grein = 1;
  }
   else {
    grein = 0;
  }
  redl = cont(redin, redl, redt);
  yell = cont(yelin, yell, yelt);
  grel = cont(grein, grel, gret);
  light_on_off(redl, yell, grel);
}
int cont(int in , int l , int t) {
  if (in == 1) {
    if (l == 1) {
      l = 0;
    }
    else {
      l = 1;
    }
  }
  if (millis() - t > 3000) {
    l = 0;
  }
  return l;
}
void light_on_off(int x, int y, int z) {
  if (x == 1) {
    digitalWrite (2, HIGH);
  }
  if (x == 0) {
    digitalWrite (2, LOW);
  }
  if (y == 1) {
      if (((millis()-yelt)/500)%2==0)digitalWrite (3, HIGH);
      else digitalWrite(3,LOW);
  }
  if (y == 0) {
    digitalWrite (3, LOW);
  }
  if (z == 1) {
    digitalWrite (4, HIGH);
    }
  if (z == 0) {
    digitalWrite (4, LOW);
    }
}
