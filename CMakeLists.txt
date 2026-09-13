cmake_minimum_required(VERSION 3.21)
project(ClariSweep VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets Concurrent)
if(MSVC)
    add_compile_options(/utf-8)
endif()

qt_add_executable(clarisweep main.cpp)
target_link_libraries(clarisweep PRIVATE Qt6::Widgets Qt6::Concurrent)
set_target_properties(clarisweep PROPERTIES WIN32_EXECUTABLE TRUE MACOSX_BUNDLE TRUE)
enable_testing()
add_executable(scanner_test scanner_test.cpp)
target_link_libraries(scanner_test PRIVATE Qt6::Widgets Qt6::Concurrent)
add_test(NAME scan_filters COMMAND scanner_test)
