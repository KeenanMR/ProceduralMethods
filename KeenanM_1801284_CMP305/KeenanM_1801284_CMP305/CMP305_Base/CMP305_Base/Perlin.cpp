#include "Perlin.h"

Perlin::Perlin()
{
	srand(static_cast <unsigned> (time(0)));

	std::random_shuffle(std::begin(permutation), std::end(permutation));
}

void Perlin::seed()
{
	std::random_shuffle(std::begin(permutation), std::end(permutation));
}




float Perlin::noise2D(double x, double y)
{
	int integerX0, integerY0, integerX1, integerY1;

	//GET THE INTEREGER PART OF THE INPUTS
	integerX0 = floor(x); //FLOOR ROUNDS DOWN TO THE LARGEST INTEGRAL VALUE THAT IS NOT > INPUT
	integerY0 = floor(y);
	integerX1 = (integerX0 + 1) & 0xff;  //GET THE INTEGERS "AHEAD" OF THE INPUTS AND WRAP THEM BETWEEN 0 AND 255
	integerY1 = (integerY0 + 1) & 0xff;


	float floatX0, floatY0, floatX1, floatY1;
	//GET THE FLOAT PART OF THE INPUTS
	floatX0 = x - integerX0;
	floatY0 = y - integerY0;
	floatX1 = floatX0 - 1.0f; //GET THE INTEGERS "BEHIND" THE INPUTS AND WRAP THEM BETWEEN 0 AND 255
	floatY1 = floatY0 - 1.0f;

	//GET THE INTEGERS OF THE INPUTS AND WRAP THEM BETWEEN 0 AND 255
	integerX0 = integerX0 & 0xff;
	integerY0 = integerY0 & 0xff;

	float fadeX, fadeY;
	//GET THE FADE VALUES FOR THE FRACTIONAL INPUTS
	fadeX = fade(floatX0);
	fadeY = fade(floatY0);

	//CALCULATE THE GRADIENTS OF THE 4 POINTS 2 AT A TIME AND LINEARLY INTERPOLATE BETWEEN THEM USING THE PREDEFINED PERMUTATION TABLE
	float linearInput0, linearInput1, linearOutput0, linearOutput1;

	linearInput0 = grad(permutation[integerX0 + permutation[integerY0]], floatX0, floatY0);
	linearInput1 = grad(permutation[integerX0 + permutation[integerY1]], floatX0, floatY1);
	linearOutput0 = lerp(fadeY, linearInput0, linearInput1);

	linearInput0 = grad(permutation[integerX1 + permutation[integerY0]], floatX1, floatY0);
	linearInput1 = grad(permutation[integerX1 + permutation[integerY1]], floatX1, floatY1);
	linearOutput1 = lerp(fadeY, linearInput0, linearInput1);

	return 0.507f * (lerp(fadeX, linearOutput0, linearOutput1));
}

