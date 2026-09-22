#pragma once

#include <vector>

namespace stats {

// Returns the arithmetic mean of values, or 0.0 when values is empty.
double mean(const std::vector<double> &values);

}  // namespace stats
