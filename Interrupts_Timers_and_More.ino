//Always include the F_CPU to reduce errors
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define the LED pins
#define EX1_LED_PIN PB0
#define EX2_LED_PIN PB1

// Define the push button pin
#define BUTTON_PIN PB2

int main(void) {
  //SET UP PINS INPUT, OUTPUTS, AND PULL UP/DOWN RESISTORS HERE:
  DDRB |= (1 << EX1_LED_PIN) | (1 << EX2_LED_PIN);  // Set LED pins as output
  DDRB &= ~(1 << BUTTON_PIN);                       // Set button pin as input
  PORTB |= (1 << BUTTON_PIN);                       // Enable pull-up resistor for the button

  // Timer Set up

  // Timer 0 8-bit timer Registers
  TCCR0A = (1 << WGM01);               // Set Timer0 to CTC mode
  TCCR0B = (1 << CS02) | (1 << CS00);  // Set prescaler to 1024
  TIMSK0 = (1 << OCIE0A);              // Enable Timer0 Compare A interrupt

  // Register for Count for Timer 0
  OCR0A = 156;  // Corresponds to 1 second delay (16,000,000 / 1024 / 156)

  // External Interrupt Set up
  EICRA = (1 << ISC01);  // Set INT0 to trigger on falling edge
  EIMSK = (1 << INT0);   // Enable INT0 interrupt

  sei();  // Enable global interrupts

  while (1) {
    // Toggle the external LED every 1 second

    //_delay_ms(1000);   // Delay for 1 second
    PORTB ^= (1 << EX1_LED_PIN);  // Toggle the external LED

    // Check the status of the push button and control the external LEDs accordingly
    if (!(PINB & (1 << BUTTON_PIN))) {
      // Button is pressed, turn ON external LED 2
      PORTB |= (1 << EX2_LED_PIN);
      //_delay_ms(500);
    } else if (PINB & (1 << EX2_LED_PIN)) {
      // Button is not pressed, turn OFF external LED 2
      PORTB &= ~(1 << EX2_LED_PIN);
      //_delay_ms(500);
    } else {
      //nothing to see here
    }
    //_delay_ms(1000);
  }
  return (0);
}

// Timer Interrupt Code, What to do once the timer has reached its count or overflow

// Timer0 Compare A interrupt to blink the LED ON and OFF
ISR(TIMER0_COMPA_vect) {
  PORTB ^= (1 << EX1_LED_PIN);  // Toggle External LED 1 state
  // the random delay and PORTBs are attempts to see what works, and it was easier to turn them off and on with //.
  //_delay_ms(1000);
  //PORTB &= ~(1 << EX1_LED_PIN);
  //_delay_ms(1000);
}

// INT0 interrupt to control the external LEDs based on the button press
ISR(INT0_vect) {
  if (!(PINB & (1 << BUTTON_PIN)))  // Check if button is pressed (logic low)
  {
    PORTB |= (1 << EX2_LED_PIN);  // Turn ON the external LED 2
                                  //_delay_ms(1000);
  } else if (PINB & (1 << BUTTON_PIN)) {
    PORTB &= ~(1 << EX2_LED_PIN);  // Turn OFF the external LED 2
                                   //delay(1000);
  } else {
    //you see nothing
  }
}