/*
 * Stop_Watch_Project.h
 *
 *  Created on: Sep 8, 2024
 *      Author: Your Name
 *
 *  Description: This header file contains all necessary function prototypes,
 *  global variables, and macro definitions for the stopwatch project.
 */

#ifndef STOP_WATCH_PROJECT_H_
#define STOP_WATCH_PROJECT_H_

#include <avr/io.h>
#include <avr/interrupt.h>

// Typedef for easy usage of uint8 data type
typedef unsigned char unit8;



// Global variables to keep track of time
extern unit8 seconds;
extern unit8 minutes;
extern unit8 hours;
extern unit8 count_down;
extern unsigned int last_TC1;

// Function prototypes
void timer1_init_CTC_mode(void);
void INT0_init(void);
void INT1_init(void);
void INT2_init(void);

void display_seconds(void);
void display_minutes(void);
void display_hours(void);

// Interrupt Service Routines
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);

#endif /* STOP_WATCH_PROJECT_H_ */
