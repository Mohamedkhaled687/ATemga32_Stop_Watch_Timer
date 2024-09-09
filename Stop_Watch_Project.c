#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Stop_Watch_Project.h"
typedef unsigned char unit8;


unit8 seconds = 0, minutes = 0, hours = 0, count_down = 0;
unsigned int last_TC1 = 0;

// Timer1 Compare Match A ISR to update the time
ISR(TIMER1_COMPA_vect) {
        // Count Down Mode
        if (count_down) {
            if (seconds == 0 && hours == 0 && minutes == 0) {
                // buzzer
            }

            if (seconds > 0) {
                seconds--;
            } else if (minutes > 0) {
                seconds = 59;
                minutes--;
            } else if (hours > 0) {
                seconds = 59;
                minutes = 59;
                hours--;
            }
        }
        // Count Up Mode
        else {
            seconds++;
            if (seconds == 60) {
                seconds = 0;
                minutes++;
            }
            if (minutes == 60) {
                minutes = 0;
                hours++;
            }
            if (hours == 24) {
                hours = 0;
            }
        }
}

// Initialize Timer1 in CTC mode
void timer1_init_CTC_mode() {
    TCNT1 = 0;  // Initialize counter
    OCR1A = 15624;  // Set compare value for 1 second interval (assuming 1MHz clock and 1024 prescaler)
    TCCR1A = 0;  // Clear TCCR1A
    TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);  // Set CTC mode and prescaler of 1024
    TIMSK |= (1 << OCIE1A);  // Enable Timer1 Compare Match A interrupt
    sei();  // Enable global interrupts
}

// External Interrupt INT0 ISR to reset the stopwatch
ISR(INT0_vect) {
        seconds = 0;
        minutes = 0;
        hours = 0;
        TCNT1 = 0;  // Reset Timer1 counter
}

// Initialize INT0 for falling edge detection
void INT0_init() {
    DDRD &= ~(1 << PD2);  // Configure PD2 (INT0) as input
    PORTD |= (1 << PD2);  // Enable internal pull-up resistor on PD2
    MCUCR |= (1 << ISC01);  // Set falling edge detection for INT0
    MCUCR &= ~(1 << ISC00);
    GICR |= (1 << INT0);  // Enable INT0
}

// External Interrupt INT1 ISR to pause the stopwatch
ISR(INT1_vect) {
        TCCR1B = 0;  // Stop Timer1
        last_TC1 = TCNT1;  // Store current Timer1 counter value
}

// Initialize INT1 for rising edge detection
void INT1_init() {
    DDRD &= ~(1 << PD3);  // Configure PD3 (INT1) as input
    MCUCR |= (1 << ISC11) | (1 << ISC10);  // Set rising edge detection for INT1
    GICR |= (1 << INT1);  // Enable INT1
}

// External Interrupt INT2 ISR to resume the stopwatch
ISR(INT2_vect) {
        TCNT1 = last_TC1;  // Restore Timer1 counter value
        TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);  // Restart Timer1
}

// Initialize INT2 for falling edge detection
void INT2_init() {
    DDRB &= ~(1 << PB2);  // Configure PB2 (INT2) as input
    MCUCSR &= ~(1 << ISC2);  // Set falling edge detection for INT2
    GICR |= (1 << INT2);  // Enable INT2
}

// Display seconds on 7-segment displays
void display_seconds() {
    PORTA &= ~(0x3F);  // Disable all displays
    int first_digit = seconds % 10;
    int second_digit = seconds / 10;

    PORTA = (PORTA & 0xC0) | (1 << PA4);  // Enable fifth display
    PORTC = (PORTC & 0xF0) | (second_digit & 0x0F);  // Send tens digit to BCD

    _delay_ms(2);

    PORTA = (PORTA & 0xC0) | (1 << PA5);  // Enable sixth display
    PORTC = (PORTC & 0xF0) | (first_digit & 0x0F);  // Send ones digit to BCD

    _delay_ms(2);  // Minimal delay for refresh
}

// Display minutes on 7-segment displays
void display_minutes() {
    PORTA &= ~(0x3F);  // Disable all displays
    int first_digit = minutes % 10;
    int second_digit = minutes / 10;

    PORTA = (PORTA & 0xC0) | (1 << PA3);  // Enable fourth display
    PORTC = (PORTC & 0xF0) | (first_digit & 0x0F);

    _delay_ms(2);

    PORTA = (PORTA & 0xC0) | (1 << PA2);  // Enable third display
    PORTC = (PORTC & 0xF0) | (second_digit & 0x0F);

    _delay_ms(2);
}

// Display hours on 7-segment displays
void display_hours() {
    PORTA &= ~(0x3F);  // Disable all displays
    int first_digit = hours % 10;
    int second_digit = hours / 10;

    PORTA = (PORTA & 0xC0) | (1 << PA1);  // Enable second display
    PORTC = (PORTC & 0xF0) | (first_digit & 0x0F);

    _delay_ms(2);

    PORTA = (PORTA & 0xC0) | (1 << PA0);  // Enable first display
    PORTC = (PORTC & 0xF0) | (second_digit & 0x0F);

    _delay_ms(2);
}

