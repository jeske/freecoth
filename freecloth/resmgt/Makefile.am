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

noinst_LTLIBRARIES = libresmgt.la

libresmgt_la_SOURCES =              \
    rcBase.cpp                      \
    rcProxyShdPtr.cpp               \
    rcShdPtr.cpp                    \
    resConfigRegistry.cpp           \
    resConfigRegistryFile.cpp       \
    resConfigRegistryUnix.cpp

myincludedir = $(includedir)/freecloth/resmgt
myinclude_HEADERS =                 \
    package.h                       \
    rcBase.h                        \
    rcProxyShdPtr.h                 \
    rcProxyShdPtr.inline.h          \
    rcShdPtr.h                      \
    rcShdPtr.inline.h               \
    resConfig.h                     \
    resConfigRegistry.h             \
    resConfigRegistry.imp.h         \
    resConfigRegistryFile.h         \
    resConfigRegistryUnix.h  

EXTRA_DIST =                        \
    resConfigRegistryUnix.cpp       \
    resConfigRegistryWindows.cpp    \
    resConfigRegistryUnix.h         \
    resConfigRegistryWindows.h
