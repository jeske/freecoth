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

lib_LTLIBRARIES = libsimulator.la
libsimulator_la_LDFLAGS = -version-info @LT_VERSION@ -release @VERSION@
libsimulator_la_LIBADD = ../base/libbase.la ../resmgt/libresmgt.la ../geom/libgeom.la

libsimulator_la_SOURCES =           \
    simMatrix.cpp                   \
    simSimulator.cpp                \
    simStepStrategy.cpp             \
    simStepStrategyAdaptive.cpp     \
    simStepStrategyBasic.cpp        \
    simVector.cpp                    

myincludedir = $(includedir)/freecloth/simulator
myinclude_HEADERS =                 \
    package.h                       \
    simMatrix.h                     \
    simMatrix.inline.h              \
    simSimulator.h                  \
    simStepStrategy.h               \
    simStepStrategyAdaptive.h       \
    simStepStrategyBasic.h          \
    simVector.h                     \
    simVector.inline.h              
