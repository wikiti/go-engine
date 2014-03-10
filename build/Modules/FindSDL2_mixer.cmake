# - Locate SDL2_mixer library
# This module defines:
#  SDL2_MIXER_FOUND - System has SDL2_mixer
#  SDL2_MIXER_INCLUDE_DIRS - SDL2_mixer include directories
#  SDL2_MIXER_LIBRARIES - Libraries needed to link against SDL2_mixer

find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
  PATH_SUFFIXES include/SDL2 include/
  HINT
    $ENV{SDL2DIR}
	$ENV{SDL2MIXERDIR}
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/SDL2
    /usr/include/SDL2
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
	$ENV{SDL2DIR}
	$ENV{SDL2MIXERDIR}
)

find_library(SDL2_MIXER_LIBRARY
  NAMES SDL2_mixer
  HINTS
    $ENV{SDL2DIR}
	$ENV{SDL2MIXERDIR}
  PATH_SUFFIXES lib lib64
  PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    $ENV{SDL2DIR}
	$ENV{SDL2MIXERDIR}
)

set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})
set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
  SDL2_mixer
  DEFAULT_MSG SDL2_MIXER_LIBRARIES SDL2_MIXER_INCLUDE_DIRS)

mark_as_advanced(SDL2_MIXER_LIBRARY SDL2_MIXER_INCLUDE_DIR)