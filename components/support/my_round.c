#include "my_round.h"
#include "math.h"


double round(double x)
{
    return signbit(x) ? ceil(x - 0.5) : floor(x + 0.5);;
}

