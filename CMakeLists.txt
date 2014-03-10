# -- Cabecera --
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(go-engine)

# -- Ficheros fuente --
SET(ENGINE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/engine/include")
SET(ENGINE_SRC_DIR "${PROJECT_SOURCE_DIR}/engine/src")

SET(USER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/user/include")
SET(USER_SRC_DIR "${PROJECT_SOURCE_DIR}/user/src")

INCLUDE_DIRECTORIES(${ENGINE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${USER_INCLUDE_DIR})
ADD_DEFINITIONS(-Wall -std=c++11)

FILE(GLOB_RECURSE CPP_FILES ${USER_SRC_DIR}/*.cpp ${ENGINE_SRC_DIR}/*.cpp)
FILE(GLOB_RECURSE H_FILES ${USER_INCLUDE_DIR}/*.h ${ENGINE_INCLUDE_DIR}/*.h)
SET(SRC_FILES ${CPP_FILES} ${H_FILES})

#IF(MINGW)
#  SET(MINGW32_LIBRARY mingw32 CACHE STRING "mingw32 for MinGW")
#ENDIF(MINGW)

# -- Modulos CMake --
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/build/Modules/")

# -- Búsqueda de Dependencias --
# - SDL2 - 
FIND_PACKAGE(SDL2)
IF(NOT SDL2_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** SDL2 Not found. Try setting the enviroment variable \"SDL2DIR\" to your SDL2 root directory, which should contain a lib/ directory and a include/SDL2 directory.")
ENDIF(NOT SDL2_FOUND)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${SDL2_LIBRARY})

# - SDL2_image - 
FIND_PACKAGE(SDL2_image)
IF(NOT SDL2IMAGE_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** SDL2_image Not found. Try setting the enviroment variable \"SDL2DIR\" to your SDL2 root directory, which should contain a lib/ directory and a include/SDL2 directory. This means, SDL2 and SDL2_image should be on the same directory.\n If it's not the case, you can set the enviromental variable \"SDL2IMAGEDIR\" to your SDL2_image location, which should contain a lib/ and a include/ or include/SDL2 directories.")
ENDIF(NOT SDL2IMAGE_FOUND)

INCLUDE_DIRECTORIES(${SDL2IMAGE_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${SDL2IMAGE_LIBRARY})

# - SDL2_mixer - 
FIND_PACKAGE(SDL2_mixer)
IF(NOT SDL2_MIXER_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** SDL2_mixer Not found. Try setting the enviroment variable \"SDL2DIR\" to your SDL2 root directory, which should contain a lib/ directory and a include/SDL2 directory. This means, SDL2 and SDL2_mixer should be on the same directory.\n If it's not the case, you can set the enviromental variable \"SDL2MIXERDIR\" to your SDL2_mixer location, which should contain a lib/ and a include/ or include/SDL2 directories.")
ENDIF(NOT SDL2_MIXER_FOUND)

INCLUDE_DIRECTORIES(${SDL2_MIXER_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${SDL2_MIXER_LIBRARY})

# - SOIL -
FIND_PACKAGE(SOIL)
IF(NOT SOIL_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** SOIL Not found. Try setting the enviroment variable \"SOILDIR\" to your SOIL root directory, which should contain a lib/ directory and a include or include/GL directory.")
ENDIF(NOT SOIL_FOUND)

INCLUDE_DIRECTORIES(${SOIL_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${SOIL_LIBRARY})

# - GLEW - 
FIND_PACKAGE(GLEW)
IF(NOT GLEW_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** GLEW Not found. Try setting the enviroment variable \"GLEWDIR\" to your Glew root directory, which should contain a lib/ directory and a include/GL or include/GLEW directory.")
ENDIF(NOT GLEW_FOUND)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${GLEW_LIBRARY})

# - OpenGL - 
FIND_PACKAGE(OPENGL)
IF(NOT OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** OpenGL Not found. This may be a problem with your installation of OpenGL or CMake (Module FindOpenGL.cmake). Try searching the cmake log in the build/CMakeFiles directory, and try to fix it manually.")
ENDIF(NOT OPENGL_FOUND)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${OPENGL_LIBRARY})

# - OpenAL - 
FIND_PACKAGE(OPENAL)
IF(NOT OPENAL_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** OpenAL Not found. Try setting the enviroment variable \"OPENALDIR\" to your OpenAL root directory, which should contain a lib/ directory and a include/AL directory.")
ENDIF(NOT OPENAL_FOUND)

INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${OPENAL_LIBRARY})

# - Assimp -
FIND_PACKAGE(ASSIMP)
IF(NOT ASSIMP_FOUND)
  MESSAGE(FATAL_ERROR "**ERROR** Assimp Not found. Try setting the enviroment variable \"ASSIMPDIR\" to your assimp root directory, which should contain a lib/ directory and a include or include/assimp directory. Also, make sure to use Assimp 3.0.1270.")
ENDIF(NOT ASSIMP_FOUND)
MESSAGE(WARNING "Make sure to use Assimp 3.0.1270!!")

INCLUDE_DIRECTORIES(${ASSIMP_INCLUDE_DIRS})
SET (LINK_LIBS ${LINK_LIBS} ${ASSIMP_LIBRARY})

# -- Compilar --
ADD_EXECUTABLE(goengine ${SRC_FILES})
TARGET_LINK_LIBRARIES(goengine ${LINK_LIBS})