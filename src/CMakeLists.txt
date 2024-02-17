qt_add_executable(Citty
        main.cpp
        OsmMapImporter.cpp
        OsmMapImporter.hpp
)

find_path(OSMIUM_INCLUDE_DIRS "osmium/version.hpp")

target_include_directories(Citty PRIVATE ${OSMIUM_INCLUDE_DIRS} interfaces)

qt_add_qml_module(Citty
        URI main
        SOURCES
        interfaces/OsmImporterInterface.cpp
        interfaces/OsmImporterInterface.hpp
        QML_FILES
        views/Citty.qml
        views/SelectionArea.qml
        views/OsmImportDialog.qml
)

qt_import_plugins(Citty
        INCLUDE_BY_TYPE platforms Qt::QWaylandIntegrationPlugin
)

target_link_libraries(Citty
        PRIVATE
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickLayouts
        Qt6::Location
        Qt6::Network
        Qt6::WaylandClient
)
