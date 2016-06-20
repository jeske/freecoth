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

#include <freecloth/clothApp/clothApp.h>
#include <freecloth/clothApp/clothAppConfig.h>
#include <freecloth/simulator/simStepStrategyBasic.h>
#include <freecloth/simulator/simStepStrategyAdaptive.h>
#include <freecloth/colour/colColourRGB.h>
#include <freecloth/geom/geMatrix4.h>
#include <freecloth/geom/geMesh.h>
#include <freecloth/geom/geMeshBuilder.h>
#include <freecloth/gfx/gfxGL.h>
#include <freecloth/gfx/gfxGLWindowGLUI.h>
#include <freecloth/gfx/gfxGLTexture.h>
#include <freecloth/gfx/gfxImage.h>
#include <freecloth/gfx/gfxImageReaderPNM.h>
#include <freecloth/gfx/gfxImageWriterPNM.h>
#include <freecloth/resmgt/resConfigRegistryFile.h>
#include <freecloth/base/baStringUtil.h>
#include <freecloth/base/GL_gl.h>
#include <freecloth/base/GL_glu.h>
#include <freecloth/base/stdio.h>
#include <freecloth/base/iomanip>

namespace {
    const Float DEFAULT_H = .01f;
    const Float DEFAULT_RHO = .1f;
    const Float DEFAULT_PCG_TOLERANCE = 1e-2f;
}

//------------------------------------------------------------------------------

class ClothAppArgs
{
public:
    ClothAppArgs( int argc, const char** argv );

    void printSyntax();

    String _settings;
    String _snapPrefix;
    String _statsPrefix;

    SimSimulator::Params _params;
    UInt32 _nbPatches;
    Float _clothSize;
    Float _h;
    Float _rho;
    Float _pcgTolerance;
    bool _adaptive;
    UInt32 _frameRate;
    Float _stretchLimit;
    ClothApp::ConstraintType _constraint;
    bool _batchFlag;
    BaTime::Instant _batchEnd;
    bool _crop;
    UInt32 _cropL, _cropB, _cropW, _cropH;

    bool _error;

private:
    template <class InputIterator>
    void parseArgs( InputIterator first, InputIterator last );

};

//------------------------------------------------------------------------------

ClothAppArgs::ClothAppArgs( int argc, const char** argv )
  : _settings( "" ),
    _snapPrefix( "" ),
    _statsPrefix( "" ),
    _nbPatches( 11 ),
    _clothSize( 1 ),
    _h( DEFAULT_H ),
    _rho( DEFAULT_RHO ),
    _pcgTolerance( DEFAULT_PCG_TOLERANCE ),
    _adaptive( true ),
    _frameRate( 25 ),
    _stretchLimit( 0.01f ),
    _constraint( ClothApp::CON_CORNERS3b ),
    _batchFlag( false ),
    _crop( false )
{
    parseArgs( argv + 1, argv + argc );
}

//------------------------------------------------------------------------------

void ClothAppArgs::printSyntax()
{
    std::cerr
        << "Syntax: clothApp [options]" << std::endl
        << "    -settings name     Settings filename" << std::endl
        << "    -snapPrefix name   Prefix for snapshot/movie filenames" << std::endl
        << "    -statsPrefix name  Save energy statistics using given prefix" << std::endl
        << "    -crop l b w h      Crop snapshots to given rectangle" << std::endl
        << "    -nbPatches n       Number of patches" << std::endl
        << "    -clothSize x       Length of cloth in metres" << std::endl
        << "    -stretch x         Stretch constant" << std::endl
        << "    -shear x           Shear constant" << std::endl
        << "    -bend x y          Bend u/v constants" << std::endl
        << "    -stretchDamp x     Stretch damping constant" << std::endl
        << "    -shearDamp x       Shear damping constant" << std::endl
        << "    -bendDamp x        Bend damping constant" << std::endl
        << "    -drag x            Drag constant" << std::endl
        // FIXME: need b_u, b_v
        << "    -gravity x         Gravity constant" << std::endl
        << "    -density x         Density, in kg/m^2" << std::endl
        << "    -tolerance x       Tolerance of PCG algorithm" << std::endl
        << "    -noAdaptive        Disable adaptive timestepping" << std::endl
        << "    -timestep x        Timestep (nonadaptive only)" << std::endl
        << "    -stretchLimit x    Stretch limit" << std::endl
        << "    -frameRate x       Framerate for adaptive stepping" << std::endl
        << "    -constraint [none|centre|corners{4,3a,3b,1c,1d}|yank|table_square|" << std::endl
        << "        table_circle]  Constraint type" << std::endl
        << "    -batch t           Run and record movie, exiting when time t is reached" << std::endl
        ;
}

//------------------------------------------------------------------------------

