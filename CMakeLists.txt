cmake_minimum_required(VERSION 3.3)
project(qt5menugen
        VERSION 1.0.0
        DESCRIPTION "Generate Mac OSX, Windows and Linux -specific toolbars and menus via a json file"
        LANGUAGES CXX)

find_package(Qt5 REQUIRED
    Widgets
)

list(APPEND QT5MENUGEN_HEADERS
	src/qt5menugen.h
	src/qt5menugen_global.h
)

list(APPEND QT5MENUGEN_SOURCES
	src/qt5menugen.cpp
)

add_library(qt5menugen SHARED
	${QT5MENUGEN_HEADERS}
	${QT5MENUGEN_SOURCES}
)

target_link_libraries(qt5menugen Qt5::Widgets)

if(APPLE)
	find_package(Qt5 REQUIRED MacExtras)
	target_link_libraries(qt5menugen Qt5::MacExtras)
elseif(MSVC)
	find_package(Qt5 HINTS "$ENV{QT5_DIR}" COMPONENTS Core Quick REQUIRED)
	set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3.1\\5.3\\msvc2013_opengl")
	set(Qt5_DIR "C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl/")
	find_package(Qt5 COMPONENTS WinExtras REQUIRED)
	target_link_libraries(qt5menugen Qt5::WinExtras)
endif()

set_target_properties(qt5menugen PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION 1
)
set_target_properties(qt5menugen PROPERTIES PUBLIC_HEADER "${QT5MENUGEN_HEADERS}")
configure_file(src/qt5menugen.pc.in src/qt5menugen.pc @ONLY)

install(TARGETS qt5menugen
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/src/qt5menugen.pc DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
