# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindOpenGL
# ----------
#
# FindModule for OpenGL and GLU.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``OPENGL_FOUND``
#  True, if the system has OpenGL.
# ``OPENGL_XMESA_FOUND``
#  True, if the system has XMESA.
# ``OPENGL_GLU_FOUND``
#  True, if the system has GLU.
# ``OPENGL_INCLUDE_DIR``
#  Path to the OpenGL include directory.
# ``OPENGL_LIBRARIES``
#  Paths to the OpenGL and GLU libraries.
#
# If you want to use just GL you can use these values:
#
# ``OPENGL_GL_LIBRARY``
#  Path to the OpenGL library.
# ``OPENGL_glu_LIBRARY``
#  Path to the GLU library.
#
# OSX Specific
# ^^^^^^^^^^^^
#
# On OSX default to using the framework version of OpenGL. People will
# have to change the cache values of OPENGL_glu_LIBRARY and
# OPENGL_GL_LIBRARY to use OpenGL with X11 on OSX.


set(_OpenGL_REQUIRED_VARS OPENGL_GL_LIBRARY)

if (CYGWIN)

  find_path(OPENGL_INCLUDE_DIR GL/gl.h )
  list(APPEND _OpenGL_REQUIRED_VARS OPENGL_INCLUDE_DIR)

  find_library(OPENGL_GL_LIBRARY opengl32 )

  find_library(OPENGL_GLU_LIBRARY glu32 )

elseif (WIN32)

  if(BORLAND)
    set (OPENGL_GL_LIBRARY import32 CACHE STRING "OpenGL library for win32")
    set (OPENGL_GLU_LIBRARY import32 CACHE STRING "GLU library for win32")
  else()
    set (OPENGL_GL_LIBRARY opengl32 CACHE STRING "OpenGL library for win32")
    set (OPENGL_GLU_LIBRARY glu32 CACHE STRING "GLU library for win32")
  endif()

elseif (APPLE)

  # The OpenGL.framework provides both gl and glu
  find_library(OPENGL_GL_LIBRARY OpenGL DOC "OpenGL library for OS X")
  find_library(OPENGL_GLU_LIBRARY OpenGL DOC
    "GLU library for OS X (usually same as OpenGL library)")
  find_path(OPENGL_INCLUDE_DIR OpenGL/gl.h DOC "Include for OpenGL on OS X")
  list(APPEND _OpenGL_REQUIRED_VARS OPENGL_INCLUDE_DIR)

else()
  if (CMAKE_SYSTEM_NAME MATCHES "HP-UX")
    # Handle HP-UX cases where we only want to find OpenGL in either hpux64
    # or hpux32 depending on if we're doing a 64 bit build.
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(_OPENGL_LIB_PATH
        /opt/graphics/OpenGL/lib/hpux32/)
    else()
      set(_OPENGL_LIB_PATH
        /opt/graphics/OpenGL/lib/hpux64/
        /opt/graphics/OpenGL/lib/pa20_64)
    endif()
  elseif(CMAKE_SYSTEM_NAME STREQUAL Haiku)
    set(_OPENGL_LIB_PATH
      /boot/develop/lib/x86)
    set(_OPENGL_INCLUDE_PATH
      /boot/develop/headers/os/opengl)
  endif()

  # The first line below is to make sure that the proper headers
  # are used on a Linux machine with the NVidia drivers installed.
  # They replace Mesa with NVidia's own library but normally do not
  # install headers and that causes the linking to
  # fail since the compiler finds the Mesa headers but NVidia's library.
  # Make sure the NVIDIA directory comes BEFORE the others.
  #  - Atanas Georgiev <atanas@cs.columbia.edu>

  find_path(OPENGL_INCLUDE_DIR GL/gl.h
    /usr/share/doc/NVIDIA_GLX-1.0/include
    /usr/openwin/share/include
    /opt/graphics/OpenGL/include /usr/X11R6/include
    ${_OPENGL_INCLUDE_PATH}
  )
  list(APPEND _OpenGL_REQUIRED_VARS OPENGL_INCLUDE_DIR)

  find_path(OPENGL_xmesa_INCLUDE_DIR GL/xmesa.h
    /usr/share/doc/NVIDIA_GLX-1.0/include
    /usr/openwin/share/include
    /opt/graphics/OpenGL/include /usr/X11R6/include
  )

  find_library(OPENGL_GL_LIBRARY
    NAMES GL MesaGL
    PATHS /opt/graphics/OpenGL/lib
          /usr/openwin/lib
          /usr/shlib /usr/X11R6/lib
          ${_OPENGL_LIB_PATH}
  )

  unset(_OPENGL_INCLUDE_PATH)
  unset(_OPENGL_LIB_PATH)

  find_library(OPENGL_GLU_LIBRARY
    NAMES GLU MesaGLU
    PATHS ${OPENGL_GL_LIBRARY}
          /opt/graphics/OpenGL/lib
          /usr/openwin/lib
          /usr/shlib /usr/X11R6/lib
  )

endif ()

if(OPENGL_GL_LIBRARY)

    if(OPENGL_xmesa_INCLUDE_DIR)
      set( OPENGL_XMESA_FOUND "YES" )
    else()
      set( OPENGL_XMESA_FOUND "NO" )
    endif()

    set( OPENGL_LIBRARIES  ${OPENGL_GL_LIBRARY} ${OPENGL_LIBRARIES})
    if(OPENGL_GLU_LIBRARY)
      set( OPENGL_GLU_FOUND "YES" )
      if(NOT "${OPENGL_GLU_LIBRARY}" STREQUAL "${OPENGL_GL_LIBRARY}")
        set( OPENGL_LIBRARIES ${OPENGL_GLU_LIBRARY} ${OPENGL_LIBRARIES} )
      endif()
    else()
      set( OPENGL_GLU_FOUND "NO" )
    endif()

    # This deprecated setting is for backward compatibility with CMake1.4
    set (OPENGL_LIBRARY ${OPENGL_LIBRARIES})

endif()

# This deprecated setting is for backward compatibility with CMake1.4
set(OPENGL_INCLUDE_PATH ${OPENGL_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set OPENGL_FOUND to TRUE if
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenGL REQUIRED_VARS ${_OpenGL_REQUIRED_VARS})
unset(_OpenGL_REQUIRED_VARS)

mark_as_advanced(
  OPENGL_INCLUDE_DIR
  OPENGL_xmesa_INCLUDE_DIR
  OPENGL_GLU_LIBRARY
  OPENGL_GL_LIBRARY
)