template <class InputIterator>
void ClothAppArgs::parseArgs( InputIterator first, InputIterator last )
{
    _error = false;
    InputIterator i;
    for ( i = first; i != last && !_error; ++i ) {
        if ( std::string( "-settings" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _settings = *i;
        }
        else if ( std::string( "-snapPrefix" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _snapPrefix = *i;
        }
        else if ( std::string( "-statsPrefix" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _statsPrefix = *i;
        }
        else if ( std::string( "-crop" ) == *i ) {
            _crop = true;
            ++i; if ( i == last ) { _error = true; break; }
            _cropL = BaStringUtil::toInt32( *i );
            ++i; if ( i == last ) { _error = true; break; }
            _cropB = BaStringUtil::toInt32( *i );
            ++i; if ( i == last ) { _error = true; break; }
            _cropW = BaStringUtil::toInt32( *i );
            ++i; if ( i == last ) { _error = true; break; }
            _cropH = BaStringUtil::toInt32( *i );
        }
        else if ( std::string( "-nbPatches" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _nbPatches = BaStringUtil::toInt32( *i );
        }
        else if ( std::string( "-clothSize" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _clothSize = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-stretch" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_stretch = BaStringUtil::toFloat( *i ) * 1000;
        }
        else if ( std::string( "-shear" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_shear = BaStringUtil::toFloat( *i ) * 1000;
        }
        else if ( std::string( "-bend" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_bend_u = BaStringUtil::toFloat( *i ) / 1000;
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_bend_v = BaStringUtil::toFloat( *i ) / 1000;
        }
        else if ( std::string( "-stretchDamp" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_stretch_damp = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-shearDamp" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_shear_damp = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-bendDamp" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_bend_damp = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-drag" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._k_drag = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-gravity" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _params._g = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-density" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _rho = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-tolerance" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _pcgTolerance = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-noAdaptive" ) == *i ) {
            _adaptive = false;
        }
        else if ( std::string( "-timestep" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _h = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-stretchLimit" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _stretchLimit = BaStringUtil::toFloat( *i );
        }
        else if ( std::string( "-frameRate" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _frameRate = BaStringUtil::toInt32( *i );
        }
        else if ( std::string( "-constraint" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            const String names[ ClothApp::NB_CONSTRAINTS ] = {
                "none",

                "centre",
                "corners4",
                "corners3a",
                "corners3b",
                "corners1c",
                "corners1d",
                "yank",
                "table_square",
                "table_circle"
            };
            String name = BaStringUtil::toLower( *i );
            _error = true;
            for ( Int32 i = 0; i < ClothApp::NB_CONSTRAINTS; ++i ) {
                if ( name == names[ i ] ) {
                    _constraint = static_cast<ClothApp::ConstraintType>( i );
                    _error = false;
                }
            }
        }
        else if ( std::string( "-batch" ) == *i ) {
            ++i; if ( i == last ) { _error = true; break; }
            _batchFlag = true;
            _batchEnd = BaTime::floatAsInstant( BaStringUtil::toFloat( *i ) );
        }
        else {
            _error = true;
        }
    }
    if ( i != last ) {
        _error = true;
    }
    if ( _error ) {
        printSyntax();
    }
}

//------------------------------------------------------------------------------

// Create mesh with double the resolution of the input mesh.
RCShdPtr<GeMesh> refineMesh(
    const GeMesh& mesh,
    UInt32 nbRows,
    UInt32 nbCols
) {
    UInt32 oldCols = nbCols;
    DGFX_ASSERT( (nbRows + 1) * (nbCols + 1) == mesh.getNbVertices() );
    nbRows *= 2;
    nbCols *= 2;

    GeMeshBuilder builder;
    UInt32 r, c;
    builder.preallocVertices( (nbRows + 1) * (nbCols + 1 ) );
    builder.preallocTextureVertices( (nbRows + 1) * (nbCols + 1 ) );
    builder.preallocFaces( nbRows * nbCols );
    for ( r = 0; r <= nbRows; ++r ) {
        for ( c = 0; c <= nbCols; ++c ) {
            if ( r % 2 == 0 ) {
                if ( c % 2 == 0 ) {
                    builder.addVertex(
                        mesh.getVertex( (r/2)*(oldCols+1) + c/2 )
                    );
                    builder.addTextureVertex(
                        mesh.getTextureVertex( (r/2)*(oldCols+1) + c/2 )
                    );
                }
                else {
                    builder.addVertex(
                        GePoint(
                            mesh.getVertex( (r/2)*(oldCols+1) + c/2 ),
                            mesh.getVertex( (r/2)*(oldCols+1) + c/2+1 ),
                            .5
                        )
                    );
                    builder.addTextureVertex(
                        GePoint(
                            mesh.getTextureVertex( (r/2)*(oldCols+1) + c/2 ),
                            mesh.getTextureVertex( (r/2)*(oldCols+1) + c/2+1 ),
                            .5
                        )
                    );
                }
            } else {
                if ( c % 2 == 0 ) {
                    builder.addVertex(
                        GePoint(
                            mesh.getVertex( (r/2)*(oldCols+1) + c/2 ),
                            mesh.getVertex( (r/2+1)*(oldCols+1) + c/2 ),
                            .5
                        )
                    );
                    builder.addTextureVertex(
                        GePoint(
                            mesh.getTextureVertex( (r/2)*(oldCols+1) + c/2 ),
                            mesh.getTextureVertex( (r/2+1)*(oldCols+1) + c/2 ),
                            .5
                        )
                    );
                }
                else {
                    builder.addVertex(
                        GePoint(
                            mesh.getVertex( (r/2)*(oldCols+1) + c/2+1 ),
                            mesh.getVertex( (r/2+1)*(oldCols+1) + c/2 ),
                            .5
                        )
                    );
                    builder.addTextureVertex(
                        GePoint(
                            mesh.getTextureVertex( (r/2)*(oldCols+1) + c/2+1 ),
                            mesh.getTextureVertex( (r/2+1)*(oldCols+1) + c/2 ),
                            .5
                        )
                    );
                }
            }
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

////////////////////////////////////////////////////////////////////////////////
// CLASS ClothApp

//------------------------------------------------------------------------------

ClothApp::ClothApp( const ClothAppArgs& args )
  : _snapPrefix( args._snapPrefix ),
    _statsPrefix( args._statsPrefix ),
    _params( args._params ),
    _nbPatches( args._nbPatches ),
    _clothSize( args._clothSize ),
    _h( args._h ),
    _rho( args._rho ),
    _pcgTolerance( args._pcgTolerance ),
    _frameRate( args._frameRate ),
    _stretchLimit( args._stretchLimit ),
    _constraints( args._constraint ),
    _batchFlag( args._batchFlag ),
    _batchEnd( args._batchEnd ),
    _quitFlag( false ),
    _rewindFlag( false ),
    _freeRunFlag( args._batchFlag ),
    _stopFlag( false ),
    _nbPendingSteps( 0 ),
    _snapFlag( false ),
    _snapCount( 0 ),
    _subStepFlag( false ),
    _cropFlag( args._crop ),
    _cropL( args._cropL ),
    _cropB( args._cropB ),
    _cropW( args._cropW ),
    _cropH( args._cropH )
{
    setupMesh();

    loadSettings( args._settings );
    _glWindow->setRadioGroup(
        ID_STEP_STRATEGY, args._adaptive ? STEP_ADAPTIVE : STEP_BASIC
    );

    // Read textures
    RCShdPtr<GfxImage> texImage;

    texImage = GfxImageReaderPNM( "marble.ppm" ).readImage();
    if ( texImage.isNull() ) {
        texImage = GfxImageReaderPNM( "clothApp/marble.ppm" ).readImage();
    }
    if ( texImage.isNull() ) {
        texImage =
            GfxImageReaderPNM( "freecloth/clothApp/marble.ppm" ).readImage();
    }
    if ( ! texImage.isNull() ) {
        _floorTexture = RCShdPtr<GfxGLTexture>(
            new GfxGLTexture( texImage, GL_RGB )
        );
        _floorTexture->generateMipmaps();
    }

    texImage = GfxImageReaderPNM( "checker3.ppm" ).readImage();
    if ( texImage.isNull() ) {
        texImage = GfxImageReaderPNM( "clothApp/checker3.ppm" ).readImage();
    }
    if ( texImage.isNull() ) {
        texImage =
            GfxImageReaderPNM( "freecloth/clothApp/checker3.ppm" ).readImage();
    }
    if ( ! texImage.isNull() ) {
        _clothTexture = RCShdPtr<GfxGLTexture>(
            new GfxGLTexture( texImage, GL_RGB )
        );
        _clothTexture->generateMipmaps();
    }

    setupSimulator();

    // Zero these files.
    if ( _statsPrefix.length() > 0 ) {
        std::ofstream( ( _statsPrefix + "stretch.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "shear.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "bend.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "stretch-mean.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "shear-mean.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "bend-mean.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "stretch-tris.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "shear-tris.txt" ).c_str() );
        std::ofstream( ( _statsPrefix + "bend-tris.txt" ).c_str() );
    }
}

//------------------------------------------------------------------------------

void ClothApp::saveSettings( const String& filename ) const
{
    ClothAppConfig config;
    config.setCameraRotate( _glWindow->getRotate( ID_CAMERA_ROTATE ) );
    config.setCameraZoom( _glWindow->getTranslateZ( ID_ZOOM ) );
    config.setLightRotate( _glWindow->getRotate( ID_LIGHT ) );
    config.setFogStart( _glWindow->getTranslateZ( ID_FOG_START ) );
    config.setFogEnd( _glWindow->getTranslateZ( ID_FOG_END ) );
    GfxConfig::WindowInfo windowInfo;
    windowInfo._width = _glWindow->getWidth();
    windowInfo._height = _glWindow->getHeight();
    // FIXME: need way to retrieve these
    windowInfo._x = 0;
    windowInfo._y = 0;
    config.setWindowInfo( windowInfo );
    ResConfigRegistryWFile reg( filename );
    config.save( reg );
}

//------------------------------------------------------------------------------

void ClothApp::loadSettings( const String& filename )
{
    ClothAppConfig config;
    if ( filename.length() > 0 ) {
        ResConfigRegistryRFile reg( filename );
        config.load( reg );
    }
    // FIXME: need way to load window size settings after startup
    setupWindow( config );

    _glWindow->setRotate( ID_CAMERA_ROTATE, config.getCameraRotate() );
    _glWindow->setTranslateZ( ID_ZOOM, config.getCameraZoom() );
    _glWindow->setRotate( ID_LIGHT, config.getLightRotate() );
    _glWindow->setTranslateZ( ID_FOG_START, config.getFogStart() );
    _glWindow->setTranslateZ( ID_FOG_END, config.getFogEnd() );
}

//------------------------------------------------------------------------------

void ClothApp::runApp()
{
    _glWindow->eventLoop();
}

//------------------------------------------------------------------------------

void ClothApp::setupMesh()
{
    _initialMesh = createRectMesh( _clothSize, _nbPatches, _nbPatches );
    DGFX_TRACE( ! _initialMesh.isNull() );
    // Centre mesh
    _meshPos = GePoint( _clothSize/-2.f, _clothSize/-2.f, 0 );

    _meshWE = RCShdPtr<GeMeshWingedEdge>(
        new GeMeshWingedEdge( _initialMesh )
    );
    _meshNormals.resize( _initialMesh->getNbVertices() );
    initMeshIndices( *_initialMesh, _meshIndices );

    _meshTextureVertices.clear();
    _meshTextureVertices.reserve( _initialMesh->getNbVertices() );

    UInt32 r,c;
    const Float NB_REPS = 16.5f;
    for ( r = 0; r <= _nbPatches; ++r ) {
        for ( c = 0; c <= _nbPatches; ++c ) {
            // Always NB_REPS repetitions of texture across cloth.
            _meshTextureVertices.push_back(
                GePoint( c * NB_REPS / _nbPatches, r * NB_REPS / _nbPatches, 0 )
            );
        }
    }
}

//------------------------------------------------------------------------------

void ClothApp::setupSimulator()
{
    _simulator = RCShdPtr<SimSimulator>(
        new SimSimulator( *_initialMesh )
    );
    _simulator->setTimestep( BaTime::floatAsDuration( _h ) );
    _simulator->setDensity( _rho );
    _simulator->setParams( _params );
    _simulator->setPCGTolerance( _pcgTolerance );
    _simulator->setStretchLimit( _stretchLimit );
    setConstraints();
    _nextMovieFrame = 0;

    setupStepper();
}

//------------------------------------------------------------------------------

void ClothApp::setupStepper()
{
    switch( _glWindow->getRadioGroup( ID_STEP_STRATEGY ) ) {
        case STEP_BASIC: {
            _stepper = RCShdPtr<SimStepStrategy>(
                new SimStepStrategyBasic( _simulator )
            );
            _glWindow->enable( ID_STEP_TIMESTEP );
            _glWindow->disable( ID_STEP_FRAME_RATE );
            _glWindow->disable( ID_STEP_STRETCH_LIMIT );
        } break;
        case STEP_ADAPTIVE: {
            _stepper = RCShdPtr<SimStepStrategy>(
                new SimStepStrategyAdaptive( _simulator, _frameRate )
            );
            _glWindow->disable( ID_STEP_TIMESTEP );
            _glWindow->enable( ID_STEP_FRAME_RATE );
            _glWindow->enable( ID_STEP_STRETCH_LIMIT );
        } break;
    }
}

//------------------------------------------------------------------------------

void ClothApp::setConstraints()
{
    const UInt32 N = _nbPatches;
    _simulator->removeAllConstraints();
    switch ( _constraints ) {
        case CON_NONE: {
        } break;

        case CON_CENTRE: {
            _simulator->setPosConstraintFull( (N+1)*(N/2) + N/2 );
        } break;

        case CON_CORNERS4: {
            _simulator->setPosConstraintFull( 0 );
            _simulator->setPosConstraintFull( N );
            _simulator->setPosConstraintFull( N*(N+1) );
            _simulator->setPosConstraintFull( (N+1)*(N+1)-1 );
        } break;

        case CON_CORNERS3a: {
            _simulator->setPosConstraintFull( N );
            _simulator->setPosConstraintFull( N*(N+1) );
            _simulator->setPosConstraintFull( (N+1)*(N+1)-1 );
        } break;

        case CON_CORNERS3b: {
            _simulator->setPosConstraintFull( 0 );
            _simulator->setPosConstraintFull( N*(N+1) );
            _simulator->setPosConstraintFull( (N+1)*(N+1)-1 );
        } break;

        case CON_CORNERS1c: {
            _simulator->setPosConstraintFull( N*(N+1) );
        } break;

        case CON_CORNERS1d: {
            _simulator->setPosConstraintFull( (N+1)*(N+1)-1 );
        } break;

        case CON_CORNERYANK: {
            _simulator->setPosConstraintFull( (N+1)*(N+1)-1 );
            _simulator->setVelConstraint(
                (N+1)*(N+1)-1, -.1f * GeVector::xAxis()
            );
        } break;

        case CON_TABLE_SQUARE: {
            const UInt32 C = BaMath::roundUInt32( std::max( 1.f, N*5/11.f ) );
            const UInt32 x = (N-C)/2;
            for (UInt32 i = 0; i <= C; ++i ) for ( UInt32 j = 0; j <= C; ++j ) {
                _simulator->setPosConstraintFull( (x + i)*(N+1) + x + j );
            }
        } break;

        case CON_TABLE_CIRCLE: {
            const Float r = 7/11.f;
            for (UInt32 i = 0; i <= N; ++i ) for ( UInt32 j = 0; j <= N; ++j ) {
                Float x = 2*i/static_cast<Float>( N ) - 1;
                Float y = 2*j/static_cast<Float>( N ) - 1;
                if ( x*x + y*y < r*r ) {
                    _simulator->setPosConstraintFull( i*(N+1) + j );
                }
            }
        } break;
    }
}

//------------------------------------------------------------------------------

void ClothApp::setupWindow( const GfxConfig& config )
{
    // Create GL window, monitor for events
    _glWindow = RCShdPtr<GfxGLWindowGLUI>( new GfxGLWindowGLUI(
        config, "Cloth test app"
    ) );
    _glWindow->addObserver( *this );
    _glWindow->enableIdleEvents( true );

    enum {
        PANEL_CAMERA,
        PANEL_SCENE, PANEL_SCENE2,
        PANEL_EXEC,
        PANEL_STEP,
        PANEL_PARAMS,
        PANEL_CON,
        PANEL_SNAPS, PANEL_SNAPS2,
        PANEL_DEBUG,
        PANEL_ENERGY,
        PANEL_TRI,
        PANEL_VERT
    };
    _glWindow->addGroup( GfxGLWindowGLUI::BORDER_RAISED, PANEL_CAMERA );
    _glWindow->addRotate( "Camera", ID_CAMERA_ROTATE, PANEL_CAMERA );
    _glWindow->addColumn( false, PANEL_CAMERA );
    _glWindow->addTranslate(
        "Zoom", GfxGLWindowGLUI::TR_Z, ID_ZOOM, PANEL_CAMERA
    );

    _glWindow->addRollout( "Scene", PANEL_SCENE, false );
    _glWindow->addGroup(
        GfxGLWindowGLUI::BORDER_NONE, PANEL_SCENE2, PANEL_SCENE
    );
        _glWindow->addRotate( "Light", ID_LIGHT, PANEL_SCENE2 );
        _glWindow->addColumn( false, PANEL_SCENE2 );
    _glWindow->addCheckbox( "Axes", ID_AXES, PANEL_SCENE );
    _glWindow->setCheckbox( ID_AXES, false );
    _glWindow->addCheckbox( "Wireframe", ID_WIREFRAME, PANEL_SCENE );
    _glWindow->setCheckbox( ID_WIREFRAME, false );
    _glWindow->addEditFloat( "Cloth size: ", ID_CLOTH_SIZE, PANEL_SCENE );
    _glWindow->setEditFloat( ID_CLOTH_SIZE, _clothSize );

    _glWindow->addPanel( "Simulation", PANEL_EXEC );
    _glWindow->addButton( "Run", ID_RUN, PANEL_EXEC );
    _glWindow->addButton( "Stop", ID_STOP, PANEL_EXEC );
    _glWindow->addButton( "Step", ID_STEP, PANEL_EXEC );
    _glWindow->addButton( "Rewind", ID_REWIND, PANEL_EXEC );
    _glWindow->addText( "Time: ", ID_TIME, PANEL_EXEC );
    _glWindow->addEditInt( "Cloth patches: ", ID_PATCHES, PANEL_EXEC );
    _glWindow->setEditInt( ID_PATCHES, _nbPatches );
    _glWindow->setEditIntLimits( ID_PATCHES, 1, 200 );
    _glWindow->addEditFloat( "Tolerance: ", ID_PCG_TOLERANCE, PANEL_EXEC );
    _glWindow->setEditFloat( ID_PCG_TOLERANCE, _pcgTolerance );

    _glWindow->addRollout( "Stepping", PANEL_STEP, false );
    _glWindow->addRadioGroup( ID_STEP_STRATEGY, PANEL_STEP );
    _glWindow->addRadioButton( ID_STEP_STRATEGY, "Basic" );
    _glWindow->addRadioButton( ID_STEP_STRATEGY, "Adaptive" );
    _glWindow->setRadioGroup( ID_STEP_STRATEGY, STEP_ADAPTIVE );
    _glWindow->addEditFloat( "Timestep: ", ID_STEP_TIMESTEP, PANEL_STEP );
    _glWindow->setEditFloat( ID_STEP_TIMESTEP, _h );
    _glWindow->addEditFloat( "Framerate: ", ID_STEP_FRAME_RATE, PANEL_STEP );
    _glWindow->setEditInt( ID_STEP_FRAME_RATE, _frameRate );
    _glWindow->setEditIntLimits( ID_STEP_FRAME_RATE, 1, 1000 );
    _glWindow->addEditFloat(
        "Stretch limit: ", ID_STEP_STRETCH_LIMIT, PANEL_STEP
    );
    _glWindow->setEditFloat( ID_STEP_STRETCH_LIMIT, _stretchLimit );

    _glWindow->addRollout( "Parameters", PANEL_PARAMS, false );
    _glWindow->addEditFloat( "k_stretch (k)", ID_PAR_K_STRETCH, PANEL_PARAMS );
    _glWindow->addEditFloat( "k_shear (k)", ID_PAR_K_SHEAR, PANEL_PARAMS );
    _glWindow->addEditFloat( "k_bend_u (m)", ID_PAR_K_BEND_U, PANEL_PARAMS );
    _glWindow->addEditFloat( "k_bend_v (m)", ID_PAR_K_BEND_V, PANEL_PARAMS );
    _glWindow->addEditFloat(
        "k_stretch_damp", ID_PAR_K_STRETCH_DAMP, PANEL_PARAMS
    );
    _glWindow->addEditFloat(
        "k_shear_damp", ID_PAR_K_SHEAR_DAMP, PANEL_PARAMS
    );
    _glWindow->addEditFloat(
        "k_bend_damp", ID_PAR_K_BEND_DAMP, PANEL_PARAMS
    );
    _glWindow->addEditFloat( "k_drag", ID_PAR_K_DRAG, PANEL_PARAMS );
    _glWindow->addEditFloat( "b_u", ID_PAR_B_U, PANEL_PARAMS );
    _glWindow->addEditFloat( "b_v", ID_PAR_B_V, PANEL_PARAMS );
    _glWindow->addEditFloat( "density", ID_PAR_RHO, PANEL_PARAMS );
    _glWindow->addEditFloat( "gravity", ID_PAR_G, PANEL_PARAMS );
    _glWindow->addButton( "Reset", ID_PAR_RESET, PANEL_PARAMS );
    updateParamsUI();

    _glWindow->addRollout( "Constraints", PANEL_CON, false );
    _glWindow->addRadioGroup( ID_CONSTRAINTS, PANEL_CON );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "None" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Centre" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (4)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (3a)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (3b)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (1c)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (1d)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Corners (1d, moving)" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Square table" );
    _glWindow->addRadioButton( ID_CONSTRAINTS, "Circular table" );
    _glWindow->setRadioGroup( ID_CONSTRAINTS, _constraints );

    _glWindow->addRollout( "Snapshots", PANEL_SNAPS, false );
    _glWindow->addButton( "Snapshot", ID_SNAPSHOT, PANEL_SNAPS );
    _glWindow->addCheckbox( "Movie", ID_MOVIE, PANEL_SNAPS );
    _glWindow->setCheckbox( ID_MOVIE, _batchFlag );
    _glWindow->addGroup(
        GfxGLWindowGLUI::BORDER_NONE, PANEL_SNAPS2, PANEL_SNAPS
    );
        _glWindow->addTranslate(
            "Fog start", GfxGLWindowGLUI::TR_Z, ID_FOG_START, PANEL_SNAPS2
        );
        _glWindow->addColumn( false, PANEL_SNAPS2 );
        _glWindow->addTranslate(
            "Fog end", GfxGLWindowGLUI::TR_Z, ID_FOG_END, PANEL_SNAPS2
        );
    _glWindow->addButton( "Load settings", ID_SETTINGS_LOAD, PANEL_SNAPS );
    _glWindow->addButton( "Save settings", ID_SETTINGS_SAVE, PANEL_SNAPS );
    _glWindow->addEditText(
        "File", ID_SETTINGS_FILENAME, PANEL_SNAPS
    );
    _glWindow->setEditText( ID_SETTINGS_FILENAME, "clothAppSettings.txt" );

    _glWindow->addRollout( "Debug", PANEL_DEBUG, false );
    _glWindow->addPanel( "Energy", PANEL_ENERGY, PANEL_DEBUG );
    _glWindow->addText( "Stretch: ", ID_EN_STRETCH, PANEL_ENERGY );
    _glWindow->addText( "Shear: ", ID_EN_SHEAR, PANEL_ENERGY );
    _glWindow->addText( "Bend: ", ID_EN_BEND, PANEL_ENERGY );

    _glWindow->addRollout( "Triangles", PANEL_TRI, false, PANEL_DEBUG );
    _glWindow->addCheckbox( "Stretch energy", ID_TRI_STRETCH, PANEL_TRI );
    _glWindow->setCheckbox( ID_TRI_STRETCH, false );
    _glWindow->addCheckbox( "Shear energy", ID_TRI_SHEAR, PANEL_TRI );
    _glWindow->setCheckbox( ID_TRI_SHEAR, false );
    _glWindow->addCheckbox( "Bend energy", ID_TRI_BEND, PANEL_TRI );
    _glWindow->setCheckbox( ID_TRI_BEND, false );

    _glWindow->addRollout( "Vertices", PANEL_VERT, false, PANEL_DEBUG );
    _glWindow->addCheckbox( "Velocity", ID_VERT_VEL, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_VEL, false );
    _glWindow->addCheckbox( "Total force", ID_VERT_FORCE, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE, false );
#if !defined( NDEBUG )
    _glWindow->addCheckbox( "Damping forces", ID_VERT_FORCE_DAMP, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE_DAMP, false );
    _glWindow->addCheckbox( "Stretch force", ID_VERT_FORCE_STRETCH, PANEL_VERT);
    _glWindow->setCheckbox( ID_VERT_FORCE_STRETCH, false );
    _glWindow->addCheckbox( "Shear force", ID_VERT_FORCE_SHEAR, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE_SHEAR, false );
    _glWindow->addCheckbox( "Bend force", ID_VERT_FORCE_BEND, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE_BEND, false );
    _glWindow->addCheckbox( "Gravity force", ID_VERT_FORCE_GRAVITY, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE_GRAVITY, false );
    _glWindow->addCheckbox( "Drag force", ID_VERT_FORCE_DRAG, PANEL_VERT );
    _glWindow->setCheckbox( ID_VERT_FORCE_DRAG, false );
#endif

    _glWindow->addButton( "Quit", ID_QUIT );

    initGL();
}

//------------------------------------------------------------------------------

void ClothApp::updateParamsUI()
{
    _glWindow->setEditFloat( ID_PAR_K_STRETCH, _params._k_stretch / 1000.f );
    _glWindow->setEditFloat( ID_PAR_K_SHEAR, _params._k_shear / 1000.f );
    _glWindow->setEditFloat( ID_PAR_K_BEND_U, _params._k_bend_u * 1000.f );
    _glWindow->setEditFloat( ID_PAR_K_BEND_V, _params._k_bend_v * 1000.f );
    _glWindow->setEditFloat( ID_PAR_K_STRETCH_DAMP, _params._k_stretch_damp );
    _glWindow->setEditFloat( ID_PAR_K_SHEAR_DAMP, _params._k_shear_damp );
    _glWindow->setEditFloat( ID_PAR_K_BEND_DAMP, _params._k_bend_damp );
    _glWindow->setEditFloat( ID_PAR_K_DRAG, _params._k_drag );
    _glWindow->setEditFloat( ID_PAR_B_U, _params._b_u );
    _glWindow->setEditFloat( ID_PAR_B_V, _params._b_v );
    _glWindow->setEditFloat( ID_PAR_RHO, _rho );
    _glWindow->setEditFloat( ID_PAR_G, _params._g );
}

//------------------------------------------------------------------------------

void ClothApp::closeReceived( GfxWindow& )
{
    _glWindow->removeObserver( *this );
    ::exit( 0 );
}

//------------------------------------------------------------------------------

void ClothApp::uiReceived( GfxWindow&, UInt32 uid )
{
    switch ( uid ) {
        case ID_CAMERA_ROTATE:
        case ID_ZOOM:
        case ID_LIGHT:
        case ID_FOG_START:
        case ID_FOG_END:
        case ID_AXES:
        case ID_WIREFRAME: {
            _glWindow->postRedisplay();
        } break;

        case ID_PATCHES:
        case ID_CLOTH_SIZE: {
            forceFinishStep();

            UInt32 oldNbPatches = _nbPatches;
            if ( uid == ID_PATCHES ) {
                _nbPatches = _glWindow->getEditInt( ID_PATCHES );
            }
            else {
                _clothSize = _glWindow->getEditFloat( ID_CLOTH_SIZE );
            }
            
            // Special case: if the user doubles the number of patches,
            // then we can refine and keep the current state.
            RCShdPtr<GeMesh> refinedMesh;
            Float ratio( static_cast<Float>( _nbPatches ) / oldNbPatches );
            if (
                _nbPatches > oldNbPatches &&
                BaMath::isInteger( ratio ) &&
                BaMath::isPow2( BaMath::roundUInt32( ratio ) )
            ) {
                refinedMesh = _simulator->getMeshPtr();
                for ( ; oldNbPatches < _nbPatches; oldNbPatches *= 2 ) {
                    refinedMesh = refineMesh(
                        *refinedMesh,
                        oldNbPatches,
                        oldNbPatches
                    );
                }
            }

            setupMesh();
            setupSimulator();
            if ( ! refinedMesh.isNull() ) {
                _simulator->setMesh( *refinedMesh );
            }
            _glWindow->postRedisplay();
        } break;

        case ID_RUN: {
            _freeRunFlag = true;
        } break;
        case ID_STOP: {
            _stopFlag = true;
        } break;
        case ID_STEP: {
            ++_nbPendingSteps;
        } break;
        case ID_REWIND: {
            _rewindFlag = true;
            _glWindow->postRedisplay();
        } break;
        case ID_STEP_STRATEGY: {
            setupStepper();
        } break;
        case ID_STEP_TIMESTEP: {
            _h = _glWindow->getEditFloat( uid );
            _simulator->setTimestep( BaTime::floatAsDuration( _h ) );
        } break;
        case ID_STEP_FRAME_RATE: {
            _frameRate = _glWindow->getEditInt( uid );
            if (
                _glWindow->getRadioGroup( ID_STEP_STRATEGY ) == STEP_ADAPTIVE
            ) {
                dynamic_cast<SimStepStrategyAdaptive*>( _stepper.get() )
                    ->setFrameRate( _frameRate );
            }
        } break;
        case ID_STEP_STRETCH_LIMIT: {
            _stretchLimit = _glWindow->getEditFloat( uid );
            _simulator->setStretchLimit( _stretchLimit );
        } break;

        case ID_PCG_TOLERANCE: {
            _pcgTolerance = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setPCGTolerance( _pcgTolerance );
        } break;

        case ID_PAR_K_STRETCH: {
            _params._k_stretch = _glWindow->getEditFloat( uid ) * 1000.f;
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_K_SHEAR: {
            _params._k_shear = _glWindow->getEditFloat( uid ) * 1000.f;
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_K_BEND_U: {
            _params._k_bend_u = _glWindow->getEditFloat( uid ) / 1000.f;
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_K_BEND_V: {
            _params._k_bend_v = _glWindow->getEditFloat( uid ) / 1000.f;
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_K_STRETCH_DAMP: {
            _params._k_stretch_damp = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;
        case ID_PAR_K_SHEAR_DAMP: {
            _params._k_shear_damp = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;
        case ID_PAR_K_BEND_DAMP: {
            _params._k_bend_damp = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;
        case ID_PAR_K_DRAG: {
            _params._k_drag = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_B_U: {
            _params._b_u = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_B_V: {
            _params._b_v = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_RHO: {
            _rho = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setDensity( _rho );
            // Must readd constraints after call to setDensity
            setConstraints();
        } break;

        case ID_PAR_G: {
            _params._g = _glWindow->getEditFloat( uid );
            forceFinishStep();
            _simulator->setParams( _params );
        } break;

        case ID_PAR_RESET: {
            _params = SimSimulator::Params();
            _rho = DEFAULT_RHO;
            forceFinishStep();
            _simulator->setParams( _params );
            _simulator->setDensity( _rho );
            updateParamsUI();
        } break;

        case ID_CONSTRAINTS: {
            _constraints = static_cast<ConstraintType>(
                _glWindow->getRadioGroup( uid )
            );
            forceFinishStep();
            setConstraints();
        } break;

        case ID_TRI_STRETCH:
        case ID_TRI_SHEAR:
        case ID_TRI_BEND: {
            _glWindow->postRedisplay();
        } break;

        case ID_VERT_VEL:
        case ID_VERT_FORCE:
        case ID_VERT_FORCE_DAMP:
        case ID_VERT_FORCE_STRETCH:
        case ID_VERT_FORCE_SHEAR:
        case ID_VERT_FORCE_BEND:
        case ID_VERT_FORCE_GRAVITY:
        case ID_VERT_FORCE_DRAG: {
            _glWindow->postRedisplay();
        } break;

        case ID_SNAPSHOT: {
            _snapFlag = true;
        } break;

        case ID_SETTINGS_LOAD: {
            loadSettings( _glWindow->getEditText( ID_SETTINGS_FILENAME ) );
            _glWindow->postRedisplay();
        } break;

        case ID_SETTINGS_SAVE: {
            saveSettings( _glWindow->getEditText( ID_SETTINGS_FILENAME ) );
        } break;

        case ID_QUIT: {
            _quitFlag = true;
        } break;
    }
}

//------------------------------------------------------------------------------

void ClothApp::idleReceived()
{
    if ( _quitFlag ) {
        if ( ! _glWindow.isNull() ) {
            _glWindow->close();
        }
        return;
    }

    bool redisplay = false;

    if ( _rewindFlag ) {
        setupSimulator();

        _rewindFlag = false;
        _freeRunFlag = false;
        _stopFlag = false;
        _subStepFlag = false;
        _snapCount = 0;
        _nbPendingSteps = 0;
        redisplay = true;
    }

    if ( _stopFlag ) {
        if ( _stepper->inStep() ) {
            _stepper->cancelStep();
        }
        _subStepFlag = false;
        _freeRunFlag = false;
        _nbPendingSteps = 0;
        _stopFlag = false;
    }
    if ( _nbPendingSteps > 0 && !_stepper->inStep() ) {
        _stepper->preSubSteps();
        _subStepFlag = true;
        --_nbPendingSteps;
    }
    if ( _freeRunFlag ) {
        if ( ! _subStepFlag ) {
            // If autorun is on and we've passed the end of autorun time,
            // exit automatically.
            if (
                _batchFlag &&
                _simulator->getTime() > _batchEnd
            ) {
                _quitFlag = true;
            }
            else {
                _stepper->preSubSteps();
                _subStepFlag = true;
            }
        }
    }
    if ( _subStepFlag ) {
        if ( _stepper->subStepsDone() ) {
            _stepper->postSubSteps();
            _subStepFlag = false;
            redisplay = true;
            // If free run is on, the substeps will restart with next idle.
            
            _h = BaTime::durationAsSeconds( _simulator->getTimestep() );
            _glWindow->setEditFloat( ID_STEP_TIMESTEP, _h );

            saveStats();
        }
        else {
            _stepper->subStep();
        }
    }

    // FIXME: this is pretty naive...
    if (
        _glWindow->getCheckbox( ID_MOVIE ) &&
        BaMath::isLess(
            _nextMovieFrame / static_cast<Float>( _frameRate ),
            BaTime::instantAsSeconds( _simulator->getTime() )
        )
    ) {
        _snapFlag = true;
        ++_nextMovieFrame; 
    }

    if ( redisplay ) {
        _glWindow->postRedisplay();
    }
}

//------------------------------------------------------------------------------

void ClothApp::forceFinishStep()
{
    while ( _stepper->inStep() ) {
        // This will continue a step.
        idleReceived();
    }
}

//------------------------------------------------------------------------------

RCShdPtr<GeMesh> ClothApp::createRectMesh(
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

//------------------------------------------------------------------------------

void ClothApp::initMeshIndices(
    const GeMesh& mesh,
    std::vector<UInt32>& indices
) {
    GeMesh::FaceConstIterator fi;
    indices.clear();
    indices.reserve( mesh.getNbFaces() * GeMesh::FaceType::NB_VERTICES );
    for ( fi = mesh.beginFace(); fi != mesh.endFace(); ++fi ) {
        for ( UInt32 i = 0; i < fi->getNbVertices(); ++i ) {
            indices.push_back( fi->getVertexId( i ) );
        }
    }
}

//------------------------------------------------------------------------------

void ClothApp::initGL()
{
    ::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    ::glShadeModel( GL_SMOOTH );

    GL::fog( ColColourRGB::BLACK, 1.f );
    ::glFogi( GL_FOG_MODE, GL_LINEAR );
    ::glFogf( GL_FOG_DENSITY, .03f );

    ::glDisable( GL_TEXTURE_2D );
    ::glEnable( GL_LIGHTING );
    ::glEnable( GL_DEPTH_TEST );
    ::glDisable( GL_FOG );

    ::glMatrixMode( GL_MODELVIEW );
    ::glLoadIdentity();

    ::glEnable( GL_LIGHT0 );

    ::glEnable( GL_COLOR_MATERIAL );
    ::glColorMaterial( GL_FRONT, GL_DIFFUSE );

    ::glDisable( GL_BLEND );
}

//------------------------------------------------------------------------------

void ClothApp::calcNormals()
{
    const RCShdPtr<GeMesh>& mesh = _simulator->getMeshPtr();
    std::vector<GeVector> faceNormals( mesh->getNbFaces() );
    GeMesh::FaceConstIterator fi;
    for ( fi = mesh->beginFace(); fi != mesh->endFace(); ++fi ) {
        faceNormals[ fi->getFaceId() ] = fi->calcNormal();
    }
    // FIXME: meshWE should be a member of clothApp.
    GeMeshWingedEdge meshWE( mesh );
    for ( GeMesh::VertexId vid = 0; vid < mesh->getNbVertices(); ++vid ) {
        GeMeshWingedEdge::VertexFaceIterator vfi;
        GeVector normal ( GeVector::zero() );
        UInt32 nbFaces = 0;
        for (
            vfi = meshWE.beginVertexFace( vid );
            vfi != meshWE.endVertexFace( vid );
            ++vfi
        ) {
            normal += faceNormals[ vfi->getFaceId() ];
            ++nbFaces;
        }
        if ( nbFaces > 0 ) {
            _meshNormals[ vid ] = normal / nbFaces;
        }
        else {
            _meshNormals[ vid ] = normal;
        }
    }
}

//------------------------------------------------------------------------------

void ClothApp::renderCloth()
{
    GL::material( GL_FRONT, GL_SPECULAR, ColColourRGB::WHITE * .2f );
    ::glMaterialf( GL_FRONT, GL_SHININESS, 128 );

    const GeMesh& mesh = _simulator->getMesh();
    if ( ! _clothTexture.isNull() ) {
        ::glBindTexture( GL_TEXTURE_2D, _clothTexture->getTextureId() );
        ::glEnable( GL_TEXTURE_2D );
        // We seem to have to do this after each glEnable( GL_TEXTURE_2D ),
        // at least with Linux GeForce3 drivers. 2003/04/22
        ::glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR
        );
        ::glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );
    }

    // Note: polygon smoothing can't possibly smooth our edges nicely.
    // Instead, as a post-processing step, we draw the outline of the
    // cloth rectangle with a smoothed line. This at least looks a little
    // nicer.
    ::glEnable( GL_BLEND );
    ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    ::glEnable( GL_LINE_SMOOTH );

    ::glEnableClientState( GL_VERTEX_ARRAY );
    ::glEnableClientState( GL_NORMAL_ARRAY );
    ::glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    if ( sizeof (Float) == sizeof(GLfloat) ) {
        ::glVertexPointer( 3, GL_FLOAT, 0, mesh.getVertexArray() );
        ::glNormalPointer( GL_FLOAT, 0, &_meshNormals.front() );
        ::glTexCoordPointer( 3, GL_FLOAT, 0, &_meshTextureVertices.front() );
    }
    else {
        ::glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexArray() );
        ::glNormalPointer( GL_DOUBLE, 0, &_meshNormals.front() );
        ::glTexCoordPointer( 3, GL_DOUBLE, 0, &_meshTextureVertices.front() );
    }
    ::glPushMatrix();
        GL::translate( _meshPos );
        ::glDrawElements(
            GL_TRIANGLES,
            _meshIndices.size(),
            GL_UNSIGNED_INT,
            &_meshIndices.front()
        );
    ::glPopMatrix();

    ::glDisable( GL_TEXTURE_2D );

    ::glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    ::glDisableClientState( GL_NORMAL_ARRAY );
    ::glDisableClientState( GL_VERTEX_ARRAY );

    ::glDisable( GL_BLEND );
    ::glDisable( GL_LINE_SMOOTH );
}

//------------------------------------------------------------------------------

void ClothApp::renderClothOutline()
{
    ::glEnable( GL_BLEND );
    ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    ::glEnable( GL_LINE_SMOOTH );
    ::glPushAttrib( GL_ENABLE_BIT );
    ::glDisable( GL_LIGHTING );
    GL::colour( ColColourRGB::BLACK );
    ::glPushMatrix();
        GL::translate( _meshPos );
        ::glBegin( GL_LINE_STRIP );
            const GeMesh& mesh = _simulator->getMesh();
            const UInt32 N( _nbPatches );
            UInt32 vid;
            for ( vid = 0; vid <= N; ++vid ) {
                GL::vertex( mesh.getVertex( vid ) );
            }
            for ( vid = 0; vid <= N; ++vid ) {
                GL::vertex( mesh.getVertex( vid * (N+1) + N ) );
            }
            for ( vid = 0; vid <= N; ++vid ) {
                GL::vertex( mesh.getVertex( N*(N+1) + (N-vid) ) );
            }
            for ( vid = 0; vid <= N; ++vid ) {
                GL::vertex( mesh.getVertex( (N-vid) * (N+1) ) );
            }
        ::glEnd();
    ::glPopMatrix();
    ::glPopAttrib();
    ::glDisable( GL_BLEND );
    ::glDisable( GL_LINE_SMOOTH );
}

//------------------------------------------------------------------------------

void ClothApp::renderClothTriDebug(
    bool showStretch,
    bool showShear,
    bool showBend
) {
    const GeMesh& mesh = _simulator->getMesh();
    const bool show[3] = { showStretch, showShear, showBend };
    const SimSimulator::ForceType f[3] = {
        SimSimulator::F_STRETCH,
        SimSimulator::F_SHEAR,
        SimSimulator::F_BEND
    };
    ::glPushAttrib( GL_ENABLE_BIT );
    ::glDisable( GL_LIGHTING );
    ::glPushMatrix();
        GL::translate( _meshPos );
        GeMesh::FaceConstIterator fi;
        for ( fi = mesh.beginFace(); fi != mesh.endFace(); ++fi ) {
            Float en[3];
            const Float area =
                _initialMesh->getFace( fi->getFaceId() ).calcArea();
            const Float MAX_EN[ 3 ] = { 10e-6f, 10e-6f, 10e-6f };
            for ( UInt32 i = 0; i < 3; ++i ) {
                en[ i ] = show[ i ]
                    ? (BaMath::log10(
                        _simulator->getTriEnergy( f[ i ], fi->getFaceId() )
                    ) + 8) / 4
                    : 0.f;
                en[ i ] = std::max( 0.f, std::min( 1.f, en[ i ] ) );
            }

            ::glColor3f( en[0], en[1], en[2] );
            ::glBegin( GL_TRIANGLES );
                GL::vertex( fi->getVertex( 0 ) );
                GL::vertex( fi->getVertex( 1 ) );
                GL::vertex( fi->getVertex( 2 ) );
            ::glEnd();
        }
    ::glPopMatrix();
    ::glPopAttrib();
}

//------------------------------------------------------------------------------

void ClothApp::renderClothVertDebug()
{
    bool showvel = _glWindow->getCheckbox( ID_VERT_VEL );
    bool showf = _glWindow->getCheckbox( ID_VERT_FORCE );
    const UInt32 NB_FS = 5;
#ifdef NDEBUG
    const bool showfs[ NB_FS ] = { false, false, false, false, false };
    const bool showdamp = false;
#else
    const bool showfs[ NB_FS ] = {
        _glWindow->getCheckbox( ID_VERT_FORCE_STRETCH ),
        _glWindow->getCheckbox( ID_VERT_FORCE_SHEAR ),
        _glWindow->getCheckbox( ID_VERT_FORCE_BEND ),
        _glWindow->getCheckbox( ID_VERT_FORCE_GRAVITY ),
        _glWindow->getCheckbox( ID_VERT_FORCE_DRAG ),
    };
    const bool showdamp = _glWindow->getCheckbox( ID_VERT_FORCE_DAMP );
#endif
    if ( !showvel && !showf && !showfs[0] && !showfs[1] && !showfs[2] &&
         !showfs[3] && !showfs[4] ) {
        return;
    }

    const GeMesh& mesh = _simulator->getMesh();
    const SimSimulator::ForceType f[ NB_FS ] = {
        SimSimulator::F_STRETCH, SimSimulator::F_SHEAR, SimSimulator::F_BEND,
        SimSimulator::F_GRAVITY, SimSimulator::F_DRAG
    };
    const ColColourRGB c[ NB_FS ] = {
        ColColourRGB::RED,
        ColColourRGB::GREEN,
        ColColourRGB::BLUE,
        ColColourRGB( .8f, .6f, .2f ),
        ColColourRGB( .7f, .2f, .7f )
    };

    // Scale factors
    const Float SF = 4.f, SV = .2f;
    GeMesh::VertexConstIterator vi;
    GeMesh::VertexId vid = 0;

    ::glPushAttrib( GL_ENABLE_BIT );
    ::glDisable( GL_LIGHTING );
    ::glDisable( GL_COLOR_MATERIAL );
    ::glPushMatrix();
    GL::translate( _meshPos );
    ::glBegin( GL_LINES );
    for ( vi = mesh.beginVertex(); vi != mesh.endVertex(); ++vi, ++vid ) {
        if ( showvel ) {
            GL::colour( ColColourRGB::WHITE );
            GL::vertex( *vi );
            GL::vertex( *vi + SV * _simulator->getVelocity( vid ) );
        }
        if ( showf ) {
            GL::colour( ColColourRGB::YELLOW );
            GL::vertex( *vi );
            GL::vertex( *vi + SF * _simulator->getForce( vid ) );
        }
        for ( UInt32 i = 0; i < NB_FS; ++i ) {
            if (showfs[ i ] ) {
                GL::colour( c[ i ] );
                GL::vertex( *vi );
                GL::vertex( *vi + SF * _simulator->getForce( f[i], vid ) );
                if ( showdamp ) {
                    GL::colour( c[ i ] * .5f );
                    GL::vertex( *vi );
                    GL::vertex(
                        *vi + SF * _simulator->getDampingForce( f[i], vid )
                    );
                }
            }
        }
    }
    ::glEnd();
    ::glPopMatrix();
    ::glPopAttrib();
}

//------------------------------------------------------------------------------

void ClothApp::renderAxes()
{
    ::glPushAttrib( GL_ENABLE_BIT );
    ::glDisable( GL_LIGHTING );
    ::glBegin( GL_LINES );
        GL::colour( ColColourRGB::RED );
        ::glVertex3f( 0, 0, 0 );
        ::glVertex3f( .1f, 0, 0 );
        GL::colour( ColColourRGB::GREEN );
        ::glVertex3f( 0, 0, 0 );
        ::glVertex3f( 0, .1f, 0 );
        GL::colour( ColColourRGB::BLUE );
        ::glVertex3f( 0, 0, 0 );
        ::glVertex3f( 0, 0, .1f );
    ::glEnd();
    ::glPopAttrib();
}

//------------------------------------------------------------------------------

void ClothApp::renderFloor()
{
    // Number of times to repeat texture
    UInt32 N = 100;
    // Size of floor, in metres
    Float size = 100;
    if ( !_floorTexture.isNull() ) {
        ::glBindTexture( GL_TEXTURE_2D, _floorTexture->getTextureId() );
        ::glEnable( GL_TEXTURE_2D );
        ::glColor3f( 1, 1, 1 );
    }
    else {
        ::glColor3f( .8f, .7f, .7f );
    }
    ::glPushMatrix();
    ::glTranslatef( 0, 0, -5 );
    ::glScalef( size, size, 1 );
    ::glBegin( GL_QUADS );
        ::glNormal3f( 0.f, 0.f, 1.f );
        ::glVertex3f( -size/2, -size/2, 0 );
        ::glTexCoord2f( 0, 0 );
        ::glVertex3f( size/2, -size/2, 0 );
        ::glTexCoord2f( N, 0 );
        ::glVertex3f( size/2, size/2, 0 );
        ::glTexCoord2f( N, N );
        ::glVertex3f( -size/2, size/2, 0 );
        ::glTexCoord2f( 0, N );
    ::glEnd();
    ::glDisable( GL_TEXTURE_2D );
    ::glPopMatrix();
}

//------------------------------------------------------------------------------

void ClothApp::displayReceived( GfxWindow& )
{
    calcNormals();

    _glWindow->setText(
        ID_TIME,
        "Time: " + BaStringUtil::fromFloat(
            BaTime::instantAsSeconds( _simulator->getTime() ), 3
        )
    );
    _glWindow->setText(
        ID_EN_STRETCH,
        "Stretch: " + BaStringUtil::fromFloat(
            _simulator->getEnergy( SimSimulator::F_STRETCH ), 6
        )
    );
    _glWindow->setText(
        ID_EN_SHEAR,
        "Shear: " + BaStringUtil::fromFloat(
            _simulator->getEnergy( SimSimulator::F_SHEAR ), 6
        )
    );
    _glWindow->setText(
        ID_EN_BEND,
        "Bend: " + BaStringUtil::fromFloat(
            _simulator->getEnergy( SimSimulator::F_BEND ), 6
        )
    );

    if ( _snapFlag ) {
        _snapFlag = false;
        ++_snapCount;

        // Snap both with and without debug stuff
        snap( false );
        snap( true );
    }

    render(
        _glWindow->getCheckbox( ID_TRI_STRETCH ),
        _glWindow->getCheckbox( ID_TRI_SHEAR ),
        _glWindow->getCheckbox( ID_TRI_BEND )
    );

    _glWindow->swapBuffers();
}

//------------------------------------------------------------------------------

void ClothApp::snap( bool debug )
{
    render( debug, debug, debug );

    String filename(
        _snapPrefix + (debug ? "energy-" : "motion-" ) +
        BaStringUtil::fromUInt32( _snapCount, 3 ) + ".ppm"
    );
    GLint params[4];
    ::glGetIntegerv( GL_VIEWPORT, params );
    if ( _cropFlag ) {
        params[0] = _cropL;
        params[1] = _cropB;
        params[2] = std::min( static_cast<UInt32>( params[2] ), _cropW );
        params[3] = std::min( static_cast<UInt32>( params[3] ), _cropH );
    }
    GfxImage image( params[2], params[3], GfxImage::RGB24 );
    ::glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    ::glReadPixels(
        params[0], params[1], params[2], params[3],
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image.getRawData()
    );
    // Flip image vertically
    for ( UInt32 y = 0; y < static_cast<UInt32>( params[ 3 ] ) / 2; ++y ) {
        for ( UInt32 x = 0; x < static_cast<UInt32>( params[ 2 ] ); ++x ) {
            for ( UInt32 c = 0; c < 3; ++c ) {
                std::swap(
                    image.getData( x, y, c ),
                    image.getData( x, params[ 3 ] - y - 1, c )
                );
            }
        }
    }
    GfxImageWriterPNM writer( GfxImageWriterPNM::BINARY );
    writer.writeImage( filename, image );
}

//------------------------------------------------------------------------------

void ClothApp::saveStats()
{
    if ( _statsPrefix.length() == 0 ) {
        return;
    }
    std::ofstream stout(
        ( _statsPrefix + "stretch.txt" ).c_str(), std::ios::app
    );
    std::ofstream shout(
        ( _statsPrefix + "shear.txt" ).c_str(), std::ios::app
    );
    std::ofstream bout(
        ( _statsPrefix + "bend.txt" ).c_str(), std::ios::app
    );
    std::ofstream stmout(
        ( _statsPrefix + "stretch-mean.txt" ).c_str(), std::ios::app
    );
    std::ofstream shmout(
        ( _statsPrefix + "shear-mean.txt" ).c_str(), std::ios::app
    );
    std::ofstream bmout(
        ( _statsPrefix + "bend-mean.txt" ).c_str(), std::ios::app
    );
    // Output totals & averages first.
    stout << _simulator->getEnergy( SimSimulator::F_STRETCH ) << " ";
    shout << _simulator->getEnergy( SimSimulator::F_SHEAR ) << " ";
    bout << _simulator->getEnergy( SimSimulator::F_BEND ) << " ";
    stmout << _simulator->getEnergy( SimSimulator::F_STRETCH )
        / _simulator->getMesh().getNbFaces() << " ";
    shmout << _simulator->getEnergy( SimSimulator::F_SHEAR )
        / _simulator->getMesh().getNbFaces() << " ";
    bmout << _simulator->getEnergy( SimSimulator::F_BEND )
        / _simulator->getMesh().getNbFaces() << " ";

    std::ofstream sttout(
        ( _statsPrefix + "stretch-tris.txt" ).c_str(), std::ios::app
    );
    std::ofstream shtout(
        ( _statsPrefix + "shear-tris.txt" ).c_str(), std::ios::app
    );
    std::ofstream btout(
        ( _statsPrefix + "bend-tris.txt" ).c_str(), std::ios::app
    );
    for ( UInt32 i = 0; i < _simulator->getMesh().getNbFaces(); ++i ) {
        sttout << _simulator->getTriEnergy( SimSimulator::F_STRETCH, i ) << " ";
        shtout << _simulator->getTriEnergy( SimSimulator::F_SHEAR, i ) << " ";
        btout << _simulator->getTriEnergy( SimSimulator::F_BEND, i ) << " ";
    }
    sttout << std::endl;
    shtout << std::endl;
    btout << std::endl;
}

//------------------------------------------------------------------------------

void ClothApp::render( bool debug_stretch, bool debug_shear, bool debug_bend )
{
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    ::glLoadIdentity();

    ::glTranslatef( 0, 0, _glWindow->getTranslateZ( ID_ZOOM ) / 10 );
    GL::multMatrix( _glWindow->getRotate( ID_CAMERA_ROTATE ) );

    ::glPushMatrix();
        GL::multMatrix( _glWindow->getRotate( ID_LIGHT ) );
        GL::lightPosition( GL_LIGHT0, GeVector::zAxis() );
        GL::light( GL_LIGHT0, GL_DIFFUSE, ColColourRGB::WHITE );
        GL::light( GL_LIGHT0, GL_SPECULAR, ColColourRGB::WHITE );
    ::glPopMatrix();

    if ( _glWindow->getCheckbox( ID_AXES ) ) {
        renderAxes();
    }
    //::glFogf( GL_FOG_DENSITY, _glWindow->getTranslateZ( ID_FOG_START ) / 10 );
    ::glFogf( GL_FOG_START, _glWindow->getTranslateZ( ID_FOG_START ) / 10 );
    ::glFogf( GL_FOG_END, _glWindow->getTranslateZ( ID_FOG_END ) / 10 );

    ::glEnable( GL_FOG );
    //renderFloor();
    ::glDisable( GL_FOG );

    ::glEnable( GL_POLYGON_OFFSET_FILL );
    ::glPolygonOffset( 1, 1 );
    if ( debug_stretch || debug_shear || debug_bend ) {
        renderClothTriDebug( debug_stretch, debug_shear, debug_bend );
        ::glDisable( GL_POLYGON_OFFSET_FILL );
    }
    else {
        //::glColor3f( .6f, .1f, .3f );
        ::glColor3f( 1, 1, 1 );
        ::glEnable( GL_FOG );
        renderCloth();
        ::glDisable( GL_FOG );
        ::glDisable( GL_POLYGON_OFFSET_FILL );
        renderClothOutline();
    }

    if ( _glWindow->getCheckbox( ID_WIREFRAME ) ) {
        ::glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        ::glPushAttrib( GL_ENABLE_BIT );
        ::glDisable( GL_LIGHTING );
        ::glColor3f( .1f, .1f, .4f );
        renderCloth();
        ::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        ::glPopAttrib();
    }

    renderClothVertDebug();
}

//------------------------------------------------------------------------------

void ClothApp::keyPressed(
    GfxWindow&,
    GfxWindow::KeyID keyId,
    GfxWindow::ModBitfield modifiers
) {
    switch( keyId ) {
        case GfxWindow::KB_ESCAPE: {
            _quitFlag = true;
        } break;

        case GfxWindow::KB_SPACE: {
            ++_nbPendingSteps;
        } break;

        default: {
        } break;
    }
}

//------------------------------------------------------------------------------

void ClothApp::windowResized(
    GfxWindow&, UInt32 x, UInt32 y, UInt32 width, UInt32 height
) {
    ::glMatrixMode( GL_PROJECTION );
    ::glLoadIdentity();
    ::glViewport( x, y, width, height );
    ::gluPerspective( 45.0f, static_cast<Float>( width )/height, 0.1f, 1000.f );
    ::gluLookAt( 0, 0, 1, 0, 0, 0, 0, 1, 0 );

    ::glMatrixMode( GL_MODELVIEW );
    ::glLoadIdentity();
}


////////////////////////////////////////////////////////////////////////////////
// GLOBALS

//------------------------------------------------------------------------------

int main( int argc, const char** argv )
{
    ClothAppArgs args( argc, argv );
    if ( args._error ) {
        return 1;
    }
    ClothApp app( args );
    app.runApp();
    return 0;
}
