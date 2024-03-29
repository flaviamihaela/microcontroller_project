#include "mbed.h"

// Define Potentiometer class for reading analog values and converting to voltage and percentage
class Potentiometer {
protected:
    AnalogIn inputSignal;  // Analog input to read potentiometer values
    float VDD;  // Reference voltage
    float currentSamplePC;  // Current sample in percentage (0.0 to 1.0)
    float currentSampleVolts;  // Current sample in volts

public:
    // Constructor initializes the analog pin and reference voltage
    Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {
        currentSamplePC = 0.0;
        currentSampleVolts = 0.0;
    }

    // Returns the amplitude of the signal in volts
    float amplitudeVolts(void) { return (inputSignal.read() * VDD); }

    // Returns the amplitude of the signal in percentage
    float amplitudePC(void) { return inputSignal.read(); }

    // Samples the current value from the potentiometer and updates member variables
    void sample(void) {
        currentSamplePC = inputSignal.read();  // Read the current value in percentage
        currentSampleVolts = currentSamplePC * VDD;  // Convert the percentage to volts
    }

    // Getters for the current sample values
    float getCurrentSampleVolts(void) { return currentSampleVolts; }
    float getCurrentSamplePC(void) { return currentSamplePC; }
};

// Defines a SamplingPotentiometer class that inherits from Potentiometer and adds functionality to sample at a fixed frequency
class SamplingPotentiometer: public Potentiometer {
private:
    Ticker tkr;  // Ticker for setting up a periodic sampling interrupt
    float fs;  // Sampling frequency

public:
    // Constructor initializes the potentiometer and the sampling frequency
    SamplingPotentiometer(PinName pin, float v, float f1) : Potentiometer(pin, v), fs(f1) {
        tkr.attach(callback(this, &SamplingPotentiometer::sample), (1 / fs));  // Attach the sample function to the ticker with the specified sampling frequency
    }
};

// Defines a PwmSpeaker class for outputting a PWM signal to a speaker
class PwmSpeaker {
private:
    PwmOut outputSignal;  // PWM output to control the speaker
    float period;  // PWM period in seconds (inverse of frequency)
    float frequency;  // PWM frequency in Hz
    float minFreq, maxFreq, frac;  // Minimum and maximum frequency and a fractional value for scaling

public:
    // Constructor initializes the PWM pin and the initial frequency
    PwmSpeaker(PinName pin, float freq) : outputSignal(pin), frequency(freq) {
        outputSignal.write(0.50f);  // Set to 50% duty cycle by default
    }

    // Setter and getter methods for frequency and scaling parameters
    void setFreq(float f) { frequency = f; }
    void setMinFreq(float fn) { minFreq = fn; }
    void setMaxFreq(float fx) { maxFreq = fx; }
    void setFrac(float frc) { frac = frc; }

    float getFreq(void) { return frequency; }
    float getMinFreq(void) { return minFreq; }
    float getMaxFreq(void) { return maxFreq; }
    float getFrac(void) { return frac; }

    // Calculates the PWM period based on the current frequency
    void calc_period() { period = 1 / frequency; }

    // Updates the frequency and PWM period based on the fractional value (fc) between the min and max frequency
    void updateFreq(float fn, float fx, float fc) {
        frequency = fc * (fx - fn) + fn;  // Scale the frequency within the specified range
        calc_period();  // Recalculate the period
        outputSignal.period(period);  // Update the PWM period
    }

    // Directly updates the frequency and PWM period
    void updatefreq(float f) {
        frequency = f;  // Set the new frequency
        calc_period();  // Recalculate the period
        outputSignal.period(period);  // Update the PWM period
    }
};

int main() {
    SamplingPotentiometer smplpot(A0, 3.3, 1000);  // Initialize a SamplingPotentiometer with pin A0, reference voltage 3.3V, and sampling frequency 1000Hz
    PwmSpeaker p(D6, 2000);  // Initialize a PwmSpeaker with pin D6 and frequency 2000Hz

    while (true) {
        p.updateFreq(1000, 9000, smplpot.getCurrentSamplePC());  // Update the speaker frequency based on the potentiometer's current sample, scaling it between 1000Hz and 9000Hz
        wait(0.01);  // Short delay to stabilize the loop
    }
}
