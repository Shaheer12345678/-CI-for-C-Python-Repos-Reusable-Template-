"""Tests for the calc module."""

from calc import mean


def test_mean_of_several_values() -> None:
    assert mean([1.0, 2.0, 3.0]) == 2.0


def test_mean_of_a_single_value() -> None:
    assert mean([5.0]) == 5.0


def test_mean_of_empty_input_is_zero() -> None:
    assert mean([]) == 0.0
