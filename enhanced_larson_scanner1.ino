// C++ code
//
//Pin array that specifies which pins I'm using,
//instead of requiring me to specify which pin everytime
int pinArray[] = {6,7,8,9,10,11};
//count is used in the for loop and 0 is specified so that
//the program knows to start at 0 and work up.
int count = 0;
//timer is specified so that the program always knows to do 50
//mili, in the delay section, instead of me putting 50 everytime
int timer = 50;

void setup()
{
  //as explained above, the for loop tells the program to start
  // from 0 and work up to the sixth pin.
  for (count=0;count<6;count ++) {
    //this assures that Void Loop understands how to read the 
    //the for loop, where it gives the pin array and the output. 
  pinMode(pinArray[count], OUTPUT);
	}
}
void loop()
{
  //This for loop counts up, from 0 to 6 
  for (count=0;count<6;count++) {
    //High means that the light will turn on
    digitalWrite(pinArray[count], HIGH);
    //delay tells the program how long the light should be on
    delay(timer);
    //LOW means the light will turn off
    digitalWrite(pinArray[count], LOW);
    //tells the program how long until the next light should turn on
    delay(timer);
  }
  //This for loop counts down, from pins 6 to 0
  for (count=5;count>=0;count--) {
  	digitalWrite(pinArray[count], HIGH);
    delay(timer);
    digitalWrite(pinArray[count], LOW);
    delay(timer);
  }
}
