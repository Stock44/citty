cmake_minimum_required(VERSION 3.16)
project(citty)

set(CMAKE_CXX_STANDARD 23)

cmake_policy(SET CMP0071 NEW)

find_package(Qt6
        REQUIRED
        COMPONENTS
        Core
        Qml
        Gui
        Quick
        QuickLayouts
        Location
        Positioning
        WaylandClient
        Network
        3DCore
        3DExtras
        3DRender
        3DInput
)

qt_standard_project_setup()

add_subdirectory(src)
