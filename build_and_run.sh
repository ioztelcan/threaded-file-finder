#!/usr/bin/env bash

PROJECT_DIR=$(pwd)

if [ "$1" == "Debug" ]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=Release
fi

BUILD_DIR=${PROJECT_DIR}/build/${BUILD_TYPE}
BIN_NAME="file_finder"

mkdir -p ${BUILD_DIR}
cmake -G "Unix Makefiles" -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DENABLE_TESTING=OFF
cmake --build ${BUILD_DIR} --target ${BIN_NAME}
${BUILD_DIR}/src/${BIN_NAME}