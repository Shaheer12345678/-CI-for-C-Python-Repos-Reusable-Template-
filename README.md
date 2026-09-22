# Reusable CI Workflows for C++ and Python

Two reusable GitHub Actions workflows that another repository can call directly,
plus small example projects that the repository's own CI builds and tests on
every push.

## What is here

| Path | Purpose |
| --- | --- |
| `.github/workflows/cpp.yml` | Reusable C++ workflow: CMake configure, build, CTest, ccache |
| `.github/workflows/python.yml` | Reusable Python workflow: pip install, black, ruff, pytest under coverage |
| `.github/workflows/ci.yml` | This repository's own CI, which calls both workflows against `examples/` |
| `examples/cpp` | Minimal CMake C++17 library and test executable |
| `examples/python` | Minimal module, pytest tests, and `requirements.txt` |

Both reusable workflows are triggered by `workflow_call` only. They do not run on
push by themselves; a caller workflow decides when they run.

## C++ workflow

`.github/workflows/cpp.yml`

Inputs:

| Input | Type | Default | Meaning |
| --- | --- | --- | --- |
| `working-directory` | string | `.` | Directory containing the CMake project |
| `build-type` | string | `Release` | Value passed to `CMAKE_BUILD_TYPE` |

Steps: checks out the repository, installs `ccache`, restores the ccache
directory, configures with `cmake -S . -B build`, builds with
`cmake --build build -j`, and runs `ctest --output-on-failure`. Every step runs
in `working-directory` and a failing step fails the job.

Caching: `CCACHE_DIR` is set to `.ccache` in the workspace and restored by
`actions/cache@v4`. The cache key combines the runner OS, the build type, and a
hash of the project's `CMakeLists.txt` and `*.cmake` files, with looser
restore keys so an unchanged compiler cache survives changes to the CMake files.
CMake is configured with `-DCMAKE_CXX_COMPILER_LAUNCHER=ccache` so compilations
go through the cache, and `ccache --show-stats` prints the hit rate at the end of
the job.

Requirements for the caller: the directory given in `working-directory` must
contain a `CMakeLists.txt` that calls `enable_testing()` and registers at least
one test with `add_test()`.

## Python workflow

`.github/workflows/python.yml`

Inputs:

| Input | Type | Default | Meaning |
| --- | --- | --- | --- |
| `working-directory` | string | `.` | Directory containing `requirements.txt` and the code to check |
| `python-version` | string | `3.11` | Python version to set up |

Steps: checks out the repository, sets up Python, installs
`requirements.txt`, runs `black --check .` and `ruff check .`, then runs
`coverage run -m pytest -q` followed by `coverage report -m`. Every step runs in
`working-directory` and a failing step fails the job.

Caching: `actions/setup-python@v5` is used with `cache: pip` and
`cache-dependency-path` pointing at `requirements.txt` inside
`working-directory`, so the pip download cache is keyed on that file.

Requirements for the caller: the directory given in `working-directory` must
contain a `requirements.txt` that installs `pytest`, `coverage`, `black`, and
`ruff`, and must contain at least one test, since `pytest` exits non-zero when it
collects nothing.

## Using these workflows from another repository

Add a caller workflow to your own repository, for example
`.github/workflows/ci.yml`:

```yaml
name: CI

on: [push, pull_request]

jobs:
  cpp:
    uses: Shaheer12345678/-CI-for-C-Python-Repos-Reusable-Template-/.github/workflows/cpp.yml@main
    with:
      working-directory: .

  python:
    uses: Shaheer12345678/-CI-for-C-Python-Repos-Reusable-Template-/.github/workflows/python.yml@main
    with:
      working-directory: .
```

Override the defaults when needed:

```yaml
  cpp:
    uses: Shaheer12345678/-CI-for-C-Python-Repos-Reusable-Template-/.github/workflows/cpp.yml@main
    with:
      working-directory: src/engine
      build-type: Debug

  python:
    uses: Shaheer12345678/-CI-for-C-Python-Repos-Reusable-Template-/.github/workflows/python.yml@main
    with:
      working-directory: services/api
      python-version: "3.12"
```

Pin `@main` to a tag or commit SHA if you want the workflow definition to stay
fixed.

## Running the examples locally

C++:

```sh
cd examples/cpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Python:

```sh
cd examples/python
pip install -r requirements.txt
black --check .
ruff check .
coverage run -m pytest -q
coverage report -m
```
