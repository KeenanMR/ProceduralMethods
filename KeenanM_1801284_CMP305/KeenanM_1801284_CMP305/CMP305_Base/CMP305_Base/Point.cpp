#include "Point.h"

//CONSTRUCTOR
Point::Point(float X, float Y)
{
	x = X;
	y = Y;
}

Point Point::GeneratePointAround(Point initialPoint, float minDist)
{
	//RANDOM NUMBER BETWEEN 0 AND 1
	float randomNumber1 = rand() / float(RAND_MAX);
	float randomNumber2 = rand() / float(RAND_MAX);
	//RANDOM RADIUS BETWEEN THE MIN DISTANCE AND THE RANDOM GENERATED NUMBER
	float randomRadius = minDist * (randomNumber1 + 1);
	//RANDOM ANGLE
	float angle = 2 * M_PI * randomNumber2;
	//GENERATE THE NEW POINT BASED ON THE RANDOMISED PARAMETERS
	float newX = initialPoint.getX() + randomRadius * cos(angle);
	float newY = initialPoint.getY() + randomRadius * sin(angle);


	return Point(newX, newY);
}

//GETTERS
int Point::getX()
{
	return x;
}

int Point::getY()
{
	return y;
}

