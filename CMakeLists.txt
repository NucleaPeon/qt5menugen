cmake_minimum_required(VERSION 3.3)
#
project(qt5menugen
        VERSION 4.0.0
        DESCRIPTION "Generate Mac OSX, Windows and Linux -specific toolbars and menus via a json file"
        LANGUAGES CXX)

include(GNUInstallDirs)  # REQUIRED TO INSTALL PKGCONFIG TO PROPER DIR, and after LANGUAGES is defined
include(CMakePackageConfigHelpers)
# Immediately set the version.h header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(QT_DEFAULT_MAJOR_VERSION "6" CACHE STRING "Qt Version defaults to 6 (latest)")

find_package(Qt${QT_DEFAULT_MAJOR_VERSION} NAMES Qt5 Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Test
)

message(STATUS "Using Qt${QT_DEFAULT_MAJOR_VERSION}")

if(APPLE)
        find_package(Qt${QT_DEFAULT_MAJOR_VERSION} NAMES Qt5 Qt6 REQUIRED COMPONENTS MacExtras)
elseif(MSVC)
        # Don't worry about qt6 on windows yet'
        find_package(Qt${QT_DEFAULT_MAJOR_VERSION} NAMES Qt5 Qt6 HINTS "$ENV{QT${QT_DEFAULT_MAJOR_VERSION}_DIR}" COMPONENTS Core Quick REQUIRED)
        set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3.1\\5.3\\msvc2013_opengl")
        set(Qt${QT_DEFAULT_MAJOR_VERSION}_DIR "C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl/")
        find_package(Qt${QT_DEFAULT_MAJOR_VERSION} NAMES Qt5 Qt6 COMPONENTS WinExtras REQUIRED)
endif()


set(PACKAGE_HEADERS
  qt5menugen.h
)

list(APPEND QT5MENUGEN_HEADERS
	src/qt5menugen.h
	src/qt5menugen_global.h
	src/version.h
)

list(APPEND QT5MENUGEN_SOURCES
	src/qt5menugen.cpp
)

add_library(${PROJECT_NAME}
	SHARED
	${QT5MENUGEN_HEADERS}
	${QT5MENUGEN_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC src/)

list(APPEND TEST_SOURCES
    tests/test_runner.cpp
    tests/test_qwidget_menu.cpp
    tests/test_json.cpp
)

list(APPEND TEST_HEADERS
    tests/test_qwidget_menu.h
    tests/test_json.h
)

list(APPEND TEST_UIS
)

# common/common.qrc contains the collection of spec file definitions
list(APPEND TEST_RES

)

# We need to include spec files from other qrc files, so add them relative to here:

list(APPEND TEST_OTHER
)

add_executable("${PROJECT_NAME}-tests"
    ${TEST_HEADERS}
    ${TEST_SOURCES}
    ${TEST_UIS}
    ${TEST_RES}
)


# Remove this line when deploying
# include_directories(BEFORE /usr/local/Qt-5.3.2/lib)
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE .. )

target_link_libraries(${PROJECT_NAME}
    Qt${QT_DEFAULT_MAJOR_VERSION}::Core
    Qt${QT_DEFAULT_MAJOR_VERSION}::Gui
    Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets
    Qt${QT_DEFAULT_MAJOR_VERSION}::Test)

target_link_libraries("${PROJECT_NAME}-tests"
    Qt${QT_DEFAULT_MAJOR_VERSION}::Core
    Qt${QT_DEFAULT_MAJOR_VERSION}::Gui
    Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets
    Qt${QT_DEFAULT_MAJOR_VERSION}::Test)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION 4
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QT5MENUGEN_HEADERS}")
configure_file(src/qt5menugen.pc.in src/qt5menugen.pc @ONLY)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# Create directory
file(MAKE_DIRECTORY ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header files" )
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    CACHE PATH "Location of configuration files" )
#...

configure_package_config_file(${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  VERSION ${VERSION}
  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )
install(FILES ${CMAKE_BINARY_DIR}/src/qt5menugen.pc DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} Qt${QT_DEFAULT_MAJOR_VERSION}::MacExtras)
elseif(MSVC)
    target_link_libraries(${PROJECT_NAME} Qt${QT_DEFAULT_MAJOR_VERSION}::WinExtras)
endif()
