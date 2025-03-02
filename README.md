- Currently using c++17 until the XCODE Clang is updated to Clang17 that has implemented the c++23 Standard
- Turning off all Optimizations i.e. O0

`clang++ -std=c++17 -O0 -Wall -Wextra -Wpedantic <input_file> -o <output_file>`
