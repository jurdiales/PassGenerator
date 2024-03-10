cmake_minimum_required(VERSION 3.16)

project(PassGenerator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

set(CMAKE_AUTORCC ON)
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/PassGenerator.rc")

qt_add_executable(appPassGenerator
    main.cpp
    ${app_icon_resource_windows}
)

qt_add_qml_module(appPassGenerator
    URI PassGenerator
    VERSION 1.0
    QML_FILES Main.qml resources/AutoClosePopup.qml
    SOURCES generator.h generator.cpp
    SOURCES connector.h connector.cpp
    RESOURCES resources/resources.qrc
    RESOURCES README.md
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPassGenerator PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPassGenerator
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPassGenerator
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appPassGenerator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
