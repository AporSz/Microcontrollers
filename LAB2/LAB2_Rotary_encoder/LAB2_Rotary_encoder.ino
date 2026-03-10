#define CLK 2
#define DT 3
#define SW 4

const int interrupt0 = 0;

int count = 0;
int lastCLK = 0;

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);

  attachInterrupt(interrupt0, ClockChanged, CHANGE);

  Serial.begin(9600);
}

void loop() {

  if (!digitalRead(SW) && count != 0) {
    count = 0;
    Serial.print("Count: ");
    Serial.println(count);
  }

  delay(1);
}

void ClockChanged() {
  int clkValue = digitalRead(CLK);
  int dtValue = digitalRead(DT);

  if (lastCLK != clkValue) {
    lastCLK = clkValue;

    count += (clkValue != dtValue ? 1 : -1);
    Serial.print("Count: ");
    Serial.println(count);
  }
}