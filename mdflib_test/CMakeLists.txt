# Copyright 2021 Ingemar Hedvall
# SPDX-License-Identifier: MIT

enable_testing()

project(TestMdf
        VERSION 1.0
        DESCRIPTION "Google unit tests for the mdf library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(test_mdf
        testcrypto.cpp
        testread.cpp testread.h
        testwrite.cpp testwrite.h
        testmdffile.cpp
        testmetadata.cpp
        testzlib.cpp testzlib.h
        util/ilogger.h util/ilogger.cpp
        util/logconfig.cpp util/logconfig.h
        util/logconsole.cpp util/logconsole.h
        util/logfile.cpp util/logfile.h
        util/logging.cpp util/logging.h
        util/logmessage.cpp util/logmessage.h
        util/logstream.cpp util/logstream.h
        util/StringUtil.cpp util/StringUtil.h
        util/TimeUtil.cpp util/TimeUtil.h
        )

target_include_directories(test_mdf PRIVATE ../include ../mdflib/src)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test_mdf PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_libraries(test_mdf PRIVATE mdf)

add_test(AllTestsInMain main)