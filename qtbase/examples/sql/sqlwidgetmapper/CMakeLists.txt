# Generated from sqlwidgetmapper.pro.

cmake_minimum_required(VERSION 3.16)
project(sqlwidgetmapper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/sql/sqlwidgetmapper")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Sql)
find_package(Qt6 COMPONENTS Widgets)

qt_add_executable(sqlwidgetmapper
    main.cpp
    window.cpp window.h
)
set_target_properties(sqlwidgetmapper PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(sqlwidgetmapper PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Widgets
)

install(TARGETS sqlwidgetmapper
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
