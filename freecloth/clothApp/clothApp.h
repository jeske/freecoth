//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_clothApp_clothApp_h
#define freecloth_clothApp_clothApp_h

#ifndef freecloth_clothApp_package_h
#include <freecloth/clothApp/package.h>
#endif

#ifndef freecloth_simulator_simSimulator_h
#include <freecloth/simulator/simSimulator.h>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

#ifndef freecloth_geom_gePoint_h
#include <freecloth/geom/gePoint.h>
#endif

#ifndef freecloth_geom_geVector_h
#include <freecloth/geom/geVector.h>
#endif

#ifndef freecloth_geom_geMeshWingedEdge_h
#include <freecloth/geom/geMeshWingedEdge.h>
#endif

#ifndef freecloth_gfx_gfxGLWindowGLUI_h
#include <freecloth/gfx/gfxGLWindowGLUI.h>
#endif

#ifndef freecloth_gfx_gfxWindowObserver_h
#include <freecloth/gfx/gfxWindowObserver.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxGLWindowGLUI;
class GfxGLTexture;
class GfxConfig;
class ClothAppArgs;

FREECLOTH_NAMESPACE_START
    class GeMesh;
    class SimStepStrategy;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ClothApp freecloth/clothApp/clothApp.h
 * \brief Demonstration application for Freecloth simulator
 *
 * This demonstration application shows off the capabilities of the Freecloth
 * simulator. A lot of auxiliary classes are needed to pull this off, mostly in
 * the gfx/ subdirectory.
 */
class ClothApp : public GfxWindowObserver
{
public:
    // ----- types and enumerations -----

    //! Constraint IDs
    enum ConstraintType {
        CON_NONE,

        CON_CENTRE,
        CON_CORNERS4,
        CON_CORNERS3a,
        CON_CORNERS3b,
        CON_CORNERS1c,
        CON_CORNERS1d,
        CON_CORNERYANK,
        CON_TABLE_SQUARE,
        CON_TABLE_CIRCLE,

        NB_CONSTRAINTS
    };
    
    // ----- member functions -----
    explicit ClothApp( const ClothAppArgs& );
    void runApp();

protected:
    // ----- member functions -----
    void keyPressed( GfxWindow&, GfxWindow::KeyID, GfxWindow::ModBitfield );
    void windowResized( GfxWindow&, UInt32 x, UInt32 y, UInt32 w, UInt32 h );
    void displayReceived( GfxWindow& );
    void closeReceived( GfxWindow& );
    void uiReceived( GfxWindow&, UInt32 uid );
    void idleReceived();

    void loadSettings( const String& filename );
    void saveSettings( const String& filename ) const;
    void render( bool debug_stretch, bool debug_shear, bool debug_bend );
    void snap( bool debug );
    void saveStats();

private:
    // ----- types and enumerations -----

    //! Control IDs
    enum {
        ID_CAMERA_ROTATE,
        ID_ZOOM,
        ID_LIGHT,
        ID_FOG_START,
        ID_FOG_END,
        ID_AXES,
        ID_WIREFRAME,
        ID_CLOTH_SIZE,

        ID_RUN,
        ID_STOP,
        ID_STEP,
        ID_REWIND,
        ID_TIME,
        ID_PATCHES,
        ID_PCG_TOLERANCE,

        ID_STEP_STRATEGY,
        ID_STEP_TIMESTEP,
        ID_STEP_FRAME_RATE,
        ID_STEP_MAX,
        ID_STEP_STRETCH_LIMIT,

        ID_PAR_K_STRETCH,
        ID_PAR_K_SHEAR,
        ID_PAR_K_BEND_U,
        ID_PAR_K_BEND_V,
        ID_PAR_K_STRETCH_DAMP,
        ID_PAR_K_SHEAR_DAMP,
        ID_PAR_K_BEND_DAMP,
        ID_PAR_K_DRAG,
        ID_PAR_B_U,
        ID_PAR_B_V,
        ID_PAR_RHO,
        ID_PAR_G,
        ID_PAR_RESET,

        ID_CONSTRAINTS,

        ID_EN_STRETCH,
        ID_EN_SHEAR,
        ID_EN_BEND,

        ID_TRI_STRETCH,
        ID_TRI_SHEAR,
        ID_TRI_BEND,

        ID_VERT_VEL,
        ID_VERT_FORCE,
        ID_VERT_FORCE_DAMP,
        ID_VERT_FORCE_STRETCH,
        ID_VERT_FORCE_SHEAR,
        ID_VERT_FORCE_BEND,
        ID_VERT_FORCE_GRAVITY,
        ID_VERT_FORCE_DRAG,

        ID_SNAPSHOT,
        ID_MOVIE,
        ID_SETTINGS_FILENAME,
        ID_SETTINGS_LOAD,
        ID_SETTINGS_SAVE,

        ID_QUIT
    };

    enum StepStrategy {
        STEP_BASIC,
        STEP_ADAPTIVE
    };

    // ----- static member functions -----
    static RCShdPtr<GeMesh> createRectMesh(
        Float size,
        UInt32 nbRows,
        UInt32 nbColumns
    );
    static void initMeshIndices(
        const GeMesh& mesh,
        std::vector<UInt32>& indices
    );

    // ----- member functions -----
    void setupMesh();
    void setupSimulator();
    void setupStepper();
    void setConstraints();
    void setupWindow( const GfxConfig& config );
    void initGL();
    void calcNormals();
    void renderCloth();
    void renderClothTriDebug( bool showStretch, bool showShear, bool showBend );
    void renderClothVertDebug();
    void renderClothOutline();
    void renderAxes();
    void renderFloor();
    void updateParamsUI();

    //! Force the completion of the current step.
    void forceFinishStep();

    // ----- data members -----
    RCShdPtr<GeMesh>            _initialMesh;
    RCShdPtr<GfxGLWindowGLUI>   _glWindow;
    RCShdPtr<GfxGLTexture>      _floorTexture, _clothTexture;
    RCShdPtr<SimSimulator>      _simulator;
    RCShdPtr<SimStepStrategy>   _stepper;

    //! Preprocessed list of indices into mesh vertex array.
    std::vector<UInt32>     _meshIndices;
    std::vector<GeVector>   _meshNormals;
    std::vector<GePoint>    _meshTextureVertices;
    RCShdPtr<GeMeshWingedEdge> _meshWE;
    GePoint                 _meshPos;

    //! Prefix for output snapshots
    String                  _snapPrefix;
    String                  _statsPrefix;
    SimSimulator::Params    _params;
    UInt32                  _nbPatches;
    Float                   _clothSize;
    Float                   _h;
    Float                   _rho;
    Float                   _pcgTolerance;
    UInt32                  _frameRate;
    Float                   _maxTimestep;
    Float                   _stretchLimit;
    ConstraintType          _constraints;
    bool                    _batchFlag;
    BaTime::Instant         _batchEnd;
    //! Times of substeps, for debugging
    std::list<Float>        _subTimes;
    UInt32                  _nbSubSteps;
    BaTime::Instant         _subLastTime;

    bool _quitFlag;
    bool _rewindFlag;
    bool _freeRunFlag;
    bool _stopFlag;
    UInt32 _nbPendingSteps;
    bool _snapFlag;
    UInt32 _snapCount;
    UInt32 _nextMovieFrame;

    bool _cropFlag;
    UInt32 _cropL, _cropB, _cropW, _cropH;

    bool _subStepFlag;
};

#endif
