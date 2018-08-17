#JSONCPP library
# from https://github.com/open-source-parsers/jsoncpp
cmake_minimum_required (VERSION 2.6)
project (JsonCPP)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_C_FLAGS "-O2")
add_library(jsoncpp
jsoncpp.cpp
)
