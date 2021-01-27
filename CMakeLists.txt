cmake_minimum_required(VERSION 3.0)

project(BoostStateMachine)

# ##############################################################################
# policys

# CMP0074: find_package uses PackageName_ROOT variables.
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# find packages
find_package(Boost 1.60 REQUIRED)

# add directories
add_subdirectory(src)
