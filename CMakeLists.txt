cmake_minimum_required(VERSION 3.16)
project(CO5625_202425_CLion)

set(CMAKE_CXX_STANDARD 20)

add_executable(Debug "template/Source.cpp"
        Competition.cpp
        Prototype.cpp
        Simulator.cpp
)

add_executable(Feedback "template/Feedback.cpp"
        Competition.cpp
        Prototype.cpp
        Simulator.cpp
)

add_executable(Submit "template/Submit.cpp")

add_executable(Sketchbook "Sketchbook.cpp"
        Competition.cpp
        Prototype.cpp
        Simulator.cpp
)

configure_file(template/tests.txt template/tests.txt COPYONLY)
configure_file(Cities.txt Cities.txt COPYONLY)
TARGET_LINK_LIBRARIES(Feedback pthread)
TARGET_LINK_LIBRARIES(Debug  pthread)
TARGET_LINK_LIBRARIES(Sketchbook pthread)