// Pin connected to the relay module's control pin (e.g., IN)
const int relayPin = 7;

// Soil Moisture Sensor Calibration Values
// These values depend on your specific sensor and soil.
// You should measure the sensor's analog output when:
// - The sensor is completely dry (or in air) -> This is 'dry'
// - The sensor is submerged in water (or very wet soil) -> This is 'wet'
const int dry = 465; // Higher analog value = drier soil
const int wet = 199; // Lower analog value = wetter soil

// Analog pin the soil moisture sensor is connected to
const int sensorPin = A0;

// Watering Threshold
// Water will be dispensed when humidity percentage drops BELOW this value.
const int wateringThreshold = 10; // Turn water ON below 10% humidity

void setup() {
  // Initialize serial communication for monitoring
  Serial.begin(9600);
  Serial.println("Soil Moisture Watering System Initializing...");

  // Set the relay pin as an output
  pinMode(relayPin, OUTPUT);

  // Set the initial state of the relay to OFF.
  // Most common relay modules are ACTIVE LOW, meaning HIGH turns them OFF.
  // If your relay is ACTIVE HIGH, you would initialize this to LOW.
  digitalWrite(relayPin, HIGH); // Start with the water OFF
  Serial.println("Relay Initialized: Water OFF");
}

void loop() {
  // Read the raw analog value from the soil moisture sensor
  int sensorVal = analogRead(sensorPin);

  // Map the raw sensor value to a percentage humidity
  // map(value, fromLow, fromHigh, toLow, toHigh)
  // Here: wet (low sensor value) maps to 100%, dry (high sensor value) maps to 0%
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);

  // Constrain the percentage to be between 0 and 100, just in case
  // sensor readings go slightly out of the calibrated wet/dry range.
  percentageHumidity = constrain(percentageHumidity, 0, 100);

  // Print the current humidity percentage to the Serial Monitor
  Serial.print("Soil Humidity: ");
  Serial.print(percentageHumidity);
  Serial.println("%");

  // Check if the humidity is below the watering threshold
  if (percentageHumidity < wateringThreshold) {
    // Soil is DRY - Turn the water ON
    Serial.println("Soil is DRY. Turning water ON.");
    // Set relay pin LOW to activate the relay (assuming Active LOW module)
    digitalWrite(relayPin, LOW);
  } else {
    // Soil is WET enough - Turn the water OFF
    Serial.println("Soil is sufficiently moist. Turning water OFF.");
    // Set relay pin HIGH to deactivate the relay (assuming Active LOW module)
    digitalWrite(relayPin, HIGH);
  }

  // Wait for a period before the next reading.
  // Adjust this delay as needed. A longer delay prevents rapid pump cycling.
  // 1000 milliseconds = 1 second
  delay(1000);
}