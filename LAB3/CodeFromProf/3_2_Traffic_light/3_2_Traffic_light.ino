

/*
Microcontrollers 3.rd LAB
GPIO - output and timer high level

finite state machine 
- a traffic light can be considered a finite state machine
*/


byte pedestrian_red = 5;
byte pedestrian_green = 6;

byte pedestrian_button = 7;

byte car_red = 8;
byte car_yellow = 9;
byte car_green = 10;

byte state_of_the_machine = 1;


/* finite state machine - it is in a given state
it can go to some other state if there are conditions
1 - normal (green for cars, red for pedestrians)
2 - switching to pedestrian: the green for cars flashes
2 - the car lamp turns to yellow
3 - the car lamp turns to red 
4 - the pedestrian lamp turns to green (hold it a while)
5 - switching back to cars: the green for pedestrians flashes
6 - the pedestrian gets a red
after this it could go back to state 1
 */
 
byte pedestrian_button_state;

void setup() 
{
  pinMode(pedestrian_red, OUTPUT);
  pinMode(pedestrian_green, OUTPUT);

  pinMode(pedestrian_button, INPUT_PULLUP);

  pinMode(car_red, OUTPUT);
  pinMode(car_yellow, OUTPUT);
  pinMode(car_green, OUTPUT);

  state_of_the_machine = 1;
  digitalWrite(pedestrian_red, 1);
  digitalWrite(pedestrian_green, 0);
  digitalWrite(car_green, 1);
  digitalWrite(car_yellow, 0);
  digitalWrite(car_red, 0);
    
  Serial.begin(9600);
}

void loop() 
{
  if (state_of_the_machine == 1)
  {
    //basic state
    digitalWrite(pedestrian_red, 1);
    digitalWrite(pedestrian_green, 0);
    digitalWrite(car_green, 1);
    digitalWrite(car_yellow,0);
    digitalWrite(car_red,0);
  }
  else if (state_of_the_machine == 2)
  {
    for(byte i=0;i<5;i++)
    {
      digitalWrite(car_green,1);
      delay(100);  
      digitalWrite(car_green,0);
      delay(100);
    }
    digitalWrite(car_yellow,1);
    delay(100);
    state_of_the_machine = 3;
  }
  else if (state_of_the_machine == 3)
  {
    delay(500); 
    digitalWrite(car_yellow,0);
    digitalWrite(car_red,1);
    delay(100);
    state_of_the_machine = 4;
  }
  else if (state_of_the_machine == 4)
  {
    delay(500);  
    digitalWrite(pedestrian_red,0);
    digitalWrite(pedestrian_green,1);
    state_of_the_machine = 5;
  }  
  else if (state_of_the_machine == 5)
  {
    delay(1500);
    for(byte i=0;i<5;i++)
    {
      digitalWrite(pedestrian_green,1);
      delay(100);  
      digitalWrite(pedestrian_green,0);
      delay(100);
    }
    
    state_of_the_machine = 6;
  }  
  else if (state_of_the_machine == 6)
  {
    delay(500);
    digitalWrite(pedestrian_red, 1);
    delay(500);

    state_of_the_machine = 1;
  }    

  pedestrian_button_state = digitalRead(pedestrian_button);

  //1 - not pressed (INPUT PULLUP) 0 - pressed
  if ((pedestrian_button_state == 0) && (state_of_the_machine == 1))
  {
    state_of_the_machine = 2;
  }

  delay(100);
}
