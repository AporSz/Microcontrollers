  
int CLK = 2;   //CLK -> D2 (clock)
int DT  = 3;   //DT  -> D3 (data)
int SW  = 4;   //SW  -> D4 (switch)

const int interrupt0 = 0; 
//interrupt number 0 is on DIGITAL PIN 2


int count = 0;     //this will count the clicks 
int lastCLK = 0;   //old clock value
 
void setup()
{
  //pinMode(SW, INPUT);
  //digitalWrite(SW, HIGH);
  pinMode(SW,INPUT_PULLUP);

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  attachInterrupt(interrupt0, ClockChanged, CHANGE);
  //which external interrupt (Arduino UNO has 0 and 1)
  //what function to jump to
  //what happens on that PIN to trigger this

  Serial.begin(9600);
}
 
void loop()
{
  
  if (!digitalRead(SW) && count != 0) 
  {
    count = 0;
    Serial.print("count:");
    Serial.println(count);
  }
}
 
void ClockChanged()
{
  int clkValue = digitalRead(CLK);   
  int dtValue = digitalRead(DT);    

  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    count += (clkValue != dtValue ? 1 : -1);
    
    Serial.print("count:");
    Serial.println(count);
  }
}
