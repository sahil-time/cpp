#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "Usage: \"$0 main.cpp <num> [options]\""
    echo "Options:"
    echo "  -f, --force     Forces re-cloning of thirdparty and build"
    echo ""
  exit 1 # Exit with an error code
fi

echo "[COMPILE_LOG] ------------------------------------------"

# CLEANUP FUNCTIONS
function cleanup_transient_files { # Cleanup Compiler created files
    rm *.gch >/dev/null 2>&1
}
function cleanup_thirdparty_libs { # Cleanup Thirdparty Libs
    rm -rf thirdparty/* >/dev/null 2>&1
}
function cleanup_staged_deps { # Cleanup Staged Libs and Headers
    rm -rf staged/hdrs/* > /dev/null 2>&1
    rm -rf staged/libs/* > /dev/null 2>&1
}
function cleanup_main { # Cleanup Binary
    cleanup_transient_files
    rm a.out >/dev/null 2>&1
}

# GITHUB CLONED & BUILT LIBS [ if '-f' OR just STAGE the libs ]
function clone_and_build_thirdparty_libs {
    for arg in "$@"; do
        if [[ "$arg" == "-f" ]]; then
            echo "[COMPILE_LOG] Cleaning, Cloning and Building Thirdparty libs..."
            cleanup_thirdparty_libs
            cd thirdparty

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

            echo "[COMPILE_LOG] - Cloning 'nlohmann/json.hpp'..."
            git clone https://github.com/nlohmann/json.git

            cd ../
            break
        fi
    done

    echo "[COMPILE_LOG] Staging Thirdparty libs & headers..."
    cp -f thirdparty/libassert/build/libassert.a staged/libs/
    cp -f thirdparty/cpptrace/build/lib/libzstd.a staged/libs/
    cp -f thirdparty/cpptrace/build/lib/libdwarf.a staged/libs/
    cp -f thirdparty/cpptrace/build/lib/libcpptrace.a staged/libs/
    cp -f thirdparty/json/single_include/nlohmann/json.hpp staged/hdrs/

}

# PERSONAL BUILT LIBS/BINARIES [ STORE IN "staged/libs" ]
function build_personal_libs {
    echo "[COMPILE_LOG] Building & Staging Personal libs/binaries..."

    if [[ "$1" == 1 ]]; then
        echo "[COMPILE_LOG]  - Building 'number.cpp'..."
        clang++ -std=c++17 \
            -DGITHUB_ASSERT \
            -g \
            -O0 \
            -Wall -Wextra -Werror \
            -lz \
            -L./staged/libs -lcpptrace -lzstd -ldwarf -I./thirdparty/cpptrace/include \
            -L./staged/libs -lassert -I./thirdparty/libassert/include \
            -shared -fPIC src/number.cpp -I./include -o staged/libs/libnumber.so

        #    echo "[COMPILE_LOG]  - Building 'lockfreeq.cpp'..."
        #    clang++ -std=c++17 \
        #        -O0 \
        #        -Wall -Wextra -Werror \
        #        -shared -fPIC libs/lockfreeq.cpp -I./libs -o personal_libs/liblockfreeq.so

    fi

    # STAGE ALL REQUIRED HEADERS
    echo "[COMPILE_LOG]  - Staging headers from 'include' to 'staged/hdrs' ..."
    cp -f include/number.hpp staged/hdrs/.
    cp -f include/assert.hpp staged/hdrs/.
    cp -f include/packet.hpp staged/hdrs/.
    cp -f include/cable.hpp staged/hdrs/.
    cp -f include/router.hpp staged/hdrs/.
    cp -f include/lockfreeq.hpp staged/hdrs/.
}

# CHECKSUM
force_build=0
if [ -f "staged/mychecksums.txt" ]; then
    if md5sum -c staged/mychecksums.txt 2>&1 | grep -q "FAILED"; then
        echo "[COMPILE_LOG] 'md5sum' detected changed files, so rebuilding Personal Libs ..."
        find src/ include/ -type f -exec md5sum {} \; > staged/mychecksums.txt
        force_build=1
    fi
else
    # CREATE A NEW 'md5sum' & FORCE BUILD LIBS
    echo "[COMPILE_LOG] No checksum file detected, creating a new one and building Personal Libs ..."
    cleanup_staged_deps
    find src/ include/ -type f -exec md5sum {} \; > staged/mychecksums.txt
    force_build=1
fi

# BUILD DEPENDENCIES
clone_and_build_thirdparty_libs "$@" # Always build first
build_personal_libs $force_build

# BUILD MAIN
cleanup_main
echo "[COMPILE_LOG] Building 'main.cpp'..."
clang++ -std=c++17 \
    -DGITHUB_ASSERT \
    -g \
    -O0 \
    -Wall -Wextra -Werror \
    -lz \
    -L./staged/libs -lcpptrace -lzstd -ldwarf -I./thirdparty/cpptrace/include \
    -L./staged/libs -lassert -I./thirdparty/libassert/include \
    -L./staged/libs -lnumber -I./staged/hdrs \
    $1

# RUN EXECUTABLE
echo "[COMPILE_LOG] Running '$1' executable..."
echo "[COMPILE_LOG] ------------------------------------------"
echo ""
./a.out $2
echo ""
