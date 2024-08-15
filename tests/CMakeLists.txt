cmake_minimum_required(VERSION 3.3)
project(qtmenugentests
        VERSION 0.0.1.0
        LANGUAGES C CXX
        DESCRIPTION "Tests for validating correctness of qt5menugen code"
)

# Immediately set the version.h header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h @ONLY)
if (APPLE)
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

# add_compile_options(-v)

###

find_package(Qt5 REQUIRED
    Core
    Gui
    Widgets
    Test
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


list(APPEND TEST_SOURCES
    test_runner.cpp
)

list(APPEND TEST_HEADERS
)

list(APPEND TEST_UIS
)

# common/common.qrc contains the collection of spec file definitions
list(APPEND TEST_RES
    # testfiles.qrc
    # ../common/common.qrc
)

# We need to include spec files from other qrc files, so add them relative to here:

list(APPEND TEST_OTHER
)

add_executable(${PROJECT_NAME}
    ${TEST_HEADERS}
    ${TEST_SOURCES}
    ${TEST_UIS}
    ${TEST_RES}
)

if(APPLE)
        find_package(Qt5 REQUIRED MacExtras)
        target_link_libraries(${PROJECT_NAME} Qt5::MacExtras)
elseif(MSVC)
        find_package(Qt5 HINTS "$ENV{QT5_DIR}" COMPONENTS Core Quick REQUIRED)
        set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3.1\\5.3\\msvc2013_opengl")
        set(Qt5_DIR "C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl/")
        find_package(Qt5 COMPONENTS WinExtras REQUIRED)
        target_link_libraries(${PROJECT_NAME} Qt5::WinExtras)
endif()

# Remove this line when deploying
include_directories(BEFORE /usr/local/Qt-5.3.2/lib)
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE .. )
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::OpenGL Qt5::Test)

#add_test(NAME TestMenuGeneration
#         COMMAND ctest TestMenuGeneration)


#add_custom_command(TARGET ${PROJECT_NAME}
#                   POST_BUILD
#                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
