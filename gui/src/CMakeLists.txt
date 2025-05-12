cmake_minimum_required(VERSION 3.16)
project(GUI)


# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)


# Adjust to your installed Qt6 path
set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.9.0/gcc_64")


# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)


# Collect source and header files
set(SOURCES
    main.cpp
    main_window/main_window.cpp
)


set(HEADERS
    main_window/main_window.h
)


# Enable automatic MOC (Meta-Object Compiler) for Qt signals/slots
set(CMAKE_AUTOMOC ON)


# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


# Link Qt libraries to the executable
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
)
