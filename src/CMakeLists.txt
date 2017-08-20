include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(objs OBJECT hcsr04.cpp usensor_array.cpp)

#these are all compiled the same way
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=gnu++11")
set(MAIN_PROGRAM main)
set(CORELIBS pigpio pthread)
set(BINARY_NAME uarray)



add_executable(${BINARY_NAME} $<TARGET_OBJECTS:objs>  main.cpp)

target_link_libraries(${BINARY_NAME} ${CORELIBS})


install(TARGETS ${BINARY_NAME} DESTINATION bin)
