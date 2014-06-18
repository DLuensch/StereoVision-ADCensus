cmake_minimum_required(VERSION 2.8)

PROJECT( OpenCVBM )
FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE( Qt4 REQUIRED )

#set(CMAKE_BUILD_TYPE Debug)

SET( QT_USE-QTGUI TRUE )

SET( OpenCVBM_SOURCES src/main.cpp
                        src/OpenCVBMWindow.cpp
                        src/blockmatchingimage.cpp
                        src/blockmatching.cpp
                        src/sgblockmatching.cpp)
SET( OpenCVBM_HEADERS src/OpenCVBMWindow.h
                        src/blockmatchingimage.h
                        src/blockmatching.h
                        src/sgblockmatching.h)
SET( OpenCVBM_FORMS src/OpenCVBMWindow.ui
                        src/blockmatchingimage.ui)

QT4_AUTOMOC( ${OpenCVBM_SOURCES} )
QT4_WRAP_CPP( OpenCVBM_HEADERS_MOC ${OpenCVBM_HEADERS} )
QT4_WRAP_UI( OpenCVBM_FORMS_HEADERS ${OpenCVBM_FORMS} )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

add_executable( OpenCVBM ${OpenCVBM_SOURCES} 
    ${OpenCVBM_HEADERS_MOC} 
    ${OpenCVBM_FORMS_HEADERS} )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

TARGET_LINK_LIBRARIES( OpenCVBM ${QT_LIBRARIES} ${OpenCV_LIBS} )
