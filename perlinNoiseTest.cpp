#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int checkRotPin = 2;
const int pulsePin = 3;
const int myNodes = 3;
const int mySteps = 5;
const int mySize = (myNodes-1)*mySteps;

void genCoordinates(float coords[], int nodes, int steps);
void genGradient(float gradx[], float grady[], int nodes);
float calcPerlinVal(float offsetsX[], 
                    float offsetsY[],
                    float gradx[],
                    float grady[]);
void calcOffsets(float offsetsX[], float offsetsY[], float coordx, float coordy);


int main(){

  float myCoords[(myNodes-1)*mySteps];
  float gradx0[myNodes];
  float grady0[myNodes];
  float gradx1[myNodes];
  float grady1[myNodes];
  float cellGradX[4];
  float cellGradY[4];
  float offsetsX[4];
  float offsetsY[4];
  float ledVals[mySize];
  int cellCount = 0;

  srand(time({}));

  genCoordinates(myCoords, myNodes, mySteps);
  genGradient(gradx0, grady0, myNodes);
  genGradient(gradx1, grady1, myNodes);// THIS NEEDS TO GO IN SETUP

  cout << "myArray = [";

  for(int i=0; i<mySteps; i++) // loop over y-coordinates
  {
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


      calcOffsets(offsetsX, offsetsY, myCoords[j]-cellCount, myCoords[i]); // coords are symmetrical in x and y
      ledVals[j] = calcPerlinVal(offsetsX, offsetsY, cellGradX, cellGradY); // we calc the whole x-direction and pass it to the LEDs. y direction gives us time dynamics
    
      cout << ledVals[j] << ",";

      if(myCoords[j]- cellCount >= 1) cellCount++;
    }

    cellCount = 0; // stepped through x-axis -> new timestep, so reset cellcount

    if(i==mySteps-1) //THIS NEEDS TO GO IN LOOP
    {
      for(int v=0;v<myNodes;v++)
      {
        gradx0[v] = gradx1[v];
        grady0[v] = grady1[v]; // turn second row array into first row array for continuity
      }

      genGradient(gradx1, grady1, myNodes); // calc new random gradients
    }

  }

  cout << "]" << endl;

  return 0;

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

float interpolant(float t)
{
  return t*t*t*(t*(t*6 - 15) + 10);
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

void genCoordinates(float coords[], int nodes, int steps)
{
  int size = (nodes-1)*steps;
  float stepSize = 1.0/steps;
  int k = 0;
  for (int j=0; j<size; j++)
  {
    coords[j] = k*stepSize;
    k++;
  }
}

void genGradient(float gradx[], float grady[], int nodes)
{
  for (int n=0;n<nodes;n++)
  {
    gradx[n] = (rand() % 255) / 255.0;
    gradx[n] = (gradx[n] - 0.5) * 2;

    grady[n] = (rand() % 255) / 255.0;
    grady[n] = (grady[n] - 0.5) * 2;

    gradx[n] = gradx[n] / sqrt(gradx[n]*gradx[n] + grady[n]*grady[n]); // normalize vector
    grady[n] = grady[n] / sqrt(gradx[n]*gradx[n] + grady[n]*grady[n]); // normalize vector
  
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