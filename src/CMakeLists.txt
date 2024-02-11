qt_add_executable(Citty
        main.cpp
        citty.qrc
)

qt_add_qml_module(Citty
        URI main
        QML_FILES
        views/Citty.qml
        views/WelcomeView.qml
        views/CreateProjectView.qml
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
        Qt6::WaylandClient
)
