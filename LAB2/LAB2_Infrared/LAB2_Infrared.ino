byte value;

void setup() {
  pinMode(2, INPUT);

  Serial.begin(9600);
}

void loop() {
  value = digitalRead(2);

  if (value == 0) Serial.println("Far away");
  else Serial.println("Close");

  delay(100);
}
