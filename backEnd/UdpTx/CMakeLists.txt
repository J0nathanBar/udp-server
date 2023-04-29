cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 20)


project(UdpTx)



include_directories(${Wirehair_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
file(GLOB_RECURSE CPP_FILES ../SharedClasses/*.cpp )
add_executable(${PROJECT_NAME} main.cpp UdpTX.cpp ${CPP_FILES} )
find_package(Boost COMPONENTS filesystem serialization thread Wirehair)
#find_package(mysqlclient REQUIRED)
target_link_libraries(${PROJECT_NAME} -lwirehair ${Boost_FILESYSTEM_LIBRARY} ${Boost_LIBRARIES} mysqlclient)
