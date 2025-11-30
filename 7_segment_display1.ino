// C++ code
// 0000 = 0 B11101110, // 0
// 0001 = 1 B10000010, // 1
// 0010 = 2 B11011100, // 2
// 0011 = 3 B11010110, // 3
// 0100 = 4 B10110010, // 4
// 0101 = 5 B01110110, // 5
// 0110 = 6 B01111110, // 6
// 0111 = 7 B11000010, // 7
// 1000 = 8 B11111110, // 8
// 1001 = 9 B11110110  // 9
const int buttonPin1 = 2; // Button connected to digital pin 2
const int buttonPin2 = 3; // Button connected to digital pin 3
const int buttonPin3 = 4; // Button connected to digital pin 4
const int buttonPin4 = 5; // Button connected to digital pin 5

const int segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // Array of segment pins
const byte digitPatterns[] = {
  B11101110, // 0
  B10000010, // 1
  B11011100, // 2
  B11010110, // 3
  B10110010, // 4
  B01110110, // 5
  B01111110, // 6
  B11000010, // 7
  B11111110, // 8
  B11110110  // 9
}; // Array of digit patterns for hexadecimal values 0-9

bool buttonStates[4] = {false}; // Array to store the state of the buttons
int number = 0; // Variable to store the combined 4-bit number

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT); // Set segment pins as outputs
  }

  pinMode(buttonPin1, INPUT_PULLUP); // Set button pins as inputs with internal pull-up resistors
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  Serial.begin(9600); // Initialize serial communication
}

void displayHex(byte digit) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], bitRead(digitPatterns[digit], i)); // Set segment pin based on the digit pattern
  }
}

void combineBits() {
  number = 0; // Reset the number

  // Combine the bits to form the number
  number |= buttonStates[3] << 3;
  number |= buttonStates[2] << 2;
  number |= buttonStates[1] << 1;
  number |= buttonStates[0];
}

void loop() {
  buttonStates[0] = digitalRead(buttonPin1); // Read the state of button 1
  buttonStates[1] = digitalRead(buttonPin2); // Read the state of button 2
  buttonStates[2] = digitalRead(buttonPin3); // Read the state of button 3
  buttonStates[3] = digitalRead(buttonPin4); // Read the state of button 4

  combineBits(); // Combine the button states into the 4-bit number

  displayHex(number); // Display the corresponding hexadecimal digit on the 7-segment display

  Serial.println(number); // Broadcast the number on the serial port for troubleshooting

  delay(100); // Add a small delay for stability
}
