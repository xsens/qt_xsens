# Generated from moveblocks.pro.

cmake_minimum_required(VERSION 3.16)
project(moveblocks LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/animation/moveblocks")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS StateMachine)
find_package(Qt6 COMPONENTS Widgets)

qt_add_executable(moveblocks
    main.cpp
)
set_target_properties(moveblocks PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(moveblocks PUBLIC
    Qt::Core
    Qt::Gui
    Qt::StateMachine
    Qt::Widgets
)

install(TARGETS moveblocks
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
