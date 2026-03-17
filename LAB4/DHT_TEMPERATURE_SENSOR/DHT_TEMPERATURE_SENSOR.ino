
/*
DHT11es sensor

Low level a Timer1 is used
Low level extrenal interrupt 0 is used
 
Connect the DHT sensor to digital pin 2

 */

//DHT sensor library from adafruit needed

int measured_time[100];//Max 100 time intervals are measured

volatile byte N_measured_time;
volatile byte index_measured_time;
volatile byte receiving_mode;

int bits[40];
byte i;
byte humint, humdec, tempint, tempdec;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Reading the DHT11 signal");
  pinMode(2, INPUT); //this is where the DHT11 line goes

  index_measured_time = 0;
  receiving_mode = 1; //I am in a receiving mode I am waiting for the signal to start

  cli();
  //Timer 1 setup
  TCCR1A = 0b00000000; //normal mode
  TCCR1B = 0b00000010; //prescaler : 8 // will count on 2MHz
  TCNT1  = 0x00;       //this will do the counting
                      // 500 ns is one step, OVF is at 65535 so at 32.7675 ms this is the max time I can measure
                      // START signal is 9 ms so the 32 ms max is enough even for that 
  TIMSK1 = 0b00000001; //overflow means timeout 
  TIFR1  = 0b00000001; //reset OVF flag

  //External interrupt setup
  EICRA = 0b00000001; // any change on PIN2
  EIMSK = 0b00000001; // want to have external interrupt 0 (on pin2)
  EIFR  = 0b00000000; // flags cleared
  sei();
}

//external interrupt 0 PIN 2
//any change so rising and falling too
ISR(INT0_vect)
{
  measured_time[index_measured_time] = TCNT1 / 2 ;
  TCNT1 = 0x00; 
  index_measured_time++;

  if (receiving_mode==1) receiving_mode = 0;
  //the signal started
}

ISR(TIMER1_OVF_vect)
{
  //overflow means everythign should be finished
  if (receiving_mode==0)
  {
    N_measured_time = index_measured_time;
    receiving_mode = 2;
  }
}

void loop() 
{
  //pull DHT line LOW to start the communication
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW); 
  TCNT1 = 0;
  while (TCNT1<=36000); //wait 18 ms this is the RESET signal
  TCNT1 = 0;
  //let the DHT11 talk
  pinMode(2,INPUT_PULLUP);

  //wait until transmission finishes
  while(receiving_mode!=2);
    
  if (receiving_mode==2)
  {
    receiving_mode = 1;
    index_measured_time = 0;     
    Serial.println("Signal finished with a timeout");  

    //raw measured_times
    for(i=0;i<N_measured_time;i=i+2)
    {
      Serial.print("HIGH/LOW measured_time[");
      Serial.print(i);
      Serial.print("/");
      Serial.print(i+1);
      Serial.print("] = ");
      Serial.print(measured_time[i]);
      Serial.print("/");
      Serial.print(measured_time[i+1]); 
      
      if (i>=6) { 
        Serial.print(" bit[");
        Serial.print((i-6)/2);
        Serial.print("] = ");

        if (measured_time[i]>50) {
          Serial.println(" 1");bits[(i-6)/2] = 1;
        }
        else {
          Serial.println(" 0");bits[(i-6)/2] = 0;
        }
      }
      else Serial.println();
    }

    humint = 0;
    humdec = 0;
    tempint = 0;
    tempdec = 0;

    Serial.print("Humidity integral    = ");
    for(i=0;i<8;i++)
    {
      Serial.print(bits[i]);
      humint += bits[i]<<(7-i);
    }
    Serial.print(" == "); 
    Serial.println(humint);

    Serial.print("Humidity decimal     = ");
    for(i=8;i<16;i++)
    {
      Serial.print(bits[i]);
      humdec += bits[i]<<(15-i);
    }
    Serial.print(" == ."); 
    Serial.print(humdec);
    Serial.println("% humidity");  
    
    Serial.print("Temperature integral = ");
    for(i=16;i<24;i++)
    {
      Serial.print(bits[i]);
      tempint += bits[i]<<(23-i);
    }
    Serial.print(" == "); 
    Serial.println(tempint);

    Serial.print("Temperature decimal  = ");
    for(i=24;i<32;i++)
    {
      Serial.print(bits[i]);
      tempdec += bits[i]<<(31-i);
    }
    
    Serial.print(" == ."); 
    Serial.print(tempdec); 
    Serial.println("C"); 

    Serial.print("Error Correction       = ");
    for(i=32;i<40;i++) Serial.print(bits[i]);
    Serial.println(); 

    delay(1000);
  }
}
