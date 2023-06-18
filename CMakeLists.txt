cmake_minimum_required(VERSION 3.5)
set(_PROJECT_NAME BubbleChat)
project(${_PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

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

    widgets/bubble.h
    widgets/bubble.cpp
    widgets/bubble.ui
    widgets/bubblehistory.h
    widgets/bubblehistory.cpp

    main.cpp
    mainui.h
    mainui.cpp
    mainui.ui
    resources.qrc
)

set(_src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE _source_list LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h")

include_directories(${PROJECT_SOURCE_DIR}/widgets)  # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败

source_group(TREE ${_src_root_path} FILES ${_source_list})

add_executable(${_PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER https://github.com/XMuli
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 多核编译
if(WIN32)
    if(MSVC)
        add_compile_options(/MP${N})
        message("--->MSVC")
    elseif(MINGW)
        set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${N}")
    endif()
elseif(UNIX)
    add_compile_options(-j${N})
endif()

# 采用 MSVC 终端上的中文不乱码
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif()
install(TARGETS ${_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BubbleDemo)
endif()
