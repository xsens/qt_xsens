cmake_minimum_required(VERSION 3.16)
project(androidnotifier LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/corelib/platform/androidnotifier")

set(PROJECT_SOURCES
        main.cpp
        notificationclient.cpp
        notificationclient.h
)

if(ANDROID)
    qt_add_executable(androidnotifier
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES})
    target_link_libraries(androidnotifier PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    set_property(TARGET androidnotifier APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(androidnotifier)
    endif()

    set(qml_resource_files
        "images/happy.png"
        "images/sad.png"
    )

    qt6_add_resources(androidnotifier "main"
        PREFIX
            "/"
        FILES
            ${qml_resource_files}
    )

    install(TARGETS androidnotifier
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
    )
endif()
