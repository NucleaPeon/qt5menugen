cmake_minimum_required(VERSION 3.3)
project(CombiningMenus
        VERSION 1.0.0
        LANGUAGES CXX
        DESCRIPTION "Example that shows how reusing a QtMenuGen* object can be done")

find_package(Qt5 REQUIRED Widgets)
find_package(qt5menugen CONFIG REQUIRED 2.3.0)

if(APPLE)
	find_package(Qt5 REQUIRED MacExtras)
elseif(MSVC)
	find_package(Qt5 HINTS "$ENV{QT5_DIR}" COMPONENTS Core WinExtras REQUIRED)
	set(Qt5_DIR "C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl/")
	set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3.1\\5.3\\msvc2013_opengl")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND OTHER_TARGETS
        toolbar.json
        common.json
)

list(APPEND SHORTCUT_TESTER_HEADERS
	mainwindow.h
)

list(APPEND SHORTCUT_TESTER_SOURCES
	mainwindow.cpp
)

list(APPEND SHORTCUT_TESTER_UIS
	mainwindow.ui
)

list(APPEND SHORTCUT_TESTER_RES
	main.qrc
)

if (APPLE)
	list(APPEND SHORTCUT_TESTER_HEADERS cocoainitializer.h)
	list(APPEND SHORTCUT_TESTER_SOURCES cocoainitializer.mm)
	add_custom_target(OTHER_FILES SOURCES ${OTHER_TARGETS})

        add_executable(${PROJECT_NAME} MACOSX_BUNDLE
		${OTHER_FILES}
		main.cpp
		${SHORTCUT_TESTER_HEADERS}
		${SHORTCUT_TESTER_SOURCES}
		${SHORTCUT_TESTER_UIS}
		${SHORTCUT_TESTER_RES}
	)
        target_sources(${PROJECT_NAME} PRIVATE
		${OTHER_TARGETS}
	)
	set_source_files_properties(
		${OTHER_TARGETS}
		PROPERTIES
		MACOSX_PACKAGE_LOCATION Resources/
	)
        set_target_properties(${PROJECT_NAME} PROPERTIES
		BUNDLE TRUE
                MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME}
                MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
	)

else()
        add_executable(${PROJECT_NAME}
		${OTHER_FILES}
		main.cpp
		${SHORTCUT_TESTER_HEADERS}
		${SHORTCUT_TESTER_SOURCES}
		${SHORTCUT_TESTER_UIS}
		${SHORTCUT_TESTER_RES}
	)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core)
target_link_libraries(${PROJECT_NAME} qt5menugen)
if(APPLE)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
		 "-Wl,-F/Library/Frameworks")
        target_link_libraries(${PROJECT_NAME} stdc++
		"-framework Cocoa"
		objc
	)
        target_link_libraries(${PROJECT_NAME} Qt5::MacExtras)
elseif(MSVC)
        target_link_libraries(${PROJECT_NAME} Qt5::WinExtras)
	QT5_WRAP_UI( UI_HEADERS ${QJSONIFY_UIS} )
endif()