int main() {
    // Configure ports for 7-segment displays
    DDRC |= 0x0F;   // Configure PORTC for 7-segment data (lower nibble)
    DDRA |= 0x3F;   // Configure PORTA for multiplexing (PA0 to PA5)


    // For incrmenting and Decrementing
    DDRB = 0x00;
    PORTB = 0xFF;

    // Intialize LEDs for Countup and Count Down;
    DDRD |= (1 << PD4) | (1 << PD5);


    PORTA &= ~(0x3F);  // Clear PA0-PA5
    PORTC &= 0xF0;     // Clear lower nibble of PORTC

    // Initialize timer and interrupts
    timer1_init_CTC_mode();
    INT0_init();
    INT1_init();
    INT2_init();

    unit8  button_pressed = 0;
    unit8 button_pressed_PB1 = 0;  // Flag for Hours increment (PB1)
    unit8 button_pressed_PB0 = 0;  // Flag for Hours decrement (PB0)
    unit8 button_pressed_PB4 = 0;  // Flag for Minutes increment (PB4)
    unit8 button_pressed_PB3 = 0;  // Flag for Minutes decrement (PB3)
    unit8 button_pressed_PB6 = 0;  // Flag for Seconds increment (PB6)
    unit8 button_pressed_PB5 = 0;  // Flag for Seconds decrement (PB5)


    while (1) {


        // Display time
        display_seconds();
        display_minutes();
        display_hours();




        // Handle mode toggle with debouncing
        if (!(PINB & (1 << PB7)) && !button_pressed) {
            _delay_ms(20);  // Debounce delay
            if (!(PINB & (1 << PB7))) {  // Check if still pressed after debounce
                count_down = !count_down;  // Toggle count mode
                button_pressed = 1;  // Set the flag to indicate button is pressed
            }
        }
        if (PINB & (1 << PB7)) {
            button_pressed = 0;
        }

        // LEDs for Countup and CountDown
        if (count_down) {
            PORTD |= (1 << PD5);  // Light up the 'count down' LED
            PORTD &= ~(1 << PD4);  // Turn off the 'count up' LED
        } else {
            PORTD |= (1 << PD4);  // Light up the 'count up' LED
            PORTD &= ~(1 << PD5);  // Turn off the 'count down' LED
        }



        // Decrement hours with PB0
        if (!(PINB & (1 << PB0)) && !button_pressed_PB0) {
            _delay_ms(20);  // Debounce delay
            if (!(PINB & (1 << PB0))) {
                if (hours > 0) hours--;
                button_pressed_PB0 = 1;  // Set the flag to indicate button is pressed
            }
        }
        if (PINB & (1 << PB0)) {
            button_pressed_PB0 = 0;  // Reset flag once the button is released
        }



        // Increment hours with PB1
        if (!(PINB & (1 << PB1)) && !button_pressed_PB1) {
            _delay_ms(20);  // Debounce delay
            if (!(PINB & (1 << PB1))) {
                hours++;
                button_pressed_PB1 = 1;  // Set the flag to indicate button is pressed
            }
        }
        if (PINB & (1 << PB1)) {
            button_pressed_PB1 = 0;  // Reset flag once the button is released
        }




        // Increment Minutes With PB4
        if(!(PINB & (1 << PB4)) && !button_pressed_PB4){
            _delay_ms(20);  // Debounce delay
            if(!(PINB & (1 << PB4))){
                if(minutes == 59){
                    minutes = 0;
                    hours++;
                }
                else {
                    minutes++;
                }
                button_pressed_PB4 = 1;  // Set the flag to indicate button is pressed
            }
        }
        if(PINB & (1 << PB4)){
            button_pressed_PB4 = 0;  // Reset flag once the button is released
        }



        // Decrement Minutes With PB3
        if(!(PINB & (1 << PB3)) && !button_pressed_PB3){
            _delay_ms(20);  // Debounce delay
            if(!(PINB & (1 << PB3))){
                if(minutes > 0){
                    minutes --;
                }

                button_pressed_PB3 = 1;  // Set the flag to indicate button is pressed
            }
        }


        if(PINB & (1 << PB3)){
            button_pressed_PB3 = 0;  // Reset flag once the button is released
        }






        // Increment Seconds With PB6


        if(!(PINB & (1 << PB6)) && !button_pressed_PB6){
            _delay_ms(20);  // Debounce delay
            if(!(PINB & (1 << PB6))){
                if(seconds == 59){
                    seconds = 0;
                    minutes++;

                }
                else
                    seconds++;

                button_pressed_PB6 = 1;  // Set the flag to indicate button is pressed
            }
        }


        if(PINB & (1 << PB6)){
            button_pressed_PB6 = 0;  // Reset flag once the button is released
        }




        // Decrement Second with PB5

        if(!(PINB & (1 << PB5)) && !button_pressed_PB5){
            _delay_ms(20);  // Debounce delay
            if(!(PINB & (1 << PB5))){

                if(seconds > 0)
                    seconds--;
                button_pressed_PB5 = 1;  // Set the flag to indicate button is pressed
            }
        }


        if(PINB & (1 << PB5)){
            button_pressed_PB5 = 0;  // Reset flag once the button is released
        }


    }


}
