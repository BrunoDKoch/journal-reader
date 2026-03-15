cmake_minimum_required(VERSION 3.10)

project(JournalReader VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

qt_add_executable(
	journal-reader
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
)

target_include_directories(journal-reader PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(journal-reader PRIVATE Qt6::Widgets Qt6::Core)