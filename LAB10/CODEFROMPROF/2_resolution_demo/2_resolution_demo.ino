


int mask;
int sensorValue = 0, maskedsensorValue = 0;

void setup() 
{
//mask = 0b0000001111111111;//10 bits
//mask = 0b0000001111111110;//9 bits
//mask = 0b0000001111111100;//8 bits
//mask = 0b0000001111000000;//4 bits
mask = 0b0000001110000000;//3 bits


Serial.begin(9600);
}


void loop() 
{
sensorValue = analogRead(A0);
maskedsensorValue = sensorValue & mask;
Serial.print("10bit_resolution:");
Serial.print(sensorValue);
Serial.print(",");
Serial.print("Low_resolution:");
Serial.print(maskedsensorValue);
Serial.print(",");
Serial.print("Min:");
Serial.print(0);
Serial.print(",");
Serial.print("Max:");
Serial.println(1023);

delay(100);

}
