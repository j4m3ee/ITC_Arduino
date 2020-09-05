#define a 2
#define b 3
#define c 4
#define d 5
#define e 6
#define f 7
#define g 8
#define DP 9

#define B1 12
#define B2 11

int n[] = { 130, 0x5B, 0x4F, 0x66, 0x6D, 0x7D};


void setup() {
  // put your setup code here, to run once:
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(f, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, 1);
  digitalWrite(11, 1);

  digitalWrite(a, 1);
  digitalWrite(b, 1);
  digitalWrite(c, 1);
  digitalWrite(d, 1);
  digitalWrite(DP, 1);
  digitalWrite(e, 1);
  digitalWrite(g, 1);
  digitalWrite(f, 1);
}

void loop() {
  for (int i = 0; i <= 7; i++) {
    if (bitRead(n[0], i) == 0) {
      digitalWrite(2 + i, 0);
    } else {
      digitalWrite(2 + i, 1);
    }
  }
}
