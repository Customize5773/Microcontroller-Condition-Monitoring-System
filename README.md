# Microcontroller-Condition-Monitoring-System

Reads temperature from an LM35 sensor and digital input status from pin D2, displaying formatted results in the Serial Monitor.

## Overview
This Arduino sketch periodically reads:
1. Analog temperature values from LM35 sensor (connected to A0)
2. Digital input state from pin D2
3. Displays both values in human-readable format every second

## Features
- Temperature conversion to Celsius (°C)
- Digital state detection (HIGH/LOW)
- Automatic serial output formatting
- 1-second polling interval

## How It Works

### 1. Hardware Setup
```arduino
const int tempSensorPin = A0;   // Analog input for LM35
const int digitalInputPin = 2;  // Digital input for D2
```
- Assigns fixed pins for sensor and digital input

### 2. Initialization (`setup()`)
```arduino
void setup() {
  Serial.begin(9600);
  pinMode(tempSensorPin, INPUT);
  pinMode(digitalInputPin, INPUT);
}
```
- Starts serial communication at 9600 baud
- Configures pins as inputs (analog pin mode declaration optional but explicit)

### 3. Main Loop (`loop()`)

#### Temperature Reading
```arduino
int rawValue = analogRead(tempSensorPin);
float temperature = (rawValue * 5.0 / 1023.0) * 100.0;
```
1. Reads raw analog value (0-1023)
2. Converts to voltage: `(rawValue × 5V) / 1024 steps`
3. Converts to Celsius: LM35 outputs 10mV/°C, so `voltage × 100`

#### Digital Input Reading
```arduino
int digitalStatus = digitalRead(digitalInputPin);
```
- Returns HIGH (1) or LOW (0) based on D2 voltage level

#### Serial Output
```arduino
Serial.print("Temperature: ");
Serial.print(temperature, 2);  // 2 decimal places
Serial.print("°C\t|\tD2 Status: ");
Serial.println(digitalStatus == HIGH ? "HIGH" : "LOW");
```
- Formats output with:
  - Fixed labels
  - Temperature precision
  - Tab-separated columns
  - Textual status (HIGH/LOW)

## Wiring Instructions

### LM35 Connections
| LM35 Pin | Arduino Connection |
|----------|---------------------|
| VCC      | 5V                  |
| OUT      | A0                  |
| GND      | GND                 |

### Digital Input (D2)
- Connect external circuit to D2 and GND
- Use INPUT_PULLUP mode for floating pins (change in setup):
  ```arduino
  pinMode(digitalInputPin, INPUT_PULLUP);
  ```

## Usage
1. Upload sketch to Arduino
2. Open Serial Monitor (Ctrl+Shift+M)
3. Set baud rate to 9600
4. View output:
   ```
   Temperature: 23.45°C    |    D2 Status: HIGH
   Temperature: 23.50°C    |    D2 Status: LOW
   ```

## Notes
- LM35 temperature range: 2°C to 150°C
- Default reference voltage: 5V (Arduino UNO)
- Add filtering capacitors if readings are noisy
- Digital input interpretation depends on circuit configuration
- For better temperature stability, consider adding:
  - Multiple sample averaging
  - Software noise filtering
