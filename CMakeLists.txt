cmake_minimum_required(VERSION 2.8)

project(simpleServer)

set(SOURCES
  src/main.cpp
  src/simplesocket.cpp
  src/ipconfiguration.cpp)

set(HEADERS
  include/simplesocket.h
  include/ipconfiguration.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
