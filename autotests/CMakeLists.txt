include_directories(.. ${CMAKE_CURRENT_BINARY_DIR})

add_executable(filedatatest
    filedatatest.cpp
)
target_link_libraries(filedatatest
    libinqludeclient
    Qt5::Core
    Qt5::Network
    Qt5::Test
)
add_test(NAME filedatatest COMMAND filedatatest)

