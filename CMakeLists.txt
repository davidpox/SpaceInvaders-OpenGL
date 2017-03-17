project(spaceinvaders)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

file(GLOB SOURCES 
	src/*.cpp
	src/*.h
)

ADD_EXECUTABLE(spaceinvaders ${SOURCES})

TARGET_LINK_LIBRARIES(spaceinvaders ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "spaceinvaders")

FILE(COPY assets/ DESTINATION bin/assets)
