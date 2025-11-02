#!/bin/bash
mkdir -p build

cmake -S . -B build
cmake --build build &&\
./build/tests

TEST_RESULT=$?
if [ $TEST_RESULT -ne 0 ]; then
    echo "One or more tests failed, aborting app launch."
    exit $TEST_RESULT
fi
./build/asteroid
