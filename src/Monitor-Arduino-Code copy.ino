// Temperature and digital input monitor

const int TEMP_SENSOR_PIN = A0;
const int DIGITAL_INPUT_PIN = 2;

// Sampling interval (ms)
const unsigned long SAMPLE_INTERVAL = 1000;

// Averaging buffer
const int NUM_TEMP_SAMPLES = 5;
float tempReadings[NUM_TEMP_SAMPLES];
int tempReadingIndex = 0;

// Debounce settings
const unsigned long DEBOUNCE_DELAY = 50;
int lastDigitalState = LOW;
int digitalState = LOW;
unsigned long lastDebounceTime = 0;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial (for native USB)
  
  pinMode(DIGITAL_INPUT_PIN, INPUT); // use INPUT_PULLUP if switching to ground

  for (int i = 0; i < NUM_TEMP_SAMPLES; i++) {
    tempReadings[i] = 0;
  }

  Serial.println("=== Temperature & Digital Input Monitor ===");
  Serial.println("Temperature Sensor: LM35 on A0");
  Serial.println("Digital Input: Pin D2");
  Serial.println("========================================\n");
}

void loop() {
  unsigned long currentMillis = millis();
  debounceDigitalInput();

  if (currentMillis - previousMillis >= SAMPLE_INTERVAL) {
    previousMillis = currentMillis;
    float temperature = readTemperatureAverage();
    displayReadings(temperature, digitalState);
    checkTemperatureAlerts(temperature);
  }
}

// Temperature functions
float readTemperatureAverage() {
  int rawValue = analogRead(TEMP_SENSOR_PIN);
  float currentTemp = convertToTemperature(rawValue);

  tempReadings[tempReadingIndex] = currentTemp;
  tempReadingIndex = (tempReadingIndex + 1) % NUM_TEMP_SAMPLES;

  float sum = 0;
  for (int i = 0; i < NUM_TEMP_SAMPLES; i++) sum += tempReadings[i];

  return sum / NUM_TEMP_SAMPLES;
}

// LM35: 10mV per °C (5V ref, 10-bit ADC)
float convertToTemperature(int rawValue) {
  float voltage = rawValue * (5.0 / 1023.0);
  return voltage * 100.0;
}

void checkTemperatureAlerts(float temp) {
  if (temp > 50.0) {
    Serial.println("⚠️  WARNING: High temperature detected!");
  } else if (temp < 0.0) {
    Serial.println("⚠️  WARNING: Temperature below freezing!");
  }
}

// Debounce digital input
void debounceDigitalInput() {
  int reading = digitalRead(DIGITAL_INPUT_PIN);

  if (reading != lastDigitalState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != digitalState) {
      digitalState = reading;
      // state change handling can go here
    }
  }

  lastDigitalState = reading;
}

void displayReadings(float temperature, int digState) {
  Serial.print("Temp: ");
  Serial.print(temperature, 2);
  Serial.print(" °C");

  Serial.print("  [");
  int barLength = map(constrain(temperature, 0, 50), 0, 50, 0, 20);
  for (int i = 0; i < barLength; i++) Serial.print("=");
  for (int i = barLength; i < 20; i++) Serial.print(" ");
  Serial.print("]");

  Serial.print("  |  D2: ");
  Serial.print(digState == HIGH ? "HIGH" : "LOW ");

  Serial.print("  |  Time: ");
  Serial.print(millis() / 1000);
  Serial.println("s");
}

// CSV format for logging
void displayReadingsCSV(float temperature, int digState) {
  Serial.print(millis());
  Serial.print(",");
  Serial.print(temperature, 2);
  Serial.print(",");
  Serial.println(digState);
}
