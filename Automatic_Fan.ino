#include <DHT.h>

#define DHTPIN 2       // Pin connected to the DHT11 data pin
#define DHTTYPE DHT11  // DHT11 or DHT22, change to DHT22 if you're using DHT22 sensor

DHT dht(DHTPIN, DHTTYPE);

#define MOTOR_PIN 9  // Pin connected to the transistor base to control the motor fan

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  // Read temperature and humidity from the DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display temperature and humidity
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Control the motor fan based on temperature
  if (temperature > 25) {
    digitalWrite(MOTOR_PIN, HIGH);  // Turn ON the fan if temperature is greater than 25°C
  } else {
    digitalWrite(MOTOR_PIN, LOW);  // Turn OFF the fan if temperature is 25°C or lower
  }

  delay(2000);  // Wait for 2 seconds before taking another reading
}