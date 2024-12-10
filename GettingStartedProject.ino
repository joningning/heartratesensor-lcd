#include <PulseSensorPlayground.h>  // Includes the PulseSensorPlayground Library.
#include <Wire.h>

int const PULSE_SENSOR_PIN = 0;   // 'S' Signal pin connected to A0
const int LED = LED_BUILTIN; 
const int PulseWire = 0;  

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 300;       // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor;

int smoothedSignal = 0;    // Variable for smoothed signal
int lastSignal = 0;        // To hold the last signal value
const int smoothingFactor = 10;  // Factor to smooth out signal fluctuations

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);  // Built-in LED will blink to your heartbeat
  Serial.begin(750);           // Set comm speed for serial plotter window

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);  // Blink the on-board LED with each heartbeat.
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created and started!");
  }
}

void loop() {
  Signal = analogRead(PULSE_SENSOR_PIN); // Read the sensor value

  // Smooth the signal to avoid sharp fluctuations
  smoothedSignal = (lastSignal + Signal) / 2;
  lastSignal = smoothedSignal;

  // Constrain the signal to a fixed range (if needed)
  smoothedSignal = constrain(smoothedSignal, 0, 1024);

  // Print the smoothed signal to the Serial Plotter
  Serial.println(smoothedSignal);

  if (pulseSensor.sawStartOfBeat()) {  // Check if a heartbeat occurred
    int myBPM = pulseSensor.getBeatsPerMinute();  // Get the BPM
    static int lastBPM = 0;  // Variable to hold the last BPM
    int smoothedBPM = (myBPM + lastBPM) / 2;  // Smooth the BPM
    lastBPM = smoothedBPM;  // Save the smoothed BPM for the next iteration

    Serial.print("BPM: ");
    Serial.println(smoothedBPM);  // Print the smoothed BPM
  }

  if (Signal > Threshold) {                // If the signal is above threshold, turn on the LED
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);     // Else turn off the LED
  }

  delay(10);
}