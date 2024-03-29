#include "mbed.h"
#include "C12832.h"

// Initialize an LCD object with specified pins
C12832 lcd(D11, D13, D12, D7, D10);

// Define an enumeration for communication direction
typedef enum { sender, receiver, senderReceiver } Direction;

// Define a Buffer class for handling circular buffering
class Buffer {
public:
    Buffer(int s);
    ~Buffer(); // Destructor to clean up dynamically allocated memory
    void put(int i);
    int get();
    bool isFull();
    bool isEmpty();

private:
    int* buf; // Pointer to the buffer's memory
    int size, count; // Size of the buffer and number of items currently in the buffer
    int in, out; // Indices for input and output in the circular buffer
};

Buffer::Buffer(int s) : size(s), in(0), out(0), count(0) {
    buf = new int[size]; // Allocate memory for buffer
}

Buffer::~Buffer() {
    delete[] buf; // Deallocate buffer memory
}

bool Buffer::isEmpty(void) {
    return (count == 0);
}

bool Buffer::isFull() {
    return (count >= size);
}

void Buffer::put(int i) {
    if (!isFull()) { // Only put an item if buffer is not full
        buf[in] = i;
        ++count;
        in = (in + 1) % size; // Increment 'in' index and wrap around if necessary
    }
}

int Buffer::get() {
    if (!isEmpty()) { // Only get an item if buffer is not empty
        int x = buf[out];
        buf[out] = 0;
        --count;
        out = (out + 1) % size; // Increment 'out' index and wrap around if necessary
        return x;
    }
    return -1; // Return an error or invalid value if the buffer is empty
}

// Define the ASCommunicator class for basic asynchronous serial communication
class ASCommunicator {
protected:
    RawSerial rs; // RawSerial object for serial communication
    int baudRate; // Baud rate for serial communication
    Direction commType;

public:
    ASCommunicator(PinName txp, PinName rxp, int br, Direction commRole);
    void setBaudRate(float br); // Set baud rate in bps
    float getBaudRate(void); // Get baud rate in bps
    void setDirection(Direction commRole); // Sets direction of communication
    Direction getDirection(void); // Gets direction of communication
};

ASCommunicator::ASCommunicator(PinName txp, PinName rxp, int br, Direction commRole) : rs(txp, rxp), baudRate(br), commType(commRole) {
    rs.baud(baudRate); // Set the baud rate for serial communication
}

void ASCommunicator::setBaudRate(float br) {
    rs.baud(br); // Set the baud rate
    baudRate = br; // Update the internal baud rate tracking
}

float ASCommunicator::getBaudRate(void) {
    return baudRate; // Return the current baud rate
}

void ASCommunicator::setDirection(Direction commRole) {
    commType = commRole; // Set the communication direction
}

Direction ASCommunicator::getDirection(void) {
    return commType; // Return the communication direction
}

// Define class that extends ASCommunicator with buffering capabilities
class BufferedASCommunicator : public ASCommunicator {
private:
    Buffer RxBuffer, TxBuffer; // Receive and transmit buffers

public:
    BufferedASCommunicator(PinName txp, PinName rxp, int br, Direction commRole, int siz1, int siz2) : ASCommunicator(txp, rxp, br, commRole), RxBuffer(siz1), TxBuffer(siz2) {
        // Pre-fill TxBuffer with pattern for demonstration purposes
        int static s = 0;
        int static n = 4;
        while (!TxBuffer.isFull()) {
            TxBuffer.put(s);
            s += n;
        }
        // Attach callback functions for Rx and Tx interrupts
        rs.attach(callback(this, &BufferedASCommunicator::RxCallback), RawSerial::RxIrq);
        rs.attach(callback(this, &BufferedASCommunicator::TxCallback), RawSerial::TxIrq);
    }

    bool canTx(void) {
        return rs.writeable(); // Returns true if USART can send
    }

    bool canRx(void) {
        return rs.readable(); // Returns true if USART can receive
    }

    void TxCallback(void) {
        if (canTx() && !TxBuffer.isEmpty()) {
            rs.putc(TxBuffer.get()); // Transmit a character from the TxBuffer
            wait(1.0);
        }
    }

    void RxCallback(void) {
        while (!canRx()) {}; // Wait for data to be available
        RxBuffer.put(rs.getc()); // Put received character into RxBuffer
        lcd.locate(0, 15);
        lcd.printf("%d ", RxBuffer.get()); // Get a character from the RxBuffer and display it on the LCD
    }
};

int main() {
    lcd.locate(3, 15);
    lcd.printf("Rx"); // Display "Rx" on LCD to indicate the program is ready to receive data
    BufferedASCommunicator asc(PA_11, PA_12, 5000, senderReceiver, 200, 200); // Initialize a buffered asynchronous serial communicator
    while (true) {}; // Main loop is empty but communication is handled via interrupts and callbacks
}
