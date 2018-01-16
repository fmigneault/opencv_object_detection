# Install script for directory: /home/nvidia/opencv_dnn_detect/jetson_tegra_gstreamer_camera

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/nvidia/opencv_dnn_detect/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/nvidia/opencv_dnn_detect/install/bin" TYPE EXECUTABLE FILES "/home/nvidia/opencv_dnn_detect/jetson_tegra_gstreamer_camera/bld/jetson_tegra_gstreamer_camera")
  if(EXISTS "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera"
         OLD_RPATH "/usr/local/cuda-8.0/lib64:/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/nvidia/opencv_dnn_detect/install/bin/jetson_tegra_gstreamer_camera")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/nvidia/opencv_dnn_detect/jetson_tegra_gstreamer_camera/bld/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
