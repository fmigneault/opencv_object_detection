#========================================
# JTGC (JTGC)
#========================================

cmake_minimum_required(VERSION 2.8)

# project
set(JTGC_PROJECT "JTGC")
project(${JTGC_PROJECT})

if(MSVC)
    set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
    add_definitions("/EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
endif()

# build type
if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()
set(CMAKE_CONFIG_DEBUG   DEBUG)
set(CMAKE_CONFIG_RELEASE RELEASE)
set(CMAKE_CONFIG $<$<CONFIG:Debug>:${CMAKE_CONFIG_DEBUG}>$<$<CONFIG:Release>:${CMAKE_CONFIG_RELEASE}>)
set(CMAKE_BUILD_TYPE     Release)

# Offer the user the choice of overriding the installation directories
set(DEFAULT_INSTALL_LIBRARY_DIR lib)
set(DEFAULT_INSTALL_BINARY_DIR  bin)
if(WIN32 AND NOT CYGWIN)
  set(DEFAULT_INSTALL_INCLUDE_DIR include)
  set(DEFAULT_INSTALL_CMAKE_DIR   cmake)
else()
  set(DEFAULT_INSTALL_INCLUDE_DIR include/${JTGC_PROJECT})
  set(DEFAULT_INSTALL_CMAKE_DIR   lib/cmake/${JTGC_PROJECT})
endif()
set(INSTALL_LIBRARY_DIR ${DEFAULT_INSTALL_LIBRARY_DIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINARY_DIR  ${DEFAULT_INSTALL_BINARY_DIR}  CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR ${DEFAULT_INSTALL_INCLUDE_DIR} CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR   ${DEFAULT_INSTALL_CMAKE_DIR}   CACHE PATH "Installation directory for CMake files")

# set JTGC paths
set(JTGC_ROOT_DIR        "${CMAKE_CURRENT_LIST_DIR}")
set(JTGC_EXECUTABLE_DIR  "${JTGC_ROOT_DIR}/bld")
set(JTGC_SOURCES_DIRS    "${JTGC_ROOT_DIR}/src")
set(JTGC_INCLUDE_DIRS    "${JTGC_ROOT_DIR}/inc")
set(JTGC_LIBRARY_DIR     "${JTGC_ROOT_DIR}/lib")
set(CMAKE_INSTALL_PREFIX    "${JTGC_ROOT_DIR}/install" CACHE STRING "")

# Make relative paths absolute (needed later on)
foreach(p LIBRARY BINARY INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# find header/source files
set(JTGC_SAMPLE_NAME "test_jetson_tegra_gstreamer_camera")
set(JTGC_HEADER_FILES "")
set(JTGC_SOURCE_FILES ${JTGC_SOURCE_FILES} ${JTGC_SOURCES_DIRS}/${JTGC_SAMPLE_NAME}.cpp)
set(JTGC_SOURCE_FILES ${JTGC_SOURCE_FILES} ${JTGC_SOURCES_DIRS}/jetson_tegra_gstreamer_camera.cpp)
set(JTGC_HEADER_FILES ${JTGC_HEADER_FILES} ${JTGC_INCLUDE_DIRS}/jetson_tegra_gstreamer_camera.hpp)

# JTGC configs
set(JTGC_INCLUDE_DIR       ${JTGC_INCLUDE_DIRS})
set(JTGC_LIBRARY_DEBUG     ${JTGC_PROJECT}${CMAKE_DEBUG_POSTFIX}${CMAKE_${JTGC_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(JTGC_LIBRARY_RELEASE   ${JTGC_PROJECT}${CMAKE_RELEASE_POSTFIX}${CMAKE_${JTGC_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(JTGC_LIBRARY_DEBUG     ${CMAKE_${JTGC_LIBRARY_TYPE}_LIBRARY_PREFIX}${JTGC_LIBRARY_DEBUG})
set(JTGC_LIBRARY_RELEASE   ${CMAKE_${JTGC_LIBRARY_TYPE}_LIBRARY_PREFIX}${JTGC_LIBRARY_RELEASE})

# JTGC root path (JTGCOptions)
add_definitions(-DJTGC_ROOT_PATH="${JTGC_ROOT_DIR}")

# find OpenMP
find_package(OpenMP)
if ("${OPENMP_FOUND}" OR "${OPENMP_C_FOUND}" OR "${OPENMP_CXX_FOUND}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(WITH_OPENMP ON)
endif()

# find Boost
##option(Boost_USE_STATIC_LIBS    "Use Boost static libraries"        OFF)
##option(Boost_USE_MULTITHREADED  "Use Boost multithreaded libraries" ON)
##option(Boost_USE_STATIC_RUNTIME "Use Boost static runtime linking"  OFF)
##find_package(Boost REQUIRED COMPONENTS filesystem system)
##if (${Boost_FOUND})
##    set(JTGC_INCLUDE_DIRS   ${JTGC_INCLUDE_DIRS}    ${Boost_INCLUDE_DIRS})
##    set(JTGC_LIBRARIES      ${JTGC_LIBRARIES}       ${Boost_LIBRARIES})
##    set(JTGC_LIBRARY_DIRS   ${JTGC_LIBRARY_DIRS}    ${Boost_LIBRARY_DIRS})
##endif()

# find OpenCV
find_package(OpenCV 3 REQUIRED)
set(JTGC_INCLUDE_DIRS   ${JTGC_INCLUDE_DIRS}    ${OpenCV_INCLUDE_DIRS})
set(JTGC_LIBRARIES      ${JTGC_LIBRARIES}       ${OpenCV_LIBRARIES})

# include/linking
add_definitions(${JTGC_LIBRARY_DEFINITION})
add_executable(${JTGC_SAMPLE_NAME} ${JTGC_LIBRARY_TYPE} ${JTGC_SOURCE_FILES} ${JTGC_HEADER_FILES})
if(MSVC)
    set_target_properties(${JTGC_SAMPLE_NAME} PROPERTIES LINKER_LANGUAGE C++)
endif()
target_include_directories(${JTGC_SAMPLE_NAME} PUBLIC ${JTGC_INCLUDE_DIRS})
target_link_libraries(${JTGC_SAMPLE_NAME} ${JTGC_LIBRARIES})
link_directories(${JTGC_LIBRARY_DIRS})

# fix config paths as required
string(REGEX REPLACE "\\\\" "/" INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_BINARY_DIR  ${INSTALL_BINARY_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_LIBRARY_DIR ${INSTALL_LIBRARY_DIR})

# update config file
##set(JTGC_CONFIG_INC_DIR ${INSTALL_INCLUDE_DIR})
##set(JTGC_CONFIG_BIN_DIR ${INSTALL_BINARY_DIR})
##set(JTGC_CONFIG_LIB_DIR ${INSTALL_LIBRARY_DIR})
##set(JTGC_CONFIG_LIB_DBG ${INSTALL_LIBRARY_DIR}/${JTGC_LIBRARY_DEBUG})
##set(JTGC_CONFIG_LIB_REL ${INSTALL_LIBRARY_DIR}/${JTGC_LIBRARY_RELEASE})
##set(JTGC_CONFIG_LIB_DEF ${JTGC_LIBRARY_DEFINITION})
##set(JTGC_CONFIG_CMK_DIR ${INSTALL_CMAKE_DIR})
##set(JTGC_CONFIG_FILE    ${JTGC_PROJECT}Config.cmake)
##if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
##    set(JTGC_CONFIG_PATH ${JTGC_CONFIG_FILE})
##else()
##    set(JTGC_CONFIG_PATH ${CMAKE_BINARY_DIR}/${JTGC_CONFIG_FILE})
##endif()
##file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
##configure_file(cmake/${JTGC_CONFIG_FILE}.in ${JTGC_CONFIG_PATH} @ONLY)

# add dependency find config files to install
##set(JTGC_FIND_CONFIG ${JTGC_FIND_CONFIG} "cmake/FindLibSVM.cmake")
##set(JTGC_FIND_CONFIG ${JTGC_FIND_CONFIG} "cmake/FindfeHOG.cmake")
##set(JTGC_FIND_CONFIG ${JTGC_FIND_CONFIG} "cmake/Findmvector.cmake")

# install
install(TARGETS ${JTGC_SAMPLE_NAME}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${JTGC_HEADER_FILES} DESTINATION ${INSTALL_INCLUDE_DIR})
##install(FILES ${JTGC_CONFIG_PATH}  DESTINATION ${INSTALL_CMAKE_DIR})
##install(FILES ${JTGC_FIND_CONFIG}  DESTINATION ${INSTALL_CMAKE_DIR})

