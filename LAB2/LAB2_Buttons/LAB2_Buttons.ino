#define first_button 5
#define second_button 8
#define third_button 11

byte first_button_value;
byte second_button_value;
byte third_button_value;

byte old_value, new_value;

void setup() {
  pinMode(first_button, INPUT_PULLUP);
  pinMode(second_button, INPUT_PULLUP);
  pinMode(third_button, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  first_button_value = 1 - digitalRead(first_button);
  second_button_value = 1 - digitalRead(second_button);
  third_button_value = 1 - digitalRead(third_button);

  new_value = third_button_value * 4 + second_button_value * 2 + first_button_value * 1;

  if (new_value != old_value) {
    Serial.print("New value: ");
    Serial.println(new_value);
  }

  delay(200);
}
