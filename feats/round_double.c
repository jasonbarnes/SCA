#include <math.h>
#include "round_double.h"

double round_double(double n, double base){
	n = n / base;
	return round(n) * base;
}
