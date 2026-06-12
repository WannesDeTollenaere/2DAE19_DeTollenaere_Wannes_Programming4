cmake_minimum_required(VERSION 3.26)
project(MiniginEngine)

if(EMSCRIPTEN)
  message("Using Emscripten!")
   set(GAME_TARGET minigin_web)
  set(GAME_PARAM )
else()
  set(GAME_TARGET game)
  set(GAME_PARAM)
endif()


include(FetchContent)

# GLM
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(glm URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip DOWNLOAD_NO_PROGRESS ON DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads)
  FetchContent_MakeAvailable(glm)
endif()

# JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# ImGui
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui GIT_TAG v1.92.5-docking GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(imgui)
add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_compile_features(imgui PUBLIC cxx_std_20)

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

# SDL3_mixer

find_package(SDL3_mixer 3.2.0 CONFIG QUIET)
if(NOT SDL3_mixer_FOUND)
  FetchContent_Declare(SDL3_mixer 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git 
    GIT_TAG release-3.2.0 
    GIT_SHALLOW TRUE 
    GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(SDL3_mixer)
endif()

if (WIN32)
  find_package(VLD CONFIG)
endif()

if(NOT EMSCRIPTEN)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
  find_package(Steamworks QUIET)
endif()


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
  Minigin/SceneLoader.cpp
  Minigin/Gamepad.cpp
  Minigin/Components/TextureComponent.cpp
  Minigin/Components/TextComponent.cpp
  Minigin/Components/DynamicTextComponent.cpp
  Minigin/Components/RotatorComponent.cpp
  Minigin/Components/HoverComponent.cpp
  Minigin/Components/HoverComponent.h
  Minigin/Commands/GameObjectCommand.h
  Minigin/Commands/Command.h
  Minigin/Components/CacheProfilerComponent.cpp
  Minigin/Components/AnimatorComponent.cpp
 "Minigin/ObserverSys/Event.h" 
 "Minigin/ObserverSys/Observer.h" 
  
 "Minigin/ObserverSys/EventManager.h" 
 "Minigin/Components/TagComponent.h" 
 "Minigin/sdbm_hash.h" 
 "Minigin/SteamAchievements/Achievement.h" 
 "Minigin/SteamAchievements/Achievement.cpp" 
 "Minigin/Components/BoxColliderComponent.h" 
 "Minigin/Components/BoxColliderComponent.cpp" 
 "Minigin/CollisionManager.h" 
 "Minigin/CollisionManager.cpp" 
 "Minigin/ObserverSys/CollisionEvent.h" 
 "Minigin/Components/BaseCollisionHandler.h" 
 "Minigin/Components/BaseCollisionHandler.cpp" 
 "Minigin/Helpers/Spritesheet.h" 
 "Minigin/Helpers/Spritesheet.cpp" 
 "Minigin/Components/LifeTimeComponent.h"
"Minigin/Components/LifeTimeComponent.cpp" 
"Minigin/Commands/QuitCommand.h" 
"Minigin/Sound/SoundSystem.h" "Minigin/Sound/ServiceLocator.h" "Minigin/Sound/SdlSoundSystem.h" "Minigin/Sound/SdlSoundSystem.cpp" "Minigin/Sound/LoggingSoundSystem.h" "Minigin/Commands/ToggleMuteCommand.h")

target_compile_features(Minigin PUBLIC cxx_std_20)

