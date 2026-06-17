cmake_minimum_required(VERSION 3.16)
project(FLStudioEditor VERSION 5.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# Enable Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/core/Project.cpp
    src/audio/AudioEngine.cpp
    src/audio/Synthesizer.cpp
    src/audio/Scheduler.cpp
    src/audio/DrumSample.cpp
    src/audio/WavExporter.cpp
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
    include/audio/AudioEngine.h
    include/audio/Synthesizer.h
    include/audio/Scheduler.h
    include/audio/DrumSample.h
    include/audio/WavExporter.h
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
    Qt6::Multimedia
)

# Platform-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
