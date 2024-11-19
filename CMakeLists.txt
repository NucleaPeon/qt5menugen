cmake_minimum_required(VERSION 3.3)
#
project(qt5menugen
        VERSION 2.2.0
        DESCRIPTION "Generate Mac OSX, Windows and Linux -specific toolbars and menus via a json file"
        LANGUAGES CXX)

include(GNUInstallDirs)  # REQUIRED TO INSTALL PKGCONFIG TO PROPER DIR, and after LANGUAGES is defined

# Immediately set the version.h header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED
    Core
    Widgets
    Gui
    Test
)

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

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_include_directories(${PROJECT_NAME} PUBLIC src/)



list(APPEND TEST_SOURCES
    tests/test_runner.cpp
    tests/test_qwidget_menu.cpp
)

list(APPEND TEST_HEADERS
    tests/test_qwidget_menu.h
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
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
target_link_libraries("${PROJECT_NAME}-tests" Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)


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

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION 2
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QT5MENUGEN_HEADERS}")
configure_file(src/qt5menugen.pc.in src/qt5menugen.pc @ONLY)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)
# Create directory
file(MAKE_DIRECTORY ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header files" )
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    CACHE PATH "Location of configuration files" )
#...
include(CMakePackageConfigHelpers)
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


