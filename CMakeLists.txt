cmake_minimum_required(VERSION 2.8)

project(simpleServer)

set (CMAKE_CXX_STANDARD 14)

set(SOURCES
  src/main.cpp
  src/simplesocket.cpp)

set(HEADERS
  include/simplesocket.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
