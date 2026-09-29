cmake_minimum_required(VERSION 3.20)
project(CS2_ESP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W3 /O2 /arch:AVX2)
else()
    add_compile_options(-std=c++17 -O2 -msse4.1 -mavx2)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ImGui\ LianFlow
    ${CMAKE_SOURCE_DIR}/ImGui\ LianFlow/backends
    ${CMAKE_SOURCE_DIR}/ImGui\ LianFlow/examples/example_win32_directx11
)

file(GLOB IMGUI_SOURCES
    "ImGui LianFlow/*.cpp"
    "ImGui LianFlow/backends/imgui_impl_win32.cpp"
    "ImGui LianFlow/backends/imgui_impl_dx11.cpp"
    "ImGui LianFlow/examples/example_win32_directx11/custom_widgets.cpp"
)

set(SOURCES
    src/core/process.cpp
    src/core/memory.cpp
    src/core/batch_memory.cpp
    src/features/esp.cpp
    src/features/entity_scanner.cpp
    src/renderer/draw.cpp
    src/config/config.cpp
    src/integration/bridge.cpp
    ${IMGUI_SOURCES}
)

add_executable(esp_fast WIN32 main.cpp ${SOURCES})

target_link_libraries(esp_fast
    d3d11
    d3dcompiler
    dwmapi
    gdi32
    user32
    winmm
    ole32
    advapi32
    ntdll
)
