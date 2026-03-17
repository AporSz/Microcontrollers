int red_value;
int green_value;
int blue_value;

int RED = 9;
int GREEN = 10;
int BLUE = 11;

byte phase = 1;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  red_value = 255;
  green_value = 0;
  blue_value = 0;
}

void loop() 
{
  analogWrite(RED, red_value);
  analogWrite(GREEN, green_value);
  analogWrite(BLUE, blue_value);
  
  // put your main code here, to run repeatedly:
  changeColor();

  delay(10);
}

void changeColor() {
  if (phase == 1) {
    if (blue_value < 255) {
      blue_value++;
    }
    else {
      phase = 2;
    }
    return;
  }
  if (phase == 2) {
    if (red_value > 0) {
      red_value--;
    }
    else {
      phase = 3;
    }
    return;
  }
  if (phase == 3) {
    if (green_value < 255) {
      green_value++;
    }
    else {
      phase = 4;
    }
    return;
  }
  if (phase == 4) {
    if (blue_value > 0) {
      blue_value--;
    }
    else {
      phase = 5;
    }
    return;
  }
  if (phase == 5) {
    if (red_value < 255) {
      red_value++;
    }
    else {
      phase = 6;
    }
    return;
  }
  if (phase == 6) {
    if (green_value > 0) {
      green_value--;
    }
    else {
      phase = 1;
    }
    return;
  }
}