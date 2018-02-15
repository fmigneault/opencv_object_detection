#========================================
# OpenCV's YOLO sample (CV_YOLO)
#========================================

cmake_minimum_required(VERSION 2.8)

# project
set(CV_YOLO_PROJECT "CV_YOLO")
project(${CV_YOLO_PROJECT})

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
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")

# Offer the user the choice of overriding the installation directories
set(DEFAULT_INSTALL_LIBRARY_DIR lib)
set(DEFAULT_INSTALL_BINARY_DIR  bin)
if(WIN32 AND NOT CYGWIN)
  set(DEFAULT_INSTALL_INCLUDE_DIR include)
  set(DEFAULT_INSTALL_CMAKE_DIR   cmake)
else()
  set(DEFAULT_INSTALL_INCLUDE_DIR include/${CV_YOLO_PROJECT})
  set(DEFAULT_INSTALL_CMAKE_DIR   lib/cmake/${CV_YOLO_PROJECT})
endif()
set(INSTALL_LIBRARY_DIR ${DEFAULT_INSTALL_LIBRARY_DIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINARY_DIR  ${DEFAULT_INSTALL_BINARY_DIR}  CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR ${DEFAULT_INSTALL_INCLUDE_DIR} CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR   ${DEFAULT_INSTALL_CMAKE_DIR}   CACHE PATH "Installation directory for CMake files")

# set CV_YOLO paths
set(CV_YOLO_ROOT_DIR        "${CMAKE_CURRENT_LIST_DIR}")
set(CV_YOLO_EXECUTABLE_DIR  "${CV_YOLO_ROOT_DIR}/bld")
set(CV_YOLO_SOURCES_DIRS    "${CV_YOLO_ROOT_DIR}/src")
set(CV_YOLO_INCLUDE_DIRS    "${CV_YOLO_ROOT_DIR}/inc")
set(CV_YOLO_LIBRARY_DIR     "${CV_YOLO_ROOT_DIR}/lib")
set(CMAKE_INSTALL_PREFIX    "${CV_YOLO_ROOT_DIR}/install" CACHE STRING "")

# Make relative paths absolute (needed later on)
foreach(p LIBRARY BINARY INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# find header/source files
set(CV_YOLO_SAMPLE_NAME "yolo_object_detection")
set(CV_YOLO_HEADER_FILES "")
set(CV_YOLO_SOURCE_FILES ${CV_YOLO_SOURCE_FILES} ${CV_YOLO_SOURCES_DIRS}/${CV_YOLO_SAMPLE_NAME}.cpp)

# CV_YOLO configs
set(CV_YOLO_INCLUDE_DIR       ${CV_YOLO_INCLUDE_DIRS})
set(CV_YOLO_LIBRARY_DEBUG     ${CV_YOLO_PROJECT}${CMAKE_DEBUG_POSTFIX}${CMAKE_${CV_YOLO_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(CV_YOLO_LIBRARY_RELEASE   ${CV_YOLO_PROJECT}${CMAKE_RELEASE_POSTFIX}${CMAKE_${CV_YOLO_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(CV_YOLO_LIBRARY_DEBUG     ${CMAKE_${CV_YOLO_LIBRARY_TYPE}_LIBRARY_PREFIX}${CV_YOLO_LIBRARY_DEBUG})
set(CV_YOLO_LIBRARY_RELEASE   ${CMAKE_${CV_YOLO_LIBRARY_TYPE}_LIBRARY_PREFIX}${CV_YOLO_LIBRARY_RELEASE})

# CV_YOLO root path (CV_YOLOOptions)
add_definitions(-DCV_YOLO_ROOT_PATH="${CV_YOLO_ROOT_DIR}")

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
##    set(CV_YOLO_INCLUDE_DIRS   ${CV_YOLO_INCLUDE_DIRS}    ${Boost_INCLUDE_DIRS})
##    set(CV_YOLO_LIBRARIES      ${CV_YOLO_LIBRARIES}       ${Boost_LIBRARIES})
##    set(CV_YOLO_LIBRARY_DIRS   ${CV_YOLO_LIBRARY_DIRS}    ${Boost_LIBRARY_DIRS})
##endif()

# find OpenCV
find_package(OpenCV 3 REQUIRED)
set(CV_YOLO_INCLUDE_DIRS   ${CV_YOLO_INCLUDE_DIRS}    ${OpenCV_INCLUDE_DIRS})
set(CV_YOLO_LIBRARIES      ${CV_YOLO_LIBRARIES}       ${OpenCV_LIBRARIES})

# include/linking
add_definitions(${CV_YOLO_LIBRARY_DEFINITION})
add_executable(${CV_YOLO_SAMPLE_NAME} ${CV_YOLO_LIBRARY_TYPE} ${CV_YOLO_SOURCE_FILES} ${CV_YOLO_HEADER_FILES})
if(MSVC)
    set_target_properties(${CV_YOLO_SAMPLE_NAME} PROPERTIES LINKER_LANGUAGE C++)
endif()
target_include_directories(${CV_YOLO_SAMPLE_NAME} PUBLIC ${CV_YOLO_INCLUDE_DIRS})
target_link_libraries(${CV_YOLO_SAMPLE_NAME} ${CV_YOLO_LIBRARIES})
link_directories(${CV_YOLO_LIBRARY_DIRS})

# fix config paths as required
string(REGEX REPLACE "\\\\" "/" INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_BINARY_DIR  ${INSTALL_BINARY_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_LIBRARY_DIR ${INSTALL_LIBRARY_DIR})

# update config file
##set(CV_YOLO_CONFIG_INC_DIR ${INSTALL_INCLUDE_DIR})
##set(CV_YOLO_CONFIG_BIN_DIR ${INSTALL_BINARY_DIR})
##set(CV_YOLO_CONFIG_LIB_DIR ${INSTALL_LIBRARY_DIR})
##set(CV_YOLO_CONFIG_LIB_DBG ${INSTALL_LIBRARY_DIR}/${CV_YOLO_LIBRARY_DEBUG})
##set(CV_YOLO_CONFIG_LIB_REL ${INSTALL_LIBRARY_DIR}/${CV_YOLO_LIBRARY_RELEASE})
##set(CV_YOLO_CONFIG_LIB_DEF ${CV_YOLO_LIBRARY_DEFINITION})
##set(CV_YOLO_CONFIG_CMK_DIR ${INSTALL_CMAKE_DIR})
##set(CV_YOLO_CONFIG_FILE    ${CV_YOLO_PROJECT}Config.cmake)
##if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
##    set(CV_YOLO_CONFIG_PATH ${CV_YOLO_CONFIG_FILE})
##else()
##    set(CV_YOLO_CONFIG_PATH ${CMAKE_BINARY_DIR}/${CV_YOLO_CONFIG_FILE})
##endif()
##file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
##configure_file(cmake/${CV_YOLO_CONFIG_FILE}.in ${CV_YOLO_CONFIG_PATH} @ONLY)

# add dependency find config files to install
##set(CV_YOLO_FIND_CONFIG ${CV_YOLO_FIND_CONFIG} "cmake/FindLibSVM.cmake")
##set(CV_YOLO_FIND_CONFIG ${CV_YOLO_FIND_CONFIG} "cmake/FindfeHOG.cmake")
##set(CV_YOLO_FIND_CONFIG ${CV_YOLO_FIND_CONFIG} "cmake/Findmvector.cmake")

# install
install(TARGETS ${CV_YOLO_SAMPLE_NAME}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${CV_YOLO_HEADER_FILES} DESTINATION ${INSTALL_INCLUDE_DIR})
##install(FILES ${CV_YOLO_CONFIG_PATH}  DESTINATION ${INSTALL_CMAKE_DIR})
##install(FILES ${CV_YOLO_FIND_CONFIG}  DESTINATION ${INSTALL_CMAKE_DIR})

