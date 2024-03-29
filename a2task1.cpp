#include "mbed.h"  // Main Mbed OS header file for general functions and definitions
#include "C12832.h"  // Makes driver code for LCD on Application Shield available for displaying data

// Creates LCD object and defines microcontroller pins for SPI communication and control
C12832 lcd(D11, D13, D12, D7, D10);

// Creates serial port object, using PA_11 as Tx and PA_12 as Rx
RawSerial usart(PA_11, PA_12);

// Callback function for receiving data
void Rxcallback(void) {
    // Loop until Rx pin is readable / data is available
    while (!usart.readable()) {};

    // Specify output position on LCD
    lcd.locate(0, 15);
    // Read a character from the serial port and display it on the LCD
    lcd.printf("%d ", usart.getc());
}

// Callback function for transmitting data
void Txcallback(void) {
    // Static variable 'i', retains its value between function calls
    static int i = 0;

    if (usart.writeable()) {
        usart.putc(i); // USART stores the i value
        wait(1.0); // Wait for 1 second
        i++; // i is incremented (counter)
    }
}

int main() {
    // Initialize digital output for controlling an LED on pin PB_4
    DigitalOut redLED(PB_4);
    // Set LED to off (assuming active high logic)
    redLED = 0;

    lcd.cls(); // Clear the LCD screen
    lcd.locate(0, 3);
    // Print "Rx", indicating readiness to receive data
    lcd.printf("Rx");

    // Set the baud rate for serial communication to 115200 bps
    usart.baud(115200); // Baud rate of both Tx and Rx

    // Attach callback functions to serial interrupts for receiving and transmitting
    usart.attach(&Rxcallback, Serial::RxIrq);
    usart.attach(&Txcallback, Serial::TxIrq);

    // Infinite loop to keep the program running
    while (1);
}
