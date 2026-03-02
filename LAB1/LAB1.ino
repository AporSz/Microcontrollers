int DIGITAL_INPUT_PIN = 2;
int ANAGOL_INPUT_PIN = A3;
int LED_PIN = LED_BUILTIN;
int 

int signal_value;
int old_signal_value;

bool active;

int time_counter;

void setup() {
  pinMode(ANAGOL_INPUT_PIN, INPUT);
  //pinMode(DIGITAL_INPUT_PIN, INPUT);
  pinMode(DIGITAL_INPUT_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  signal_value = 0;
  old_signal_value = 0;

  time_counter = 0;

  active = true;

  Serial.begin(9600);
}

void loop() {
  //readTouchSensor(DIGITAL_INPUT_PIN);
  readTouchSensor(ANALOG_INPUT_PIN);

  time_counter++;
  delay(1);
}

void singlePressMode() {
  Serial.print("Time since last button press: ")
  Serial.print(time_counter);
  Serial.println(" ms");
  time_counter = 0;
}

void doublePressMode() {
  if (active) {
    Serial.print("Button pressed again! Delay since last press: ");
    Serial.print(time_counter);
    Serial.println(" ms");
  }
  else {
    Serial.println("Button pressed");
    time_counter = 0;
  }
}

void readButtonBreadBoard(int pin) {
  //enable pullup for this

  //VCC = DIGITAL
  //GND = GND
  signal_value = digitalRead(pin);

  if (signal_value == 0) {
    active = !active;
    doublePressMode();
  }
}

void readButtonChip(int pin) {
  //S = SIGNAL
  //nothing = VCC
  //- = GND

  //pressed = 0

  signal_value = digitalRead(pin);

  if (signal_value == 0) {
    active = !active; 
    doublePressMode();
  }
}

void readTouchSensor(int pin) {
  //GND = 0 V
  //I/O = SIGNAL
  //VCC = 5 V

  signal_value = (digitalRead(pin));

  if (old_signal_value != signal_value) {
    printSignal();
    manageTime();
  }
}

void printSignal() {
  if (signal_value == 1) {
    Serial.print("Close. value = ")
    Serial.println(signal_value);
  }
  else {
    Serial.print("Far away. value = ")
    Serial.println(signal_value);
  }
}