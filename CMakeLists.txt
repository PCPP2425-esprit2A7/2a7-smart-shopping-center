cmake_minimum_required(VERSION 3.16)

project(espace VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Find Qt components, including Charts and 3D modules
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Charts 3DCore 3DRender 3DExtras)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Charts 3DCore 3DRender 3DExtras)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        espace.h
        espace.cpp
        connection.h
        connection.cpp
        IMG.qrc
        modifierespacesdialog.h
        modifierespacesdialog.cpp
        modifierespacesdialog.ui



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(espace
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(espace SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(espace
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# ✅ Link QtCharts and Qt3D modules along with other libraries
target_link_libraries(espace PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Sql 
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::3DCore
    Qt${QT_VERSION_MAJOR}::3DRender
    Qt${QT_VERSION_MAJOR}::3DExtras
)

set_target_properties(espace PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS espace
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(espace)
endif()
