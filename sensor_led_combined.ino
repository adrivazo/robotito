/* HC-SR04 Sensor
   https://www.dealextreme.com/p/hc-sr04-ultrasonic-sensor-distance-measuring-module-133696
   This sketch reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.
   The circuit:
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 2
	* ECHO connection of the sensor attached to digital pin 4
   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius
   This code is based on https://gist.github.com/flakas/3294829
   This example code is in the public domain.
 */

#define publish_delay 30000 // public delay for motion
#define publish_delay_button 2000
unsigned int lastPublish = 0;
unsigned int lastPublishButton = 0;

const int trigPin = 2;
const int echoPin = 4;
//const int greenPin = 5;
//const int redPin = 6;
const int button = 6;

const int led1 = A0;
const int led2 = A1;
const int led3 = A2;
const int led4 = A3;


int lastLedStatus = 0;//0 for off, 1 for on


// -----------------------------------
// Controlling LEDs over the Internet
// -----------------------------------

// First, let's create our "shorthand" for the pins
// Same as in the Blink an LED example:
// led1 is D0, led2 is D7

// int led1 = D0;
 int led7 = D7;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
   //usr sensor status leds 
   // pinMode(greenPin, OUTPUT);
 // pinMode(redPin, OUTPUT);
 
    //http leds
   // pinMode(led1, OUTPUT);
   
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
   pinMode(led4, OUTPUT);

    pinMode(led7, OUTPUT);
    
    pinMode(button, INPUT);
    
    // We are also going to declare a Spark.function so that we can turn the LED on and off from the cloud.
   Spark.function("led",ledToggle);
   // This is saying that when we ask the cloud for the function "led", it will employ the function ledToggle() from this app.

   // For good measure, let's also make sure both LEDs are off when we start:
   //digitalWrite(led1, LOW);
   digitalWrite(led7, LOW);
   
   Spark.function("slackOff",speakToSlack);
}

void loop()
{
   unsigned long now = millis();
  
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
  

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance

  cm = microsecondsToCentimeters(duration);
  if (cm < 10 ){//if close enough and more than the desired delay has occured
  
    //check if it's new motion!
    if( (now - lastPublish) > publish_delay){
    
    // digitalWrite(greenPin, LOW);
    // digitalWrite(redPin, HIGH);
    
    speakToSlack("move");
    ledToggle("toggle");
    lastPublish = now;
    }
    
  } else {
    // digitalWrite(redPin, LOW);
    // digitalWrite(greenPin, HIGH);
  }
  Serial.println("V 01");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  if(digitalRead(button)==HIGH && (now - lastPublishButton) > publish_delay_button){
      speakToSlack("laugh");
      lastPublishButton = now;
  }

  delay(100);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

/*
 * pulseIn Function for the Spark Core - Version 0.1.1 (Beta)
 * Copyright (2014) Timothy Brown - See: LICENSE
 *
 * Due to the current timeout issues with Spark Cloud
 * this will return after 10 seconds, even if the
 * input pulse hasn't finished.
 *
 * Input: Trigger Pin, Trigger State
 * Output: Pulse Length in Microseconds (10uS to 10S)
 *
 */

unsigned long pulseIn(uint16_t pin, uint8_t state) {

    GPIO_TypeDef* portMask = (PIN_MAP[pin].gpio_peripheral); // Cache the target's peripheral mask to speed up the loops.
    uint16_t pinMask = (PIN_MAP[pin].gpio_pin); // Cache the target's GPIO pin mask to speed up the loops.
    unsigned long pulseCount = 0; // Initialize the pulseCount variable now to save time.
    unsigned long loopCount = 0; // Initialize the loopCount variable now to save time.
    unsigned long loopMax = 20000000; // Roughly just under 10 seconds timeout to maintain the Spark Cloud connection.

    // Wait for the pin to enter target state while keeping track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) != state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
    }

    // Iterate the pulseCount variable each time through the loop to measure the pulse length; we also still keep track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) == state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
        pulseCount++;
    }

    // Return the pulse time in microseconds by multiplying the pulseCount variable with the time it takes to run once through the loop.
    return pulseCount * 0.405; // Calculated the pulseCount++ loop to be about 0.405uS in length.
}



int ledToggle(String command) {
    /* Spark.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */

    if (command=="on") {
        //digitalWrite(led1,HIGH);
        digitalWrite(led7,HIGH);
        lastLedStatus = 1;
        return 1;
    }
    else if (command=="off") {
        //digitalWrite(led2,LOW);
        digitalWrite(led7,LOW);
        lastLedStatus = 0;
        return 0;
    }
    else if (command=="toggle"){
        switch(lastLedStatus){
        case 0:
           // digitalWrite(led1,HIGH);
            digitalWrite(led7,HIGH);
            lastLedStatus = 1;
            return 1;
        case 1:
            //digitalWrite(led1,LOW);
            digitalWrite(led7,LOW);
            lastLedStatus = 0;
            return 0;
        default:
            return -1;
            break;
        }
    }
    else if (command=="speak") {
        speakToSlack("speak now");
        return ledToggle("toggle");
    }
    
    else {
        return -1;
    }
}

int speakToSlack(String command) {
        Spark.publish("hislackbot_", command, 60, PRIVATE);
        circle_bl();
        return 1;
}

void circle_bl() {
   // for (int i = 0; i < 2; i++) { 
    digitalWrite(led1, HIGH);
    delay(125);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    delay(125);
    digitalWrite(led3, HIGH);
    digitalWrite(led2, LOW);
    delay(125);
    digitalWrite(led4, HIGH);
    digitalWrite(led3, LOW);
    delay(125);
    digitalWrite(led4, LOW);
   // }
}