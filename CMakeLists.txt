cmake_minimum_required(VERSION 3.16)

project(employe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)  # ✅ Ajout du module SQL

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(employe
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        employe.h employe.cpp
        connection.h connection.cpp
        maintenance.cpp maintenance.h

    )
else()
    if(ANDROID)
        add_library(employe SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(employe
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(employe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)  # ✅ Ajout du module SQL

set_target_properties(employe PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS employe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(employe)
endif()
