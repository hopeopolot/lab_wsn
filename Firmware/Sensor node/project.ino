#include <Wire.h>
#include "Adafruit_SHT31.h"

// ---------- Pins ----------
const int PIR_PIN    = 27;
const int LDR_PIN    = 34;
const int LED_PIN    = 26;
const int BUZZER_PIN = 25;

// ---------- Settings ----------
const float HIGH_TEMP = 30.0;            // C: LED solid above this
const float LOW_TEMP  = 18.0;            // C: LED blinks below this
const unsigned long BUZZ_TIME   = 2000;  // ms buzzer stays on after motion
const unsigned long PRINT_EVERY = 1000;  // ms
const unsigned long BLINK_EVERY = 400;   // ms

// ---------- Calibration settings ----------
const float REF_TEMP    = 28.0;   // reference thermometer reading (C) used at boot
const int   CAL_SAMPLES = 50;     // number of readings averaged
const int   CAL_DELAY   = 200;    // ms between calibration readings
const unsigned long SETTLE_TIME = 10000;  // ms to let the sensor settle before calibrating
float tempOffset = 0.0;           // correction added to every reading
bool  calibrated = false;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

float temperature = NAN, humidity = NAN;   // temperature is the CALIBRATED value
unsigned long lastPrint = 0, lastBlink = 0, buzzStart = 0;
bool buzzing = false, ledBlinkState = false;
bool motionSeen = false;   // remembers any motion since the last print

// ---------- Calibration function ----------
// Averages many readings, compares to the reference thermometer,
// and stores the difference as tempOffset.
void calibrateTemperature(float refTemp) {
  Serial.println();
  Serial.println("Calibrating... keep the sensor next to the reference thermometer.");
  Serial.printf("Letting sensor settle for %lu s...\n", SETTLE_TIME / 1000);
  delay(SETTLE_TIME);

  float sum = 0, minT = 1000, maxT = -1000;
  int count = 0;

  for (int i = 0; i < CAL_SAMPLES; i++) {
    float t = sht31.readTemperature();
    if (!isnan(t)) {
      sum += t;
      count++;
      if (t < minT) minT = t;
      if (t > maxT) maxT = t;
    }
    delay(CAL_DELAY);
  }

  if (count == 0) {
    Serial.println("Calibration FAILED: no valid readings.");
    return;
  }

  float avg = sum / count;
  tempOffset = refTemp - avg;
  calibrated = true;

  Serial.println("----- CALIBRATION RESULT -----");
  Serial.printf("Reference temp : %.2f C\n", refTemp);
  Serial.printf("Sensor average : %.2f C (%d samples)\n", avg, count);
  Serial.printf("Sensor min/max : %.2f / %.2f C (spread %.2f C)\n", minT, maxT, maxT - minT);
  Serial.printf("Offset applied : %+.2f C\n", tempOffset);
  Serial.printf("Check          : %.2f + (%+.2f) = %.2f C\n", avg, tempOffset, avg + tempOffset);
  Serial.println("------------------------------");
  Serial.println("Tip: type a new reference temp (e.g. 27.5) in the Serial Monitor to recalibrate.");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1500);                 // lets the Serial Monitor connect
  Serial.println("Starting...");

  Wire.begin(21, 22);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  analogReadResolution(12);

  // SHT31: try both possible addresses
  if (!sht31.begin(0x44)) {
    if (!sht31.begin(0x45)) {
      Serial.println("SHT31 not found. Check SDA/SCL wiring and power.");
      while (1) delay(1000);
    }
  }
  Serial.println("SHT31 OK");

  // Calibrate against the reference thermometer
  calibrateTemperature(REF_TEMP);

  Serial.println("Warming up PIR (30 s)...");
  delay(30000);
  Serial.println("System ready.");
}

void loop() {
  unsigned long now = millis();

  // ----- Optional: recalibrate by typing the reference temperature -----
  if (Serial.available()) {
    float ref = Serial.parseFloat();
    while (Serial.available()) Serial.read();   // clear leftovers
    if (ref > 0 && ref < 60) {
      calibrateTemperature(ref);
    } else {
      Serial.println("Enter a reference temperature between 0 and 60, e.g. 28.5");
    }
  }

  // ----- Read PIR once per loop -----
  bool motion = (digitalRead(PIR_PIN) == HIGH);
  if (motion) motionSeen = true;   // catches short pulses between prints

  // ----- Motion -> buzzer -----
  if (motion) {
    buzzing = true;
    buzzStart = now;            // restarts the timer while motion continues
  }
  if (buzzing) {
    digitalWrite(BUZZER_PIN, HIGH);
    if (now - buzzStart >= BUZZ_TIME) {
      buzzing = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  // ----- Read sensors and print every second -----
  if (now - lastPrint >= PRINT_EVERY) {
    lastPrint = now;

    float rawT = sht31.readTemperature();
    float h = sht31.readHumidity();
    if (!isnan(rawT)) temperature = rawT + tempOffset;   // calibrated value
    if (!isnan(h)) humidity = h;

    int light = analogRead(LDR_PIN);

    Serial.printf("Raw: %.1f C | Calibrated: %.1f C | Hum: %.1f %% | Light: %d | Motion: %s\n",
                  rawT, temperature, humidity, light, motionSeen ? "YES" : "no");
    motionSeen = false;         // reset after printing
  }

  // ----- LED temperature indicator (uses calibrated temperature) -----
  if (!isnan(temperature)) {
    if (temperature > HIGH_TEMP) {
      digitalWrite(LED_PIN, HIGH);               // too hot: solid
    } else if (temperature < LOW_TEMP) {
      if (now - lastBlink >= BLINK_EVERY) {      // too cold: blink
        lastBlink = now;
        ledBlinkState = !ledBlinkState;
        digitalWrite(LED_PIN, ledBlinkState);
      }
    } else {
      digitalWrite(LED_PIN, LOW);                // normal: off
    }
  }
}