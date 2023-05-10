/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-neopixel-led-strip
 */

#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_NEO_PIXEL 10  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 20
#define NUM_TRAFFIC 6
#define NUM_STARS 14
#define STARS_START 3
#define SERVO_PIN 9

const int TRIGGER = 2;
const int DONE = 12;

const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

int servoStart = 0;
int carWheelsOn = 0;
int end = 0;

unsigned long timer;
unsigned long end_timer;

int TRAFFIC_LIGHTS[NUM_TRAFFIC] = { 0, 1, 2, 17, 18, 19 };

int pos = 0;
int startClock = 0;
int turned = 0;
int ten_degree_turned = 0;
int reset_ten_degree = 0;
unsigned long ten_degree;

int thirty_degree_turned = 0;
int reset_thirty_degree = 0;
unsigned long thirty_degree;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Servo myservo;

void setup() {

  // START and END SIGNAL
  pinMode(TRIGGER, INPUT);
  pinMode(DONE, OUTPUT);

  // CAR WHEELS
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  NeoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  randomSeed(analogRead(0));
  timer = millis();
  ten_degree = millis();
  thirty_degree = millis();
  Serial.begin(9600);
  stars();
  myservo.write(0);
  trafficLightsOff();
}

void loop() {

  unsigned long current = millis();
  if(end && current - end_timer < 200){
      digitalWrite(DONE, 1);
  }else{
    end = 0;
    digitalWrite(DONE, 0);
  }

  int inputVal = digitalRead(TRIGGER);
  Serial.println(inputVal);
  if (inputVal == 1){
    Serial.println("Trigger");

    if (!startClock){
      if(pos == 0){
          myservo.write(120);
          pos = 120;
        }
        else{
          myservo.write(0);
          pos = 0;
        }
    }

    startClock = 1;
  }

  if (startClock){
    unsigned long currentTime = millis();
    if (currentTime - timer > 300000) {
      timer = currentTime;
      ten_degree = currentTime;
      thirty_degree = currentTime;
      carWheelsOn = 0;

      Serial.println("turning 180");

      // turn hourglass
      if (pos == 120) {
        myservo.write(0);
        pos = 0;
        // send signal to end
      } else {
        myservo.write(120);
        // turned = 1;
        pos = 120;
      }

      startClock = 0;
      end = 1;
      end_timer = millis();

      stars();
      trafficLightsOff();
    }

    // Serial.println(carWheelsOn);
    if (carWheelsOn){
      carWheels();
    }else{
      carWheelsOff();
    }


    // turn hourglass 10 degress left and right at 3 mins and dim lights
    currentTime = millis();
    if (currentTime - ten_degree > 180000) {
      // Serial.println("turning 10");
      ten_degree = currentTime;
      ten_degree_turned = 1;

      dimStars();
      trafficLights();
      carWheelsOn = 1;

      pos += 20;
      myservo.write(pos);
    }

    currentTime = millis();
    if (ten_degree_turned && (currentTime - ten_degree > 10000)) {
      // Serial.println("turning back 10");
      ten_degree = currentTime;
      ten_degree_turned = 0;
      reset_ten_degree = 1;
      pos -= 40;
      myservo.write(pos);
    }

    currentTime = millis();
    if (reset_ten_degree && (currentTime - ten_degree > 10000)) {
      // Serial.println("resting 10");
      reset_ten_degree = 0;
      pos += 20;
      myservo.write(pos);
    }

  }
  
}

/////// CAR WHEELS//////////
void carWheels(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carWheelsOff(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stars(){
   // Stars
      for (int pixel = STARS_START; pixel < NUM_STARS + STARS_START; pixel++) { // for each pixel
          NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 255, 255)); // it only takes effect if pixels.show() is called
          NeoPixel.show();
      }
}

void dimStars(){
   // Stars
      for (int pixel = STARS_START; pixel < NUM_STARS + STARS_START; pixel++) { // for each pixel
          NeoPixel.setPixelColor(pixel, NeoPixel.Color(25, 25, 25)); // it only takes effect if pixels.show() is called
          NeoPixel.show();
      }
}

void trafficLights(){
   // Traffic lights
    for (int pixel = 0; pixel < NUM_TRAFFIC; pixel++) { // for each pixel
      NeoPixel.setPixelColor(TRAFFIC_LIGHTS[pixel], NeoPixel.Color(0, 255, 0)); // it only takes effect if pixels.show() is called
    }
    NeoPixel.show();
  }

void trafficLightsOff(){
   // Traffic lights
    for (int pixel = 0; pixel < NUM_TRAFFIC; pixel++) { // for each pixel
      NeoPixel.setPixelColor(TRAFFIC_LIGHTS[pixel], NeoPixel.Color(255, 0, 0)); // it only takes effect if pixels.show() is called
    }
    NeoPixel.show();
  }
