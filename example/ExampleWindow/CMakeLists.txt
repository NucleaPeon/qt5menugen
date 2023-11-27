cmake_minimum_required(VERSION 3.3)
project(ExampleWindow
        VERSION 1.0.0
        LANGUAGES CXX
        DESCRIPTION "Example MainWindow with Qt5MenuGen library building toolbars and menus")

find_package(Qt5 REQUIRED Widgets)
include(FindPkgConfig)
find_package(qt5menugen CONFIG REQUIRED 2.0.1)

if(APPLE)
	find_package(Qt5 REQUIRED MacExtras)
elseif(MSVC)
	find_package(Qt5 HINTS "$ENV{QT5_DIR}" COMPONENTS Core WinExtras REQUIRED)
	set(Qt5_DIR "C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl/")
	set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3.1\\5.3\\msvc2013_opengl")
endif()

# find_library(qt5menugen HINTS "/usr/lib" "/usr/local/lib" REQUIRED )
# find_library(CMAKE_LIBRARY_PATH qt5menugen HINTS "/usr/local/lib" "/usr/lib")
message(STATUS ${CMAKE_PREFIX_PATH})
#find_package(qt5menugen CONFIG REQUIRED)
# include_directories(${PROJECT_SOURCE_DIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND OTHER_TARGETS
	menugen.json
)

list(APPEND EXAMPLE_WINDOW_HEADERS
	mainwindow.h
	preferences.h
)

list(APPEND EXAMPLE_WINDOW_SOURCES
	mainwindow.cpp
	about.cpp
	preferences.cpp
)

list(APPEND EXAMPLE_WINDOW_UIS
	mainwindow.ui
	about.ui
	preferences.ui
)

list(APPEND EXAMPLE_WINDOW_RES
	menu.qrc
)


#
if (APPLE)
	list(APPEND EXAMPLE_WINDOW_HEADERS cocoainitializer.h)
	list(APPEND EXAMPLE_WINDOW_SOURCES cocoainitializer.mm)
	add_custom_target(OTHER_FILES SOURCES ${OTHER_TARGETS})

	add_executable(examplewindow MACOSX_BUNDLE
		${OTHER_FILES}
		main.cpp
		${EXAMPLE_WINDOW_HEADERS}
		${EXAMPLE_WINDOW_SOURCES}
		${EXAMPLE_WINDOW_UIS}
		${EXAMPLE_WINDOW_RES}
	)
	target_sources(examplewindow PRIVATE
		${OTHER_TARGETS}
	)
	set_source_files_properties(
		${OTHER_TARGETS}
		PROPERTIES
		MACOSX_PACKAGE_LOCATION Resources/
	)
	set_target_properties(examplewindow PROPERTIES
		BUNDLE TRUE
		MACOSX_BUNDLE_GUI_IDENTIFIER org.peondevelopments.examplewindow
		MACOSX_BUNDLE_BUNDLE_NAME ExampleWindow
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
	)

else()
	add_executable(examplewindow
		${OTHER_FILES}
		main.cpp
		${EXAMPLE_WINDOW_HEADERS}
		${EXAMPLE_WINDOW_SOURCES}
		${EXAMPLE_WINDOW_UIS}
		${EXAMPLE_WINDOW_RES}
	)
endif()

target_link_libraries(examplewindow Qt5::Widgets Qt5::Core)
target_link_libraries(examplewindow qt5menugen)
if(APPLE)
	set_target_properties(examplewindow PROPERTIES LINK_FLAGS
		 "-Wl,-F/Library/Frameworks")
	target_link_libraries(examplewindow stdc++
		"-framework Cocoa"
		objc
	)
	target_link_libraries(examplewindow Qt5::MacExtras)
elseif(MSVC)
	target_link_libraries(examplewindow Qt5::WinExtras)
	QT5_WRAP_UI( UI_HEADERS ${QJSONIFY_UIS} )
endif()
