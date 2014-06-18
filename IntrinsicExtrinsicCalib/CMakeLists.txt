project(IntrinsicExtrinsicCalib)
cmake_minimum_required(VERSION 2.8)

#add cmake script files
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_work")

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Under Windows the system variable "OPENCV_ROOT" must be set to the location of the root directory of OpenCV.
find_package(OpenCV 2.4 REQUIRED)
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

#Eigen3 Windows only! The following lines have no effect under Linux!
#Use the defined system variable "EIGEN_ROOT" to the root directory of eigen or set it manually.
#SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN_ROOT}" )
SET( EIGEN3_INCLUDE_DIR "C:/Frameworks/Eigen" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN_ROOT to the root directory of your Eigen3 installation or set manually." )
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

FIND_PACKAGE(Config++ REQUIRED)
INCLUDE_DIRECTORIES(${LIBCONFIG_INCLUDE_DIR})
SET(LIBS ${LIBS} ${LIBCONFIG_LIBRARIES})

add_executable(extrinsic extrinsic.cpp
                         intrinsicextrinsiccalib.cpp
                         paramcombination.cpp)
target_link_libraries(extrinsic ${LIBS})

add_executable(intrinsic intrinsic.cpp
                         intrinsicextrinsiccalib.cpp
                         paramcombination.cpp)
target_link_libraries(intrinsic ${LIBS})


