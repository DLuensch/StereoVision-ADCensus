project(ImageRectify)
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

#add cmake script files
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_work")

FIND_PACKAGE(OpenCV 2.4 REQUIRED)
SET(LIBS ${LIBS} ${OpenCV_LIBS})

# Under Windows the system variable "BOOST_ROOT" must be set to the location of the root directory of Boost.
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC ON)
endif(WIN32)
SET(Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0")
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.40.0 COMPONENTS filesystem system REQUIRED)
set(BOOST_FOUND ${Boost_FOUND})
set(BOOST_INCLUDE_DIRS "${Boost_INCLUDE_DIR}")
set(BOOST_LIBRARY_DIRS "${Boost_LIBRARY_DIRS}")
set(BOOST_LIBRARIES ${Boost_LIBRARIES})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

#check if libconfig is present (FindConfig.cmake and FindConfig++.cmake files) and edit for your system (Linux, Windows)!
#FIND_PACKAGE(Config REQUIRED)
FIND_PACKAGE(Config++ REQUIRED)
INCLUDE_DIRECTORIES(${LIBCONFIG_INCLUDE_DIR})
SET(LIBS ${LIBS} ${LIBCONFIG_LIBRARIES})

aux_source_directory(./src SRC_LIST)
add_executable(ImageRectify ${SRC_LIST})
target_link_libraries(ImageRectify ${LIBS})
