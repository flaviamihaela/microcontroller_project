# microcontroller_project
This repository contains microcontroller tasks utilizing Mbed OS libraries for handling various functionalities like LCD display and serial communication. The tasks demonstrate sending and receiving serial data, displaying data on an LCD, and controlling a PWM-driven speaker based on analog input from a potentiometer.

## Task 1: Serial Communication
This task involves setting up serial communication and an LCD display on a microcontroller using Mbed OS libraries. The program sends and receives serial data, displaying the received data on an LCD screen. 

Key points:
- Initialization of an LCD and setup of serial communication on specific pins.
- Implementation of callback functions for handling received data (Rxcallback) and data transmission (Txcallback).
- Display of received data on the LCD and transmission of a simple counter value serially at 1-second intervals.
- The main function initializes the system, clears the LCD, prints an "Rx" message to indicate readiness to receive data, sets up the baud rate for serial communication, and attaches the callback functions to serial Rx and Tx interrupts.

## Task 2: Analog Signal Processing with Potentiometer and PWM Speaker
This task demonstrates reading analog values from a potentiometer, sampling these values periodically, and using them to adjust the frequency of a PWM-driven speaker.

Key points: 

- Potentiometer Class: Reads analog values from a specified pin and converts these readings into voltage and percentage.
- SamplingPotentiometer Class: Extends Potentiometer, adding functionality to sample the analog signal at a fixed frequency using a Ticker.
- PwmSpeaker Class: Generates a PWM signal to drive a speaker, with functionality to adjust the PWM frequency based on external inputs.
- The main function instantiates a SamplingPotentiometer and PwmSpeaker, entering an infinite loop where the speaker frequency is adjusted based on the potentiometer's readings.

## Task 3.1: Basic Asynchronous Serial Communication
This task introduces the ASCommunicator class for asynchronous serial communication, demonstrating sending and receiving characters over serial, with received characters displayed on an LCD.

Key points:
- Encapsulation of functionality for asynchronous serial communication in the ASCommunicator class.
- Definition of communication direction (sender, receiver, or both) using the Direction enum.
- Usage of a loop in the main function to transmit characters incrementally and display received characters on an LCD screen.
- Consideration of potential impacts on communication rates and program responsiveness due to the wait(1.0) delay in the transmission method and the blocking call in the reception method.

## Task 3.2: Buffered Asynchronous Serial Communication

This task extends asynchronous serial communication with buffered input and output, utilizing the Buffer, ASCommunicator, and BufferedASCommunicator classes. 

Key points:
- Implementation of a circular buffer for storing integers and managing full/empty states in the Buffer class.
- Basic asynchronous serial communication setup in the ASCommunicator class.
- Extension of ASCommunicator with buffering capabilities in the BufferedASCommunicator class, including pre-filling the transmit buffer and handling data transmission and reception using interrupts.
- Example usage in the main function, demonstrating management of serial communication with buffered input and output, and displaying received data on an LCD.

## Build & Run

1. Clone the repository to your local machine.

2. Navigate to the task directory.

3. Set target and toolchain:

```bash
>> mbed target NUCLEO_F401RE
>> mbed toolchain GCC_ARM
```

4. Complile
```bash
>> mbed compile -f
```

5. Drag and drop the .bin file (of that specific task) to the board's storage to deploy and flash

## Usage

Tuning Baud Rate: usart.baud(115200), sets USART communication speed.

USART pins: RawSerial usart(PA_11, PA_12), defines which board pins are used for USART communication.

LED pins: DigitalOut redLED(PB_4), for status indication.

LCD output: lcd.locate(0.15), sets x-y position of output on LCD for printing.

## Dependencies

- STM32-based Mbed board, Mbed Application Shield, USB cable

- Mbed CLI with CMake


