const int tempSensorPin = A0;  // LM35 connected to analog pin A0
const int digitalInputPin = 2; // Digital input connected to pin D2

void setup() {
  Serial.begin(9600);          // Initialize serial communication
  pinMode(tempSensorPin, INPUT); // Analog pin automatically in input mode
  pinMode(digitalInputPin, INPUT); // Set D2 as input (add INPUT_PULLUP if needed)
}

void loop() {
  // Read temperature from LM35
  int rawValue = analogRead(tempSensorPin);
  float temperature = (rawValue * 5.0 / 1023.0) * 100.0; // Convert to Celsius
  
  // Read digital pin status
  int digitalStatus = digitalRead(digitalInputPin);
  
  // Display results in Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); // Show two decimal places
  Serial.print("°C\t|\tD2 Status: ");
  Serial.println(digitalStatus == HIGH ? "HIGH" : "LOW");
  
  delay(1000); // Wait for 1 second before next reading
}
