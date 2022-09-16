#include "Poisson.h"

//CONSTRUCTOR
Poisson::Poisson(int res)
{
	resolution = res;
}

//GET THE GENERATED MAP
std::vector <Point> Poisson::getPoisson()
{
	return poissonMap;
}

//RUN THE POISSON SAMPLING
void Poisson::PoissonSampling(int pointsPerCycle, float minDistance)
{
	//CREATE A LIST AND VECTOR OF POINTS
	std::list <Point> samplePoints;
	std::vector <Point> processPoints;

	//CREATE A RANDOM FIRST POINT
	Point firstPoint(rand() % resolution, rand() % resolution);

	//ADD THE FIRST POINT
	samplePoints.push_back(firstPoint);
	processPoints.push_back(firstPoint);

	while (!processPoints.empty())
	{
		//SHUFFLE THE POINTS AND CHOOSE A NEW CURRENT POINT
		std::random_shuffle(processPoints.begin(), processPoints.end());
		Point currentPoint = processPoints.back();
		processPoints.pop_back();

		//CREATE A NEW POINT AND CHECK IF ITS TOO CLOSE, IF IT IS, DISCARD IT
		for (int i = 0; i < pointsPerCycle; i++)
		{
			Point newPoint(0, 0);
			newPoint = newPoint.GeneratePointAround(currentPoint, minDistance);

			if (!isPointNearby(samplePoints, newPoint, minDistance))//not nearby
			{
				processPoints.push_back(newPoint);
				samplePoints.push_back(newPoint);
			}
		}
	}

	//ADD THE GENERATED POINTS TO THE MAP
	for (auto it : samplePoints)
	{
		poissonMap.push_back(it);
	}

}

//FUNCTION TO FIND THE DISTANCE BETWEEN THE POINTS AND CHECK IF THEY ARE TOO CLOSE
bool Poisson::isPointNearby(std::list <Point> samplePoints, Point pointChecked, float minDistance)
{

	for (auto it : samplePoints)
	{
		if (sqrt(pow(it.getX() - pointChecked.getX(), 2) + pow(it.getY() - pointChecked.getY(), 2) < minDistance))
		{
			return true;
		}

	}
	return false;
}
