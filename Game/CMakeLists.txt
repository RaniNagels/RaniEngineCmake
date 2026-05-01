# --- Game Project -------------
if(EMSCRIPTEN)
  set(TARGET_NAME Game_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME Game)
  set(TARGET_PARAM )#WIN32)
endif()

# --- List Source Files -------------
set(SOURCE_FILES
	"Main.cpp"
    "Player.cpp"

    "Commands/MoveCommand.cpp"
    "Commands/PlaceBombCommand.cpp"

    "Components/BombComponent.cpp"
    "Components/GridComponent.cpp"
    "Components/DebugGridRenderComponent.cpp"
    "Components/UILivesComponent.cpp"
    "Components/UIScoreComponent.cpp"
    "Components/BombermanCollisionComponent.cpp"
    "Components/DebugBoundsRenderComponent.cpp"
)


# --- Game Executable -------------
add_executable(${TARGET_NAME} ${TARGET_PARAM} ${SOURCE_FILES})
message(STATUS "Game Executable Created")

# --- Set C++23 for Game executable --------------
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_23)

# --- Set Default Startup Project -------------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

# --- Link Libraries -------------
target_link_libraries(${TARGET_NAME} PRIVATE 
	REC
	engine_warnings # from engine
)

if (WIN32)
    # Add VLD (Visual Leak Detector) for memory leak detection on Windows
    find_package(VLD CONFIG)
    target_include_directories(${TARGET_NAME} PUBLIC ${VLD_INCLUDE_DIR})
    target_link_libraries(${TARGET_NAME} PUBLIC ${VLD_LIBRARIES})
endif()

if(EMSCRIPTEN)
  #set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g -frtti")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_CURRENT_SOURCE_DIR}/Data@/\"")

  # Use the Emscripten toolchain
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  # Additional settings for Emscripten build
  set_target_properties(${TARGET_NAME} PROPERTIES
      #COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  )

  # Have emscripten generate a html page too.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif(WIN32)
  # Set minigin as the default startup project
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  # Set working directory to output folder
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")
endif()

if(NOT EMSCRIPTEN)
  message("starting to copy data folder")
  # Copy Data folder to output directory after build
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
endif()

