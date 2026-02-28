# --- Engine Project -------------
set(TARGET_NAME REC)

# --- List Source Files -------------
set(SOURCE_FILES
	"src/Components/Component.cpp"
	"src/Components/DebugGridRenderComponent.cpp"
	"src/Components/FPSComponent.cpp"
	"src/Components/GridComponent.cpp"
	"src/Components/RotatorComponent.cpp"
	"src/Components/SpriteRenderComponent.cpp"
	"src/Components/TextRenderComponent.cpp"
	"src/Components/TransformComponent.cpp"

	"src/Input/InputSystem.cpp"
	"src/Input/InputDevices/Controller.cpp"
	"src/Input/InputDevices/Keyboard.cpp"

	"src/FileReader.cpp"
	"src/Font.cpp"
	"src/GameObject.cpp"
	"src/Minigin.cpp"
	"src/Renderer.cpp"
	"src/ResourceManager.cpp"
	"src/Scene.cpp"
	"src/SceneManager.cpp"
	"src/Texture2D.cpp"
	"src/TimeSystem.cpp"
	"src/Window.cpp"
)

# --- Include Libraries -------------
include(FetchContent)

# add glm
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_MakeAvailable(glm)
endif()

# Add SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3
      URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-3.4.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3_ttf
      URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.2
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
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

# --- Create Dynamic/Static Library -------------
if(EMSCRIPTEN)
	add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
else()
    add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
	#add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})
endif()
message(STATUS "GameEngine Library Created")

# --- Include Directories -------------
target_include_directories(${TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- Link Libraries -------------
target_link_libraries(${TARGET_NAME} 
    PUBLIC glm::glm
    PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf
)
if (NOT EMSCRIPTEN)
	target_link_libraries(${TARGET_NAME} PRIVATE xinput)
endif()

# --- Set C++20 for library and all dependents -------------
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
message(STATUS "Set Language to C++ 20")

# --- compile at warning level 4 + treat warnings as errors -------------
add_library(engine_warnings INTERFACE)
if (MSVC)
    target_compile_options(engine_warnings INTERFACE /W4 /WX)
else()
    target_compile_options(engine_warnings INTERFACE -Wall -Wextra -Werror)
endif()
target_link_libraries(${TARGET_NAME} PUBLIC engine_warnings)

if(WIN32)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:${TARGET_NAME}>
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            $<TARGET_FILE_DIR:${TARGET_NAME}>
    )
endif()

