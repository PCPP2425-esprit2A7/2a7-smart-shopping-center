cmake_minimum_required(VERSION 3.16)

project(espace VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Sql Network Charts 3DCore 3DExtras 3DRender 3DInput 3DLogic REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    espace.cpp
    modifierespacesDialog.cpp
    dimensionsdialog.cpp
    espace3dview.cpp
    clickablerectitem.cpp
    connection.cpp
)

set(HEADERS
    mainwindow.h
    espace.h
    modifierespacesDialog.h
    dimensionsdialog.h
    espace3dview.h
    clickablerectitem.h
    connection.h
)

set(FORMS
    mainwindow.ui
    modifierespacesDialog.ui
    DimensionsDialog.ui
)

set(RESOURCES
    IMG.qrc
    resources.qrc
)

add_executable(espace
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(espace PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Qt6::Charts
    Qt6::3DCore
    Qt6::3DExtras
    Qt6::3DRender
    Qt6::3DInput
    Qt6::3DLogic
)
