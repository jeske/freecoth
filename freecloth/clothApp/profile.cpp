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

#include <freecloth/clothApp/package.h>
#include <freecloth/simulator/simSimulator.h>
#include <freecloth/simulator/simStepStrategyAdaptive.h>
#include <freecloth/simulator/simStepStrategyBasic.h>
#include <freecloth/geom/geMatrix3.h>
#include <freecloth/geom/geMesh.h>
#include <freecloth/geom/geMeshBuilder.h>

namespace {

//------------------------------------------------------------------------------

RCShdPtr<GeMesh> createRectMesh(
    Float size,
    UInt32 nbRows,
    UInt32 nbCols
) {
    GeMeshBuilder builder;
    UInt32 r, c;
    builder.preallocVertices( (nbRows + 1) * (nbCols + 1 ) );
    builder.preallocTextureVertices( (nbRows + 1) * (nbCols + 1 ) );
    builder.preallocFaces( nbRows * nbCols );
    for ( r = 0; r <= nbRows; ++r ) {
        for ( c = 0; c <= nbCols; ++c ) {
            builder.addVertex(
                GePoint( c * size / nbCols, r * size / nbRows, 0 )
            );
            builder.addTextureVertex(
                GePoint( c * size / nbCols, r * size / nbRows, 0 )
            );
        }
    }
    const UInt32 rstride=nbCols+1;
    for( r = 0; r < nbRows; ++r ) {
        for ( c = 0; c < nbCols; ++c ) {
            GeMesh::FaceId fid = builder.addFace(
                r*rstride + c, r*rstride + c+1, (r+1)*rstride + c,
                r*rstride + c, r*rstride + c+1, (r+1)*rstride + c
            );
            fid = builder.addFace(
                (r+1)*rstride + c, r*rstride + c+1, (r+1)*rstride + c+1,
                (r+1)*rstride + c, r*rstride + c+1, (r+1)*rstride + c+1
            );
        }
    }
    return builder.createMesh();
}

}

////////////////////////////////////////////////////////////////////////////////
// GLOBALS

//------------------------------------------------------------------------------

int main()
{
    const UInt32 NB_ITER = 10;
    const UInt32 NB_PATCHES = 31;

    RCShdPtr< GeMesh > mesh( createRectMesh( 1, NB_PATCHES, NB_PATCHES ) );
    RCShdPtr<SimSimulator> simulator(
        RCShdPtr<SimSimulator>( new SimSimulator( *mesh ) )
    );
    simulator->setDensity( .1f );
    SimStepStrategyAdaptive stepper( simulator, 25 );
    {
        const UInt32 N = NB_PATCHES;
#if 1
        simulator->setPosConstraintFull( 0 );
        simulator->setPosConstraintFull( N );
        simulator->setPosConstraintFull( N*(N+1) );
        simulator->setPosConstraintFull( N*(N+1) + N );
#else
        const UInt32 C = BaMath::roundUInt32( std::max( 1.f, N*5/11.f ) );
        const UInt32 x = (N-C)/2;
        for (UInt32 i = 0; i <= C; ++i ) for ( UInt32 j = 0; j <= C; ++j ) {
            simulator->setPosConstraintFull( (x + i)*(N+1) + x + j );
        }
#endif
    }

    while ( simulator->getTime() < BaTime::S ) {
        std::cout << "Time "
            << BaTime::instantAsSeconds( simulator->getTime() ) << std::endl;
        stepper.step();
    }
    return 0;
}
