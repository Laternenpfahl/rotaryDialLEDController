#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int checkRotPin = 2;
const int pulsePin = 3;

void genCoordinates(float coords[], int nodes, int steps);
void genGradient(float grad[], int nodes);

int main(){

  int myNodes = 3;
  int mySteps = 5;
  float myCoords[(myNodes-1)*mySteps];
  float gradx[myNodes];
  float grady[myNodes];

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

void genCoordinates(float coords[], int nodes, int steps)
{
  int size = (nodes-1)*steps;
  float stepSize = 1.0/steps;
  int k = 0;
  for (int j=0; j<size; j++)
  {
    coords[j] = k*stepSize;
    k++;

    //cout << stepSize << endl;

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