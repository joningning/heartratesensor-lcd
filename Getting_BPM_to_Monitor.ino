#include <PulseSensorPlayground.h>  // Includes the PulseSensorPlayground Library.  
#include <Wire.h> 

// Variables
const int PulseWire = 0;        // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;     // The on-board Arduino LED, close to PIN 13.
int Threshold = 600;            // Adjusted Threshold for detecting beats (increase if needed)
int Signal;                     // To hold the raw sensor signal value.

PulseSensorPlayground pulseSensor;  // Creates a PulseSensor object called "pulseSensor"

// Timing variables for different sampling intervals
unsigned long lastSignalReadTime1 = 0;  // To control when to read the signal for Wave 1
unsigned long lastSignalReadTime2 = 0;  // To control when to read the signal for Wave 2
unsigned long lastSignalReadTime3 = 0;  // To control when to read the signal for Wave 3

unsigned long signalReadInterval1 = 40;  // Signal read interval for Wave 1 (fast sampling)
unsigned long signalReadInterval2 = 100; // Signal read interval for Wave 2 (medium sampling)
unsigned long signalReadInterval3 = 200; // Signal read interval for Wave 3 (slow sampling)

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud rate

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);  // Blink the on-board LED with each heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the PulseSensor object creation.
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created and started!");
  }

  // Initialize headers for the Serial Plotter (optional, to label the axes)
  Serial.println("Wave1\tWave2\tWave3");  // Print header labels (this line will not show in the plot)
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time

  // Read the signal at regular intervals for Wave 1 (fast sampling)
  if (currentMillis - lastSignalReadTime1 >= signalReadInterval1) {
    lastSignalReadTime1 = currentMillis;  // Update the last signal read time for Wave 1
    Signal = analogRead(PulseWire);       // Read the sensor value
    Serial.print("Wave1: ");               // Label the first waveform
    Serial.print(Signal);                 // Print the raw signal value for Wave 1
    Serial.print("\t");                   // Separate the values with a tab for the next waveform
  }

  // Read the signal at regular intervals for Wave 2 (medium sampling)
  if (currentMillis - lastSignalReadTime2 >= signalReadInterval2) {
    lastSignalReadTime2 = currentMillis;  // Update the last signal read time for Wave 2
    Signal = analogRead(PulseWire);       // Read the sensor value
    Serial.print("Wave2: ");               // Label the second waveform
    Serial.print(Signal);                 // Print the raw signal value for Wave 2
    Serial.print("\t");                   // Separate the values with a tab for the next waveform
  }

  // Read the signal at regular intervals for Wave 3 (slow sampling)
  if (currentMillis - lastSignalReadTime3 >= signalReadInterval3) {
    lastSignalReadTime3 = currentMillis;  // Update the last signal read time for Wave 3
    Signal = analogRead(PulseWire);       // Read the sensor value
    Serial.print("Wave3: ");               // Label the third waveform
    Serial.println(Signal);               // Print the raw signal value for Wave 3
  }

  // Calculate and print BPM at regular intervals (just like in your original code)
  static unsigned long lastBPMTime = 0;  // To control when to print BPM
  if (currentMillis - lastBPMTime >= 100) {
    lastBPMTime = currentMillis;  // Update the last BPM print time
    if (pulseSensor.sawStartOfBeat()) {  // Check if a heartbeat occurred
      int myBPM = pulseSensor.getBeatsPerMinute();  // Get the BPM
      static int lastBPM = 0;  // Variable to hold the last BPM
      int smoothedBPM = (myBPM + lastBPM) / 2;  // Smooth the BPM
      lastBPM = smoothedBPM;  // Save the smoothed BPM for the next iteration

      Serial.println("♥  A HeartBeat Happened!");    // Print message on beat detection
      Serial.print("BPM: ");                        // Print "BPM: "
      Serial.println(smoothedBPM);                  // Print the smoothed BPM
    }
  }

  delay(10);  // Fine-tune if needed, but avoid too long delays to not miss beats
}
