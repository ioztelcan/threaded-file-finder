#!/usr/bin/env bash

PROJECT_DIR=$(pwd)

if [ "$1" == "Debug" ]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=Release
fi

BUILD_DIR=${PROJECT_DIR}/build/${BUILD_TYPE}

mkdir -p ${BUILD_DIR}
cmake -G "Unix Makefiles" -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DENABLE_TESTING=ON
cmake --build ${BUILD_DIR} --target build_and_test -j 6