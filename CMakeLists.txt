cmake_minimum_required(VERSION 3.16)

# Configuration de l'encodage UTF-8
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8)
endif()

project(EmployeeManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Vérification des dépendances Python
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import cv2; import numpy"
    RESULT_VARIABLE PYTHON_DEPS_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
)

if(NOT PYTHON_DEPS_RESULT EQUAL 0)
    message(FATAL_ERROR "Les dépendances Python requises ne sont pas installées. Veuillez exécuter 'pip install -r requirements.txt'")
endif()

# Définir les fichiers source et en-têtes
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    employeedialog.cpp
    facelogindialog.cpp
    connection.cpp
)

set(PROJECT_HEADERS
    mainwindow.h
    employeedialog.h
    facelogindialog.h
    connection.h
)

set(PROJECT_FORMS
    mainwindow.ui
    employeedialog.ui
    facelogindialog.ui
)

# Créer l'exécutable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_FORMS}
)

# Lier les bibliothèques Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

# Copier les fichiers Python et ressources dans le dossier de build
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/face_recognition.py ${CMAKE_BINARY_DIR}/debug/face_recognition.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_default.xml ${CMAKE_BINARY_DIR}/debug/haarcascade_frontalface_default.xml COPYONLY)

# Installation des fichiers
install(FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/employeedialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/facelogindialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/face_recognition.py
    ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_default.xml
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin) 