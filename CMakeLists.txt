cmake_minimum_required(VERSION 2.8.10)


set(PROJECT_NAME ftdi_unitTests)
project(${PROJECT_NAME})





set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "-std=c++11")
if ( UNIX )
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif ( UNIX )



add_subdirectory(ftdi)


add_executable(${PROJECT_NAME} unitTests/unitTests_ftdi.cpp)
target_link_libraries (${PROJECT_NAME} ftdi boost_unit_test_framework)

