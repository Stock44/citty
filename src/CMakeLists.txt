qt_add_executable(Citty
        main.cpp
        osm/OsmRoadNetworkImporter.cpp
        osm/OsmRoadNetworkImporter.hpp
        roadNetwork/RoadNetwork.cpp
        roadNetwork/RoadNetwork.hpp
        osm/NodeExportHandler.cpp
        osm/NodeExportHandler.hpp
        osm/BoundsHandler.cpp
        osm/BoundsHandler.hpp
        osm/WayExportHandler.cpp
        osm/WayExportHandler.hpp
        qmlElements/CittyCameraController.cpp
        qmlElements/CittyCameraController.hpp
)

find_path(OSMIUM_INCLUDE_DIRS "osmium/version.hpp")

target_include_directories(Citty PRIVATE ${OSMIUM_INCLUDE_DIRS} interfaces models qmlElements)

qt_add_qml_module(Citty
        URI citty
        RESOURCE_PREFIX /github.com/Stock44/imports
        SOURCES
        interfaces/OsmImporterInterface.cpp
        interfaces/OsmImporterInterface.hpp
        interfaces/SceneInterface.cpp
        interfaces/SceneInterface.hpp
        models/RoadModel.cpp
        models/RoadModel.hpp
        models/NodeModel.cpp
        models/NodeModel.hpp
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
        Qt6::3DCore
        Qt6::3DExtras
        Qt6::3DRender
        Qt6::3DInput
)
