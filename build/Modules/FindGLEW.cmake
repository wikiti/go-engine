# - Find the OpenGL Extension Wrangler Library (GLEW)
# This module defines the following variables:
#  GLEW_INCLUDE_DIRS - include directories for GLEW
#  GLEW_LIBRARIES - libraries to link against GLEW
#  GLEW_FOUND - true if GLEW has been found and can be used

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)


find_path(GLEW_INCLUDE_DIR glew.h
  PATH_SUFFIXES include include/GL include/GLEW
  HINT
    $ENV{GLEWDIR}
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/GL
	/usr/local/include/GLEW
    /usr/include/GL
	/usr/include/GLEW
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
	$ENV{GLEWDIR}
)

find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64
  PATH_SUFFIXES lib64 lib
  HINT
    $ENV{GLEWDIR}
  PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    $ENV{GLEWDIR}
)

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(GLEW
                                  REQUIRED_VARS GLEW_INCLUDE_DIR GLEW_LIBRARY)

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)
