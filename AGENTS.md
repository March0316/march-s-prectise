## Cursor Cloud specific instructions

This is a minimal C/C++ educational project containing a single source file (`期末练习.cpp`) with student programming exercises (final exam practice problems in C).

### Build & Run

- **Compiler:** `g++` (pre-installed on the VM)
- **Compile:** `g++ -o 期末练习 期末练习.cpp -lm -Wall`
- **Run:** `./期末练习` (the active code is a bubble sort that reads 10 integers from stdin)
- **Quick test:** `echo "9 3 7 1 8 2 6 4 10 5" | ./期末练习`

### Notes

- There is no build system (no Makefile, CMake, etc.) — compile directly with `g++`.
- There are no external dependencies beyond the C standard library and `math.h`.
- Most code in the file is commented out; only the last function (bubble sort) is active.
- The file uses ISO-8859 encoding with Chinese characters in comments and `printf` strings.
- No lint tool or automated test framework is configured for this project.
