cmake_minimum_required (VERSION 2.6)
project (TinyXML)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_C_FLAGS "-O2")

add_library(tinyxml
tinyxml2.cpp
)
