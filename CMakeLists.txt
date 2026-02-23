cmake_minimum_required(VERSION 3.26)
project(BurgerTimeProject)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

#engine
add_library(Minigin STATIC
  Minigin/Font.cpp
  Minigin/GameObject.cpp
  Minigin/InputManager.cpp
  Minigin/Minigin.cpp
  Minigin/Renderer.cpp
  Minigin/ResourceManager.cpp
  Minigin/Scene.cpp
  Minigin/SceneManager.cpp
  Minigin/Texture2D.cpp
  Minigin/Transform.cpp
  Minigin/Components/TextureComponent.cpp 
  Minigin/Components/TextComponent.cpp 
  Minigin/Components/DynamicTextComponent.cpp 
  Minigin/Components/RotatorComponent.cpp
)

target_compile_features(Minigin PUBLIC cxx_std_20)

include(FetchContent)
# GLM
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(glm URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip DOWNLOAD_NO_PROGRESS ON DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads)
  FetchContent_MakeAvailable(glm)
endif()

# SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(SDL3 URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip DOWNLOAD_NO_PROGRESS ON DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads)
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-3.4.0 GIT_SHALLOW TRUE GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

# SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    FetchContent_Declare(SDL3_ttf URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip DOWNLOAD_NO_PROGRESS ON DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads)
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(SDL3_ttf GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git GIT_TAG release-3.2.2 GIT_SHALLOW TRUE GIT_PROGRESS TRUE)
    set(SDL3TTF_INSTALL OFF)
    if(EMSCRIPTEN)
      list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
      set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
      set(SDL3TTF_VENDORED OFF)
      set(SDLTTF_PLUTOSVG OFF)
      add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
      add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
    endif()
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()

target_include_directories(Minigin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Minigin/)
target_link_libraries(Minigin PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf glm::glm)

# Warnings as Errors Requirement
if(MSVC)
  target_compile_options(Minigin PRIVATE /W4 /WX)
else()
  target_compile_options(Minigin PRIVATE -Wall -Wextra -Werror)
endif()


if(EMSCRIPTEN)
  set(TARGET_NAME BurgerTime_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME BurgerTime)
  set(TARGET_PARAM WIN32)
endif()

# NOTE: Put your Burger Time source files here!
add_executable(${TARGET_NAME} ${TARGET_PARAM}
  BurgerTime/Main.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_link_libraries(${TARGET_NAME} PRIVATE Minigin)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror)
endif()


if(WIN32 AND NOT EMSCRIPTEN)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  set(SDL_RUNTIME_DLLS SDL3::SDL3 SDL3_ttf::SDL3_ttf)
  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${dllTarget}>" "$<TARGET_FILE_DIR:${TARGET_NAME}>" VERBATIM)
  endforeach()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data")
endif()