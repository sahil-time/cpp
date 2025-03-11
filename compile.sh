#!/bin/bash

if [[ $# -eq 0 ]]; then
  echo "ERROR: Provide name of the 'main' file"
  exit 1 # Exit with an error code
fi

echo "[COMPILE_LOG] ------------------------------------------"

# CLEANUP FUNCTIONS
function cleanup_transient_files { # Cleanup Compiler created files
    rm *.gch >/dev/null 2>&1
}
function cleanup_github_libs { # Cleanup GITHUB libs
    rm -rf github_libs >/dev/null 2>&1
    mkdir github_libs
}
function cleanup_personal_libs { # Cleanup User created libs
    rm -rf personal_libs > /dev/null 2>&1
    mkdir personal_libs
}
function cleanup_main { # Cleanup Binary
    rm a.out >/dev/null 2>&1
}

# CLEANUP EVERYTHING [ JUST IN-CASE CLEANUP AT THE END FAILS ]
cleanup_github_libs
cleanup_personal_libs
cleanup_transient_files
cleanup_main

# GITHUB CLONED & BUILT LIBS
function clone_and_build_github_libs {
    cd github_libs

    echo "[COMPILE_LOG]  - Cloning and Building 'libassert' from 'https://github.com/jeremy-rifkin/libassert..."
    git clone https://github.com/jeremy-rifkin/libassert.git
    git checkout v2.1.5
    mkdir libassert/build
    cd libassert/build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.
    make -j
    cd ../../

    echo "[COMPILE_LOG]  - Cloning and Building 'cpptrace' needed by 'libassert'..."
    git clone https://github.com/jeremy-rifkin/cpptrace.git
    git checkout v0.8.2
    mkdir cpptrace/build
    cd cpptrace/build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.
    make -j
    make install
    cd ../../

    cd ../
}

# PERSONAL BUILT LIBS
function build_personal_libs {
    echo "[COMPILE_LOG]  - Building 'number.cpp'..."
    clang++ -std=c++17 \
        -DGITHUB_ASSERT \
        -O0 \
        -Wall -Wextra -Werror \
        -lz \
        -L./github_libs/cpptrace/build/lib -lcpptrace -lzstd -ldwarf -I./github_libs/cpptrace/include \
        -L./github_libs/libassert/build -lassert -I./github_libs/libassert/include \
        -shared -fPIC libs/number.cpp -I./libs -o personal_libs/libnumber.so
}

# START COMPILATION PROCESS
echo "[COMPILE_LOG] Cloning and Building GITHUB libs..."
clone_and_build_github_libs
echo "[COMPILE_LOG] Building Personal libs..."
build_personal_libs

# BUILD MAIN
echo "[COMPILE_LOG] Building 'main.cpp'..."
clang++ -std=c++17 \
    -DGITHUB_ASSERT \
    -DQLEN=100 \
    -O0 \
    -Wall -Wextra -Werror \
    -lz \
    -L./github_libs/cpptrace/build/lib -lcpptrace -lzstd -ldwarf -I./github_libs/cpptrace/include \
    -L./github_libs/libassert/build -lassert -I./github_libs/libassert/include \
    -L./github_libs/libassert/build -lassert \
    -L./personal_libs -lnumber -I./libs \
    $1

# RUN EXECUTABLE
echo "[COMPILE_LOG] Running '$1' executable..."
echo "[COMPILE_LOG] ------------------------------------------"
echo ""
./a.out $2
echo ""
