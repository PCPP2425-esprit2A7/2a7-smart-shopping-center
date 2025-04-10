cmake_minimum_required(VERSION 3.16)

project(gestion_services VERSION 0.1 LANGUAGES CXX)

# Activer la gestion automatique des fichiers Qt
set(CMAKE_AUTOUIC ON)  # Pour les fichiers .ui
set(CMAKE_AUTOMOC ON)  # Pour les fichiers moc (Meta-Object Compiler)
set(CMAKE_AUTORCC ON)  # Pour les fichiers .qrc

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver Qt6 avec Widgets, Sql, Charts et Network
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Charts Network)

# Vérification de la version Qt
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Charts Network)

# Ajouter les fichiers de votre projet
set(PROJECT_SOURCES
    main.cpp
    connection.cpp
    connection.h
    service.cpp
    service.h
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    img.qrc
    openaiclient.h
    openaiclient.cpp  # Ajout du client OpenAI
)

# Création de l'exécutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gestion_services
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        config.txt
        mailsender.h mailsender.cpp

    )
else()
    if(ANDROID)
        add_library(gestion_services SHARED ${PROJECT_SOURCES})
    else()
        add_executable(gestion_services ${PROJECT_SOURCES})
    endif()
endif()

# Ajouter le chemin des en-têtes de QtXlsx (si nécessaire)
include_directories(${CMAKE_SOURCE_DIR}/QXlsx/header)

# Lier les bibliothèques Qt, y compris Network
target_link_libraries(gestion_services PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Network  # Ajout de Network ici
)

# Définir les propriétés pour macOS et Windows
set_target_properties(gestion_services PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation
include(GNUInstallDirs)
install(TARGETS gestion_services
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION .
)

# Finalisation pour Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gestion_services)
endif()
