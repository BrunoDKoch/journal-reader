cmake_minimum_required(VERSION 3.10)

project(journal-reader VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

qt_add_executable(
	${PROJECT_NAME}
	include/setup.hpp
	src/main.cpp
	include/enums/priority_enum.hpp
	include/widgets/main_window.hpp
	src/widgets/main_window.cpp
	include/widgets/main_frame.hpp
	src/widgets/main_frame.cpp
	include/structs/journal_model.hpp
	src/journal_model.cpp
	include/widgets/filter_frame.hpp
	src/widgets/filter_frame.cpp
	include/widgets/page_widget.hpp
	src/widgets/page_widget.cpp
	include/widgets/details_view.hpp
	src/widgets/details_view.cpp
	include/widgets/boot_widget.hpp
	src/widgets/boot_widget.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core)

install(TARGETS ${PROJECT_NAME}
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES assets/${PROJECT_NAME}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES assets/${PROJECT_NAME}.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps)