#include <Adafruit_NeoPixel.h>

#define LED_PIN     5 // The pin where the data line is connected
#define LED_COUNT  61 // Number of LEDs in the strip

// Initialize the NeoPixel library.
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int checkRotPin = 2;
const int pulsePin = 3;
int delayTime = 30;
uint16_t rgb[3]={0,0,0};

// rainbow stuff

// remembers position during loop
uint16_t offset = 0;
// colorchange per pixel -> high number skips more colors, but u get more rainbowcycles within one strip
int rainbowSpeed = 3;
uint16_t colorIndex = 0;
bool rainbowAnimation = 0;
bool ocean = 0;


// Perlin Noise Stuff
const int myNodes = 5;
const int mySteps = 15;
const int mySize = (myNodes-1)*mySteps;
float myCoords[(myNodes-1)*mySteps];

float Rgradx0[myNodes];
float Rgrady0[myNodes];
float Rgradx1[myNodes];
float Rgrady1[myNodes];

float Ggradx0[myNodes];
float Ggrady0[myNodes];
float Ggradx1[myNodes];
float Ggrady1[myNodes];

float Bgradx0[myNodes];
float Bgrady0[myNodes];
float Bgradx1[myNodes];
float Bgrady1[myNodes];

float RledVals[mySize];
float GledVals[mySize];
float BledVals[mySize];

bool oceanAnimation = 0;
const float PI_F = 3.14159265f;

void setup() {
  Serial.begin(9600);

  // Perlin Noise Stuff
  randomSeed(analogRead(A0));
  genCoordinates(myCoords, myNodes, mySteps);

  genGradient(Rgradx0, Rgrady0, myNodes);
  genGradient(Rgradx1, Rgrady1, myNodes);// THIS NEEDS TO GO IN SETUP

  genGradient(Ggradx0, Ggrady0, myNodes);
  genGradient(Ggradx1, Ggrady1, myNodes);// THIS NEEDS TO GO IN SETUP
    
  genGradient(Bgradx0, Bgrady0, myNodes);
  genGradient(Bgradx1, Bgrady1, myNodes);// THIS NEEDS TO GO IN SETUP


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
  delay(delayTime);

  }


