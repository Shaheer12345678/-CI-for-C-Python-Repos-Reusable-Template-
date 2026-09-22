"""Small library used to exercise the Python CI template."""

from collections.abc import Sequence


def mean(values: Sequence[float]) -> float:
    """Return the arithmetic mean of values, or 0.0 when values is empty."""
    if not values:
        return 0.0
    return sum(values) / len(values)
