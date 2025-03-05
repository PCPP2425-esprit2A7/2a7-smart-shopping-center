cmake_minimum_required(VERSION 3.16)

project(gestion_services VERSION 0.1 LANGUAGES CXX)

# Activer la gestion automatique des fichiers Qt
set(CMAKE_AUTOUIC ON)  # Pour les fichiers .ui
set(CMAKE_AUTOMOC ON)  # Pour les fichiers moc (Meta-Object Compiler)
set(CMAKE_AUTORCC ON)  # Pour les fichiers .qrc

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver Qt6 avec Widgets et QtSql
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Ajouter les fichiers sources
set(PROJECT_SOURCES
    main.cpp
    connection.cpp
    connection.h
    service.cpp
    service.h
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    img.qrc  # Vérifiez que ce fichier existe bien
)

# Création de l'exécutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gestion_services
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        modifierservicedialog.h modifierservicedialog.cpp modifierservicedialog.ui
    )
else()
    if(ANDROID)
        add_library(gestion_services SHARED ${PROJECT_SOURCES})
    else()
        add_executable(gestion_services ${PROJECT_SOURCES})
    endif()
endif()

# Lier les bibliothèques Qt
target_link_libraries(gestion_services PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Définir les propriétés pour macOS et Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.gestion_services)
endif()

set_target_properties(gestion_services PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation
include(GNUInstallDirs)
install(TARGETS gestion_services
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalisation pour Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gestion_services)
endif()
