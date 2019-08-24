#include "utilities.h"

#define _USE_MATH_DEFINES
#include <cmath>

float wrapFloat(float num, float min, float max, int* dist) {
	float step = max - min;
	float distance = num - min;
	int nbr_steps = (int)floor(distance / step);
	if (dist) *dist = nbr_steps;
	return num - nbr_steps * step;
}
