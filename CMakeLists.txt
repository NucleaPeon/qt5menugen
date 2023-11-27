cmake_minimum_required(VERSION 3.3)

set(VERSION_MAJOR 2 CACHE STRING "Project major version number.")
set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION)
project(qt5menugen
        VERSION ${VERSION}
        DESCRIPTION "Generate Mac OSX, Windows and Linux -specific toolbars and menus via a json file"
        LANGUAGES CXX)

include(GNUInstallDirs)  # REQUIRED TO INSTALL PKGCONFIG TO PROPER DIR, and after LANGUAGES is defined


find_package(Qt5 REQUIRED
    Widgets
)

set(PACKAGE_HEADERS
  qt5menugen.h
)

list(APPEND QT5MENUGEN_HEADERS
	src/qt5menugen.h
	src/qt5menugen_global.h
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

include(GNUInstallDirs)
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
  VERSION 2.0.1
  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )
install(FILES ${CMAKE_BINARY_DIR}/src/qt5menugen.pc DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
