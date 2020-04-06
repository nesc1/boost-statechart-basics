cmake_minimum_required(VERSION 3.0)

project(BoostStateMachine VERSION "1.0.0")

# ##############################################################################
# policys

# CMP0074: find_package uses PackageName_ROOT variables.
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# find packages
find_package(Boost 1.60 REQUIRED)

# exe
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)

# install information
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
