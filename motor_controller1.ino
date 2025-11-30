// Motor control pins
const int motor1A = 2;  // Motor 1 input pin A
const int motor1B = 3;  // Motor 1 input pin B
const int motor2A = 4;  // Motor 2 input pin A
const int motor2B = 5;  // Motor 2 input pin B

// Encoder pins
const int encoder1A = 6;  // Encoder 1 output A pin
const int encoder1B = 7;  // Encoder 1 output B pin
const int encoder2A = 8;  // Encoder 2 output A pin
const int encoder2B = 9;  // Encoder 2 output B pin

// Speed control (enabler) pins
const int enabler1 = 10;  // Motor 1 enabler pin
const int enabler2 = 11;  // Motor 2 enabler pin

// Encoder pulse count variables
volatile long encoderCount1 = 0;
volatile long encoderCount2 = 0;

// Interrupt service routines for encoder pulses
void encoderInterrupt1A() {
  if (digitalRead(encoder1A) == digitalRead(encoder1B)) {
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

void encoderInterrupt1B() {
  if (digitalRead(encoder1A) != digitalRead(encoder1B)) {
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

void encoderInterrupt2A() {
  if (digitalRead(encoder2A) == digitalRead(encoder2B)) {
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}

void encoderInterrupt2B() {
  if (digitalRead(encoder2A) != digitalRead(encoder2B)) {
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}

// Motor control functions
void driveRobotForward() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void driveRobotReverse() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void stopRobot() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
}

void turnRobotLeft() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void turnRobotRight() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void setSpeed(int speed) {
  analogWrite(enabler1, speed);  // Motor 1 enabler pin
  analogWrite(enabler2, speed);  // Motor 2 enabler pin
}

// Serial communication function to print encoder counts
void printEncoderCounts() {
  Serial.print("Encoder Count 1: ");
  Serial.print(encoderCount1);
  Serial.print(", Encoder Count 2: ");
  Serial.println(encoderCount2);
}

void setup() {
  // Initialize motor control pins as outputs
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  // Initialize encoder input pins as inputs
  pinMode(encoder1A, INPUT);
  pinMode(encoder1B, INPUT);
  pinMode(encoder2A, INPUT);
  pinMode(encoder2B, INPUT);

  // Initialize enabler pins as outputs
  pinMode(enabler1, OUTPUT);
  pinMode(enabler2, OUTPUT);

  // Attach interrupt service routines to encoder input pins
  attachInterrupt(digitalPinToInterrupt(encoder1A), encoderInterrupt1A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder1B), encoderInterrupt1B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2A), encoderInterrupt2A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2B), encoderInterrupt2B, CHANGE);

  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  // Drive the robot forward for 2 seconds, then stop
  driveRobotForward();
  setSpeed(255); // Full speed
  delay(2000);
  stopRobot();
  delay(1000);

  // Reverse the robot for 1 second, then stop
  driveRobotReverse();
  setSpeed(255); // Full speed
  delay(1000);
  stopRobot();
  delay(1000);

  // Turn the robot left for 1 second, then stop
  turnRobotLeft();
  setSpeed(200); // Moderate speed
  delay(1000);
  stopRobot();
  delay(1000);

  // Turn the robot right for 1 second, then stop
  turnRobotRight();
  setSpeed(200); // Moderate speed
  delay(1000);
  stopRobot();
  delay(1000);

  // Print encoder counts
  printEncoderCounts();
}
