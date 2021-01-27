cmake_minimum_required(VERSION 3.0)

# set version
set(PROJ_MAJOR_VERSION 1)
set(PROJ_MINOR_VERSION 0)
set(PROJ_PATCH_VERSION 0)
set(PROJ_VERSION
    ${PROJ_MAJOR_VERSION}.${PROJ_MINOR_VERSION}.${PROJ_PATCH_VERSION})
# set project name
project(PersonWithMoney VERSION ${PROJ_VERSION} LANGUAGES CXX)

# exe
add_executable(${PROJECT_NAME}
    main.cpp
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Boost::boost
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src/common
)

# install information
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
