
cmake_minimum_required(VERSION 3.16)

project(qmake_cmake_files)

enable_testing()

find_package(Qt5Core REQUIRED)

include("${_Qt5CTestMacros}")

expect_pass(test_qremoteobjects_module)
expect_pass(test_cmake_macros)
