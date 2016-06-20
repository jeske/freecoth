//////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef freecloth_base_config_h
#define freecloth_base_config_h

////////////////////////////////////////////////////////////////////////////////
// Compiler definitions
//
// COMPILER_MSVC
// COMPILER_GCC
//

#ifdef _MSC_VER
#define COMPILER_MSVC 1
#else
#define COMPILER_MSVC 0
#endif

#ifdef __GNUC__
#define COMPILER_GCC 1
#else
#define COMPILER_GCC 0
#endif

////////////////////////////////////////////////////////////////////////////////
// Operating system definitions
//
// OPSYS_WIN32
// OPSYS_CYGWIN32   (should also define OPSYS_WIN32)
//

#ifdef _WIN32
#define OPSYS_WIN32 1
#else
#define OPSYS_WIN32 0
#endif

#ifdef __CYGWIN32__
#define OPSYS_CYGWIN32 1
#else
#define OPSYS_CYGWIN32 0
#endif

#if !OPSYS_WIN32
#define OPSYS_UNIX
#else
#define OPSYS_UNIX
#endif



////////////////////////////////////////////////////////////////////////////////
// Configuration-dependent settings

#if COMPILER_MSVC
// Disable "C++ Exception Specification ignored" warning
#pragma warning( disable: 4290 )
// Disable "identifier was truncated to 255 characters in the debug
// information" warning
#pragma warning( disable: 4786 )
//#define HAVE_TEMPLATE_FRIENDS
//#define HAVE_MEMBER_TEMPLATES

// Strange MSVC bug: if a template class is contained in a namespace, the
// namespace must be explicitly specified for class.
#define TEMPLATE_NAMESPACE_PREFIX freecloth::
#else
#define TEMPLATE_NAMESPACE_PREFIX
#endif

#if COMPILER_GCC
#include <freecloth/base/autoconf.h>
#undef PACKAGE
#undef VERSION
#endif

#if OPSYS_WIN32
#define NOMINMAX
#endif

#endif
