#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int checkRotPin = 2;
const int pulsePin = 3;
const int myNodes = 3;
const int mySteps = 5;

void genCoordinates(float coords[], int nodes, int steps);
void genGradient(float grad[], int nodes);

int main(){

  float myCoords[(myNodes-1)*mySteps];
  float gradx0[myNodes];
  float grady0[myNodes];
  float gradx1[myNodes];
  float grady1[myNodes];
  float offsetsX[4];
  float offsetsY[4];
  float dotProducts[4];

  srand(time(0));

  genCoordinates(myCoords, myNodes, mySteps);
  genGradient(gradx, myNodes);
  genGradient(grady, myNodes);


  for(int i=0; i<myNodes; i++)
  {
    cout << "x:" << gradx[i] << ", y:" << grady[i] << endl;
  }

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

  f0 = valsToInterpolate[0] 

  return perlinVal;

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

    cout << coords[j] << endl;

    if(k==steps) k=0;
  }
}

void genGradient(float grad[], int nodes)
{
  for (int n=0;n<nodes;n++)
  {
    grad[n] = (rand() % 255) / 255.0;
    grad[n] = (grad[n] - 0.5) * 2;
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