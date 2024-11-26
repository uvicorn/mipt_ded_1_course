#include "interval.hpp"

const Interval Interval::Empty    = Interval(+Inf, -Inf);
const Interval Interval::Universe = Interval(-Inf, +Inf);
