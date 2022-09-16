#pragma once

#define _USE_MATH_DEFINES
#include <random>
#include <math.h>

//CLAS TO REPORESENT A POINT IN POISSON SAMPLING
class Point
{
private:
		//POSITION FLOATS
		float x;
		float y;
public:

	//CONSTRUCTOR
	Point(float X, float Y);

	//GENERATE A NEARBY POINT USING POISSON SAMPLING
	Point GeneratePointAround(Point initialPoint, float minDist);

	//GETTERS
	int getX();
	int getY();


};

