# TreesHomeWork

## 

The problem requires the implementation of a custom set data structure that supports two types of operations: an insertion command, denoted by 'k' followed by an integer value, which adds the specified number to the set, and a range query command, denoted by 'q' followed by two integer arguments (number1 and number2), which must return the count of elements within the closed interval [number1, number2]. The performance of this custom implementation must be at least as efficient as that of the standard `std::set` in C++.

## Building the tests

To build examples and tests, follow this steps

1. Clone the repository:
```bash
git clone https://github.com/OlejaCat/TreesHomeWork.git
```

2. Navigate to the project directory:
```bash
cd TreesHomeWork
```

3. Generate and compile files (with release flags)
```bash
cmake -B build -S . && cmake --build build
```

After this steps, you will find executable file 'tests' in the `build` directory

## Running tests

The `tests` executable provides unit tests on AVL tree and tests on task

## Using in project

You can use my implementation in your own projects by including the file `src/tree/trees.hpp` via the `#include` directive.
