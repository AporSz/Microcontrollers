byte row_pins[4] = {7,6,5,4};
byte column_pins[4] = {11,10,9,8};

byte value;

char characters[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void setup() {
  byte i;

  for (i = 0; i < 4; i++) {
    pinMode(row_pins[i], OUTPUT);
  }

  for (i = 0; i < 4; i++) {
    pinMode(column_pins[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  byte column;
  byte row;
  digitalWrite(row_pins[0], LOW);

  for (row = 0; row < 4; row++) {

    for (column = 0; column < 4; column++) {
      digitalWrite(row_pins[column], HIGH);
    }
    digitalWrite(row_pins[row], LOW);
    
    for (column = 0; column < 4; column++) {
      value = digitalRead(column_pins[column]);

      if (value == 0) {
        Serial.print("Found it, column = ");
        Serial.print(column);
        Serial.print(" row = ");
        Serial.print(row);
        Serial.print(" character = ");
        Serial.println(characters[3-row][3-column]);
      }
    }
  }

  delay(100);
}
