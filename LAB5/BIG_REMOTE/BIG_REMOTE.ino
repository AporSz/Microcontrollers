
/*
IR Receiver low level code

Low level using Timer1
Low level extrenal interrupt 1 
on Digital Pin 3
 */

int measured_time[100];

volatile byte N_times;
volatile byte index_time;
volatile byte in_receiving_mode;
//finite state of the machine

byte received_pulses[150];
byte i_pulse;
byte N_pulses;

byte received_bits[100];
byte i_bit;
byte N_bits;


void setup() {

  Serial.begin(9600);
  Serial.println("Ready to receive the IR signal from the remote");
  pinMode(3, INPUT);

  index_time = 0;
  in_receiving_mode = 1;  //I am in receiving mode I am waiting for the signal

  cli();
  //Timer 1 setup
  TCCR1A = 0b00000000;  //normal mode
  TCCR1B = 0b00000010;  //prescaler : 8 // the counter will count with 2MHz
  TCNT1 = 0x00;         //we count in this register
                        // one count every 500 ns, OVF is 65535
                        //so 32.7675 ms is the max time it can measure
  TIMSK1 = 0b00000001;  //we need overflow interrupt to stop receiving (unless we know the protocol)
  TIFR1 = 0b00000001;   //reset the OVF flag

  //Kulso interrupt beallitasa
  EICRA = 0b00000100;  // any change on PIN3 will trigger the interrupt
  EIMSK = 0b00000010;  // I want to have External Interrupt 1
  EIFR = 0b00000000;   // zero all flags
  sei();
}

//external interrupt 1 on PIN 3
//any change on the PIN will get me into this routine
ISR(INT1_vect) {
  measured_time[index_time] = TCNT1 / 2;
  TCNT1 = 0x00;
  index_time++;

  if (in_receiving_mode == 1) in_receiving_mode = 0;
  //in_receiving_mode set to 0
  //I am now receiving not just waiting for the signal to start
}

ISR(TIMER1_OVF_vect) {
  //if I have an overflow on the timer it means the signal must have ended
  if (in_receiving_mode == 0) {
    N_times = index_time;  // this is how many times I measured
    in_receiving_mode = 2;
    //this means the signal has ended I am now going to mode 2, write it out
  }
}

void loop() {
  if (in_receiving_mode == 2) {
    //write out the raw data that I got
    index_time = 0;
    Serial.println("Received signal, it ended with a timer overflow");

    for (byte i = 0; i < N_times; i = i + 2) {
      //assume it starts with a dark time like RC5 protocol
      Serial.print("dark/light times[");
      Serial.print(i);
      Serial.print("/");
      Serial.print(i + 1);
      Serial.print("] = ");
      Serial.print(measured_time[i]);
      Serial.print("/");
      Serial.println(measured_time[i + 1]);

      if (i == 0) {
        received_pulses[0] = 0;
        received_pulses[1] = 1;
        i_pulse = 2;
      } else {
        //always a short one
        received_pulses[i_pulse] = 0;
        i_pulse++;
        //sometimes a longer one, then there should be two 0s
        if (measured_time[i] >= 1000) {
          received_pulses[i_pulse] = 0;
          i_pulse++;
        }
        //always a short one
        received_pulses[i_pulse] = 1;
        i_pulse++;
        //sometimes a longer one, then there should be two 0s
        if (measured_time[i + 1] >= 1000) {
          received_pulses[i_pulse] = 1;
          i_pulse++;
        }
      }
    }
    N_pulses = i_pulse;

    if ((N_pulses % 2 == 1) && (received_pulses[N_pulses - 1] == 1)) {
      received_pulses[N_pulses] = 0;
      N_pulses++;
      Serial.println("Added a final 0 (dark time)");
    }

    Serial.print(N_pulses);
    Serial.print(" pulses = ");
    for (byte i = 0; i < N_pulses; i++) {
      Serial.print(received_pulses[i]);
    }
    Serial.println();

    //format this pulse data into bits if I am receiving the RC5 protocol

    i_bit = 0;

    for (byte i = 0; i < N_pulses; i = i + 2) {
      //dark and then light is a 1 bit
      if ((received_pulses[i] == 0) && (received_pulses[i + 1] == 1)) received_bits[i_bit] = 1;
      //light and then dark is a 0 bit
      if ((received_pulses[i] == 1) && (received_pulses[i + 1] == 0)) received_bits[i_bit] = 0;
      i_bit++;
    }
    N_bits = i_bit;

    Serial.print(N_bits);
    Serial.print(" bits = ");
    for (byte i = 0; i < N_bits; i++) {
      Serial.print(received_bits[i]);
    }
    Serial.println();

    // --- Replace your 'last_bits' and 'character' logic with this ---

  int command = 0;
  byte bits_to_extract = 6; // Set to 8 if your remote uses 8-bit commands

  // Ensure we don't go out of bounds if the signal was too short
  if (N_bits >= bits_to_extract) {
      for (byte i = 0; i < bits_to_extract; i++) {
          // We look at the bits from the end of the array
          // (N_bits - bits_to_extract) is the starting point
          if (received_bits[(N_bits - bits_to_extract) + i] == 1) {
              // Shift 1 into the correct position (MSB first)
              command |= (1 << (bits_to_extract - 1 - i));
          }
      }

      Serial.print("Decoded Command (Last ");
      Serial.print(bits_to_extract);
      Serial.print(" bits): ");
      Serial.println(command);
  } else {
      Serial.println("Error: Not enough bits received.");
  }

    delay(250);
    in_receiving_mode = 1;
  }
}
