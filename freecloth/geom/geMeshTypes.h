//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_geom_geMeshTypes_h
#define freecloth_geom_geMeshTypes_h

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//
class GePoint;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshTypes freecloth/geom/geMeshTypes.h
 * \brief A class to collect a few common typedefs.
 *
 * These typedefs are used by the various GeMesh* classes.
 */
class GeMeshTypes {

public:

    // ----- types and enumerations -----
    
    //! Face index
    typedef UInt32 FaceId;
    //! Vertex index
    typedef UInt32 VertexId;
    //! Texture vertex index
    typedef UInt32 TextureVertexId;
    //! Index of a vertex within a face
    typedef UInt32 FaceVertexId;

    //! Vertex
    typedef GePoint VertexType;
    //! Texture vertex, i.e. texture co-ordinates.
    typedef GePoint TextureVertexType;

    //! Container for vertices
    typedef std::vector<VertexType>         VertexContainer;
    //! Iterator for vertices
    typedef VertexContainer::iterator       VertexIterator;
    //! ConstIterator for vertices
    typedef VertexContainer::const_iterator VertexConstIterator;
    //! Container for texture vertices
    typedef std::vector<TextureVertexType>          TextureVertexContainer;
    //! Iterator for texture vertices
    typedef TextureVertexContainer::iterator        TextureVertexIterator;
    //! ConstIterator for texture vertices
    typedef TextureVertexContainer::const_iterator  TextureVertexConstIterator;
};

FREECLOTH_NAMESPACE_END

#endif
