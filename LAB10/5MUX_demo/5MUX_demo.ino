#define A_PIN 4
#define B_PIN 3
#define C_PIN 2


void setup() {
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  digitalWrite(A_PIN, 0);
  digitalWrite(B_PIN, 0);
  digitalWrite(C_PIN, 0);
  int first_value = analogRead(A0);

  digitalWrite(A_PIN, 1);
  digitalWrite(B_PIN, 0);
  digitalWrite(C_PIN, 0);
  int second_value = analogRead(A0);

  Serial.print("First:");
  Serial.print(first_value);
  Serial.print(",Second:");
  Serial.println(second_value);
  delay(1);
}
