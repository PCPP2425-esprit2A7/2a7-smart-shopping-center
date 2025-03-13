cmake_minimum_required(VERSION 3.16)

project(Projetemploye VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Inclusion des modules Qt nécessaires
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Charts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    connection.h connection.cpp
    employe.h employe.cpp
    modifierdialog.h modifierdialog.cpp modifierdialog.ui
)

# ✅ Création de l'exécutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Projetemploye MANUAL_FINALIZATION ${PROJECT_SOURCES}
        img.qrc)
else()
    if(ANDROID)
        add_library(Projetemploye SHARED ${PROJECT_SOURCES})
    else()
        add_executable(Projetemploye ${PROJECT_SOURCES})
    endif()
endif()

# ✅ Ajout correct des bibliothèques APRÈS la création de l'exécutable
target_link_libraries(Projetemploye PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Charts
)

# ✅ Configuration des propriétés de l'exécutable
set_target_properties(Projetemploye PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ✅ Installation
include(GNUInstallDirs)
install(TARGETS Projetemploye
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ✅ Finalisation si Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Projetemploye)
endif()
