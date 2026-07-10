cmake_minimum_required(VERSION 3.16)
project(FLStudioEditor VERSION 5.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/core/Project.cpp
    src/ui/MainWindow.cpp
    src/ui/PianoRollWidget.cpp
    src/ui/StepSequencerWidget.cpp
    src/ui/MixerWidget.cpp
    src/ui/TransportWidget.cpp
    src/ui/InstrumentEditor.cpp
    src/ui/PianoKeyboard.cpp
    src/utils/AudioBuffer.cpp
)

set(HEADERS
    include/core/Project.h
    include/core/Track.h
    include/core/Note.h
    include/ui/MainWindow.h
    include/ui/PianoRollWidget.h
    include/ui/StepSequencerWidget.h
    include/ui/MixerWidget.h
    include/ui/TransportWidget.h
    include/ui/InstrumentEditor.h
    include/ui/PianoKeyboard.h
    include/utils/AudioBuffer.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# Platform-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
    # MSVC: 修复 M_PI 未定义问题 + UTF-8 编码
    if(MSVC)
        target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)
        target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    endif()
elseif(UNIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
endif()

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# ====================================================================
# Windows 部署: windeployqt（自动复制 Qt 运行时库）
# ====================================================================
if(WIN32)
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E env "PATH=${_qt_bin_dir}"
                "${WINDEPLOYQT_EXECUTABLE}"
                --no-compiler-runtime
                --no-translations
                "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt to deploy Qt runtime libraries..."
            VERBATIM
        )
    else()
        message(WARNING "windeployqt not found - you need to manually deploy Qt libraries")
    endif()
endif()

# ====================================================================
# CPack 配置（跨平台）
# ====================================================================
set(CPACK_PACKAGE_NAME "flstudio-editor")
set(CPACK_PACKAGE_VENDOR "FLStudioEditor")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FL Studio style music editor - v5 Qt6 native desktop application")
set(CPACK_PACKAGE_DESCRIPTION "A FL Studio style music editing software with piano roll, step sequencer, mixer, transport controls, project management, and WAV export. Built with C++17 and Qt6.")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "FLStudioEditor")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "developer@flstudio-editor.local")

if(WIN32)
    # ===== Windows 安装包：NSIS 和 ZIP
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_PACKAGE_NAME "FL Studio Editor")
    set(CPACK_NSIS_DISPLAY_NAME "FL Studio Editor")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_MENU_LINKS
        "bin/${PROJECT_NAME}.exe" "FL Studio Editor"
    )
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\FL Studio Editor.lnk' '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\FL Studio Editor.lnk'"
    )
elseif(UNIX AND NOT APPLE)
    # ===== Linux 安装包：DEB 和 TGZ
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "FLStudioEditor Developer")
    set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6gui6, libqt6core6, libqt6multimedia6, libqt6network6, libqt6dbus6, libstdc++6, libc6")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
elseif(APPLE)
    # ===== macOS 安装包：DragNDrop 和 TGZ
    set(CPACK_GENERATOR "DragNDrop;TGZ")
    set(CPACK_DMG_VOLUME_NAME "FL Studio Editor")
endif()

include(CPack)
