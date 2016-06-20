#////////////////////////////////////////////////////////////////////
# Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

noinst_PROGRAMS = clothApp profile

clothApp_LDFLAGS = @GLUI_LIBS@
clothApp_CFLAGS = @GLUI_CFLAGS@
# FIXME: point to actual installed simulator library.
# FIXME: should be dynamic, not static.
clothApp_LDADD = ../colour/libcolour.la ../gfx/libgfx.la ../simulator/libsimulator.la
clothApp_SOURCES = \
    clothApp.cpp                    \
    clothAppConfig.cpp

profile_LDADD = ../colour/libcolour.la ../gfx/libgfx.la ../simulator/libsimulator.la
profile_SOURCES = \
    profile.cpp

noinst_HEADERS =                    \
    package.h                       \
    clothAppConfig.h                \
    clothApp.h

EXTRA_DIST = checker3.ppm marble.ppm
