#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address 0x27 and 16x2 size
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define sensor A0
#define Highpulse 540

int Svalue = 0;
int count = 0;
unsigned long lastSampleTime = 0;
unsigned long samplingInterval = 100; // Start with 100 ms (10 Hz sampling rate)

long Stime = 0;
long Ltime = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on the backlight
  lcd.clear();       // Clear any existing content on the LCD
}

void loop() {
  unsigned long currentTime = millis();

  // Read data and update Serial Plotter at specified intervals
  if (currentTime - lastSampleTime >= samplingInterval) {
    lastSampleTime = currentTime;

    Svalue = analogRead(sensor);
    Serial.println(Svalue); // Print to Serial Plotter

    // Dynamically adjust sampling interval
    if (currentTime > 5000 && currentTime <= 10000) {
      samplingInterval = 50; // Faster sampling after 5 seconds (20 Hz)
    } else if (currentTime > 10000) {
      samplingInterval = 200; // Slower sampling after 10 seconds (5 Hz)
    }

    // Update LCD with BPM
    BPM();
    lcd.setCursor(0, 0);
    lcd.print("BPM: ");
    lcd.setCursor(5, 0);
    lcd.print(count); // Show the BPM count
  }
}

void BPM() {
  if (Svalue > Highpulse) {
    Stime = millis() - Ltime;
    count++;

    if (Stime / 1000 >= 60) {
      Ltime = millis();
      Serial.println(count);

      // Update the BPM count on the LCD
      lcd.setCursor(5, 0);
      lcd.print(count);
      count = 0;
    }
  }
}
