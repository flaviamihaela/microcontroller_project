#include "mbed.h"
#include "C12832.h"

// Initialize LCD object with specified pins
C12832 lcd(D11, D13, D12, D7, D10);

// Define enumeration for direction of communication
typedef enum { sender, receiver, senderReceiver } Direction;

// Define ASCommunicator class for asynchronous serial communication
class ASCommunicator {
private:
    RawSerial rs; // RawSerial object for serial communication
    int baudRate; // Baud rate for serial communication
    Direction commType; // Direction of communication (sender, receiver, or both)

public:
    ASCommunicator(PinName txp, PinName rxp, int br, Direction commRole);

    void setBaudRate(float br); // Set Baud rate in bps

    float getBaudRate(void); // Get Baud rate in bps

    void setDirection(Direction commRole); // Set direction of communication

    Direction getDirection(void); // Get direction of communication

    bool canTx(void) { return rs.writeable(); } // Returns true if USART can send

    bool canRx(void) { return rs.readable(); } // Returns true if USART can receive

    // Transmit a character over serial
    void Tx(char c) {
        if (canTx()) {
            rs.putc(c);
            wait(1.0);
        }
    }

    // Receive a character from serial
    char Rx(void) {
        while (!canRx()) {};
        return rs.getc();
    }
};

ASCommunicator::ASCommunicator(PinName txp, PinName rxp, int br, Direction commRole) : rs(txp, rxp), baudRate(br), commType(commRole) {
    rs.baud(baudRate); // Apply the baud rate to the serial object
}

void ASCommunicator::setBaudRate(float br) {
    rs.baud(br);
}

float ASCommunicator::getBaudRate(void) {
    return baudRate;
}

void ASCommunicator::setDirection(Direction commRole) {
    commType = commRole;
}

Direction ASCommunicator::getDirection(void) {
    return commType;
}

int main() {
    char i = 0; // Initialize a character counter

    // Create an ASCommunicator object for both sending and receiving, with a baud rate of 5000
    ASCommunicator asc(PA_11, PA_12, 5000, senderReceiver);
    while (true) {
        lcd.locate(3, 15);
        asc.Tx(i); // Transmit the current character
        i++; // Increment the character
        lcd.printf("%d ", asc.Rx()); // Display the received character on the LCD
    }
}
