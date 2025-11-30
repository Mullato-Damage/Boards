#include <Servo.h>

const int trigPin = 2;    // Trig pin of the ultrasonic sensor
const int echoPin = 3;    // Echo pin of the ultrasonic sensor
const int servoPin = 11;  // Pin for the servo motor

Servo servoMotor;

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoMotor.attach(servoPin);
  servoMotor.write(90);  // Set the initial position of the servo to 90 degrees (center position)
}

void loop() {
  long duration, distance_cm;

  // Send a pulse to the ultrasonic sensor to trigger the measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse from the ultrasonic sensor
  duration = pulseIn(echoPin, HIGH, 30000);  // Use a timeout of 30ms (30,000 microseconds)

  // Calculate the distance in centimeters (you can use other conversion factors if needed)
  distance_cm = duration * 0.034 / 2;

  // Print the distance for debugging
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  // Move the servo based on the measured distance
  // Adjust the following values according to your needs
  if (distance_cm > 20) {
    servoMotor.write(0);  // Rotate the servo to 0 degrees (rightmost position) if the distance is greater than 20 cm
  } else if (distance_cm > 5) {
    servoMotor.write(90);  // Rotate the servo to 90 degrees (center position) if the distance is between 5 and 20 cm
  } else {
    servoMotor.write(180);  // Rotate the servo to 180 degrees (leftmost position) if the distance is less than 5 cm
  }

  delay(500);  // Add a small delay to prevent rapid and continuous servo movements
