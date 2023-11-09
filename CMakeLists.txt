# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(systray_microphone LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(systray_microphone
    main.cpp
    window.cpp window.h
    card_info.cpp card_info.h
    card_profile_info.cpp card_profile_info.h
)

set_target_properties(systray_microphone PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(systray_microphone PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    pulse
)

# Resources:
set(systray_microphone_resource_files
    "images/bad.png"
    "images/heart.png"
    "images/trash.png"
)

qt_add_resources(systray_microphone "systray_microphone"
    PREFIX
        "/"
    FILES
        ${systray_microphone_resource_files}
)

install(TARGETS systray_microphone
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
