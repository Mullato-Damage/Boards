#include <Servo.h>


const int photoresistorPin = A0;    // Photoresistor analog input pin
const int ledPins[] = { 3, 5, 6 };  // LED PWM output pins
const int numLeds = 3;              // Number of LEDs


const int trigPin = 2;    // Trig pin of the ultrasonic sensor
const int echoPin = 4;    // Echo pin of the ultrasonic sensor
const int servoPin = 11;  // Pin for the servo motor


Servo servoMotor;


void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoMotor.attach(servoPin);
  servoMotor.write(90);  // Set the initial position of the servo to 90 degrees (center position)


  Serial.begin(9600);  // Initialize serial communication for debugging
}


void loop() {
  //Light level being recorded by photoresistor
  int lightLevel = analogRead(photoresistorPin);
  // Invert the light level before mapping to the brightness range (0 to 255)
  // Need to invert the light level so the LEDs get brighter as the light level goes down.
  int invertedLightLevel = 1023 - lightLevel;
  int brightness = map(invertedLightLevel, 0, 1023, 0, 255);
  // Set the brightness for all LEDs using PWM
  for (int i = 0; i < numLeds; i++) {
    analogWrite(ledPins[i], brightness);
  }


  long duration, distance_cm;
  // Send a pulse to the ultrasonic sensor to trigger the measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  // Measure the duration of the pulse from the ultrasonic sensor
  duration = pulseIn(echoPin, HIGH, 30000);  // Use a timeout of 30ms (30,000 microseconds)
  // Calculate the distance in centimeters (you can use other conversion factors if needed)
  distance_cm = duration * 0.034 / 2;
  // Move the servo based on the measured distance
  if (distance_cm > 20) {  // Rotate the servo to 10 degrees if the distance is greater than 20 cm
    servoMotor.write(10);
  } else if (distance_cm > 5) {  // Rotate the servo to 0 degrees (rightmost position) if the distance is greater than 5 cm
    servoMotor.write(0);
  } else {  // Rotate the servo to 90 degrees (center position) if the distance is less than 5 cm
    servoMotor.write(90);
  }


  Serial.print("Light Level: ");  //The light level that the photoresistor is recording/receiving
  Serial.print(lightLevel);
  Serial.print(", Brightness: ");  //The brightness of the LEDs
  Serial.print(brightness);
  Serial.print(", Distance: ");  //Prints the distance from an object and the Ultrasonic Distance Sensor
  Serial.print(distance_cm);
  Serial.println(" cm");


  delay(500);  // delay that controls how fast the LEDs and servo motor update based on what the distance sensor and photoresistor record.
}