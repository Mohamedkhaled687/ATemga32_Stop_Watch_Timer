 # ATemga32_Stop_Watch_Timer ⏱️

This project implements a versatile stopwatch using the **ATmega32 microcontroller**, featuring both increment and countdown modes. The display is handled through **multiplexed seven-segment displays**, and various functions such as reset, pause, and resume are controlled via external interrupts.

## 🚀 Features

- **Increment Mode (Default)**: Stopwatch counts up from 0, displaying hours, minutes, and seconds.
- **Countdown Mode**: Set a custom countdown time and the stopwatch will count down until it reaches zero.
- **Multiplexing Technique**: Six seven-segment displays are controlled using multiplexing to display time continuously.
- **Reset Function**: Resets the time in both modes.
- **Pause & Resume**: Pause the stopwatch at any moment and resume from where it was paused.
- **Mode Toggle**: Easily switch between increment and countdown modes.
- **Buzzer Alarm**: Triggers when the countdown reaches zero or during alert conditions.

## 🛠️ Hardware Components

- **Microcontroller**: ATmega32
- **Display**: Six multiplexed seven-segment displays (common anode) driven by a 7447 BCD to seven-segment decoder.
- **Input**: 
  - 10 push buttons for reset, pause, resume, mode toggle, and time adjustments (hours, minutes, seconds).
  - **Reset Button**: Connected to PD2 (INT0) - Resets the stopwatch.
  - **Pause Button**: Connected to PD3 (INT1) - Pauses the stopwatch.
  - **Resume Button**: Connected to PB2 (INT2) - Resumes the stopwatch.
  - **Mode Toggle**: Connected to PB7 - Switches between increment and countdown modes.
  - **Time Adjustments**: Increment and decrement buttons for hours, minutes, and seconds.
- **Alarm/Buzzer**: Connected to PD0 and activated when countdown reaches zero.
- **LED Indicators**: 
  - Counting Up LED: PD4 (Red)
  - Counting Down LED: PD5 (Yellow)

## ⚙️ System Requirements

1. **Microcontroller**: ATmega32
2. **System Frequency**: 16 MHz
3. **Timer1 Configuration**: CTC mode to track time accurately.
4. **Multiplexed Display**: Six 7-segment displays connected via the multiplexing method.
5. **External Interrupts**:
   - INT0 (PD2): Falling edge to reset the time.
   - INT1 (PD3): Rising edge to pause the time.
   - INT2 (PB2): Falling edge to resume the time.


## 🎥 Watch the Demo

Check out the live demo of the project in action:  
**[Stopwatch Timer Demo](https://www.linkedin.com/feed/update/urn:li:activity:7239276678693212160/)**



