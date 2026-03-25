
//Mikrokontrollers Lab 5
//UART speed demo

char sample_text[] = " Sending the text again after a delay of 1ms. Time this took is = ";

unsigned long time;
unsigned long old_time;

void setup() {
  // Serial.begin(9600);
  //change to 115200 and check how much faster that is
  //also change the speed in the Serial Monitor
  // Serial.begin(115200);
  Serial.begin(2000000);
  time = millis();
}

void loop() {
  
  old_time = time;
  time = millis();
  Serial.print(time);
  Serial.print(sample_text);
  Serial.println(time - old_time);
  //Serial.println(strlen(sample_text));

  //N bytes sent s N*(8+2 bits because +start+stop) = M bits (66)
  //9600 bits/sec megy M/9600=? how many ms + 1 ms delay = ?

  //115200 baud M/115200 = ? ms + 1 ms delay = ?
  //change to 115200

  //try 2 million baud
  //M/2000000 =? ms

  delay(1);
}
