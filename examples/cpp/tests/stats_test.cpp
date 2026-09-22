#include "stats.hpp"

#include <cmath>
#include <cstdio>
#include <vector>

namespace {

int failures = 0;

// Checks are explicit rather than assert-based so they still run in
// optimised build types, where NDEBUG disables assert.
void expect_near(double actual, double expected, const char *label) {
  if (std::fabs(actual - expected) > 1e-9) {
    std::printf("FAIL %s: expected %f, got %f\n", label, expected, actual);
    ++failures;
  }
}

}  // namespace

int main() {
  expect_near(stats::mean({1.0, 2.0, 3.0}), 2.0, "mean of three values");
  expect_near(stats::mean({5.0}), 5.0, "mean of a single value");
  expect_near(stats::mean({}), 0.0, "mean of empty input");

  if (failures > 0) {
    std::printf("%d check(s) failed\n", failures);
    return 1;
  }
  std::printf("all checks passed\n");
  return 0;
}
