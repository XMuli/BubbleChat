cmake_minimum_required(VERSION 3.5)

project(BubbleDemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES

    widgets/chatinputedit.h
    widgets/chatinputedit.cpp
#    widgets/chatbubble.h
#    widgets/chatbubble.cpp
#    widgets/chatbubble.ui
#    widgets/chathistory.h
#    widgets/chathistory.cpp

    widgets/bubble.h
    widgets/bubble.cpp
    widgets/bubble.ui
    widgets/bubblehistory.h
    widgets/bubblehistory.cpp

    main.cpp
    mainui.h
    mainui.cpp
    mainui.ui

#    widget.cpp
#    widget.h
#    widget.ui
)

set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")

source_group(TREE ${_src_root_path} FILES ${_source_list})

add_executable(BubbleDemo
    ${PROJECT_SOURCES}
)

target_link_libraries(BubbleDemo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(BubbleDemo PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS BubbleDemo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BubbleDemo)
endif()
