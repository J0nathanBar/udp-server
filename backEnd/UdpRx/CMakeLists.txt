cmake_minimum_required(VERSION 3.22.1)

project(UdpRx)


include_directories(${Boost_INCLUDE_DIRS})
file(GLOB_RECURSE CPP_FILES ../SharedClasses/*.cpp )


add_executable(${PROJECT_NAME} Main.cpp UdpReceiver.cpp ${CPP_FILES})

find_package(Boost COMPONENTS filesystem serialization)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_FILESYSTEM_LIBRARY} ${Boost_LIBRARIES})