target_include_directories(Minigin PUBLIC 
  ${CMAKE_SOURCE_DIR}/Minigin/
  ${glm_SOURCE_DIR}
  ${sdl3_SOURCE_DIR}/include
  ${sdl3_ttf_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  ${json_SOURCE_DIR}/include
  ${VLD_INCLUDE_DIR}
)

target_link_libraries(Minigin PUBLIC 
  SDL3::SDL3 
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  imgui
  glm::glm
  nlohmann_json::nlohmann_json
  ${VLD_LIBRARIES}
)

if(Steamworks_FOUND)
  target_link_libraries(Minigin PUBLIC Steamworks::Steamworks)
  target_compile_definitions(Minigin PUBLIC USE_STEAMWORKS=1)
endif()

if (WIN32)
  target_link_libraries(Minigin PUBLIC xinput)
endif()

if(MSVC)
  target_compile_options(Minigin PRIVATE /W4 /WX)
else()
  target_compile_options(Minigin PRIVATE -Wall -Wextra -Werror)
endif()



add_executable(${GAME_TARGET} ${GAME_PARAM}
  Game/Main.cpp
  "Game/Components/CharacterControllerComponent.cpp"
  Game/Commands/MoveCommand.h
 "Game/Components/HealthComponent.h" 
 "Game/Components/HealthComponent.cpp" 
 "Game/Components/HealthDisplayComponent.h" 
 "Game/Components/HealthDisplayComponent.cpp" 
 "Game/Events/PlayerDiedEvent.h" 
 "Game/Events/DealDamageEvent.h" 
 "Game/Commands/DamageCommand.h" 
 "Game/Events/ScoreIncreaseEvent.h" 
 "Game/Commands/IncreaseScoreCommand.h" 
 "Game/Components/ScoreDisplayComponent.h" 
 "Game/Components/ScoreDisplayComponent.cpp" 
 "Game/Achievements/BurgerTimeAchievements.h" 
 "Game/Events/ScoreChangedEvent.h" 
 "Game/Components/ScoreComponent.h" 
 "Game/Components/ScoreComponent.cpp" 
 "Game/Achievements/AchievementManager.h" 
 "Game/Achievements/AchievementManager.cpp" 
 "Game/Events/PlayerTookDamageEvent.h" 
 "Game/Helpers/LevelGrid.h" 
 "Game/Components/Movement/GridMovementComponent.h" 
 "Game/Components/Movement/GridMovementComponent.cpp" 
 "Game/Components/Movement/GridDebugRenderer.h" 
 "Game/Components/Movement/GridDebugRenderer.cpp" 
 "Game/Components/Movement/GridRenderComponent.h" 
 "Game/Components/Movement/GridRenderComponent.cpp" 
 "Game/Components/Ingredients/BurgerIngredientComponent.h" 
 "Game/Components/Ingredients/BurgerIngredientComponent.cpp" 
  
 "Game/Components/Movement/EnemyWanderComponent.h" 
 "Game/Components/Movement/EnemyWanderComponent.cpp" 
 "Game/Events/EnemyCrushedEvent.h" 
 "Game/Components/PlayerCharacter/SaltComponent.h" 
 "Game/Components/PlayerCharacter/SaltComponent.cpp" 
 "Game/Commands/ThrowSaltCommand.h" 
 "Game/Events/BurgerDroppedEvent.h" 
 "Game/Events/BurgerCascadeEvent.h"
 "Game/Components/ScoreVFXComponent.h" 
 "Game/Components/ScoreVFXComponent.cpp" 
 "Game/Events/BurgerCompletedEvent.h" 
 "Game/Components/Ingredients/PlateComponent.h"
 "Game/Components/Ingredients/PlateComponent.cpp" 
 "Game/Components/PlayerCharacter/WinConditionComponent.h" 
 "Game/Components/PlayerCharacter/WinConditionComponent.cpp" 
 "Game/GameManager.h" 
 "Game/Events/SaltThrownEvent.h" 
 "Game/GameManager.cpp" 
 "Game/Helpers/PrefabFactory.h" 
 "Game/Helpers/PrefabFactory.cpp" 
 "Game/Events/SaltChangedEvent.h" 
 "Game/Components/PlayerCharacter/SaltDisplayComponent.h" 
 "Game/Components/PlayerCharacter/SaltDisplayComponent.cpp"  
 "Game/Events/LivesChangedEvent.h" 
 "Game/Events/LivesLostEvent.h" 
 "Game/Helpers/HighScoreManager.h" 
 "Game/Helpers/HighScoreManager.cpp"
 "Game/Components/HighScoreInputComponent.h" 
 "Game/Commands/HighScoreCommands.h" 
 "Game/Components/HighScoreInputComponent.cpp" 
 "Game/Components/PlayerCharacter/HighScoreDisplayComponent.h"
 "Game/Components/PlayerCharacter/HighScoreDisplayComponent.cpp"
 "Game/Components/FinalScoreDisplayComponent.h"
 "Game/Components/FinalScoreDisplayComponent.cpp"
 "Game/Components/LeaderboardComponent.h" 
 "Game/Components/LeaderboardComponent.cpp" 
 "Game/Commands/LoadSceneCommand.h" 
 "Game/Components/SceneTranistionComponent.h" 
 "Game/Components/SceneTransitionComponent.cpp"  
 "Game/Commands/MenuNavigationCommand.h" 
 "Game/Components/UI/MenuItemComponent.h" 
 "Game/Components/UI/MenuComponent.h" 
 "Game/Components/UI/MenuComponent.cpp" 
 "Game/Components/Enemy/EnemyComponent.h" 
 "Game/Components/PlayerCharacter/PlayerEnemyController.h" 
 "Game/Components/PlayerCharacter/PlayerEnemyController.cpp" 
 "Game/Components/Enemy/EnemyComponent.cpp" 
 "Game/Components/PlayerCharacter/SaltManagerComponent.h" 
 "Game/Components/PlayerCharacter/SaltManagerComponent.cpp" 
 "Game/Commands/StartGameCommand.h" 
 "Game/Helpers/SoundIDs.h"
 "Game/States/PlayerState.h" 
 "Game/States/PlayerState.cpp" "Game/States/EnemyState.h" "Game/States/EnemyState.cpp"  "Game/Components/Ingredients/PickupComponent.h" "Game/Components/Ingredients/PickupComponent.cpp"
 "Game/Components/Ingredients/PickupSpawnerComponent.h" "Game/Components/Ingredients/PickupSpawnerComponent.cpp"
 "Game/Events/PickupCollectedEvent.h")

 set_target_properties(${GAME_TARGET} PROPERTIES
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)

target_compile_features(${GAME_TARGET} PUBLIC cxx_std_20)

target_include_directories(${GAME_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/Game/)

target_link_libraries(${GAME_TARGET} PUBLIC Minigin)

if(MSVC)
  target_link_options(${GAME_TARGET} PRIVATE "/WHOLEARCHIVE:$<TARGET_FILE:Minigin>")
elseif(NOT EMSCRIPTEN)
  target_link_options(${GAME_TARGET} PRIVATE "-Wl,--whole-archive" "$<TARGET_FILE:Minigin>" "-Wl,--no-whole-archive")
endif()

if(MSVC)
  target_compile_options(${GAME_TARGET} PRIVATE /W4 /WX)
else()
  target_compile_options(${GAME_TARGET} PRIVATE -Wall -Wextra -Werror)
endif()


if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  set_target_properties(${GAME_TARGET} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${GAME_TARGET}>/"
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".html")


elseif(WIN32)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_TARGET})
  set_target_properties(${GAME_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${GAME_TARGET}>")

  set(SDL_RUNTIME_DLLS SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer)

  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${dllTarget}>" "$<TARGET_FILE_DIR:${GAME_TARGET}>"
      VERBATIM
    )
  endforeach()

  if(Steamworks_FOUND)
    target_link_libraries(${GAME_TARGET} PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${GAME_TARGET}>")
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )

elseif(APPLE)
  set_target_properties(${GAME_TARGET} PROPERTIES
    MACOSX_BUNDLE TRUE
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${GAME_TARGET}>"
    COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${GAME_TARGET}>"
  )

  if(Steamworks_FOUND)
    target_link_libraries(${GAME_TARGET} PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    )
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )

else()
 if(Steamworks_FOUND)
    target_link_libraries(${GAME_TARGET} PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${GAME_TARGET}>"
    )
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )
endif()