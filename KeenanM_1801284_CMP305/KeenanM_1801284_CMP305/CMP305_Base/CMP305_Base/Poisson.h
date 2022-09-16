#pragma once
#include <list>
#include <vector>
#include "Point.h"

//CLASS FOR ATTEMPTED IMPLEMNTATION OF POISSON SAMPLING - CURRNETLY NON FUNCTIONAL
class Poisson
{

public:

	//CONSTRUCTOR
	Poisson(int res);

	//RETURNS THE VECTOR OF THE POINT GENERATED
	std::vector <Point> getPoisson();

	//GENERATES A SET OF POINTS USING POISSON SAMPLING
	void PoissonSampling(int pointsPerCycle, float minDistance);

	//CHECKS IF A POINT IS NEARBY
	bool isPointNearby(std::list <Point> samplePoints, Point pointChecked, float minDistance);


private:

	//MAP OF POISSON POINTS
	std::vector <Point> poissonMap;

	//RESOLUTION OF HEIGHT MAP
	int resolution;
};

