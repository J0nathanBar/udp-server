cmake_minimum_required(VERSION 3.22.1)

project(UdpRx)

add_executable(${PROJECT_NAME} Main.cpp UdpReceiver.cpp)