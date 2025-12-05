#include <Adafruit_NeoPixel.h>

#define LED_PIN     5 // The pin where the data line is connected
#define LED_COUNT  61 // Number of LEDs in the strip

// Initialize the NeoPixel library.
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int checkRotPin = 2;
const int pulsePin = 3;

void setup() {
  Serial.begin(9600);

  // Pullup mode for pins so voltage is not floating when closed.
  pinMode(checkRotPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);
  
  Serial.println("Init done");

  strip.begin();
  strip.show();

  for(int i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();

}

void loop() {
  // put your main code here, to run repeatedly:

}
