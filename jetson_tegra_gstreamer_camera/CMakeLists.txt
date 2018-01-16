#========================================
# GJTC (GJTC)
#========================================

cmake_minimum_required(VERSION 2.8)

# project
set(GJTC_PROJECT "GJTC")
project(${GJTC_PROJECT})

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

# Offer the user the choice of overriding the installation directories
set(DEFAULT_INSTALL_LIBRARY_DIR lib)
set(DEFAULT_INSTALL_BINARY_DIR  bin)
if(WIN32 AND NOT CYGWIN)
  set(DEFAULT_INSTALL_INCLUDE_DIR include)
  set(DEFAULT_INSTALL_CMAKE_DIR   cmake)
else()
  set(DEFAULT_INSTALL_INCLUDE_DIR include/${GJTC_PROJECT})
  set(DEFAULT_INSTALL_CMAKE_DIR   lib/cmake/${GJTC_PROJECT})
endif()
set(INSTALL_LIBRARY_DIR ${DEFAULT_INSTALL_LIBRARY_DIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINARY_DIR  ${DEFAULT_INSTALL_BINARY_DIR}  CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR ${DEFAULT_INSTALL_INCLUDE_DIR} CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR   ${DEFAULT_INSTALL_CMAKE_DIR}   CACHE PATH "Installation directory for CMake files")

# set GJTC paths
set(GJTC_ROOT_DIR        "${CMAKE_CURRENT_LIST_DIR}")
set(GJTC_EXECUTABLE_DIR  "${GJTC_ROOT_DIR}/bld")
set(GJTC_SOURCES_DIRS    "${GJTC_ROOT_DIR}/src")
set(GJTC_INCLUDE_DIRS    "${GJTC_ROOT_DIR}/inc")
set(GJTC_LIBRARY_DIR     "${GJTC_ROOT_DIR}/lib")
set(CMAKE_INSTALL_PREFIX    "${GJTC_ROOT_DIR}/install" CACHE STRING "")

# Make relative paths absolute (needed later on)
foreach(p LIBRARY BINARY INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# find header/source files
set(GJTC_SAMPLE_NAME "jetson_tegra_gstreamer_camera")
set(GJTC_HEADER_FILES "")
set(GJTC_SOURCE_FILES ${GJTC_SOURCE_FILES} ${GJTC_SOURCES_DIRS}/${GJTC_SAMPLE_NAME}.cpp)

# GJTC configs
set(GJTC_INCLUDE_DIR       ${GJTC_INCLUDE_DIRS})
set(GJTC_LIBRARY_DEBUG     ${GJTC_PROJECT}${CMAKE_DEBUG_POSTFIX}${CMAKE_${GJTC_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(GJTC_LIBRARY_RELEASE   ${GJTC_PROJECT}${CMAKE_RELEASE_POSTFIX}${CMAKE_${GJTC_LIBRARY_TYPE}_LIBRARY_SUFFIX})
set(GJTC_LIBRARY_DEBUG     ${CMAKE_${GJTC_LIBRARY_TYPE}_LIBRARY_PREFIX}${GJTC_LIBRARY_DEBUG})
set(GJTC_LIBRARY_RELEASE   ${CMAKE_${GJTC_LIBRARY_TYPE}_LIBRARY_PREFIX}${GJTC_LIBRARY_RELEASE})

# GJTC root path (GJTCOptions)
add_definitions(-DGJTC_ROOT_PATH="${GJTC_ROOT_DIR}")

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
##    set(GJTC_INCLUDE_DIRS   ${GJTC_INCLUDE_DIRS}    ${Boost_INCLUDE_DIRS})
##    set(GJTC_LIBRARIES      ${GJTC_LIBRARIES}       ${Boost_LIBRARIES})
##    set(GJTC_LIBRARY_DIRS   ${GJTC_LIBRARY_DIRS}    ${Boost_LIBRARY_DIRS})
##endif()

# find OpenCV
find_package(OpenCV 3 REQUIRED)
set(GJTC_INCLUDE_DIRS   ${GJTC_INCLUDE_DIRS}    ${OpenCV_INCLUDE_DIRS})
set(GJTC_LIBRARIES      ${GJTC_LIBRARIES}       ${OpenCV_LIBRARIES})

# include/linking
add_definitions(${GJTC_LIBRARY_DEFINITION})
add_executable(${GJTC_SAMPLE_NAME} ${GJTC_LIBRARY_TYPE} ${GJTC_SOURCE_FILES} ${GJTC_HEADER_FILES})
if(MSVC)
    set_target_properties(${GJTC_SAMPLE_NAME} PROPERTIES LINKER_LANGUAGE C++)
endif()
target_include_directories(${GJTC_SAMPLE_NAME} PUBLIC ${GJTC_INCLUDE_DIRS})
target_link_libraries(${GJTC_SAMPLE_NAME} ${GJTC_LIBRARIES})
link_directories(${GJTC_LIBRARY_DIRS})

# fix config paths as required
string(REGEX REPLACE "\\\\" "/" INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_BINARY_DIR  ${INSTALL_BINARY_DIR})
string(REGEX REPLACE "\\\\" "/" INSTALL_LIBRARY_DIR ${INSTALL_LIBRARY_DIR})

# update config file
##set(GJTC_CONFIG_INC_DIR ${INSTALL_INCLUDE_DIR})
##set(GJTC_CONFIG_BIN_DIR ${INSTALL_BINARY_DIR})
##set(GJTC_CONFIG_LIB_DIR ${INSTALL_LIBRARY_DIR})
##set(GJTC_CONFIG_LIB_DBG ${INSTALL_LIBRARY_DIR}/${GJTC_LIBRARY_DEBUG})
##set(GJTC_CONFIG_LIB_REL ${INSTALL_LIBRARY_DIR}/${GJTC_LIBRARY_RELEASE})
##set(GJTC_CONFIG_LIB_DEF ${GJTC_LIBRARY_DEFINITION})
##set(GJTC_CONFIG_CMK_DIR ${INSTALL_CMAKE_DIR})
##set(GJTC_CONFIG_FILE    ${GJTC_PROJECT}Config.cmake)
##if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
##    set(GJTC_CONFIG_PATH ${GJTC_CONFIG_FILE})
##else()
##    set(GJTC_CONFIG_PATH ${CMAKE_BINARY_DIR}/${GJTC_CONFIG_FILE})
##endif()
##file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
##configure_file(cmake/${GJTC_CONFIG_FILE}.in ${GJTC_CONFIG_PATH} @ONLY)

# add dependency find config files to install
##set(GJTC_FIND_CONFIG ${GJTC_FIND_CONFIG} "cmake/FindLibSVM.cmake")
##set(GJTC_FIND_CONFIG ${GJTC_FIND_CONFIG} "cmake/FindfeHOG.cmake")
##set(GJTC_FIND_CONFIG ${GJTC_FIND_CONFIG} "cmake/Findmvector.cmake")

# install
install(TARGETS ${GJTC_SAMPLE_NAME}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${GJTC_HEADER_FILES} DESTINATION ${INSTALL_INCLUDE_DIR})
##install(FILES ${GJTC_CONFIG_PATH}  DESTINATION ${INSTALL_CMAKE_DIR})
##install(FILES ${GJTC_FIND_CONFIG}  DESTINATION ${INSTALL_CMAKE_DIR})

