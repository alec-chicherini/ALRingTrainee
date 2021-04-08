#include "helper_calc.h"
#include <cmath>
template<typename T1,typename T2>
float distance_between(T1 x1, T1 y1, T2 x2, T2 y2)
{
    return sqrtf(
        powf(float(x1) - float(x2), 2) +
        powf(float(y1) - float(y2), 2)
    );
};