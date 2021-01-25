#!/bin/bash
pushd /code/lammps/build >> /dev/null

cmake -GNinja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_C_FLAGS_DEBUG="-g -O0" \
    -DCMAKE_CXX_FLAGS_DEBUG="-g -O0" \
    -DCMAKE_C_COMPILER=/usr/bin/clang-10 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-10 \
    -DCMAKE_INSTALL_PREFIX=/code/lammps/install-native \
    /code/lammps/cmake

popd >> /dev/null

# export LD_LIBRARY_PATH=/build/faabric/install/lib; ninja

# ninja install
