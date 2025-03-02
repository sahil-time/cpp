#!/bin/bash

# CLEANUP FUNCTIONS
function cleanup_transient_files { # Cleanup Compiler created files
    rm *.gch >/dev/null 2>&1
}
function cleanup_github_libs { # Cleanup GITHUB libs
    rm -rf github_libs >/dev/null 2>&1
    mkdir github_libs
}
function cleanup_user_libs { # Cleanup User created libs
    rm -rf personal_libs > /dev/null 2>&1
    mkdir personal_libs
}
function cleanup_main { # Cleanup Binary
    rm a.out >/dev/null 2>&1
}

# GITHUB CLONED & BUILT LIBS
function clone_and_build_github_libs { # Clone & Build 'libassert' => https://github.com/jeremy-rifkin/libassert
    # remove all github libs
    cleanup_github_libs
    cd github_libs

    # clone 'libassert', build it
    echo "Cloning and Building 'libassert'..."
    git clone https://github.com/jeremy-rifkin/libassert.git
    git checkout v2.1.5
    mkdir libassert/build
    cd libassert/build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.
    make -j

    cd ../../

    # clone 'cpptrace' & build, NEEDED for 'libassert'
    echo "Cloning and Building 'cpptrace' needed by 'libassert'..."
    git clone https://github.com/jeremy-rifkin/cpptrace.git
    git checkout v0.8.2
    mkdir cpptrace/build
    cd cpptrace/build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.
    make -j
    make install

    # clone ...

    # back to root of ws
    cd ../../../
}

# PRE BUILDING PROTOCOL
cleanup_transient_files
cleanup_user_libs
cleanup_main

# BUILD LIBRARIES
echo "Cloning and Building GITHUB libs..."
clone_and_build_github_libs

echo "Building 'number.cpp'..."
clang++ -std=c++17 \
    -O0 \
    -Wall -Wextra -Werror \
    -lz \
    -L./github_libs/cpptrace/build/lib -lcpptrace -lzstd -ldwarf \
    -I./github_libs/libassert/include -I./github_libs/cpptrace/include \
    -L./github_libs/libassert/build -lassert \
    -shared -fPIC number.cpp -o personal_libs/libnumber.so

# BUILD MAIN
echo "Building 'main.cpp'..."
clang++ -std=c++17 \
    -O0 \
    -Wall -Wextra -Werror \
    -L./personal_libs -lnumber \
    -lz \
    -L./github_libs/cpptrace/build/lib -lcpptrace -lzstd -ldwarf \
    -I./github_libs/libassert/include -I./github_libs/cpptrace/include \
    -L./github_libs/libassert/build -lassert \
    $1

# RUN EXECUTABLE
echo "Running '$1' executable..."
echo ""
./a.out $2
echo ""

# CLEANUP
