cmake_minimum_required(VERSION 3.20)
project(Armor_identification)


add_executable(Armor_identification main.cpp src/DNN_detect.cpp include/DNN_detect.h src/Armordetection.cpp include/Armordetection.h)
add_executable(my_ori my_ori.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(Armor_identification ${OpenCV_LIBS})










