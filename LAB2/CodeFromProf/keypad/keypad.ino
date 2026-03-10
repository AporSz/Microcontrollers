
byte row_pins[4] = {5,6,7,8};
byte column_pins[4] = {9,10,11,12};

void setup() 
{
  byte i;

  for(i=0;i<4;i++)
    pinMode(row_pins[i],OUTPUT);

  for(i=0;i<4;i++)
    pinMode(column_pins[i],INPUT_PULLUP);
 
}

void loop() 
{
  byte column;

  for(row=0;row<4;row++)
    {
      for(column=0;column<4;column++)
        digitalWrite(row_pins[column],HIGH);
      digitalWrite(row_pins[row],LOW);

      for(column=0;column<4;column++)
        {
          value = digitalRead(column_pins[column]);
          if (value==0)
            {
              Serial.print("Found it! column = ");
              Serial.print(column);
              Serial.print("row = ");
              Serial.print(row);

            }
        }
    }

delay(1);

}
