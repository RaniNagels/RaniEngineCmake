cmake_minimum_required(VERSION 4.1)
project(Minigin)

add_subdirectory(Engine)
add_subdirectory(Game)

if(CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS MATCHES "NOTFOUND")
  message(WARNING "clang-scan-deps not found, C++ module dependency scanning will be disabled")
  set(CMAKE_CXX_SCAN_FOR_MODULES OFF CACHE BOOL "Disable C++ module scanning" FORCE)
endif()

if (NOT EMSCRIPTEN)
# Set output build directories for all specified targets (it is either this or set it per target))
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/$<CONFIG>")
set_target_properties(REC Game PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
)
endif()
