
int red_value;
int green_value;
int blue_value;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
  red_value = 200;
  green_value = 200;
  blue_value = 255;
}

void loop() 
{
  analogWrite(9,red_value);
  analogWrite(10,green_value);
  analogWrite(11,blue_value);
  
  // put your main code here, to run repeatedly:
  delay(100);
}
