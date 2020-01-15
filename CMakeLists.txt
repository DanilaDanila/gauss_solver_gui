cmake_minimum_required(VERSION 3.5)

project(gauss_solver LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(gauss_solver SHARED
    main.cpp
    widget.cpp
    widget.h
  )
else()
  add_executable(gauss_solver
    main.cpp
    widget.cpp
    widget.h

    # My libs
    solver/fract.cpp
    solver/fract.hpp
    #solver/slau.cpp
    solver/slau.hpp
    solver/tools.hpp
    #solver/vector.cpp
    solver/vector.hpp
  )
endif()

target_link_libraries(gauss_solver PRIVATE Qt5::Widgets)
