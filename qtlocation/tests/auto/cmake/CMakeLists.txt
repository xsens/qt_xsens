
cmake_minimum_required(VERSION 3.16)

project(qmake_cmake_files)

enable_testing()

find_package(Qt5Core REQUIRED)

include("${_Qt5CTestMacros}")

set(qt_module_includes
    Location QPlaceCategory
    Positioning QGeoRectangle
)

test_module_includes(
    ${qt_module_includes}
)
