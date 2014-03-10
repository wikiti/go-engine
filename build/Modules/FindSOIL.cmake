# Find SOIL
# Find the SOIL includes and library
#
#  SOIL_INCLUDE_DIRS - where to find SOIL.h, etc.
#  SOIL_LIBRARIES    - List of libraries when using SOIL.
#  SOIL_FOUND        - True if SOIL found.
#
# Based on the FindZLIB.cmake module.

IF (SOIL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SOIL_FIND_QUIETLY TRUE)
ENDIF (SOIL_INCLUDE_DIR)

FIND_PATH(SOIL_INCLUDE_DIR SOIL.h
  HINTS
    $ENV{SOILDIR}
  PATH_SUFFIXES include/SOIL include include/GL
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/SOIL
    /usr/include/SOIL
    /sw/include/SOIL # Fink
    /opt/local/include/SOIL # DarwinPorts
    /opt/csw/include/SOIL # Blastwave
    /opt/include/SOIL
	$ENV{SOILDIR}
)

SET(SOIL_NAMES SOIL Soil soil)
FIND_LIBRARY(SOIL_LIBRARY NAMES ${SOIL_NAMES}
  HINTS
    $ENV{SOILDIR}
  PATH_SUFFIXES lib lib64
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
	$ENV{SOILDIR}
    $ENV{SOILDIR}
)
MARK_AS_ADVANCED( SOIL_LIBRARY SOIL_INCLUDE_DIR )

# Per-recommendation
SET(SOIL_INCLUDE_DIRS "${SOIL_INCLUDE_DIR}")
SET(SOIL_LIBRARIES    "${SOIL_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set SOIL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOIL DEFAULT_MSG SOIL_LIBRARIES SOIL_INCLUDE_DIRS)