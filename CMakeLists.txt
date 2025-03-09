cmake_minimum_required(VERSION 3.15)

project(OB-Xd-GPL VERSION 2.11.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_ASIO "Enable ASIO support" OFF)

if(ENABLE_ASIO)
    if(WIN32)
        if(DEFINED ENV{ASIO_SDK_HOME})
            set(ASIO_HOME $ENV{ASIO_SDK_HOME})
            if(EXISTS "${ASIO_HOME}/common/iasiodrv.h")
                message(STATUS "ASIO SDK found at: ${ASIO_HOME}")
                add_compile_definitions(JUCE_ASIO=1)
            else()
                message(WARNING "ASIO headers not found at ${ASIO_HOME}, disabling ASIO")
                set(ENABLE_ASIO OFF)
                add_compile_definitions(JUCE_ASIO=0)
            endif()
        else()
            message(WARNING "ASIO_SDK_HOME not set, disabling ASIO")
            set(ENABLE_ASIO OFF)
            add_compile_definitions(JUCE_ASIO=0)
        endif()
    else()
        message(STATUS "ASIO support only available on Windows")
        set(ENABLE_ASIO OFF)
        add_compile_definitions(JUCE_ASIO=0)
    endif()
else()
    add_compile_definitions(JUCE_ASIO=0)
endif()

add_compile_definitions(JUCE_WEB_BROWSER=0)
add_compile_definitions(JUCE_USE_CURL=0)

include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(JUCE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    Include (FetchContent)
    FetchContent_Declare(melatonin_inspector
            GIT_REPOSITORY https://github.com/sudara/melatonin_inspector.git
            GIT_TAG origin/main
            SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/melatonin_inspector)
    FetchContent_MakeAvailable(melatonin_inspector)
endif()

FetchContent_Declare(
        clap_juce_extensions
        GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
        GIT_TAG main
)
FetchContent_MakeAvailable(clap_juce_extensions)

juce_add_plugin(OB-Xd-GPL
    COMPANY_NAME "Moby Pixel"
    BUNDLE_ID "com.mobypixel.obxd"
    LV2URI "urn:com.mobypixel.obxd"
    PLUGIN_MANUFACTURER_CODE MBPX
    PLUGIN_CODE Obxd
    FORMATS AU VST3 LV2 Standalone
    PRODUCT_NAME "OB-Xd-GPL"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(OB-Xd-GPL PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/Utils.cpp
    Source/Components/ScaleComponent.cpp
    Source/Components/PresetBar.cpp
    Source/Components/SetPresetNameWindow.cpp
    Source/MTS/libMTSClient.cpp
)

target_include_directories(OB-Xd-GPL PRIVATE
    Source
    Source/Components
    Source/Engine
    Source/Gui
    Source/MTS
    $<$<BOOL:${ENABLE_ASIO}>:${ASIO_HOME}/common>
)

target_link_libraries(OB-Xd-GPL PRIVATE
    $<$<CONFIG:Debug>:melatonin_inspector>
    BinaryData
    juce::juce_audio_basics
    juce::juce_audio_processors
    juce::juce_core
    juce::juce_gui_basics
    juce::juce_audio_utils
    juce::juce_audio_devices
)

if(MSVC)
    target_compile_options(OB-Xd-GPL PRIVATE /W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(OB-Xd-GPL PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wno-language-extension-token
            $<$<CONFIG:Release>:-O2>
            $<$<AND:$<CONFIG:Release>,$<NOT:$<AND:$<PLATFORM_ID:Darwin>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},arm64>>>>:-march=x86-64-v3>
    )
else()
    target_compile_options(OB-Xd-GPL PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(Images)

clap_juce_extensions_plugin(TARGET OB-Xd-GPL
        CLAP_ID "com.mobypixel.obxd"
        CLAP_FEATURES instrument synthesizer "virtual analog" analog)

target_compile_definitions(OB-Xd-GPL PRIVATE JUCE_MODAL_LOOPS_PERMITTED=1)
