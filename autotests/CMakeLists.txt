include_directories(.. ${CMAKE_CURRENT_BINARY_DIR})

add_executable(filedatatest
    filedatatest.cpp
    ../dataprovider.cpp
    ../filedataprovider.cpp
    ../httpdataprovider.cpp
    ../commandhandler.cpp
)
target_link_libraries(filedatatest
    Qt5::Core
    Qt5::Network
    Qt5::Test
)
add_test(NAME filedatatest COMMAND filedatatest)

