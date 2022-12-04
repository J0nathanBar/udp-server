cmake_minimum_required(VERSION 3.22.1)

project(UdpTx)

#SET(CMAKE_CXX_FLAGS "-DBOOST_NO_CXX11_SCOPED_ENUMS")

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} main.cpp UdpTX.cpp JsonParser.cpp File.cpp FileParser.cpp)

find_package(Boost COMPONENTS filesystem serialization)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_FILESYSTEM_LIBRARY} ${Boost_LIBRARIES})