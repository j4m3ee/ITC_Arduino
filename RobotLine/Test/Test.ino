void setup() {
  Serial.begin(9600);

}

const unsigned int BUFFER_SIZE = 32;
const unsigned int BAUD_RATE = 9600;
int buffer[BUFFER_SIZE];
int buffer_pos = { 0 };

double get_axis() { //average value
  delay(1);
  buffer[buffer_pos] = analogRead(A0);
  buffer_pos = (buffer_pos + 1) % BUFFER_SIZE;
  double sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[i];
  return round(sum / BUFFER_SIZE);
}

void loop() {
  Serial.println(get_axis());
}
