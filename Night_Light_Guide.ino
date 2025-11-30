// Pins 
const int photoresistorPin = A0;
const int ledPins[] = {3, 5, 6};
const int numLeds = 3;

const int trigPin = 2;
const int echoPin = 4;

const int LeftMotors  = 10;   // MOSFET gate (left side)
const int RightMotors = 9;    // MOSFET gate (right side)

const int SystemStart = 11;   // switch to GND = ON  (INPUT_PULLUP)

// Tuning 
const int baseSpeed      = 200;         // 0–255
const int turnSpeed      = 200;         // 0–255
const int stopDistanceCm = 20;          // avoid threshold
const unsigned long echoTimeoutUs = 30000UL;

const unsigned long pivotTimeMs   = 450; // how long to pivot when blocked
const unsigned long loopDtMs      = 25;  // loop pacing

// LDR threshold: adjust after you look at serial values.
// Higher threshold = needs to be darker before it moves.
const int darkThreshold = 800;          // light level that determines if the system powers on or not

// State for non-blocking pivot 
bool pivoting = false;
unsigned long pivotStart = 0;

// Helpers 
void motorsStop() {
  analogWrite(LeftMotors,  0);
  analogWrite(RightMotors, 0);
}

void motorsForward(int spd) { // both pairs power on, moving forward.
  analogWrite(LeftMotors,  constrain(spd, 0, 255));
  analogWrite(RightMotors, constrain(spd, 0, 255));
}

void motorsPivotLeft(int spd) {  // right ON, left OFF
  analogWrite(LeftMotors,  0);
  analogWrite(RightMotors, constrain(spd, 0, 255));
}

long readDistanceCm() { // how the distance sensor reads the data and relays it
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long dur = pulseIn(echoPin, HIGH, echoTimeoutUs);
  if (dur == 0) return -1;                 // -1 = no reading
  return (long)(dur * 0.034 / 2.0);
}

void setAllLeds(int value) {
  for (int i = 0; i < numLeds; i++) {
    analogWrite(ledPins[i], value);
  }
}

// Setup 
void setup() {
  for (int i = 0; i < numLeds; i++) pinMode(ledPins[i], OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(LeftMotors,  OUTPUT);
  pinMode(RightMotors, OUTPUT);
  motorsStop();

  pinMode(SystemStart, INPUT_PULLUP);

  Serial.begin(9600);
}

// Loop 
void loop() {
  bool enabled = (digitalRead(SystemStart) == LOW); // LOW = switch ON

  if (!enabled) {
    // Full system OFF (drive + LEDs)
    motorsStop();
    setAllLeds(0);
    pivoting = false;
    delay(loopDtMs);
    return;
  }

  // Read ambient light
  int lightLevel = analogRead(photoresistorPin);

  // Decide if it's dark enough to run the guide-bot
  bool darkEnough = (lightLevel < darkThreshold);

  if (!darkEnough) {
    // Room is bright: no guide needed; everything idle
    motorsStop();
    setAllLeds(0);

    // Debug
    Serial.print("EN= 1  (bright, idle)  LDR= ");
    Serial.print(lightLevel);
    Serial.print("  LED= 0  Dist= N/A");
    Serial.println();

    delay(250);
    return;  // skip obstacle-avoidance logic
  }

  // DARK: guide-bot active
  int brightness = map(1023 - lightLevel, 0, 1023, 0, 255);
  setAllLeds(brightness);

  long dcm = readDistanceCm();

  // Control logic
  if (!pivoting) {
    if (dcm > 0 && dcm < stopDistanceCm) {
      // obstacle; start a timed pivot
      motorsStop();
      delay(80);     // brief pause for stability
      motorsPivotLeft(turnSpeed);
      pivoting = true;
      pivotStart = millis();
    } else {
      // clear or no reading
      motorsForward(baseSpeed);
    }
  } else {
    // currently pivoting; stop after pivotTimeMs
    if (millis() - pivotStart >= pivotTimeMs) {
      pivoting = false;
      motorsStop();
      delay(40);
    }
  }

  // Debug
  Serial.print("EN= 1  (dark, active)  LDR= ");
  Serial.print(lightLevel);
  Serial.print("  LED= ");
  Serial.print(brightness);
  Serial.print("  Dist= ");
  Serial.print(dcm);
  Serial.println(" cm");

  delay(250);
}
