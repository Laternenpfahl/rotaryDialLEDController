#include <Adafruit_NeoPixel.h>

#define LED_PIN     5 // The pin where the data line is connected
#define LED_COUNT  61 // Number of LEDs in the strip

// Initialize the NeoPixel library.
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int checkRotPin = 2;
const int pulsePin = 3;
uint8_t rgb[3]={0,0,0};



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
  int flip1 = 0;
  int flip2 = 0;
  int count = 0;
  
  if(rainbowAnimation && checkRot)
  {

    for (int i = 0; i < strip.numPixels(); i++) 
    {
      // offset + pixel index → makes colors "move"
      colorIndex = (i * rainbowSpeed + offset) & 255;
      wheel(colorIndex, rgb);
      strip.setPixelColor(i, strip.Color(rgb[0], rgb[1], rgb[2]));
    }

  strip.show();
  offset++;   // shift rainbow
  delay(25);

  }

  else if(rainbowAnimation)
  {
    strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
    strip.show();
  }

  while (!checkRot) // Wheel is being turned
  {
      pulseState = digitalRead(pulsePin);
      checkRot = digitalRead(checkRotPin);

      if(pulseState && !checkRot)
      {
      
        if (flip1==0)
        {
          flip1=1;
          count++;
        }
      }

      else
      {
        flip1=0;
      }


      //strip.setPixelColor(i, strip.Color(180,0,0));
      
    }

    if(count>0)
    {
      rainbowAnimation = 0;
      Serial.print("r-Value: ");
      Serial.println(rgb[0]);
      softClose(rgb);

////////////////////////////////

      if (count == 1)
      {
        for (int h=0;h<120;h=h+2)
        {
          rgb[0] = 2*h;
          rgb[1] = 0.5*h;
          rgb[2] = 0;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(25);
        }
      }

////////////////////////////////

      if (count == 2)
      {} // turns off

////////////////////////////////

      if (count == 3)
      {
        for (int h=0;h<120;h=h+4)
        {
          rgb[0] = 0;
          rgb[1] = 0.5*h;
          rgb[2] = 2*h;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(25);
        }
      }

////////////////////////////////

      if (count == 4)
      {
        for (int h=0;h<100;h=h+4)
        {
          rgb[0]= 2.5*h;
          rgb[1] = 2.1*h;
          rgb[2] = 0.5*h;

          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          
          delay(25);
        }
      }

////////////////////////////////

      if (count == 6)
      {
        for (int h=0;h<250;h=h+5)
        {
          rgb[0] = h;
          rgb[1] = 0;
          rgb[2] = 0;

          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();

          delay(25);
        }
      }

////////////////////////////////

      if (count == 7)
      {
        for (int h=0;h<100;h=h+2)
        {
          rgb[0] = 2*h;
          rgb[1] = 2*h;
          rgb[2] = 2*h;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(25);
        }

        rainbowAnimation = 1; // turn on animation
      }

////////////////////////////////
      
          Serial.println(count);
          flip1=0;
          count=0;
        }

      //for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(0,0,0));
      //strip.show();
}

void wheel(byte pos, uint8_t rgb[]) 
{
  if (pos < 85) 
  {
    rgb[0] = pos * 3;
    rgb[1] = 255 - pos * 3;
    rgb[2] = 0;
  } 
  
  else if (pos < 170) 
  {
    pos -= 85;

    rgb[2] = pos * 3;
    rgb[0] = 255 - pos * 3;
    rgb[1] = 0;

  }
  else 
  {
    pos -= 170;

    rgb[1] = pos * 3;
    rgb[2] = 255 - pos * 3;
    rgb[0] = 0;
  }
}

void softClose(uint8_t rgb[])
{
    float dimming=0.65;

    for (int h=0;h<15;h++)
    {

      rgb[0] = dimming*rgb[0];
      rgb[1] = dimming*rgb[1];
      rgb[2] = dimming*rgb[2];
  
      strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
      strip.show();
      
      delay(25);
    }

    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;

    strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
    strip.show();
}