cmake_minimum_required(VERSION 2.6)
project(inqludeclient)
find_package(Qt5 CONFIG REQUIRED Core Network Test)

set(CMAKE_AUTOMOC true)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(autotests)

set(inqludeclient_SRCS
   inqludeclient.cpp
   dataprovider.cpp
   filedataprovider.cpp
   httpdataprovider.cpp
   main.cpp)

add_executable(inqlude-client ${inqludeclient_SRCS})
target_link_libraries(inqlude-client
   Qt5::Core
   Qt5::Network
)

install(TARGETS inqlude-client RUNTIME DESTINATION bin)
