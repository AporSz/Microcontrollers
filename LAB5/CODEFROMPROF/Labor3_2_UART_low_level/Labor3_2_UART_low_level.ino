#include <util/delay.h>
#include <avr/io.h>  

void UART_init()
{

//set baud rate
//baud = fosc / (16 * (UBRR+1) )
//UBRR = 103 -> baud = 16,000,000 / (16 * (103+1)) = 9615 so it is about 9600 baud
//UBRR = 8 -> baud = 16,000,000 / (16 * (8+1)) = 111,111 about 115,200 baud
//reverse:
//UBRR = fosc/ (16 BAUD) - 1
//baud  = 250,000
// 16,000,000 / (16 * 250,000) - 1 = 3
//baud = 500,000
// 16,000,000 / (16 * 500,000) - 1 = 1

UBRR0H = 0;
UBRR0L = 103; //9600 baud 
//UBRR0L = 8; // 115200 baud
//UBRR0L = 3; // 250000 baud
//UBRR0L = 1;

//UCSRA = RXC TXC UDRE flags FE DOR PE errors U2X double speed MPCM multi processor
UCSR0A = 0b00000000;

//UCSRB =  RXCIE TXCIE UDRIE  interrupt enable RXEN TXEN enable 
// UCSZ2 character size setting RXB8 TXB8 9.th bit locations
// RXEN = 1 TXEN = 1
//char size == 8 bits UCSZ2 = 0 and then UCSZ1 = 1 UCSZ0 =1 in the next register
//a nice way of writing this is:
//UCSR0C = 1<<RXEN | 1<<TXEN;
UCSR0B = 0b00011000;


//UCSRC =  URSEL = 1 so we would write UCSRC
//UMSEL = 0 mode = async
//UPM1, UPM0 = 00 disabled parity  USBS = 0 -> 1  stop bit 
//UCSZ1,UCSZ0 = 1,1 + UCSZ2 -> 8 bit packages
//UCPOL = 0 no clock -> this has to be 0
//a nice way of writing this is:
//UCSR0C = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;
UCSR0C = 0b10000110;

}

void UART_transmit(unsigned char data)
{
  while ((UCSR0A & (1<<UDRE0)) == 0) {}; 
  //wait until I can send more
  UDR0 = data;
}

unsigned char UART_receive()
{
  unsigned char data;

  while ((UCSR0A & (1<<RXC0)) == 0) {};
  data = UDR0;

  return data;
}



char sample_text[] = "Hello world\n";
int i;

void setup()
{
  UART_init();
}


void loop()
{
  
  for(i = 0;i < strlen(sample_text);i++)
    {
      UART_transmit(sample_text[i]);
    }
      
    //CR and NL
    UART_transmit(13);
    UART_transmit(10);    
    delay(100);
}
