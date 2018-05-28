cmake_minimum_required(VERSION 2.8.3)
project(auto_car)
## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)
find_package(ZED 2.2 REQUIRED)
find_package(CUDA 8.0 REQUIRED)
find_package(OpenCV REQUIRED)

################################################
## Declare ROS messages, services and actions ##
################################################
## Generate messages in the 'msg' folder
add_message_files(
   FILES
   MsgControl.msg
)
## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
)
###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES auto_car
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${ZED_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/auto_car.cpp
# )
add_definitions(-std=c++11)
set(LINK_LIBRARIES
		${catkin_LIBRARIES}
		${ZED_LIBRARIES}
		${CUDA_LIBRARIES} ${CUDA_nppi_LIBRARY} ${CUDA_npps_LIBRARY}
		${OpenCV_LIBRARIES}
)

add_executable(auto_car src/main.cpp src/my_ZED.cpp src/my_CV.cpp)
add_dependencies(auto_car ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(auto_car ${LINK_LIBRARIES})
