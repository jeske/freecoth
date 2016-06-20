#////////////////////////////////////////////////////////////////////
# Copyright (c) 2001-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later
# version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

include $(top_srcdir)/Makefile.am.include

noinst_LTLIBRARIES = libbase.la

# Should be ${PLATFORM}.. but doesn't work under redhat 9
libbase_la_SOURCES =                \
    baMath.cpp                      \
    baStringUtil.cpp                \
    baTimeUnix.cpp                  \
    baTraceEntry.cpp                \
    baTraceStream.cpp               

myincludedir = $(includedir)/freecloth/base
myinclude_HEADERS =                 \
    algorithm                       \
    baMath.h                        \
    baMath.inline.h                 \
    baStringUtil.h                  \
    baTime.h                        \
    baTime.inline.h                 \
    baTraceEntry.h                  \
    baTraceStream.h                 \
    config.h                        \
    ctype.h                         \
    debug.h                         \
    fstream                         \
    functional                      \
    iomanip                         \
    iostream                        \
    limits.h                        \
    list                            \
    map                             \
    math.h                          \
    memory                          \
    package.h                       \
    set                             \
    stdio.h                         \
    stdlib.h                        \
    string                          \
    typeinfo                        \
    types.h                         \
    vector                          \
    windows.h                       \
    GL_gl.h                         \
    GL_glu.h                        \
    GL_glut.h                       \
    glui.h                          

EXTRA_DIST =                        \
    baTimeUnix.cpp                  \
    baTimeWindows.cpp