//////////////////////////////////////////

  if(oceanAnimation && checkRot)
  {
    for(int i=0;i<mySteps;i++)
    {
      checkRot = digitalRead(checkRotPin);
      
      calcOcean(Rgradx0, Rgrady0, Rgradx1, Rgrady1, RledVals, myCoords, myCoords[i]);
      calcOcean(Ggradx0, Ggrady0, Ggradx1, Ggrady1, GledVals, myCoords, myCoords[i]);
      calcOcean(Bgradx0, Bgrady0, Bgradx1, Bgrady1, BledVals, myCoords, myCoords[i]);

      for(int led=0; led<mySize; led++) //light em up
      {

        if(ocean)
        {

          rgb[0] = round(RledVals[led]/5.0);
          //use unused perlin noise data for light intensity modulation
          RledVals[led]=RledVals[led]/140;
          if(RledVals[led] > 1) RledVals[led]=1;

          rgb[1] = round(GledVals[led] * RledVals[led]);
          rgb[2] = round((BledVals[led]) * 2 * RledVals[led]);

          if(rgb[1]>255)rgb[1]=255;
          if(rgb[2]>255)rgb[2]=255;

          strip.setPixelColor(led, strip.Color(rgb[0],rgb[1],rgb[2]));
        }

        else
        {
          // fire animation

          //use unused perlin noise data for light intensity modulation
          BledVals[led]=BledVals[led]/140;
          if(BledVals[led] > 1) BledVals[led]=1;

          rgb[2] = 0;
          rgb[1] = round(GledVals[led] * BledVals[led] / 1.4);
          rgb[0] = round(255*BledVals[led]);


          strip.setPixelColor(led, strip.Color(rgb[0],rgb[1],rgb[2]));
        }
      }      

      strip.setPixelColor(60, strip.Color(rgb[0],rgb[1],rgb[2])); // setting last LED to second to last color bc I cut the strip at a stupid place

      if(!checkRot) break;

      strip.show();
      delay(delayTime);

      if(i==mySteps-1) //THIS NEEDS TO GO IN LOOP
      {
        for(int v=0;v<myNodes;v++)
        {
          Rgradx0[v] = Rgradx1[v];
          Rgrady0[v] = Rgrady1[v]; // turn second row array into first row array for continuity
          
          Ggradx0[v] = Ggradx1[v];
          Ggrady0[v] = Ggrady1[v]; // turn second row array into first row array for continuity
          
          Bgradx0[v] = Bgradx1[v];
          Bgrady0[v] = Bgrady1[v]; // turn second row array into first row array for continuity

        }
        
        genGradient(Rgradx1, Rgrady1, myNodes); // calc new random gradients
        genGradient(Ggradx1, Ggrady1, myNodes); // calc new random gradients
        genGradient(Bgradx1, Bgrady1, myNodes); // calc new random gradients
      }
    }
  }

  while (!checkRot) // Wheel is being turned
  {
      pulseState = digitalRead(pulsePin);
      checkRot = digitalRead(checkRotPin);
      delayTime = 25;

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
      oceanAnimation = 0;
      ocean = 0;
      rainbowAnimation = 0;
      Serial.print("r-Value: ");
      Serial.println(rgb[0]);
      softClose(rgb);

////////////////////////////////

      if (count == 8)
      {
        for (int h=0;h<120;h=h+2)
        {
          rgb[0] = 2*h;
          rgb[1] = 0.5*h;
          rgb[2] = 0;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(delayTime);
        }
      }

////////////////////////////////

      if (count == 2)
      {} // turns off

////////////////////////////////

      if (count == 7)
      {
        for (int h=0;h<120;h=h+4)
        {
          rgb[0] = 0;
          rgb[1] = 0.5*h;
          rgb[2] = 2*h;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(delayTime);
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
          
          delay(delayTime);
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

          delay(delayTime);
        }
      }

////////////////////////////////

      if (count == 3)
      {
        for (int h=0;h<125;h=h+5)
        {
          rgb[0] = 0;
          rgb[1] = h;
          rgb[2] = 2*h;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(delayTime);
        }

        delayTime = 20;

        ocean = 1; //if 0 its in fire mode
        oceanAnimation = 1; // turn on animation
      }

//////////////////////////////

      if (count == 1)
      {
        for (int h=0;h<125;h=h+5)
        {
          rgb[2] = 0;
          rgb[1] = h/2;
          rgb[0] = 2*h;
          strip.fill(strip.Color(rgb[0], rgb[1], rgb[2]));
          strip.show();
          delay(delayTime);
        }
        delayTime = 15;
        oceanAnimation = 1; // turn on animation
      }
/////////////////////////////////

      if (count == 5)
      {
        for (float h=0;h<1;h=h+0.05)
        {

          for (int i = 0; i < strip.numPixels(); i++) 
          {
            // offset + pixel index → makes colors "move"
            colorIndex = (i * rainbowSpeed) + offset & 255;
            wheel(colorIndex, rgb);
            rgb[0] = rgb[0] * h;
            rgb[1] = rgb[1] * h;
            rgb[2] = rgb[2] * h;

            strip.setPixelColor(i, strip.Color(rgb[0], rgb[1], rgb[2]));
          }

          strip.show();
          delay(delayTime);
        }
        delayTime = 15;
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


void wheel(byte pos, uint16_t rgb[]) // color wheel for rainbow
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

void genCoordinates(float coords[], int nodes, int steps)
{
  int mySize = (nodes-1)*steps;
  float stepSize = 1.0/steps;
  int k = 0;
  for (int j=0; j<mySize; j++)
  {
    coords[j] = k*stepSize;
    k++;
  }
}


void genGradient(float gradx[], float grady[], int nodes)
{
  float angle;

  for (int n=0;n<nodes;n++)
  {
    angle = random(0, 65536) * (2.0 * PI_F / 65536.0);

    gradx[n] = cos(angle);
    grady[n] = sin(angle);
  }
}

void calcOffsets(float offsetsX[], float offsetsY[], float coordx, float coordy)
{
    offsetsX[0] = coordx; // offset 0 is 00 corner
    offsetsY[0] = coordy;

    offsetsX[1] = 1 - coordx; // offset 1 is 01 corner
    offsetsY[1] = coordy;

    offsetsX[2] = coordx; // offset 2 is 10 corner
    offsetsY[2] = 1 - coordy;

    offsetsX[3] = 1 - coordx; // offset 3 is 11 corner
    offsetsY[3] = 1 - coordy;

}


float calcPerlinVal(float offsetsX[], 
                    float offsetsY[],
                    float gradx[],
                    float grady[])
{

  // following the mathematical description on wikipedia

  float perlinVal;
  float f0;
  float f1;
  float valsToInterpolate[4];

  for(int i=0;i<4;i++)
  {
    valsToInterpolate[i] = offsetsX[i] * gradx[i] + offsetsY[i] * grady[i];
  }

  f0 = valsToInterpolate[0] * interpolant(1-offsetsX[0]) + valsToInterpolate[1] * interpolant(offsetsX[0]);
  f1 = valsToInterpolate[2] * interpolant(1-offsetsX[0]) + valsToInterpolate[3] * interpolant(offsetsX[0]);
  perlinVal = f0 * interpolant(1-offsetsY[0]) + f1 * interpolant(offsetsY[0]);

  return 255 * (perlinVal+1)/2;

}

float interpolant(float t)
{
  return t*t*t*(t*(t*6 - 15) + 10);
}

void calcOcean(float gradx0[], float grady0[], float gradx1[], float grady1[], float ledVals[], float coords[], float yChoord)
{
  float cellGradX[4];
  float cellGradY[4];

  float offsetsX[4];
  float offsetsY[4];

  int cellCount = 0;

    for(int j=0; j<mySize; j++)
    {

      cellGradX[0] = gradx0[cellCount];     // a bit confusing, since we have xy in gradient coordinates and xy in "corner" coordinates...
      cellGradX[1] = gradx0[cellCount + 1]; // cellGradX[0] and cellgradY[0] are gradient xy-values for 00 corner of current cell, [1] is 01 etc
      cellGradX[2] = gradx1[cellCount];
      cellGradX[3] = gradx1[cellCount + 1];

      cellGradY[0] = grady0[cellCount];
      cellGradY[1] = grady0[cellCount + 1];
      cellGradY[2] = grady1[cellCount];
      cellGradY[3] = grady1[cellCount + 1];


      calcOffsets(offsetsX, offsetsY, coords[j]-cellCount, yChoord); // coords are symmetrical in x and y
      ledVals[j] = calcPerlinVal(offsetsX, offsetsY, cellGradX, cellGradY); // we calc the whole x-direction and pass it to the LEDs. y direction gives us time dynamics

      if(myCoords[j]- cellCount >= 1) cellCount++;
    }
  }


void softClose(uint16_t rgb[])
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