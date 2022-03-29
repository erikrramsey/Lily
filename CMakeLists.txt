PROJECT(LilyApplication)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")

FIND_PACKAGE(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
SET(TARGET_LIBRARIES "${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${ASSIMP_LIBRARIES}")


IF(MSVC)
  set(CMAKE_SUPPRESS_REGENERATION true)
ENDIF(MSVC)

IF(UNIX)
  ADD_DEFINITIONS(-DUNIX)
ENDIF(UNIX)

IF(NOT APPLE)
  IF(GLEW_FOUND)
      INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
      LINK_LIBRARIES(${GLEW_LIBRARIES})
  ENDIF(GLEW_FOUND)
ENDIF(NOT APPLE)

# Setting library locations
SET(SDL2_LIBRARY     "${PROJECT_SOURCE_DIR}/vendor/SDL2-2.0.14/lib/x64/SDL2.lib")
set(SDL2_MAIN_LIB  "${PROJECT_SOURCE_DIR}/vendor/SDL2-2.0.14/lib/x64/SDL2main.lib")
SET(ASSIMP_LIBRARIES "${PROJECT_SOURCE_DIR}/vendor/assimp-5.0.1/lib/Release/assimp-vc142-mt.lib")
SET(GLEW_LIBRARY     "${PROJECT_SOURCE_DIR}/vendor/glew-2.1.0/lib/Release/x64/glew32.lib")


SET(IMGUI_DIR "${PROJECT_SOURCE_DIR}/vendor/imgui")

SET(IMGUI_SOURCES
    # Main Imgui files
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"

    # SDL2+OpenGL-specific files
    "${IMGUI_DIR}/backends/imgui_impl_sdl.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Set sources
FILE(GLOB_RECURSE ENGINE_SOURCES "LilyEngine/src/*.cpp")
FILE(GLOB_RECURSE APPLICATION_SOURCES "LilyApplication/src/*.cpp")
ADD_LIBRARY(LilyEngine STATIC ${ENGINE_SOURCES} ${IMGUI_SOURCES})
ADD_EXECUTABLE(${PROJECT_NAME} ${APPLICATION_SOURCES} ${IMGUI_SOURCES})


# Includes for Engine
TARGET_INCLUDE_DIRECTORIES(
  LilyEngine
  PUBLIC "${PROJECT_SOURCE_DIR}/LilyEngine/include"
  PUBLIC "${PROJECT_SOURCE_DIR}/LilyEngine/"
  PUBLIC "${PROJECT_SOURCE_DIR}/vendor/glew-2.1.0/include"
  PUBLIC "${PROJECT_SOURCE_DIR}/vendor/SDL2-2.0.14/include"
  PUBLIC "${PROJECT_SOURCE_DIR}/vendor/glm"
  PUBLIC "${PROJECT_SOURCE_DIR}/vendor/assimp-5.0.1/include"
  PUBLIC "${IMGUI_DIR}"
  PUBLIC "${IMGUI_DIR}/backends"
)

# Includes for Application
target_include_directories(${PROJECT_NAME} 
    PUBLIC "${PROJECT_SOURCE_DIR}/LilyEngine"
    PUBLIC "${PROJECT_SOURCE_DIR}/LilyApplication/include"

    PUBLIC "${IMGUI_DIR}"
    PUBLIC "${IMGUI_DIR}/backends"
)


# Preproccesor def for windows
IF(MSVC)
  target_compile_definitions(LilyEngine PUBLIC "LILY_DLL")
  target_compile_definitions(LilyEngine PUBLIC "LILY_WINDOWS")
  target_compile_definitions(${PROJECT_NAME} PUBLIC "LILY_WINDOWS")
ENDIF(MSVC)


set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
)

if(NOT MSVC)
  add_custom_target("${PROJECT_NAME}_SUCCESSFUL" ALL
                    DEPENDS ${PROJECT_NAME}
                    COMMAND ${CMAKE_COMMAND} -E echo ""
                    COMMAND ${CMAKE_COMMAND} -E echo "====================="
                    COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                    COMMAND ${CMAKE_COMMAND} -E echo "====================="
                    COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                  )
endif(NOT MSVC)

TARGET_LINK_OPTIONS(LilyEngine PUBLIC "/INCREMENTAL:NO")
TARGET_LINK_OPTIONS(${PROJECT_NAME} PUBLIC "/INCREMENTAL:NO")

TARGET_LINK_LIBRARIES(LilyEngine ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${SDL2_LIBRARY} ${SDL2_MAIN_LIB} ${ASSIMP_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} LilyEngine)

# Post build
add_custom_command(TARGET LilyEngine
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/vendor/glew-2.1.0/bin/Release/x64/glew32.dll" "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
                   COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/vendor/SDL2-2.0.14/lib/x64/SDL2.dll" "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
                   COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/vendor/assimp-5.0.1/lib/Release/assimp-vc142-mt.dll" "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
                   COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:LilyEngine>" "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")