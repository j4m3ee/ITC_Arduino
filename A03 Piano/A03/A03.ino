#define c 261
#define d 294
#define e 330
#define f 349
#define g 392
#define a 440
#define b 494
#define c8 523

int rec[100], rectime[100];
int note[8] = {c, d, e, f, g, a, b, c8};

void setup() {
  pinMode (13, INPUT);
  pinMode (12, INPUT);
  pinMode (11, INPUT);
  pinMode (10, INPUT);
  pinMode (9, INPUT);
  pinMode (8, INPUT);
  pinMode (7, INPUT);
  pinMode (6, INPUT);
  pinMode (5, INPUT);
  digitalWrite (3, LOW);
}

int t1 = 0, n =  0, timer = 0;

void loop() {
  for (int i = 0; i <= 7; i++) {
    if (digitalRead(i + 5)) {
      tone(3, note[i], 500);
      if (millis() > t1 + 100) {
        t1 = millis();
        rec[n] = i;
        rectime[n] = millis() - timer;
        timer = millis();
        n++;
      }
    }
  }
  
  if (digitalRead(13)) {
    for (int i = 0; i <= n; i++) {
      tone(3, note[rec[i]], 500);
      if (i != n - 1) delay(rectime[i + 1]);
      else delay(500);
    }
    noTone(3);
  }
}
