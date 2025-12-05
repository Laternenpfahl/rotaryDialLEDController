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

  int pulseState = digitalRead(pulsePin);
  int checkRot = digitalRead(checkRotPin);
  int flip = 0;
  int count = 0;

  if (pulseState && !checkRot)
  {
    for(int i=0; i<strip.numPixels(); i++)
    {

      pulseState = digitalRead(pulsePin);
      checkRot = digitalRead(checkRotPin);

      Serial.print("Pulse: ");
      Serial.print(pulseState);      // LOW = contact closed, HIGH = open
      Serial.print(" | CheckRot: ");
      Serial.println(checkRot);

      if(pulseState && !checkRot)
      {
      strip.setPixelColor(i, strip.Color(180,0,0));
      strip.show();
      delay(25);
      flip = 1;
      }

      else if (!checkRot)
      {
        strip.setPixelColor(i, strip.Color(0,180,0));
        strip.show();
        delay(25);
      }

      else
      {
        strip.setPixelColor(i, strip.Color(180,180,100));
        strip.show();
        delay(25);
      }

      //strip.setPixelColor(i, strip.Color(180,0,0));
      
    }
  }
  if (flip)
  {
    delay(5000);
    flip=0;
    strip.fill(strip.Color(180,180,100));  // green
    strip.show();
  }

  //for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(0,0,0));
  //strip.show();
}

void genCoordinates(float coords[], int nodes, int steps)
{
  int size = nodes*steps;
  float stepSize = 1/steps;
  int k = 0;
  for (int j=0; j<size; j++)
  {
    coords[j] = k*stepSize;
    k++;

    if(k==steps) k=0;
  }
}

void perlinNoise(float color[],
float gradientx1[],
float gradientx2[],
float gradienty1[],
float gradienty2[],
int cSize, int gSize)
{
  for(int i=0; i<cSize; i++)
  {

  }
}