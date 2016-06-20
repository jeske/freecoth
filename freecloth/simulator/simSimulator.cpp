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

#include <freecloth/simulator/simSimulator.h>
#include <freecloth/geom/geVector.h>
#include <freecloth/geom/geMatrix3.h>
#include <freecloth/base/fstream>
#include <freecloth/base/iomanip>
#include <freecloth/base/baMath.h>
#include <freecloth/base/baStringUtil.h>

#ifdef NDEBUG
    #define DO_DEBUG(x)
#else
    #define DO_DEBUG(x) x
#endif

// If set, avoid [BarWit98]'s assumption that stretch always keeps the
// edge lengths to a fixed size. Not very sure that it works...
#define DO_NORM             0
// If set, adjust [BarWit98]'s condition functions to be scale invariant
#define DO_SCALE_INVARIANT  1
// If set, adjust [BarWit98]'s modified PCG algorithm to use [AscBox03]'s
// improvements.
#define DO_ASCHER_BOXERMAN 1

namespace {
    using namespace freecloth;

    typedef Float Float_3[3];
    typedef Float Float_33[3][3];
    typedef Float Float_4[4];
    typedef GePoint GePoint_3[3];
    typedef GePoint GePoint_4[4];
    typedef GeVector GeVector_3[3];
    typedef GeVector GeVector_33[3][3];
    typedef GeVector GeVector_3333[3][3][3][3];
    typedef GeVector GeVector_4[4];
    typedef GeVector GeVector_43[4][3];
    typedef GeVector GeVector_443[4][4][3];
    typedef GeVector GeVector_4433[4][4][3][3];
    typedef GeMatrix3 GeMatrix3_33[3][3];
    typedef GeMatrix3 GeMatrix3_44[4][4];

    const bool PRINT_STATS = false;

    const bool DEBUG_REWIND = false;
    const bool DEBUG_MASS = false;
    const bool DEBUG_STEP = false;
    const bool DEBUG_COMMON = false;
    const bool VERIFY_COMMON = false;
    const bool DEBUG_STRETCH = false;
    const bool VERIFY_STRETCH = false;
    const bool DEBUG_SHEAR = false;
    const bool VERIFY_SHEAR = false;
    const bool DEBUG_BEND = false;
    const bool DEBUG_PCG = false;
    // Verification - for each derivative da/db calculated, verify the
    // calculation by performing (a(b0+db)-a(b0))/db calculation, and asserting
    // that this matches the actual derivative calculation.
    const bool VERIFY_BEND = false;

    const bool DUMP_DISTS = false;
    const bool DUMP_FORCES = false;




//------------------------------------------------------------------------------

    inline GeVector makeSkewRow( const GeVector& v, UInt32 row )
    {
        DGFX_ASSERT( row < 3 );

        switch( row ) {
            case 0: 
                return GeVector( 0, -v._z, v._y );
            case 1:
                return GeVector( v._z, 0, -v._x );
            case 2:
                return GeVector( -v._y, v._x, 0 );
        }
        return GeVector();
    }


//------------------------------------------------------------------------------

    template <class E1>
    bool isSymmetric( const E1& A )
    {
        DGFX_ASSERT( A.nbRows() == A.nbColumns() );
        for ( UInt32 i = 0; i < A.nbRows(); ++i ) {
            for ( UInt32 j = i + 1; j < A.nbRows(); ++j ) {
                if ( A( i, j ) != A( j, i ).transpose() ) {
                    return false;
                }
            }
        }
        return true;
    }
}

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::CommonVars

/*!
 * \class SimSimulator::CommonVars
 * \brief Variables common to stretch and shear force computation.
 */
class SimSimulator::CommonVars
{
public:
    // ----- member functions -----

    void calc( const FaceConsts& fc, const GePoint p[3] );
    void debugOutput( std::ostream& os );

    // ----- member variables -----

    GeVector _wu,_wv;
    //! Normalised versions of wu and wv ($\frac{\wu}{\norm{\wu}}$)
    GeVector _whatu,_whatv;
    //! Inverse magnitude of wu and wv ($\frac{1}{\norm{\wu}}$)
    Float _wuim,_wvim;

    Float_3 _dwux_dxmx, _dwvx_dxmx;
#if DO_NORM
    GeVector_33 _dwhatu_dxm, _dwhatv_dxm;
    GeVector_3333 _d2whatu_dxmdxn, _d2whatv_dxmdxn;
#endif
};

////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::StretchVars

class SimSimulator::StretchVars {
public:
    // ----- member functions -----

    void calc(
        const FaceConsts& fc,
        const CommonVars& cv,
        const GeVector v0[ 3 ],
        Float b_u, Float b_v
    );
    void debugOutput( std::ostream& os );

    // ----- member variables -----
    Float _Cu, _Cv;
    GeVector_3 _dCu_dxm, _dCv_dxm;
    Float _dCu_dt, _dCv_dt;
    GeMatrix3_33 _d2Cu_dxmdxn, _d2Cv_dxmdxn;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::ShearVars

/*!
 * \brief Variables used in shear force computation.
 */
class SimSimulator::ShearVars {
public:
    // ----- member functions -----

    void calc(
        const FaceConsts& fc,
        const CommonVars& cv,
        const GeVector v0[ 3 ]
    );
    void debugOutput( std::ostream& os );

    // ----- member variables -----
    Float _C;
    GeVector_3 _dC_dxm;
    Float _dC_dt;
    //! In reality, d2C_dxmdxn is this times an identity matrix.
    Float_33 _d2C_dxmdxn;
};


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::BendVars

/*!
 * \brief Variables used in bend force computation.
 */
class SimSimulator::BendVars
{
public:
    // ----- member functions -----

    //! Calculate condition function, etc. Inputs are vertex positions and
    //! velocities.
    //! Optimisation: _nhatA, _nhatB, _nAim, _nBim should be set before
    //! calling this function. These can be calculated on a per-face basis,
    //! making them more efficient to precalculate and pass in than to
    //! calculate in place.
    void calc(
        const GePoint_4 x,
        const GeVector_4 v
    );
    //! This variant is for use when verifying, especially without DO_NORM.
    void calc(
        const GePoint_4 x,
        const GeVector_4 v,
        const BendVars& bv
    );
    void debugOutput( std::ostream& os );

    // ----- member variables -----
    //! Unit vectors
    GeVector _nhatA, _nhatB, _ehat;
    //! Inverse magnitude of nA, nB, e
    Float _nAim, _nBim, _eim;
    Float _costh, _sinth;
    Float _C;

    GeVector_4 _qA, _qB;
    Float_4 _qe;

    GeVector_43 _dnhatA_dxm, _dnhatB_dxm, _dehat_dxm;
    GeVector_4433 _d2nA_dxmdxn, _d2nB_dxmdxn;
#if DO_NORM
    GeVector_4433 _d2e_dxmdxn;
#endif

    GeVector_4 _dcosth_dxm, _dsinth_dxm;
    GeMatrix3_44 _d2costh_dxmdxn, _d2sinth_dxmdxn;

    GeVector_4 _dC_dxm;
    Float _dC_dt;
    GeMatrix3_44 _d2C_dxmdxn;

private:

    // ----- member functions -----
    void mainCalc( const GePoint_4 x, const GeVector_4 v );
    void calcN( const GePoint_4 x );
    void calcE( const GePoint_4 x );
    void calcThetas();
    void calcQs( const GePoint_4 x );
    void calcNEderiv();
    void calcD2NE();
    void calcCosSinDeriv();
    void calcD2cossinth();
    void calcCderivs();
};


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator

//------------------------------------------------------------------------------

SimSimulator::SimSimulator( const GeMesh& initialMesh )
  : _initialMesh( new GeMesh( initialMesh ) ),
    _rho( .01f ),
    _h( .02f ),
    _stretchLimit( .03f )
{
    rewind();
    setupMass();
    removeAllConstraints();
}

//------------------------------------------------------------------------------

void SimSimulator::rewind()
{
    _initialMeshWingedEdge = RCShdPtr<GeMeshWingedEdge>(
        new GeMeshWingedEdge( _initialMesh )
    );
    if ( DEBUG_REWIND ) {
        std::cout << "winged edges = " << _initialMeshWingedEdge << std::endl;
    }
    _mesh = RCShdPtr<GeMesh>( new GeMesh( *_initialMesh ) );

    const UInt32 N = _mesh->getNbVertices();
    _sd._v0 = SimVector( N );
    _sd._f0 = SimVector( N );
    _sd._lastDeltaV0 = SimVector::zero( N );
    _df_dx = SimMatrix( N, N );
    _df_dv = SimMatrix( N, N );
    _modPCG._b = SimVector( N );
    _modPCG._A = SimMatrix( N, N );
    UInt32 i;
    ForceType ftypes[] = { F_STRETCH, F_SHEAR, F_BEND };
    for( i = 0; i < 3; ++i ) {
        UInt32 f( ftypes[ i ] );
        _sd._f0i[ f ] = SimVector( N );
        _sd._d0i[ f ] = SimVector( N );

        _sd._fenergy[ i ] = 0;
    }
    _sd._f0i[ F_GRAVITY ] = SimVector( N );
    _sd._f0i[ F_DRAG ] = SimVector( N );

    for( i = 0; i < NB_FORCES; ++i ) {
        _sd._trienergy[ i ].resize( _initialMesh->getNbFaces() );
    }
    _sd._Cu.resize( _initialMesh->getNbFaces() );
    _sd._Cv.resize( _initialMesh->getNbFaces() );
    _faceUnitNormals.resize( _initialMesh->getNbFaces() );
    _faceNormalIMs.resize( _initialMesh->getNbFaces() );
    _sd._venergy = 0;
    _sd._energy = 0;
    _sd._time = 0.f;

    _savedVertices.resize( _initialMesh->getNbVertices() );
    _savedStepData = _sd;

    calcFaceConsts();

    // Force postSubStepsFinale() to be done at start of preSubSteps();
    _doFinaleInPre = true;

    _inStep = false;
    _stepSuccessFlag = true;
}

//------------------------------------------------------------------------------

void SimSimulator::setupMass()
{
    const UInt32 N = _mesh->getNbVertices();
    _M = SimMatrix( N, N );
    _totalMass = 0;

    GeMesh::FaceConstIterator fi;
    GeMesh::VertexId vid;
    for( fi = _initialMesh->beginFace(); fi != _initialMesh->endFace(); ++fi ) {
        Float m = _rho * fi->calcArea() / 3;
        for ( UInt32 i = 0; i < 3; ++i ) {
            vid = fi->getVertexId( i );
            // NOTE: taking a reference will *not* work with uBLAS
            GeMatrix3 M( _M( vid, vid ) );
            M( 0, 0 ) += m;
            M( 1, 1 ) += m;
            M( 2, 2 ) += m;
            _M( vid, vid ) = M;
            _totalMass += m;
        }
    }

    if ( DEBUG_MASS ) {
        std::cout << "_M = " << _M << std::endl;
        std::cout << "totalMass = " << _totalMass << std::endl;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::removeAllConstraints()
{
    const UInt32 N = _mesh->getNbVertices();
    GeMesh::VertexId vid;
    _modPCG._S.resize( N );
    for( vid = 0; vid < N; ++vid ) {
        _modPCG._S[ vid ] = GeMatrix3::identity();
    }
    _z0 = SimVector( N );
    _z0.clear();

    // This is isn't strictly necessary, but generally gets nicer results
    // when the user interactively changes constraints.
    _sd._v0.clear();
}

//------------------------------------------------------------------------------

void SimSimulator::setPosConstraintPlane(
    GeMesh::VertexId vid,
    const GeVector& p
) {
    DGFX_ASSERT( ! inStep() );

    GeVector pu( p.getUnit() );
    DGFX_ASSERT( vid < _modPCG._S.size() );
    _modPCG._S[ vid ] =
        GeMatrix3::identity() - GeMatrix3::outerProduct( pu, pu );
}

//------------------------------------------------------------------------------

void SimSimulator::setPosConstraintLine(
    GeMesh::VertexId vid,
    const GeVector& v
) {
    DGFX_ASSERT( ! inStep() );
    GeVector vu( v.getUnit() );

    // Construct a vector perpendicular to l.
    GeVector p;
    if ( !BaMath::isEqual( vu._z, 0 ) ) {
        p = GeVector( 1, 1, ( vu._x + vu._y ) / vu._z );
    }
    else if ( !BaMath::isEqual( vu._y, 0 ) ) {
        p = GeVector( 1, ( vu._x + vu._z ) / vu._y, 1 );
    }
    else {
        p = GeVector( ( vu._y + vu._z ) / vu._x, 1, 1 );
    }
    GeVector pu( p.getUnit() );
    GeVector qu( vu.cross( pu ) );

    DGFX_ASSERT( BaMath::isEqual( qu.length(), 1 ) );
    DGFX_ASSERT( BaMath::isEqual( pu.dot( vu ), 0 ) );
    DGFX_ASSERT( BaMath::isEqual( qu.dot( vu ), 0 ) );
    DGFX_ASSERT( BaMath::isEqual( pu.dot( qu ), 0 ) );

    DGFX_ASSERT( vid < _modPCG._S.size() );
    _modPCG._S[ vid ] =
        GeMatrix3::identity() - GeMatrix3::outerProduct( pu, pu ) -
        GeMatrix3::outerProduct( qu, qu );
}

//------------------------------------------------------------------------------

void SimSimulator::setPosConstraintFull( GeMesh::VertexId vid )
{
    DGFX_ASSERT( ! inStep() );
    DGFX_ASSERT( vid < _modPCG._S.size() );
    _modPCG._S[ vid ] = GeMatrix3::zero();
}

//------------------------------------------------------------------------------

void SimSimulator::setVelConstraint( GeMesh::VertexId vid, const GeVector& v )
{
    DGFX_ASSERT( ! inStep() );
    DGFX_ASSERT( vid < _z0.size() );
    _z0[ vid ] = v;
}

//------------------------------------------------------------------------------

void SimSimulator::step()
{
    DGFX_ASSERT( ! inStep() );
    preSubSteps();
    while ( ! subStepsDone() ) {
        subStep();
    }
    postSubSteps();
}

//------------------------------------------------------------------------------

void SimSimulator::postSubStepsFinale()
{
    UInt32 i;

    for( i = 0; i < NB_FORCES; ++i ) {
        _sd._fenergy[ i ] = 0;
    }

     _sd._venergy = .5 * _sd._v0.dot( _M * _sd._v0 );

    // Clear all forces
    _sd._f0.clear();

    // *= 0 is better than clear() here - it retains the sparsity pattern
    // within the matrix, to avoid unnecessary reallocations after the first
    // call to step().
    _df_dx *= 0;
    _df_dv *= 0;
    for( i = 0; i < NB_FORCES; ++i ) {
        _sd._f0i[ i ].clear();
        _sd._d0i[ i ].clear();
    }

    _stepSuccessFlag = true;

    GeMesh::FaceConstIterator fi;
    for( fi = _mesh->beginFace(); fi != _mesh->endFace(); ++fi ) {
        calcStretchShear( *fi );
    }
}

//------------------------------------------------------------------------------

void SimSimulator::preSubSteps()
{
    DGFX_ASSERT( ! inStep() );
    if ( PRINT_STATS ) {
        std::cout << "****************************************" << std::endl;
        std::cout << "Simulating at time " << _sd._time
            << " (h=" << _h << ")" << std::endl;
    }

    // FIXME: need a "restarting" flag that recalculates the stretch energies -
    // this is for cases where mesh has changed substantially e.g., refinement,
    // change of stretch constant, etc. etc.

    if ( _doFinaleInPre ) {
        postSubStepsFinale();
        _doFinaleInPre = false;
        // We ignore the success flag here in this case
    }

    _inStep = true;

    GeMesh::FaceConstIterator fi;
    for( fi = _mesh->beginFace(); fi != _mesh->endFace(); ++fi ) {
        // Calculate face normal information in preparation for bend
        // calculation.
        GeVector normal( fi->calcNonUnitNormal() );
        _faceNormalIMs[ fi->getFaceId() ] = 1.f / normal.length();
        _faceUnitNormals[ fi->getFaceId() ] =
            normal * -_faceNormalIMs[ fi->getFaceId() ];
    }
    GeMeshWingedEdge::EdgeIterator ei;

    for(
        ei = _initialMeshWingedEdge->beginEdge();
        ei != _initialMeshWingedEdge->endEdge();
        ++ei
    ) {
        // Only do edges that aren't on the boundary.
        if ( ei->hasTwin() ) {
            calcBend( *ei );
            if ( VERIFY_BEND ) {
                verifyBend( *ei );
            }
        }
    }

    for( UInt32 i = 0; i < _mesh->getNbVertices(); ++i ) {
        const GeMatrix3& M = _M( i, i );
        Float gravity = -M( 2, 2 ) * _params._g;
        _sd._f0[ i ]._z += gravity;
        DO_DEBUG( _sd._f0i[ F_GRAVITY ][ i ]._z = gravity );
    }

    for ( fi = _mesh->beginFace(); fi != _mesh->endFace(); ++fi ) {
        Float area( fi->calcArea() );
        const GeVector normal( fi->calcNormal() );
        GeMesh::FaceVertexId fvid;
        for ( fvid = 0; fvid < fi->getNbVertices(); ++fvid ) {
            UInt32 vid( fi->getVertexId( fvid ) );
            Float vel( _sd._v0[ vid ].dot( normal ) );
            const GeVector force( ( -_params._k_drag * vel * area ) * normal );
            _sd._f0[ vid ] += force;
            DO_DEBUG( _sd._f0i[ F_DRAG ][ vid ] += force );
            // FIXME: include df_dv component
        }
    }

    _sd._energy = _sd._venergy + _sd._fenergy[ F_STRETCH ] +
        _sd._fenergy[ F_SHEAR ] + _sd._fenergy[ F_BEND ];
    if ( DEBUG_STEP ) {
        for ( UInt32 i = 0; i < NB_FORCES; ++i ) {
            const char* s[] = { "stretch","shear","bend","gravity","drag" };
            std::cout << "f0[" << s[i] << "] = " << _sd._f0i[ i ] << std::endl;
            std::cout << std::endl;
            std::cout << "d0[" << s[i] << "] = " << _sd._d0i[ i ] << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
        std::cout << "f0 = " << _sd._f0 << std::endl;
        std::cout << "df_dx = " << _df_dx << std::endl;
        std::cout << "df_dv = " << _df_dv << std::endl;
    }

    // Paper eq. (16)
    // FIXME: do it more efficiently, without memory reallocations per-step
    _modPCG._A = _df_dx * (-_h * _h) + _df_dv * -_h + _M;
    _modPCG._b = _h * ( _h * _df_dx * _sd._v0 + _sd._f0 );

    if ( DEBUG_STEP ) {
        std::cout << "A = " << _modPCG._A << std::endl;
        std::cout << "b = " << _modPCG._b << std::endl;
    }

    // Now, we're left with a system Ax=b, where x corresponds to deltav
    
    _modPCG._z = _h * _z0;
    _modPCG._y = _sd._lastDeltaV0;
    _modPCG.preStep();
}

//------------------------------------------------------------------------------

bool SimSimulator::subStepsDone() const
{
    return _modPCG.done();
}

//------------------------------------------------------------------------------

void SimSimulator::subStep()
{
    DGFX_ASSERT( inStep() );
    _modPCG.step();
}

//------------------------------------------------------------------------------

void SimSimulator::cancelStep()
{
    DGFX_ASSERT( inStep() );
    if ( PRINT_STATS ) {
        std::cout << "Step cancelled" << std::endl;
    }

    _inStep = false;
    // Force postSubStepsFinale to be done in the preSubSteps() stage
    // next time.
    _doFinaleInPre = true;
}

//------------------------------------------------------------------------------

void SimSimulator::postSubSteps()
{
    DGFX_ASSERT( inStep() && subStepsDone() );
    _modPCG.step();
    if ( DEBUG_STEP ) {
        std::cout << "PCG Iterations = " << _modPCG.getNbSteps() << std::endl;
        std::cout << "deltav = " << _modPCG.result() << std::endl;
        std::cout << "Ax = " << _modPCG._A * _modPCG.result() << std::endl;
    }

    // Save persistent data that will be destroyed by performing timestep.
    _savedStepData = _sd;
    std::copy(
        _mesh->beginVertex(), _mesh->endVertex(), _savedVertices.begin()
    );

    // Update data: this is the actual effect of the timestep.
    _sd._lastDeltaV0 = _modPCG.result();
    _sd._v0 += _modPCG.result();
    GeMesh::VertexIterator vi;
    UInt32 i = 0;
    for ( vi = _mesh->beginVertex(), i; vi != _mesh->endVertex(); ++vi, ++i ) {
        (*vi) += _h * _sd._v0[ i ];
    }
    _sd._time += _h;

    // Calculate next step's stretch/shear.
    postSubStepsFinale();

    _inStep = false;

    if ( ! stepSucceeded() ) {
        if ( PRINT_STATS ) {
            std::cout << "Step failed" << std::endl;
        }
        // Revert to old data
        _sd = _savedStepData;
        std::copy(
            _savedVertices.begin(), _savedVertices.end(), _mesh->beginVertex ()
        );

        // Force postSubStepsFinale to be done in the preSubSteps() stage
        // next time.
        _doFinaleInPre = true;
        return;
    }

    if ( PRINT_STATS ) {
        std::cout.precision( 4 );
        std::cout.width( 6 );
        std::cout.setf( std::ios::fixed );
        std::cout << "Total energy: "
            << std::setprecision( 5 ) << std::setw( 7 )
            << std::setiosflags( std::ios::fixed )
            << _sd._energy
            << "( stretch: " << _sd._fenergy[ F_STRETCH ]
            << " / shear: " << _sd._fenergy[ F_SHEAR ]
            << " / bend: " << _sd._fenergy[ F_BEND ]
            //<< " / kinetic: " << _venergy
            << ")" << std::resetiosflags( std::ios::fixed ) << std::endl;
        std::cout << std::endl;
    }

    if ( DUMP_DISTS ) {
        static std::ofstream* fptr;
        if ( fptr == 0 ) {
            Float distance( (
                _initialMesh->getVertex( 1 ) - _initialMesh->getVertex( 0 )
            ).length() * 3 );
            String filename(
                "dists-" + BaStringUtil::fromFloat( distance, 1 ) + ".txt"
            );
            fptr = new std::ofstream( filename.c_str() );
        }
        std::ofstream& out = *fptr;
        GeMesh::VertexConstIterator vbi = _initialMesh->beginVertex();
        GeMesh::VertexConstIterator vi = _mesh->beginVertex();
        for ( ; vi != _mesh->endVertex(); ++vi, ++vbi ) {
            out << ( *vi - *vbi ).length() << " ";
        }
        out << std::endl;
    }

    if ( DUMP_FORCES ) {
        static std::ofstream* fptr;
        if ( fptr == 0 ) {
            Float distance( (
                _initialMesh->getVertex( 1 ) - _initialMesh->getVertex( 0 )
            ).length() * 3 );
            String filename(
                "forces-" + BaStringUtil::fromFloat( distance, 2 ) + ".txt"
            );
            fptr = new std::ofstream( filename.c_str() );
        }
        std::ofstream& out = *fptr;
        for ( UInt32 i = 0; i < _sd._f0i[ F_STRETCH ].size(); ++i ) {
            out << _sd._f0i[ F_STRETCH ][ i ].length() << " ";
            out << _sd._f0i[ F_SHEAR ][ i ].length() << " ";
            out << _sd._f0i[ F_BEND ][ i ].length() << " ";
            out << _sd._d0i[ F_STRETCH ][ i ].length() << " ";
            out << _sd._d0i[ F_SHEAR ][ i ].length() << " ";
            out << _sd._d0i[ F_BEND ][ i ].length() << " ";
        }
        out << std::endl;
    }
}

//------------------------------------------------------------------------------

bool SimSimulator::inStep() const
{
    return _inStep;
}

//------------------------------------------------------------------------------

bool SimSimulator::stepSucceeded() const
{
    return _stepSuccessFlag;
}

//------------------------------------------------------------------------------

const GeMesh& SimSimulator::getMesh() const
{
    return *_mesh;
}

//------------------------------------------------------------------------------

const RCShdPtr<GeMesh>& SimSimulator::getMeshPtr() const
{
    return _mesh;
}

//------------------------------------------------------------------------------

BaTime::Instant SimSimulator::getTime() const
{
    return BaTime::floatAsInstant( _sd._time );
}

//------------------------------------------------------------------------------

void SimSimulator::setTimestep( BaTime::Duration h )
{
    DGFX_ASSERT( ! inStep() );
    _h = BaTime::durationAsSeconds( h );
}

//------------------------------------------------------------------------------

void SimSimulator::setDensity( Float rho )
{
    DGFX_ASSERT( ! inStep() );
    _rho = rho;
    setupMass();
}

//------------------------------------------------------------------------------

void SimSimulator::setPCGTolerance( Float tolerance )
{
    DGFX_ASSERT( ! inStep() );
    _modPCG.setTolerance( tolerance );
}

//------------------------------------------------------------------------------

void SimSimulator::setStretchLimit( Float stretchLimit )
{
    DGFX_ASSERT( ! inStep() );
    _stretchLimit = stretchLimit;
}

//------------------------------------------------------------------------------

void SimSimulator::setParams( const Params& params )
{
    DGFX_ASSERT( ! inStep() );
    _params = params;
}

//------------------------------------------------------------------------------

BaTime::Duration SimSimulator::getTimestep() const
{
    return BaTime::floatAsDuration( _h );
}

//------------------------------------------------------------------------------

Float SimSimulator::getDensity() const
{
    return _rho;
}

//------------------------------------------------------------------------------

Float SimSimulator::getPCGTolerance() const
{
    return _modPCG.getTolerance();
}

//------------------------------------------------------------------------------

Float SimSimulator::getStretchLimit() const
{
    return _stretchLimit;
}

//------------------------------------------------------------------------------

const SimSimulator::Params& SimSimulator::getParams() const
{
    return _params;
}

//------------------------------------------------------------------------------

GeVector SimSimulator::getVelocity( GeMesh::VertexId vid ) const
{
    DGFX_ASSERT( vid < _savedStepData._v0.size() );
    return _sd._v0[ vid ];
}

//------------------------------------------------------------------------------

GeVector SimSimulator::getForce( GeMesh::VertexId vid ) const
{
    DGFX_ASSERT( vid < _savedStepData._f0.size() );
    return _savedStepData._f0[ vid ];
}

//------------------------------------------------------------------------------

GeVector SimSimulator::getForce( ForceType type, GeMesh::VertexId vid ) const
{
    DGFX_ASSERT( type < NB_FORCES );
    DGFX_ASSERT( vid < _savedStepData._f0i[ type ].size() );
    return _savedStepData._f0i[ type ][ vid ];
}

//------------------------------------------------------------------------------

GeVector SimSimulator::getDampingForce(
    ForceType type,
    GeMesh::VertexId vid
) const {
    DGFX_ASSERT( type < NB_FORCES );
    DGFX_ASSERT( vid < _savedStepData._d0i[ type ].size() );
    return _savedStepData._d0i[ type ][ vid ];
}

//------------------------------------------------------------------------------

Float SimSimulator::getEnergy( ForceType type ) const
{
    DGFX_ASSERT( type < NB_FORCES );
    return _savedStepData._fenergy[ type ];
}

//------------------------------------------------------------------------------

Float SimSimulator::getTriEnergy( ForceType type, GeMesh::FaceId fid ) const
{
    DGFX_ASSERT( type < F_GRAVITY );
    DGFX_ASSERT( fid < _savedStepData._trienergy[ type ].size() );
    return _savedStepData._trienergy[ type ][ fid ];
}

//------------------------------------------------------------------------------

void SimSimulator::setMesh( const GeMesh& mesh )
{
    DGFX_ASSERT( ! inStep() );
    DGFX_ASSERT( mesh.getNbVertices() == _initialMesh->getNbVertices() );
    DGFX_ASSERT( mesh.getNbFaces() == _initialMesh->getNbFaces() );
    _mesh = RCShdPtr<GeMesh>( new GeMesh( mesh ) );
}



//------------------------------------------------------------------------------

void SimSimulator::calcFaceConsts()
{
    _faceConsts.resize( _initialMesh->getNbFaces() );
    GeMesh::FaceConstIterator fi;
    for( fi = _initialMesh->beginFace(); fi != _initialMesh->endFace(); ++fi ) {
        GeMesh::TextureVertexType tp[3];
        for( UInt32 m = 0; m < 3; ++m ) {
            tp[ m ] = _initialMesh->getTextureVertex( fi->getVertexId( m ) );
        }
        _faceConsts[ fi->getFaceId() ].calc( tp );
    }
}

//------------------------------------------------------------------------------

void SimSimulator::calcStretchShear(
    const GeMesh::FaceWrapper& face
) {
    UInt32 m;

    // My x[0],x[1],x[2] are equivalent to [BarWit98]'s xi,xj,xk.
    GeMesh::VertexType x[ 3 ];
    GeVector v0[ 3 ];
    for( m = 0; m < 3; ++m ) {
        UInt32 vid = face.getVertexId( m );
        x[ m ] = _mesh->getVertex( vid );
        v0[ m ] = _sd._v0[ vid ];
    }
    if ( DEBUG_COMMON ) {
        std::cout << "x0 = " << x[0] << " x1 = " << x[1]
            << " x2 = " << x[2] << std::endl;
    }

    CommonVars cv;
    FaceConsts const& fc = _faceConsts[ face.getFaceId() ];
    cv.calc( fc, x );
    if ( DEBUG_COMMON ) {
        cv.debugOutput( std::cout );
    }

    calcStretch( face, fc, cv, v0 );
    if ( VERIFY_STRETCH ) {
        verifyStretch( fc, cv, x, v0 );
    }

    // FIXME: keep individual Cu/Cv terms per-triangle. Maybe.
    if (
        BaMath::abs(
            _savedStepData._Cu[ face.getFaceId() ] - _sd._Cu[ face.getFaceId() ]
        ) > _stretchLimit ||
        BaMath::abs(
            _savedStepData._Cv[ face.getFaceId() ] - _sd._Cv[ face.getFaceId() ]
        ) > _stretchLimit
    ) {
        _stepSuccessFlag = false;
    }

    calcShear( face, fc, cv, v0 );
    if ( VERIFY_SHEAR ) {
        verifyShear( fc, cv, x, v0 );
    }
    if ( VERIFY_COMMON ) {
        verifyCommon();
    }
}


//------------------------------------------------------------------------------

void SimSimulator::calcStretch(
    const GeMesh::FaceWrapper& face,
    const FaceConsts& fc,
    const CommonVars& cv,
    const GeVector v0[ 3 ]
) {
    UInt32 m,n;

    StretchVars stv;
    stv.calc( fc, cv, v0, _params._b_u, _params._b_v );

    if ( DEBUG_STRETCH ) {
        stv.debugOutput( std::cout );
    }

    for ( m = 0; m < 3; ++m ) {
        UInt32 vid = face.getVertexId( m );
        GeVector val;

        // As per [BarWit98] eq. (7)
        val = -_params._k_stretch * (
            stv._dCu_dxm[ m ] * stv._Cu + stv._dCv_dxm[ m ] * stv._Cv
        );
        _sd._f0[ vid ] += val;
        DO_DEBUG( _sd._f0i[ F_STRETCH ][ vid ] += val );
        if ( DEBUG_STRETCH ) {
            std::cout << "f0[stretch][ " << m << " ] = " << val << std::endl;
        }

        val = -_params._k_stretch_damp * (
            stv._dCu_dxm[ m ] * stv._dCu_dt
            + stv._dCv_dxm[ m ] * stv._dCv_dt
        );
        _sd._f0[ vid ] += val;

        DO_DEBUG( _sd._d0i[ F_STRETCH ][ vid ] += val );
        if ( DEBUG_STRETCH ) {
            std::cout << "d0[stretch][ " << m << " ] = " << val << std::endl;
        }
    }
    for ( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        UInt32 vidm = face.getVertexId( m );
        UInt32 vidn = face.getVertexId( n );
        GeMatrix3 val;

        // As per [BarWit98] eq. (8)
        val = -_params._k_stretch * (
            GeMatrix3::outerProduct( stv._dCu_dxm[ m ], stv._dCu_dxm[ n ] )
            + GeMatrix3::outerProduct( stv._dCv_dxm[ m ], stv._dCv_dxm[ n ] )
            + stv._d2Cu_dxmdxn[ m ][ n ] * stv._Cu
            + stv._d2Cv_dxmdxn[ m ][ n ] * stv._Cv
        );
        _df_dx[ vidn ][ vidm ] += val;
        if ( DEBUG_STRETCH ) {
            std::cout << "df_dx[stretch][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = -_params._k_stretch_damp * (
            stv._d2Cu_dxmdxn[ m ][ n ] * stv._dCu_dt
            + stv._d2Cv_dxmdxn[ m ][ n ] * stv._dCv_dt
        );
        _df_dx[ vidn ][ vidm ] += val;
        if ( DEBUG_STRETCH ) {
            std::cout << "dd_dx[stretch][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = -_params._k_stretch_damp * (
            GeMatrix3::outerProduct( stv._dCu_dxm[m], stv._dCu_dxm[n] )
            + GeMatrix3::outerProduct( stv._dCv_dxm[m], stv._dCv_dxm[n] )
        );
        _df_dv[ vidn ][ vidm ] += val;
        if ( DEBUG_STRETCH ) {
            std::cout << "dd_dv[stretch][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }
    }

    Float E =
        _params._k_stretch * .5 * ( stv._Cu * stv._Cu + stv._Cv * stv._Cv );
    _sd._trienergy[ F_STRETCH ][ face.getFaceId() ] = E;
    _sd._Cu[ face.getFaceId() ] = stv._Cu / fc._alpha;
    _sd._Cv[ face.getFaceId() ] = stv._Cv / fc._alpha;
    _sd._fenergy[ F_STRETCH ] += E;

    if ( DEBUG_STRETCH ) {
        std::cout << std::endl;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::calcShear(
    const GeMesh::FaceWrapper& face,
    const FaceConsts& fc,
    const CommonVars& cv,
    const GeVector v0[ 3 ]
) {
    UInt32 m,n;

    ShearVars shv;
    shv.calc( fc, cv, v0 );

    if ( DEBUG_SHEAR ) {
        shv.debugOutput( std::cout );
    }

    for ( m = 0; m < 3; ++m ) {
        UInt32 vid = face.getVertexId( m );
        GeVector val;

        val = -_params._k_shear * shv._dC_dxm[ m ] * shv._C;
        _sd._f0[ vid ] += val;
        DO_DEBUG( _sd._f0i[ F_SHEAR ][ vid ] += val );
        if ( DEBUG_SHEAR ) {
            std::cout << "f0[shear][ " << m << " ] = " << val << std::endl;
        }

        val = -_params._k_shear_damp
            * shv._dC_dxm[ m ] * shv._dC_dt;
        _sd._f0[ vid ] += val;

        DO_DEBUG( _sd._d0i[ F_SHEAR ][ vid ] += val );
        if ( DEBUG_SHEAR ) {
            std::cout << "d0[shear][ " << m << " ] = " << val << std::endl;
        }
    }
    for ( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        UInt32 vidm = face.getVertexId( m );
        UInt32 vidn = face.getVertexId( n );
        GeMatrix3 val;
        val = -_params._k_shear * (
            GeMatrix3::outerProduct( shv._dC_dxm[ m ], shv._dC_dxm[ n ] )
            + ( shv._d2C_dxmdxn[ m ][ n ] * shv._C ) * GeMatrix3::identity()
        );
        _df_dx[ vidn ][ vidm ] += val;
        if ( DEBUG_SHEAR ) {
            std::cout << "df_dx[shear][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = GeMatrix3::identity() * ( -_params._k_shear_damp
            * shv._d2C_dxmdxn[ m ][ n ] * shv._dC_dt );
        _df_dx[ vidn ][ vidm ] += val;
        if ( DEBUG_SHEAR ) {
            std::cout << "dd_dx[shear][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = -_params._k_shear_damp
            * GeMatrix3::outerProduct( shv._dC_dxm[ m ], shv._dC_dxm[ n ] );
        _df_dv[ vidn ][ vidm ] += val;

        if ( DEBUG_SHEAR ) {
            std::cout << "dd_dv[shear][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }
    }

    Float E = _params._k_shear * .5 * shv._C * shv._C;
    _sd._trienergy[ F_SHEAR ][ face.getFaceId() ] = E;
    _sd._fenergy[ F_SHEAR ] += E;

    if ( DEBUG_SHEAR ) {
        std::cout << std::endl;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::calcBend(
    const GeMeshWingedEdge::HalfEdgeWrapper& edge
) {
    UInt32 m, n;

    GeMeshWingedEdge::HalfEdgeWrapper he_next( edge.getNextHalfEdge() );
    GeMeshWingedEdge::HalfEdgeWrapper he_prev( edge.getPrevHalfEdge() );
    GeMeshWingedEdge::HalfEdgeWrapper he_twin( edge.getTwinHalfEdge() );

    GeMesh::VertexId vid[ 4 ];
    vid[ 0 ] = he_prev.getOriginVertexId();
    vid[ 1 ] = edge.getOriginVertexId();
    vid[ 2 ] = he_next.getOriginVertexId();
    vid[ 3 ] = he_twin.getPrevHalfEdge().getOriginVertexId();
    GeMesh::VertexType x[ 4 ];
    GeMesh::TextureVertexType tv[ 4 ];
    GeVector v[ 4 ];
    for( m = 0; m < 4; ++m ) {
        x[ m ] = _mesh->getVertex( vid[ m ] );
        tv[ m ] = _initialMesh->getTextureVertex( vid[ m ] );
        v[ m ] = _sd._v0[ vid[ m ] ];
    }

    const Float du = tv[ 1 ]._x - tv[ 2 ]._x;
    const Float dv = tv[ 1 ]._y - tv[ 2 ]._y;
    const Float k = (
        _params._k_bend_u * du*du + _params._k_bend_v * dv*dv ) /
        (du*du + dv*dv);

    BendVars bv;
    GeMesh::FaceId fidA( edge.getFaceId() );
    GeMesh::FaceId fidB( he_twin.getFaceId() );
    bv._nhatA = _faceUnitNormals[ fidA ];
    bv._nAim = _faceNormalIMs[ fidA ];
    bv._nhatB = _faceUnitNormals[ fidB ];
    bv._nBim = _faceNormalIMs[ fidB ];
    bv.calc( x, v );
    if ( DEBUG_BEND ) {
        std::cout << "x0 = " << x[0] << "  x1 = " << x[1]
            << "  x2 = " << x[2] << "  x3 = " << x[3] << std::endl;
        std::cout << "k = " << k << std::endl;
        bv.debugOutput( std::cout );
    }

    // Same finale as stretch/shear
    for ( m = 0; m < 4; ++m ) {
        GeVector val;

        val = -k * bv._dC_dxm[ m ] * bv._C;
        _sd._f0[ vid[ m ] ] += val;
        DO_DEBUG( _sd._f0i[ F_BEND ][ vid[ m ] ] += val );
        if ( DEBUG_BEND ) {
            std::cout << "f0[bend][ " << m << " ] = " << val << std::endl;
        }

        val = -_params._k_bend_damp * bv._dC_dxm[ m ] * bv._dC_dt;
        _sd._f0[ vid[ m ] ] += val;

        DO_DEBUG( _sd._d0i[ F_BEND ][ vid[ m ] ] += val );
        if ( DEBUG_BEND ) {
            std::cout << "d0[bend][ " << m << " ] = " << val << std::endl;
        }
    }

    Float E = .5 * k * bv._C * bv._C;
    // Spread energy to both triangles for debugging
    _sd._trienergy[ F_BEND ][ edge.getFaceId() ] += E * .5;
    _sd._trienergy[ F_BEND ][ he_twin.getFaceId() ] += E * .5;
    _sd._fenergy[ F_BEND ] += E;

    for ( m = 0; m < 4; ++m ) for ( n = 0; n < 4; ++n ) {
        GeMatrix3 val;
        val = -k * (
            GeMatrix3::outerProduct( bv._dC_dxm[ m ], bv._dC_dxm[ n ] )
            + bv._d2C_dxmdxn[ m ][ n ] * bv._C
        );
        _df_dx[ vid[ n ] ][ vid[ m ] ] += val;
        if ( DEBUG_BEND ) {
            std::cout << "df_dx[bend][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = -_params._k_bend_damp
            * bv._d2C_dxmdxn[ m ][ n ] * bv._dC_dt;
        _df_dx[ vid[ n ] ][ vid[ m ] ] += val;
        if ( DEBUG_BEND ) {
            std::cout << "dd_dx[bend][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }

        val = -_params._k_bend_damp
            * GeMatrix3::outerProduct( bv._dC_dxm[ m ], bv._dC_dxm[ n ] );
        _df_dv[ vid[ n ] ][ vid[ m ] ] += val;

        if ( DEBUG_BEND ) {
            std::cout << "dd_dv[bend][ " << m << " ][ " << n << " ] = "
                << val << std::endl;
        }
    }
    if ( DEBUG_BEND ) {
        std::cout << std::endl << std::endl;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::verifyCommon()
{
#if DO_NORM
    const Float MAX_VERIFY_ERROR = .01; // maximum allowed
    const Float MAX_VERIFY_ERROR_M = MAX_VERIFY_ERROR * 3; // for matrices
    const Float VERIFY_STEP = .001;     // db
    CommonVars ver_cv[3][3];
    for( m = 0; m < 3; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        GePoint testx[3] = { x[0], x[1], x[2] };
        testx[ m ][ s ] += VERIFY_STEP;
        ver_cv[ m ][ s ].calc( testx );
    }

    bool DISPLAY;

    UInt32 n,s,t;
    DISPLAY = false;
    for( m = 0; m < 3; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        const CommonVars& testcv = ver_cv[m][s];

        GeVector test_dwhatu_dxm = ( testcv._whatu - cv._whatu ) / VERIFY_STEP;
        GeVector test_dwhatv_dxm = ( testcv._whatv - cv._whatv ) / VERIFY_STEP;

        Float err;

        if ( DISPLAY ) {
            std::cout << "test_dwhatu_dxm[ " << m << " ][ " << s << " ] = "
                << test_dwhatu_dxm << std::endl;
        }
        err = ( test_dwhatu_dxm - cv._dwhatu_dxm[m][s] ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );

        if ( DISPLAY ) {
            std::cout << "test_dwhatv_dxm[ " << m << " ][ " << s << " ] = "
                << test_dwhatv_dxm << std::endl;
        }
        err = ( test_dwhatv_dxm - cv._dwhatv_dxm[m][s] ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );
    }

    DISPLAY = false;
    for ( n = 0; n < 3; ++n ) for( m = 0; m < 3; ++m ) {
        for( s = 0; s < 3; ++s ) for( t = 0; t < 3; ++t ) {
            const CommonVars& testcv = ver_cv[n][t];
            GeVector test_d2whatu_dxmdxn =
                (testcv._dwhatu_dxm[m][s] - cv._dwhatu_dxm[m][s]) / VERIFY_STEP;
            GeVector test_d2whatv_dxmdxn =
                (testcv._dwhatv_dxm[m][s] - cv._dwhatv_dxm[m][s]) / VERIFY_STEP;

            Float err;

            if ( DISPLAY ) {
                std::cout << "test_d2whatu_dxmdxn[" << m << "][" << n << "]["
                    << s << "][" << t << "] = "
                    << test_d2whatu_dxmdxn << std::endl;
            }
            err = (
                test_d2whatu_dxmdxn - cv._d2whatu_dxmdxn[m][n][s][t]
            ).infinityNorm();
            DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );

            if ( DISPLAY ) {
                std::cout << "test_d2whatv_dxmdxn[" << m << "][" << n << "]["
                    << s << "][" << t << "] = "
                    << test_d2whatv_dxmdxn << std::endl;
            }
            err = (
                test_d2whatv_dxmdxn - cv._d2whatv_dxmdxn[m][n][s][t]
            ).infinityNorm();
            DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
        }
    }
#endif
}

//------------------------------------------------------------------------------

void SimSimulator::verifyStretch(
    const FaceConsts& fc,
    const CommonVars& cv,
    const GePoint x[3],
    const GeVector v0[ 3 ]
) {
    const Float MAX_VERIFY_ERROR = .01f; // maximum allowed
    const Float MAX_VERIFY_ERROR_M = MAX_VERIFY_ERROR * 3; // for matrices
    const Float VERIFY_STEP = .001f;     // db
    UInt32 m,n,s,t;

    StretchVars stv;
    stv.calc( fc, cv, v0, _params._b_u, _params._b_v );

    StretchVars ver_stv[3][3];
    for( m = 0; m < 3; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        GePoint testx[3] = { x[0], x[1], x[2] };
        testx[ m ][ s ] += VERIFY_STEP;
        CommonVars ver_cv;
        ver_cv.calc( fc, testx );
        ver_stv[m][s].calc( fc, ver_cv, v0, _params._b_u, _params._b_v );
    }
    bool DISPLAY;

    DISPLAY = false;
    for( m = 0; m < 3; ++m ) {
        GeVector testdCu_dxm, testdCv_dxm;
        for( UInt32 s = 0; s < 3; ++s ) {
            const StretchVars& teststv = ver_stv[m][s];
            testdCu_dxm[ s ] = ( teststv._Cu - stv._Cu ) / VERIFY_STEP;
            testdCv_dxm[ s ] = ( teststv._Cv - stv._Cv ) / VERIFY_STEP;
        }
        Float err;
        if ( DISPLAY ) {
            std::cout << "testdCu_dxm[ " << m << " ] = " << testdCu_dxm
                << std::endl;
            std::cout << "testdCv_dxm[ " << m << " ] = " << testdCv_dxm
                << std::endl;
        }
        err = (
            testdCu_dxm - stv._dCu_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
        err = (
            testdCv_dxm - stv._dCv_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }

    DISPLAY = false;
    for( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        GeMatrix3 testd2Cu_dxmdxn, testd2Cv_dxmdxn;
        for( t = 0; t < 3; ++t ) {
            const StretchVars& teststv = ver_stv[n][t];
            const GeVector diffu(
                teststv._dCu_dxm[ m ] - stv._dCu_dxm[ m ]
            );
            const GeVector diffv(
                teststv._dCv_dxm[ m ] - stv._dCv_dxm[ m ]
            );
            for( s = 0; s < 3; ++s ) {
                testd2Cu_dxmdxn( t, s ) = diffu[ s ] / VERIFY_STEP;
                testd2Cv_dxmdxn( t, s ) = diffv[ s ] / VERIFY_STEP;
            }
        }

        Float err;
        if ( DISPLAY ) {
            std::cout << "testd2Cu_dxmdxn[ " << m << " ][ " << n
                << " ] = " << testd2Cu_dxmdxn << std::endl;
        }
        err = (
            testd2Cu_dxmdxn - stv._d2Cu_dxmdxn[ m ][ n ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );

        if ( DISPLAY ) {
            std::cout << "testd2Cv_dxmdxn[ " << m << " ][ " << n
                << " ] = " << testd2Cv_dxmdxn << std::endl;
        }
        err = (
            testd2Cv_dxmdxn - stv._d2Cv_dxmdxn[ m ][ n ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }
}

//------------------------------------------------------------------------------

void SimSimulator::verifyShear(
    const FaceConsts& fc,
    const CommonVars& cv,
    const GePoint x[3],
    const GeVector v0[ 3 ]
) {
    const Float MAX_VERIFY_ERROR = .01f; // maximum allowed
    const Float MAX_VERIFY_ERROR_M = MAX_VERIFY_ERROR * 3; // for matrices
    const Float VERIFY_STEP = .001f;     // db
    UInt32 m,n,s,t;

    ShearVars shv;
    shv.calc( fc, cv, v0 );

    ShearVars ver_shv[3][3];
    for( m = 0; m < 3; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        GePoint testx[3] = { x[0], x[1], x[2] };
        testx[ m ][ s ] += VERIFY_STEP;
        CommonVars ver_cv;
        ver_cv.calc( fc, testx );
        ver_shv[ m ][ s ].calc( fc, ver_cv, v0 );
    }
    bool DISPLAY;

    DISPLAY = false;
    for( m = 0; m < 3; ++m ) {
        GeVector testdC_dxm;
        for( UInt32 s = 0; s < 3; ++s ) {
            const ShearVars& testshv = ver_shv[m][s];
            testdC_dxm[ s ] = ( testshv._C - shv._C ) / VERIFY_STEP;
        }
        Float err;
        if ( DISPLAY ) {
            std::cout << "testdC_dxm[ " << m << " ] = " << testdC_dxm
                << std::endl;
        }
        err = (
            testdC_dxm - shv._dC_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }

    DISPLAY = false;
    for( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        GeMatrix3 testd2C_dxmdxn;
        for( t = 0; t < 3; ++t ) {
            const ShearVars& testshv = ver_shv[n][t];
            const GeVector diff( testshv._dC_dxm[ m ] - shv._dC_dxm[ m ] );
            for( s = 0; s < 3; ++s ) {
                testd2C_dxmdxn( t, s ) = diff[ s ] / VERIFY_STEP;
            }
        }

        Float err;
        if ( DISPLAY ) {
            std::cout << "testd2C_dxmdxn[ " << m << " ][ " << n
                << " ] = " << testd2C_dxmdxn << std::endl;
        }
        err = (
            testd2C_dxmdxn - shv._d2C_dxmdxn[ m ][ n ] * GeMatrix3::identity()
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }
}

//------------------------------------------------------------------------------

void SimSimulator::verifyBend(
    const GeMeshWingedEdge::HalfEdgeWrapper& edge
) {
    const Float MAX_VERIFY_ERROR = .01f; // maximum allowed
    const Float MAX_VERIFY_ERROR_M = MAX_VERIFY_ERROR * 3; // for matrices
    const Float VERIFY_STEP = .001f;     // db

    UInt32 m, n, s, t;

    GeMeshWingedEdge::HalfEdgeWrapper he_next( edge.getNextHalfEdge() );
    GeMeshWingedEdge::HalfEdgeWrapper he_prev( edge.getPrevHalfEdge() );
    GeMeshWingedEdge::HalfEdgeWrapper he_twin( edge.getTwinHalfEdge() );

    GeMesh::VertexId vid[ 4 ];
    vid[ 0 ] = he_prev.getOriginVertexId();
    vid[ 1 ] = edge.getOriginVertexId();
    vid[ 2 ] = he_next.getOriginVertexId();
    vid[ 3 ] = he_twin.getPrevHalfEdge().getOriginVertexId();
    GeMesh::VertexType x[ 4 ];
    GeVector v[ 4 ];
    for( m = 0; m < 4; ++m ) {
        x[ m ] = _mesh->getVertex( vid[ m ] );
        v[ m ] = _sd._v0[ vid[ m ] ];
    }

    BendVars bv;
    GeMesh::FaceId fidA( edge.getFaceId() );
    GeMesh::FaceId fidB( he_twin.getFaceId() );
    bv._nhatA = _faceUnitNormals[ fidA ];
    bv._nAim = _faceNormalIMs[ fidA ];
    bv._nhatB = _faceUnitNormals[ fidB ];
    bv._nBim = _faceNormalIMs[ fidB ];
    bv.calc( x, v );

    BendVars ver_bv[4][3];
    for( m = 0; m < 4; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        GePoint testx[4] = { x[0], x[1], x[2], x[3] };
        testx[ m ][ s ] += VERIFY_STEP;
        ver_bv[ m ][ s ].calc( testx, v, bv );
    }
    bool DISPLAY;

    DISPLAY = false;
    for( m = 0; m < 4; ++m ) for( UInt32 s = 0; s < 3; ++s ) {
        const BendVars& testbv = ver_bv[m][s];

        GeVector test_dnhatA_dxm =
            ( testbv._nhatA - bv._nhatA ) / VERIFY_STEP;
        GeVector test_dnhatB_dxm =
            ( testbv._nhatB - bv._nhatB ) / VERIFY_STEP;
        GeVector test_dehat_dxm = ( testbv._ehat - bv._ehat ) / VERIFY_STEP;

        Float err;

        if ( DISPLAY ) {
            std::cout << "test_dnhatA_dxm[ " << m << " ][ " << s << "] = "
                << test_dnhatA_dxm << std::endl;
        }
        err = ( test_dnhatA_dxm - bv._dnhatA_dxm[m][s] ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );

        if ( DISPLAY ) {
            std::cout << "test_dnhatB_dxm[ " << m << " ][ " << s << "] = "
                << test_dnhatB_dxm << std::endl;
        }
        err = ( test_dnhatB_dxm - bv._dnhatB_dxm[m][s] ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );

        if ( DISPLAY ) {
            std::cout << "test_dehat_dxm[ " << m << " ][ " << s << "] = "
                << test_dehat_dxm << std::endl;
        }
        err = ( test_dehat_dxm - bv._dehat_dxm[m][s] ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );
    }

    DISPLAY = false;
    for( n = 0; n < 4; ++n ) {
        GeVector test_d2nA_dxmdxn[ 4 ][ 3 ][ 3 ];
        GeVector test_d2nB_dxmdxn[ 4 ][ 3 ][ 3 ];
        GeVector test_d2e_dxmdxn[ 4 ][ 3 ][ 3 ];
        for( t = 0; t < 3; ++t ) {
            const BendVars& testbv = ver_bv[n][t];
            for( m = 0; m < 4; ++m ) for( s = 0; s < 3; ++s ) {
                test_d2nA_dxmdxn[ m ][ s ][ t ] = (
                    testbv._dnhatA_dxm[ m ][ s ] - bv._dnhatA_dxm[ m ][ s ]
                ) / VERIFY_STEP;
                test_d2nB_dxmdxn[ m ][ s ][ t ] = (
                    testbv._dnhatB_dxm[ m ][ s ] - bv._dnhatB_dxm[ m ][ s ]
                ) / VERIFY_STEP;
                test_d2e_dxmdxn[ m ][ s ][ t ] = (
                    testbv._dehat_dxm[ m ][ s ] - bv._dehat_dxm[ m ][ s ]
                ) / VERIFY_STEP;
            }
        }
        Float err;
        for ( m = 0; m < 4; ++m ) {
            for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
                if ( DISPLAY ) {
                    std::cout << "test_d2nA_dxmdxn[ " << m << " ][ "
                        << n << " ][ " << s << " ][ " << t << " ] = "
                        << test_d2nA_dxmdxn[ m ][ s ][ t ] << std::endl;
                }
                test_d2nA_dxmdxn[m][s][t] -= bv._d2nA_dxmdxn[m][n][s][t];
                err = test_d2nA_dxmdxn[m][s][t].infinityNorm();
                DGFX_ASSERT( err < MAX_VERIFY_ERROR );

                if ( DISPLAY ) {
                    std::cout << "test_d2nB_dxmdxn[ " << m << " ][ "
                        << n << " ][ " << s << " ][ " << t << " ] = "
                        << test_d2nB_dxmdxn[ m ][ s ][ t ] << std::endl;
                }
                test_d2nB_dxmdxn[m][s][t] -= bv._d2nB_dxmdxn[m][n][s][t];
                err = test_d2nB_dxmdxn[m][s][t].infinityNorm();
                DGFX_ASSERT( err < MAX_VERIFY_ERROR );

                if ( DISPLAY ) {
                    std::cout << "test_d2e_dxmdxn[ " << m << " ][ "
                        << n << " ][ " << s << " ][ " << t << " ] = "
                        << test_d2e_dxmdxn[ m ][ s ][ t ] << std::endl;
                }
#if DO_NORM
                test_d2e_dxmdxn[m][s][t] -= bv._d2e_dxmdxn[m][n][s][t];
#endif
                err = test_d2e_dxmdxn[m][s][t].infinityNorm();
                DGFX_ASSERT( err < MAX_VERIFY_ERROR );
            }
        }
    }

    DISPLAY = false;
    for( m = 0; m < 4; ++m ) {
        GeVector test_dcosth_dxm, test_dsinth_dxm;
        for( UInt32 s = 0; s < 3; ++s ) {
            const BendVars& testbv = ver_bv[m][s];

            test_dcosth_dxm[ s ] =
                (testbv._costh - bv._costh ) / VERIFY_STEP;
            test_dsinth_dxm[ s ] =
                (testbv._sinth - bv._sinth ) / VERIFY_STEP;
        }
        Float err;

        if ( DISPLAY ) {
            std::cout << "test_dcosth_dxm[ " << m << " ] = "
                << test_dcosth_dxm << std::endl;
        }
        err = (
            test_dcosth_dxm - bv._dcosth_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );
        if ( DISPLAY ) {
            std::cout << "test_dsinth_dxm[ " << m << " ] = "
                << test_dsinth_dxm << std::endl;
        }
        err = (
            test_dsinth_dxm - bv._dsinth_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR );
    }

    DISPLAY = false;
    for( n = 0; n < 4; ++n ) {
        for( m = 0; m < 4; ++m ) {
            GeMatrix3 test_d2costh_dxmdxn, test_d2sinth_dxmdxn;
            for( t = 0; t < 3; ++t ) {
                const BendVars& testbv = ver_bv[n][t];
                const GeVector diffc(
                    ( testbv._dcosth_dxm[ m ] - bv._dcosth_dxm[ m ] )
                    / VERIFY_STEP
                );
                const GeVector diffs(
                    ( testbv._dsinth_dxm[ m ] - bv._dsinth_dxm[ m ] )
                    / VERIFY_STEP
                );
                for ( s = 0; s < 3; ++s ) {
                    test_d2costh_dxmdxn( t, s ) = diffc[ s ];
                    test_d2sinth_dxmdxn( t, s ) = diffs[ s ];
                }
            }

            Float err;

            if ( DISPLAY ) {
                std::cout << "test_d2costh_dxmdxn[ " << m << " ][ " << n
                    << " ] = " << test_d2costh_dxmdxn << std::endl;
            }
            err = (
                test_d2costh_dxmdxn - bv._d2costh_dxmdxn[ m ][ n ]
            ).infinityNorm();
            DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );

            if ( DISPLAY ) {
                std::cout << "test_d2sinth_dxmdxn[ " << m << " ][ " << n
                    << " ] = " << test_d2sinth_dxmdxn << std::endl;
            }
            err = (
                test_d2sinth_dxmdxn - bv._d2sinth_dxmdxn[ m ][ n ]
            ).infinityNorm();
            DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
        }
    }

    DISPLAY = false;
    for( m = 0; m < 4; ++m ) {
        GeVector testdC_dxm;
        for( UInt32 s = 0; s < 3; ++s ) {
            const BendVars& testbv = ver_bv[m][s];
            testdC_dxm[ s ] = ( testbv._C - bv._C ) / VERIFY_STEP;
        }
        Float err;
        if ( DISPLAY ) {
            std::cout << "testdC_dxm[ " << m << " ] = "
                << testdC_dxm << std::endl;
        }
        err = (
            testdC_dxm - bv._dC_dxm[ m ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }

    DISPLAY = false;
    for ( n = 0; n < 4; ++n ) for( m = 0; m < 4; ++m ) {
        GeMatrix3 testd2C_dxmdxn;
        for( t = 0; t < 3; ++t ) {
            const BendVars& testbv = ver_bv[n][t];
            const GeVector diff( testbv._dC_dxm[ m ] - bv._dC_dxm[ m ] );
            for( s = 0; s < 3; ++s ) {
                testd2C_dxmdxn( t, s ) = diff[ s ] / VERIFY_STEP;
            }
        }

        Float err;
        if ( DISPLAY ) {
            std::cout << "testd2C_dxmdxn[ " << m << " ][ " << n
                << " ] = " << testd2C_dxmdxn << std::endl;
        }
        err = (
            testd2C_dxmdxn - bv._d2C_dxmdxn[ m ][ n ]
        ).infinityNorm();
        DGFX_ASSERT( err < MAX_VERIFY_ERROR_M );
    }
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::FaceConsts

//------------------------------------------------------------------------------

void SimSimulator::FaceConsts::calc( const GePoint_3 tp )
{
    _du1 = tp[1]._x - tp[0]._x;
    _dv1 = tp[1]._y - tp[0]._y;
    _du2 = tp[2]._x - tp[0]._x;
    _dv2 = tp[2]._y - tp[0]._y;
    const Float det = _du1 * _dv2 - _du2 * _dv1;
    //DGFX_ASSERT( ! BaMath::isEqual( det, 0 ) );
    _detInv = 1 / det;

    // Area in u/v co-ordinates.
    // $ a = \frac{1}{2} \norm{
    //          \begin{pmatrix} \du_1 \\ \dv_1 \\ 0 \end{pmatrix} \cross
    //          \begin{pmatrix} \du_2 \\ \dv_2 \\ 0 \end{pmatrix} } $
    _alpha = .5 * ( tp[ 1 ] - tp[ 0 ] ).cross( tp[ 2 ] - tp[ 0 ] ).length();

#if DO_SCALE_INVARIANT
    // _alpha = _alpha ^ 3/4
    _alpha = BaMath::sqrt( _alpha );
    _alpha = _alpha * BaMath::sqrt( _alpha );
#endif
}

//------------------------------------------------------------------------------

void SimSimulator::FaceConsts::debugOutput( std::ostream& os )
{
    os << "alpha = " << _alpha << std::endl;
    os << "(du1,dv1) = " << _du1 << "," << _dv1 << std::endl;
    os << "(du2,dv2) = " << _du2 << "," << _dv2 << std::endl;
    os << "detInv = " << _detInv << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::CommonVars

//------------------------------------------------------------------------------

void SimSimulator::CommonVars::calc(
    const FaceConsts& fc,
    const GePoint x[3]
) {
    // $ \wu = \frac{ (\x_1 - \x_0) \dv_2 - (\x_2 - \x_0) \dv_1}
    //     {\du_1 \dv_2 - \du_2 \dv_1} $
    _wu = ((x[1]-x[0]) * fc._dv2 + (x[2]-x[0]) * (-fc._dv1)) * fc._detInv;
    const Float wu_len = _wu.length();
    DGFX_ASSERT( BaMath::isGreater( wu_len, 0 ) );
    _wuim = 1 / wu_len;
    // $ \whatu = \frac{ \wu }{\norm {\wu}} $
    _whatu = _wu * _wuim;

    // $ \wv = \frac{ -(\x_1 - \x_0) \du_2 + (\x_2 - \x_0) \du_1}
    //      {\du_1 \dv_2 - \du_2 \dv_1} $
    _wv = (-(x[1]-x[0]) * fc._du2 + (x[2]-x[0]) * fc._du1) * fc._detInv;
    const Float wv_len = _wv.length();
    DGFX_ASSERT( BaMath::isGreater( wv_len, 0 ) );
    _wvim = 1 / wv_len;
    // $ \whatv = \frac{ \wv }{\norm {\wv}} $
    _whatv = _wv * _wvim;

    // Array subscript refers to *which* x this is a derivative
    // of - xi, xj or xk. Here, we only have a scalar value - 
    // _dwux_dxmx, the derivative of the x-component of wu with
    // respect to the x-component of xi (or xj, or xk, etc.).
    // dwu_dxm should be dwux_dxmx times a 3x3 identity matrix.
    // $ \pfrac{\wux}{\x_{0_x}} = \frac{\dv_1 - \dv_2}{\du_1\dv_2 - \du_2\dv_1}$
    // $ \pfrac{\wux}{\x_{1_x}} = \frac{\dv_2}{\du_1\dv_2 - \du_2\dv_1} $
    // $ \pfrac{\wux}{\x_{2_x}} = \frac{-\dv_1}{\du_1\dv_2 - \du_2\dv_1} $
    _dwux_dxmx[ 0 ] = (fc._dv1 - fc._dv2) * fc._detInv;
    _dwux_dxmx[ 1 ] = fc._dv2 * fc._detInv;
    _dwux_dxmx[ 2 ] =-fc._dv1 * fc._detInv;
    // $ \pfrac{\wvx}{\x_{0_x}} = \frac{\du_2 - \du_1}{\du_1\dv_2 - \du_2\dv_1}$
    // $ \pfrac{\wvx}{\x_{1_x}} = \frac{-\du_2}{\du_1\dv_2 - \du_2\dv_1} $
    // $ \pfrac{\wvx}{\x_{2_x}} = \frac{\du_1}{\du_1\dv_2 - \du_2\dv_1} $
    _dwvx_dxmx[ 0 ] = (fc._du2 - fc._du1) * fc._detInv;
    _dwvx_dxmx[ 1 ] =-fc._du2 * fc._detInv;
    _dwvx_dxmx[ 2 ] = fc._du1 * fc._detInv;

#if DO_NORM
    UInt32 m,n,s,t;

    for( m = 0; m < 3; ++m ) {
        for ( s = 0; s < 3; ++s ) {
            // $ \pfrac{\whatu}{\xms} =
            //      \frac{1}{\norm{\wu}}
            //      \pfrac{\wux}{\xmx} \left( \Is - \whatus \whatu \right) $
            _dwhatu_dxm[ m ][ s ] = _wuim * _dwux_dxmx[ m ] *
                ( GeVector::axis( s ) - _whatu[ s ] * _whatu );
            // $ \pfrac{\whatv}{\xms} =
            //      \frac{1}{\norm{\wv}}
            //      \pfrac{\wvx}{\xmx} \left( \Is - \whatvs \whatv \right) $
            _dwhatv_dxm[ m ][ s ] = _wvim * _dwvx_dxmx[ m ] *
                ( GeVector::axis( s ) - _whatv[ s ] * _whatv );
        }
    }

    for( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        for( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
            // $ \pfractwo{\whatu}{\xms}{\xnt} =
            //      -\frac{1}{\norm{\wu}} \pfrac{\wux}{\xmx} \left(
            //          \whatus \pfrac{\whatu}{\xnt}
            //          + \pfrac{\whatus}{\xnt} \whatu
            //      \right) -
            //      \pfrac{\wux}{\xmx} \pfrac{\wux}{\xnx}
            //      \frac{( \Is - \whatus \whatu ) \whatut}{\norm{\wu}^2} $
            _d2whatu_dxmdxn[m][n][s][t] =
                -_wuim * _dwux_dxmx[ m ] *
                (_whatu[s] * _dwhatu_dxm[n][t] + _dwhatu_dxm[n][t][s] * _whatu)
                - _wuim*_wuim * _dwux_dxmx[m] * _dwux_dxmx[n] *
                (GeVector::axis(s) - _whatu[s]*_whatu) * _whatu[t];

            // $ \pfractwo{\whatv}{\xms}{\xnt} =
            //     -\frac{1}{\norm{\wv}} \pfrac{\wvx}{\xmx} \left(
            //         \whatvs \pfrac{\whatv}{\xnt}
            //         + \pfrac{\whatvs}{\xnt} \whatv
            //     \right) -
            //     \pfrac{\wvx}{\xmx} \pfrac{\wvx}{\xnx}
            //     \frac{( \Is - \whatvs \whatv ) \whatvt}{\norm{\wv}^2} $
            _d2whatv_dxmdxn[m][n][s][t] =
                -_wvim * _dwvx_dxmx[ m ] *
                (_whatv[s] * _dwhatv_dxm[n][t] + _dwhatv_dxm[n][t][s] * _whatv)
                - _wvim*_wvim * _dwvx_dxmx[m] * _dwvx_dxmx[n] *
                (GeVector::axis(s) - _whatv[s]*_whatv) * _whatv[t];
        }
    }
#endif
}

//------------------------------------------------------------------------------

void SimSimulator::CommonVars::debugOutput( std::ostream& os )
{
    UInt32 m;

    os << "whatu = " << _whatu << "  _wuim = " << _wuim << "  wu = "
        << _wu << std::endl;
    os << "whatv = " << _whatv << "  _wvim = " << _wvim << "  wv = "
        << _wv << std::endl;
    for ( m = 0; m < 3; ++m ) {
        os << "_dwux_dxmx[" << m << "] = " << _dwux_dxmx[ m ] << std::endl;
        os << "_dwvx_dxmx[" << m << "] = " << _dwvx_dxmx[ m ] << std::endl;
    }
#if DO_NORM
    UInt32 n,s,t;

    for ( m = 0; m < 3; ++m ) for ( s = 0; s < 3; ++s ) {
        std::cout << "_dwhatu_dxm[" << m << "][" << s << "] = "
            << _dwhatu_dxm[m][s] << std::endl;
        std::cout << "_dwhatv_dxm[" << m << "][" << s << "] = "
            << _dwhatv_dxm[m][s] << std::endl;
    }
    for ( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
            std::cout << "_d2whatu_dxmdxn[" << m << "][" << n << "][" << s
                << "][" << t << "] = " << _d2whatu_dxmdxn[m][n][s][t]
                << std::endl;
            std::cout << "_d2whatv_dxmdxn[" << m << "][" << n << "][" << s
                << "][" << t << "] = " << _d2whatv_dxmdxn[m][n][s][t]
                << std::endl;
        }
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::StretchVars

//------------------------------------------------------------------------------

void SimSimulator::StretchVars::calc(
    const SimSimulator::FaceConsts& fc,
    const SimSimulator::CommonVars& cv,
    const GeVector v0[ 3 ],
    Float b_u, Float b_v
) {
    UInt32 m,n;

    // As per [BarWit98] eq. (10)
    // $ \C = \alpha \left(
    //      \begin{pmatrix}
    //          \norm{\wu} - b_u \\ %
    //          \norm{\wv} - b_v
    //      \end{pmatrix} \right) $
    _Cu = fc._alpha * ( cv._wu.length() - b_u );
    _Cv = fc._alpha * ( cv._wv.length() - b_v );

    for ( m = 0; m < 3; ++m ) {
        // $ \pfrac{\Cu}{\xm} = \alpha \pfrac{\wux}{\xmx} \whatu $
        _dCu_dxm[ m ] = ( fc._alpha * cv._dwux_dxmx[ m ] ) * cv._whatu;
        // $ \pfrac{\Cv}{\xm} = \alpha \pfrac{\whatvx}{\xmx} \whatv $
        _dCv_dxm[ m ] = ( fc._alpha * cv._dwvx_dxmx[ m ] ) * cv._whatv;
    }

    // As per [BarWit98] equation just before (11) - but corrected to be
    // a sum.
    // $ \pfrac{\bf C}{t} = \sum_m
    //      \left( \pfrac{\bf C}{\xm} \right)^T \pfrac{\xm}{t} $
    _dCu_dt = 0;
    _dCv_dt = 0;
    for( m = 0; m < 3; ++m ) {
        _dCu_dt += _dCu_dxm[ m ].dot( v0[ m ] );
        _dCv_dt += _dCv_dxm[ m ].dot( v0[ m ] );
    }

    // Second derivative of C, split into u and v components
    // FIXME: this is symmetric - if you switch pm and pn, you get the same
    // result.
    for ( m = 0; m < 3; ++m ) for ( n = m; n < 3; ++n ) {
        // $ \pfractwo{\Cu}{\xm}{\xn} =
        //      \frac{\alpha}{\norm{\wu}}
        //      \pfrac{\wux}{\xmx} \pfrac{\wux}{\xnx}
        //      \left( \I - \whatu \whatu^T \right) $

        _d2Cu_dxmdxn[ m ][ n ] = _d2Cu_dxmdxn[ n ][ m ] =
            ( fc._alpha * cv._wuim * cv._dwux_dxmx[ m ] * cv._dwux_dxmx[ n ] )
            * ( GeMatrix3::identity() -
                GeMatrix3::outerProduct( cv._whatu, cv._whatu ) );

        // $ \pfractwo{\Cv}{\xm}{\xn} =
        //      \frac{\alpha}{\norm{\wv}}
        //      \pfrac{\wvx}{\xmx} \pfrac{\wvx}{\xnx}
        //      \left( \I - \whatv \whatv^T \right) $

        _d2Cv_dxmdxn[ m ][ n ] = _d2Cv_dxmdxn[ n ][ m ] =
            ( fc._alpha * cv._wvim * cv._dwvx_dxmx[ m ] * cv._dwvx_dxmx[ n ] )
            * ( GeMatrix3::identity() -
                GeMatrix3::outerProduct( cv._whatv, cv._whatv ) );

    }
}

//------------------------------------------------------------------------------

void SimSimulator::StretchVars::debugOutput( std::ostream& os )
{
    UInt32 m,n;
    os << "C = ( " << _Cu << ", " << _Cv << " ) " << std::endl;
    for ( m = 0; m < 3; ++m ) {
        os << "dCu_dxm[ " << m << " ] = " << _dCu_dxm[ m ] << std::endl;
        os << "dCv_dxm[ " << m << " ] = " << _dCv_dxm[ m ] << std::endl;
    }
    os << "dCu_dt = " << _dCu_dt << std::endl;
    os << "dCv_dt = " << _dCv_dt << std::endl;
    for ( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        os << "d2Cu_dxmdxn[ " << m << "," << n << " ] = "
            << _d2Cu_dxmdxn[ m ][ n ] << std::endl;
        os << "d2Cv_dxmdxn[ " << m << "," << n << " ] = "
            << _d2Cv_dxmdxn[ m ][ n ] << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::ShearVars

//------------------------------------------------------------------------------

void SimSimulator::ShearVars::calc(
    const SimSimulator::FaceConsts& fc,
    const SimSimulator::CommonVars& cv,
    const GeVector v0[ 3 ]
) {
    UInt32 m,n,s;

    // As per [BarWit98] section 4.3
    // $ C = \alpha \wu \cdot \wv $
#if DO_NORM
    // Correction: use normalised wu, wv
    _C = fc._alpha * cv._whatu.dot( cv._whatv );
#else
    _C = fc._alpha * cv._wu.dot( cv._wv );
#endif

    // $ \pfrac{C}{\xms} = \alpha \left(
    //      \pfrac{\wux}{\xmx} \wvs + \wus \pfrac{\wvx}{\xmx}
    //   \right) $
    for ( m = 0; m < 3; ++m ) {
        for ( s = 0; s < 3; ++s ) {
            _dC_dxm[ m ][ s ] = fc._alpha * (
#if DO_NORM
                cv._dwhatu_dxm[m][s].dot( cv._whatv ) +
                cv._whatu.dot( cv._dwhatv_dxm[m][s] )
#else
                cv._dwux_dxmx[m] * cv._wv[s] +
                cv._wu[s] * cv._dwvx_dxmx[m]
#endif
            );
        }
    }

    // As per [BarWit98] equation just before (11) - but corrected to be
    // a sum.
    // $ \pfrac{\bf C}{t} = \sum_m
    //      \left( \pfrac{\bf C}{\xm} \right)^T \pfrac{\xm}{t} $
    _dC_dt = 0;
    for( m = 0; m < 3; ++m ) {
        _dC_dt += _dC_dxm[ m ].dot( v0[ m ] );
    }

    for ( m = 0; m < 3; ++m ) for ( n = m; n < 3; ++n ) {
#if DO_NORM
        // $ \pfractwo{C}{\xms}{\xnt} = \alpha \left(
        //      \pfractwo{\whatu}{\xms}{\xnt} \cdot \whatv +
        //      \pfrac{\whatu}{\xms} \cdot \pfrac{\whatv}{\xnt} +
        //      \pfrac{\whatu}{\xnt} \cdot \pfrac{\whatv}{\xms} +
        //      \whatu \cdot \pfractwo{\whatu}{\xms}{\xnt} \right) $
        for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
            _d2C_dxmdxn[m][n]( t, s ) = fc._alpha * (
                cv._d2whatu_dxmdxn[m][n][s][t].dot( cv._whatv ) +
                cv._dwhatu_dxm[m][s].dot( cv._dwhatv_dxm[n][t] ) +
                cv._dwhatu_dxm[n][t].dot( cv._dwhatv_dxm[m][s] ) +
                cv._whatu.dot( cv._d2whatv_dxmdxn[m][n][s][t] )
            );
        }
#else
        // $ \pfractwo{C}{\xms}{\xnt} = \alpha \left(
        //      \pfractwo{\wu}{\xms}{\xnt} \cdot \wv + 
        //      \pfrac{\wu}{\xms} \cdot \pfrac{\wv}{\xnt} +
        //      \pfrac{\wu}{\xnt} \cdot \pfrac{\wv}{\xms} +
        //      \wu \cdot \pfractwo{\wu}{\xms}{\xnt} \right) $
        _d2C_dxmdxn[m][n] = _d2C_dxmdxn[n][m] =
            fc._alpha * (
                cv._dwux_dxmx[m] * cv._dwvx_dxmx[n] +
                cv._dwux_dxmx[n] * cv._dwvx_dxmx[m]
            );
#endif
    }
}

//------------------------------------------------------------------------------

void SimSimulator::ShearVars::debugOutput( std::ostream& os )
{
    UInt32 m,n;

    os << "C = " << _C << std::endl;
    for ( m = 0; m < 3; ++m ) {
        os << "dC_dxm[ " << m << " ] = " << _dC_dxm[ m ] << std::endl;;
    }
    os << "dC_dt = " << _dC_dt << std::endl;;
    for ( m = 0; m < 3; ++m ) for ( n = 0; n < 3; ++n ) {
        os << "d2C_dxmdxn[ " << m << "," << n << " ] = "
            << _d2C_dxmdxn[ m ][ n ] << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::BendVars

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calc(
    const GePoint_4 x,
    const GeVector_4 v
) {
    // My x[0],x[1],x[2],x[3] are equivalent to Macri's ph,pi,pj,pk
    // My nhatA,nhatB are equivalent to Baraff & Witkin's n1,n2
    // i.e. unit vectors
    // My nA, nB are unscaled versions of nhatA,nhatB
    // _nAim = 1 / || nA ||      "nA inverse magnitude"
    
    // nhatA, nhatB, nAim, and nBim must be set before calling this method.
    DGFX_ASSERT( _nhatA.isUnit() );
    DGFX_ASSERT( _nhatB.isUnit() );

    // Calculate edge vector, mag
    calcE( x );

    mainCalc( x, v );
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calc(
    const GePoint_4 x,
    const GeVector_4 v,
    const BendVars& bv
) {
    calcN( x );
    calcE( x );
#if !DO_NORM
    // Enforce the assumption that the normals have constant magnitude - don't
    // scale by 1/||nA||, instead scale by the inverse of the *old* normal's
    // magnitude.
    _nhatA *= bv._nAim / _nAim;
    _nAim = bv._nAim;
    _nhatB *= bv._nBim / _nBim;
    _nBim = bv._nBim;
    _ehat *= bv._eim / _eim;
    _eim = bv._eim;
#endif
    mainCalc( x, v );
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::mainCalc(
    const GePoint_4 x,
    const GeVector_4 v0
) {
    UInt32 m;

    calcThetas();
    calcQs( x );
    calcNEderiv();
    calcD2NE();
    calcCosSinDeriv();
    calcD2cossinth();
    calcCderivs();

    // As per [BarWit98] equation just before (11) - but corrected to be
    // a sum.
    // $ \pfrac{\bf C}{t} = \sum_m
    //      \left( \pfrac{\bf C}{\xm} \right)^T \pfrac{\xm}{t} $
    _dC_dt = 0;
    for( m = 0; m < 4; ++m ) {
        _dC_dt += _dC_dxm[ m ].dot( v0[ m ] );
    }
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcN( const GePoint_4 x )
{
    // $ \n^A = (\x_2 - \x_0) \cross (\x_1 - \x_0) $
    const GeVector nA( (x[2] - x[0]).cross( x[1] - x[0] ) );
    // nA inverse magnitude
    _nAim = 1 / nA.length();
    // $ \nhat^A = \frac{\n^A}{\norm{\n^A}} $
    _nhatA = _nAim * nA;

    // $ \n^B = (\x_1 - \x_3) \cross (\x_2 - \x_3) $
    const GeVector nB( (x[1] - x[3]).cross( x[2] - x[3] ) );
    _nBim = 1 / nB.length();
    // $ \nhat^B = \frac{\n^B}{\norm{\n^B}} $
    _nhatB = _nBim * nB;
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcE( const GePoint_4 x )
{
    // $ \e = \x_1 - \x_2 $
    const GeVector e( x[1] - x[2] );
    _eim = 1 / e.length();
    // $ \ehat = \frac{\e}{\norm{\e}} $
    _ehat = _eim * e;
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcThetas()
{
    // $ \cos \theta = \nhat^A \cdot \nhat^B $
    _costh = _nhatA.dot( _nhatB );
    // $ \sin \theta = (\nhat^A \cross \nhat^B) \cdot \ehat $
    _sinth = _nhatA.cross( _nhatB ).dot( _ehat );
    // $ C = \theta = \arctan \frac{\sin \theta}{\cos \theta} $
    // In range [-pi,+pi] - Macri uses abs of this!
    _C = BaMath::arctan2( _sinth, _costh );
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcQs( const GePoint_4 x )
{
    // $ \pfrac{\n^A}{\xms} = S_s(\q^A_m) $
    // $ \q^A = \{ \x_2 - \x_1, \x_0 - \x_2, \x_1 - \x_0, {\bf 0} \} $
    _qA[ 0 ] = x[2] - x[1];
    _qA[ 1 ] = x[0] - x[2];
    _qA[ 2 ] = x[1] - x[0];
    _qA[ 3 ] = GeVector::zero();

    // $ \pfrac{\n^B}{\xms} = S_s(\q^B_m) $
    // $ \q^B = \{ {\bf 0}, \x_2 - \x_3, \x_3 - \x_1, \x_1 - \x_2 \} $
    _qB[ 0 ] = GeVector::zero();
    _qB[ 1 ] = x[2] - x[3];
    _qB[ 2 ] = x[3] - x[1];
    _qB[ 3 ] = x[1] - x[2];

    // $ \pfrac{\ehat}{\xms} = q^e_m \Is $
    // $ q^e = \{ 0, 1, -1, 0 \} $
    _qe[ 0 ] = 0;
    _qe[ 1 ] = 1;
    _qe[ 2 ] = -1;
    _qe[ 3 ] = 0;
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcNEderiv()
{
    // $ \pfrac{\n^A}{\xms} = S_s(\q^A_m) $
    // $ \pfrac{\n^B}{\xms} = S_s(\q^B_m) $
    // $ \pfrac{\e}{\xms} = q^e_m \Is $
#if DO_NORM
    const GeMatrix3 InAnAt =
        GeMatrix3::identity() - GeMatrix3::outerProduct( _nhatA, _nhatA );
    const GeMatrix3 InBnBt =
        GeMatrix3::identity() - GeMatrix3::outerProduct( _nhatB, _nhatB );
    const GeMatrix3 Ieet =
        GeMatrix3::identity() - GeMatrix3::outerProduct( _ehat, _ehat );

    for ( UInt32 m = 0; m < 4; ++m ) {
        for ( UInt32 s = 0; s < 3; ++s ) {
            // $ \pfrac{\nhat^A}{\xms} = \left(\I - \nhat^A(\nhat^A)^T\right)
            //      \frac{ S_s( \q^A_m ) }{ \norm{\n^A} } $
            _dnhatA_dxm[ m ][ s ] = InAnAt * makeSkewRow( _qA[ m ], s ) * _nAim;
            // $ \pfrac{\nhat^B}{\xms} = \left(\I - \nhat^B(\nhat^B)^T\right)
            //      \frac{ S_s( \q^B_m ) }{ \norm{\n^B} } $
            _dnhatB_dxm[ m ][ s ] = InBnBt * makeSkewRow( qB[ m ], s ) * _nBim;
            // $ \pfrac{\ehat}{\xms} = (\I - \ehat\ehat^T)
            //      \frac{ q^e_m \Is }{ \norm{\e} } $
            _dehat_dxm[ m ][ s ] = Ieet * GeVector::axis( s ) * _qe[ m ] * _eim;
        }
    }
#else
    for ( UInt32 m = 0; m < 4; ++m ) {
        for ( UInt32 s = 0; s < 3; ++s ) {
            // $ \pfrac{\nhat^A}{\xms} =
            //      \frac{1}{\norm{\n^A}} \pfrac{\n^A}{\xms} $
            _dnhatA_dxm[ m ][ s ] = makeSkewRow( _qA[ m ], s ) * _nAim;
            // $ \pfrac{\nhat^B}{\xms} =
            //      \frac{1}{\norm{\n^B}} \pfrac{\n^B}{\xms} $
            _dnhatB_dxm[ m ][ s ] = makeSkewRow( _qB[ m ], s ) * _nAim;
            // $ \pfrac{\ehat}{\xms} =
            //      \frac{1}{\norm{\e}} \pfrac{\e}{\xms} $
            _dehat_dxm[ m ][ s ] = GeVector::axis( s ) * _qe[ m ] * _eim;
        }
    }
#endif
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcD2NE()
{
    // Row represents m, column represents n
    static const Int32 D1[4][4] = {
        { 0, -1, 1, 0 },
        { 1, 0, -1, 0 },
        { -1, 1, 0, 0 },
        { 0, 0, 0, 0 }
    };
    static const Int32 D2[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 1, -1 },
        { 0, -1, 0, 1 },
        { 0, 1, -1, 0 }
    };
    UInt32 m,n,s,t;
#if DO_NORM
    GeVector_443 dqAm_dxn, dqBm_dxn;
    for( UInt32 m = 0; m < 4; ++m ) {
        for( UInt32 n = 0; n < 4; ++n ) {
            for( UInt32 t = 0; t < 3; ++t ) {
                dqAm_dxn[m][n][t] = GeVector::zero();
                dqAm_dxn[m][n][t][t] = D1[m][n];
                dqBm_dxn[m][n][t] = GeVector::zero();
                dqBm_dxn[m][n][t][t] = D2[m][n];
            }
        }
    }
    for ( m = 0; m < 4; ++m ) for ( n = 0; n < 4; ++n ) {
        for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
            GeMatrix3 tempm;
            GeVector tempv;

            // $ \pfractwo{\nhat^A}{\xms}{\xnt} =
            // - \left(
            //      \pfrac{\nhat^A}{\xnt} (\nhat^A)^T +
            //      \nhat^A \left( \pfrac{\nhat^A}{\xnt} \right)^T
            //   \right) \frac{S_s(\q^A_m)}{\norm{\n^A}} +
            //   \frac{\I - \nhat^A(\nhat^A)^T}{\norm{\n^A}^2}
            //   \left(
            //      \norm{\n^A} S_s\left( \pfrac{\q^A_m}{\xnt} \right) -
            //      \left( S_t(\q^A_n) \cdot \nhat^A \right) S_s(\q^A_m)
            //   \right) $

            GeVector SqAm = makeSkewRow( _qA[ m ], s );
            GeVector SqAn = makeSkewRow( _qA[ n ], t );
            tempm = GeMatrix3::outerProduct( _dnhatA_dxm[ n ][ t ], _nhatA );
            tempm += GeMatrix3::outerProduct( _nhatA, _dnhatA_dxm[ n ][ t ] );
            _d2nA_dxmdxn[m][n][s][t] = tempm * SqAm * -_nAim;
            tempm = (
                GeMatrix3::identity()
                - GeMatrix3::outerProduct( _nhatA, _nhatA )
            ) * (_nAim * _nAim);
            tempv = makeSkewRow( _dqAm_dxn[ m ][ n ][ t ], s ) / _nAim;
            tempv -= SqAn.dot( _nhatA ) * SqAm;
            _d2nA_dxmdxn[m][n][s][t] += tempm * tempv;

            // $ \pfractwo{\nhat^B}{\xms}{\xnt} =
            // - \left(
            //      \pfrac{\nhat^B}{\xnt} (\nhat^B)^T +
            //      \nhat^B \left( \pfrac{\nhat^B}{\xnt} \right)^T
            //   \right) \frac{S_s(\q^B_m)}{\norm{\n^B}} +
            //   \frac{\I - \nhat^B(\nhat^B)^T}{\norm{\n^B}^2}
            //   \left(
            //      \norm{\n^B} S_s\left( \pfrac{\q^B_m}{\xnt} \right) -
            //      \left( S_t(\q^B_n) \cdot \nhat^B \right) S_s(\q^B_m)
            //   \right) $

            GeVector SqBm = makeSkewRow( qB[ m ], s );
            GeVector SqBn = makeSkewRow( qB[ n ], t );
            tempm = GeMatrix3::outerProduct( _dnhatB_dxm[ n ][ t ], _nhatB );
            tempm += GeMatrix3::outerProduct( _nhatB, _dnhatB_dxm[ n ][ t ] );
            _d2nB_dxmdxn[m][n][s][t] = tempm * SqBm * -_nBim;
            tempm = (
                GeMatrix3::identity()
                - GeMatrix3::outerProduct( _nhatB, _nhatB )
            ) * (_nBim * _nBim);
            tempv = makeSkewRow( _dqBm_dxn[ m ][ n ][ t ], s ) / _nBim;
            tempv -= SqBn.dot( _nhatB ) * SqBm;
            _d2nB_dxmdxn[m][n][s][t] += tempm * tempv;

            // $ \pfractwo{\ehat}{\xms}{\xnt} =
            // - \left(
            //      \pfrac{\ehat}{\xnt} \ehat^T +
            //      \ehat \left( \pfrac{\ehat}{\xnt} \right)^T
            //   \right) \frac{S_s(q^e_m)}{\norm{\e}} -
            //   \frac{\I - \ehat\ehat^T}{\norm{\e}^2}
            //      q^e_m q^e_n \ehat_t \Is $

            tempm = GeMatrix3::outerProduct( _dehat_dxm[ n ][ t ], _ehat );
            tempm += GeMatrix3::outerProduct( _ehat, _dehat_dxm[ n ][ t ] );
            _d2e_dxmdxn[m][n][s][t] = tempm * GeVector::axis( s ) *
                (-_qe[ m ] * _eim);
            tempm =
                GeMatrix3::identity() - GeMatrix3::outerProduct( _ehat, _ehat );
            tempv = _eim * _eim * _qe[ n ] * _qe[ m ] * _ehat[ t ] *
                GeVector::axis( s );
            _d2e_dxmdxn[m][n][s][t] -= tempm * tempv;
        }
    }
#else
    for ( m = 0; m < 4; ++m ) {
        // By symmetry, d2nA_dxmsdxnt = d2nA_dxntdxms. Symmetric cases are
        // handled here.
        for ( n = 0; n < m; ++n ) {
            GeVector_33& d2nA_dxmdxn = _d2nA_dxmdxn[m][n];
            GeVector_33& d2nB_dxmdxn = _d2nB_dxmdxn[m][n];
#if COMPILER_MSVC
            // Visual C++ can't handle the cast to const.
            GeVector_33& d2nA_dxndxm = _d2nA_dxmdxn[n][m];
            GeVector_33& d2nB_dxndxm = _d2nB_dxmdxn[n][m];
#else
            const GeVector_33& d2nA_dxndxm = _d2nA_dxmdxn[n][m];
            const GeVector_33& d2nB_dxndxm = _d2nB_dxmdxn[n][m];
#endif
            for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
                d2nA_dxmdxn[s][t] = d2nA_dxndxm[t][s];
                d2nB_dxmdxn[s][t] = d2nB_dxndxm[t][s];
            }
        }
        for ( n = m; n < 4; ++n ) {
            GeVector_33& d2nA_dxmdxn = _d2nA_dxmdxn[m][n];
            GeVector_33& d2nB_dxmdxn = _d2nB_dxmdxn[m][n];
            for ( t = 0; t < 3; ++t ) {
                GeVector dqAm_dxn_t( GeVector::zero() );
                dqAm_dxn_t[t] = D1[m][n] * _nAim;
                GeVector dqBm_dxn_t( GeVector::zero() );
                dqBm_dxn_t[t] = D2[m][n] * _nBim;
                for ( s = 0; s < 3; ++s ) {
                    // $ \pfractwo{\n^A}{\xms}{\xnt} =
                    //      S_s\left(\pfrac{\q^A_m}{\xnt}\right) $
                    d2nA_dxmdxn[s][t] = makeSkewRow( dqAm_dxn_t, s );
                    // $ \pfractwo{\n^B}{\xms}{\xnt} =
                    //      S_s\left(\pfrac{\q^B_m}{\xnt}\right) $
                    d2nB_dxmdxn[s][t] = makeSkewRow( dqBm_dxn_t, s );
                    // $ \pfractwo{\e}{\xms}{\xnt} = {\bf 0} $
                }
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcCosSinDeriv()
{
    UInt32 m,s;

    // $ \pfrac{ \cos \theta }{\xms} =
    //   \pfrac{ \nhat^A \cdot \nhat^B }{\xms} =
    //      \pfrac{\nhat^A}{\xms} \cdot \nhat^B +
    //      \nhat^A \cdot \pfrac{\nhat^B}{\xms} $
    for( m = 0; m < 4; ++m ) {
        for ( s = 0; s < 3; ++s ) {
            _dcosth_dxm[m][s] =
                _dnhatA_dxm[m][s].dot( _nhatB ) +
                _nhatA.dot( _dnhatB_dxm[m][s] );
        }
    }

    // $ \pfrac{ \sin \theta }{\xms} =
    //   \pfrac{ (\nhat^A \cross \nhat^B) \cdot \ehat}{\xms} =
    //      \left(
    //          \pfrac{\nhat^A}{\xms} \cross \nhat^B +
    //          \nhat^A \cross \pfrac{\nhat^B}{\xms}
    //      \right) \cdot \ehat +
    //      (\nhat^A \cross \nhat^B) \cdot \pfrac{\ehat}{\xms} $
    GeVector nhatAxnhatB( _nhatA.cross( _nhatB ) );
    for( m = 0; m < 4; ++m ) {
        for ( s = 0; s < 3; ++s ) {
            _dsinth_dxm[m][s] =
                (
                    _dnhatA_dxm[m][s].cross( _nhatB ) +
                    _nhatA.cross( _dnhatB_dxm[m][s] )
                ).dot( _ehat ) + nhatAxnhatB.dot( _dehat_dxm[m][s] );
        }
    }
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcD2cossinth()
{
    UInt32 m,n,s,t;

    for( m = 0; m < 4; ++m ) {
        for( n = 0; n < m; ++n ) {
            // By symmetry, d2costh_dxmsdxnt = d2costh_dxntdxms. Symmetric
            // cases are handled here.
            _d2costh_dxmdxn[m][n] = _d2costh_dxmdxn[n][m].getTranspose();
            _d2sinth_dxmdxn[m][n] = _d2sinth_dxmdxn[n][m].getTranspose();
        }
        for( n = m; n < 4; ++n ) {
            const GeVector_3& dnhatA_dxm = _dnhatA_dxm[m];
            const GeVector_3& dnhatB_dxm = _dnhatB_dxm[m];
            const GeVector_3& dehat_dxm = _dehat_dxm[m];
            GeMatrix3& d2costh_dxmdxn = _d2costh_dxmdxn[m][n];
            GeMatrix3& d2sinth_dxmdxn = _d2sinth_dxmdxn[m][n];
#if COMPILER_MSVC
            // Visual C++ can't handle cast to const.
            GeVector_33& d2nA_dxmdxn = _d2nA_dxmdxn[m][n];
            GeVector_33& d2nB_dxmdxn = _d2nB_dxmdxn[m][n];
#else
            const GeVector_33& d2nA_dxmdxn = _d2nA_dxmdxn[m][n];
            const GeVector_33& d2nB_dxmdxn = _d2nB_dxmdxn[m][n];
#endif
            const GeVector_3& dnhatA_dxn = _dnhatA_dxm[n];
            const GeVector_3& dnhatB_dxn = _dnhatB_dxm[n];
            const GeVector_3& dehat_dxn = _dehat_dxm[n];
            for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
                const GeVector& dnhatA_dxm_s = dnhatA_dxm[s];
                const GeVector& dnhatB_dxm_s = dnhatB_dxm[s];
                const GeVector& d2nA_dxmdxn_st = d2nA_dxmdxn[s][t];
                const GeVector& d2nB_dxmdxn_st = d2nB_dxmdxn[s][t];
                const GeVector& dnhatA_dxn_t = dnhatA_dxn[t];
                const GeVector& dnhatB_dxn_t = dnhatB_dxn[t];
                // $ \pfractwo{\cos \theta}{\xms}{\xnt} =
                //      \pfractwo{\nhat^A}{\xms}{\xnt} \cdot \nhat^B + 
                //      \pfrac{\nhat^B}{\xnt} \cdot \pfrac{\nhat^A}{\xms} +
                //      \pfrac{\nhat^A}{\xnt} \cdot \pfrac{\nhat^B}{\xms} + 
                //      \nhat^A \cdot \pfractwo{\nhat^B}{\xms}{\xnt} $
                d2costh_dxmdxn(t,s) =
                    d2nA_dxmdxn_st.dot( _nhatB ) +
                    dnhatA_dxm_s.dot( dnhatB_dxn_t ) +
                    dnhatA_dxn_t.dot( dnhatB_dxm_s ) +
                    d2nB_dxmdxn_st.dot( _nhatA );

                // $ \pfractwo{\sin \theta}{\xms}{\xnt} =
                //     \left(
                //         \pfractwo{\nhat^A}{\xms}{\xnt} \cross \nhat^B +
                //         \pfrac{\nhat^A}{\xms} \cross \pfrac{\nhat^B}{\xnt} +
                //         \pfrac{\nhat^A}{\xnt} \cross \pfrac{\nhat^B}{\xms} +
                //         \nhat^A \cross \pfractwo{\nhat^B}{\xms}{\xnt}
                //     \right) \cdot \ehat +
                //     \left(
                //         \pfrac{\nhat^A}{\xms} \cross \nhat^B +
                //         \nhat^A \cross \pfrac{\nhat^B}{\xms}
                //     \right) \cdot \pfrac{\ehat}{\xnt} +
                //     \left(
                //         \pfrac{\nhat^A}{\xnt} \cross \nhat^B +
                //         \nhat^A \cross \pfrac{\nhat^B}{\xnt}
                //     \right) \cdot \pfrac{\ehat}{\xms} +
                //     (\nhat^A \cross \nhat^B) \pfractwo{\ehat}{\xms}{\xnt} $
                d2sinth_dxmdxn( t, s ) = (
                    d2nA_dxmdxn_st.cross( _nhatB ) +
                    dnhatA_dxm_s.cross( dnhatB_dxn_t ) +
                    dnhatA_dxn_t.cross( dnhatB_dxm_s ) +
                    _nhatA.cross( d2nB_dxmdxn_st )
                ).dot( _ehat ) + (
                    dnhatA_dxm_s.cross( _nhatB ) + _nhatA.cross( dnhatB_dxm_s )
                ).dot( dehat_dxn[t] ) + (
                    dnhatA_dxn_t.cross( _nhatB ) + _nhatA.cross( dnhatB_dxn_t )
                ).dot( dehat_dxm[s] );
#if DO_NORM
                d2sinth_dxmdxn( t, s ) +=
                    _nhatA.cross( _nhatB ).dot( _d2e_dxmdxn[m][n][s][t] );
#endif
            }
        }
    }
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::calcCderivs()
{
    UInt32 m,n,s,t;

    // $ \pfrac{C}{\xms} = \pfrac{\theta}{\xms} =
    //      \cos \theta \pfrac{\sin \theta}{\xms} -
    //      \sin \theta \pfrac{\cos \theta}{\xms} $
    for ( m = 0; m < 4; ++m ) {
        _dC_dxm[ m ] = _costh * _dsinth_dxm[ m ] - _sinth * _dcosth_dxm[ m ];
    }
    // $ \pfractwo{C}{\xms}{\xnt} = \pfractwo{\theta}{\xms}{\xnt} =
    //      \pfrac{\cos \theta}{\xnt}\pfrac{\sin \theta}{\xms} +
    //      \cos \theta \pfractwo{\sin \theta}{\xms}{\xnt} -
    //      \pfrac{\sin \theta}{\xnt} \pfrac{\cos \theta}{\xms} -
    //      \sin \theta \pfractwo{\cos \theta}{\xms}{\xnt} $
    for ( m = 0; m < 4; ++m ) {
        // FIXME: For some reason, this is a sum of a symmetric (a) and
        // skew-symmetric (b) matrix. Why?
        for ( n = m; n < 4; ++n ) {
            GeMatrix3& d2C_dxmdxn = _d2C_dxmdxn[m][n];
            GeMatrix3& d2C_dxndxm = _d2C_dxmdxn[n][m];
            const GeMatrix3& d2sinth_dxmdxn = _d2sinth_dxmdxn[m][n];
            const GeMatrix3& d2costh_dxmdxn = _d2costh_dxmdxn[m][n];
            for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
                Float a =
                    _costh * d2sinth_dxmdxn(t,s)
                    - _sinth * d2costh_dxmdxn(t,s);
                Float b =
                    _dcosth_dxm[n][t] * _dsinth_dxm[m][s]
                    - _dsinth_dxm[n][t] * _dcosth_dxm[m][s];
                d2C_dxmdxn(t,s) = a + b;
                d2C_dxndxm(s,t) = a - b;
            }
        }
    }
}

//------------------------------------------------------------------------------

void SimSimulator::BendVars::debugOutput( std::ostream& os )
{
    UInt32 m,n,s,t;

    os << "nhatA = " << _nhatA << "  _nAim = " << _nAim << std::endl;
    os << "nhatB = " << _nhatB << "  _nBim = " << _nBim << std::endl;
    os << "ehat = " << _ehat << "  _eim = " << _eim << std::endl;

    os  << "cos(theta) = " << _costh << "  sin(theta) = " << _sinth
        << "  C = theta = " << _C << " radians ("
        << _C * 180 / M_PI << " degrees)"
        << std::endl;

    for( m = 0; m < 4; ++m ) {
        for( s = 0; s < 3; ++s ) {
            os << "dnhatA_dxm[ " << m << " ][ " << s << " ] = "
                << _dnhatA_dxm[ m ][ s ] << std::endl;
            os << "dnhatB_dxm[ " << m << " ][ " << s << " ] = "
                << _dnhatB_dxm[ m ][ s ] << std::endl;
            os << "dehat_dxm[ " << m << " ][ " << s << " ] = "
                << _dehat_dxm[ m ][ s ] << std::endl;
        }
    }

    for ( m = 0; m < 4; ++m ) for ( n = 0; n < 4; ++n ) {
        for ( s = 0; s < 3; ++s ) for ( t = 0; t < 3; ++t ) {
            os << "d2nA_dxmdxn[ " << m << " ][ " << n << " ][ "
                << s << " ][ " << t << " ] = " << _d2nA_dxmdxn[m][n][s][t]
                << std::endl;
            os << "d2nB_dxmdxn[ " << m << " ][ " << n << " ][ "
                << s << " ][ " << t << " ] = " << _d2nB_dxmdxn[m][n][s][t]
                << std::endl;
#if DO_NORM
            os << "d2e_dxmdxn[ " << m << " ][ " << n << " ][ "
                << s << " ][ " << t << " ] = " << _d2e_dxmdxn[m][n][s][t]
                << std::endl;
#endif
        }
    }

    for ( m = 0; m < 4; ++m ) {
        os << "dcosth_dxm[ " << m << " ] = " << _dcosth_dxm[ m ] << std::endl;
        os << "dsinth_dxm[ " << m << " ] = " << _dsinth_dxm[ m ] << std::endl;
    }

    for( m = 0; m < 4; ++m ) for( n = 0; n < 4; ++n ) {
        os << "d2costh_dxmdxn[ " << m << " ][ " << n << " ] = "
            << _d2costh_dxmdxn[ m ][ n ] << std::endl;
        os << "d2sinth_dxmdxn[ " << m << " ][ " << n << " ] = "
            << _d2sinth_dxmdxn[ m ][ n ] << std::endl;
    }

    for ( m = 0; m < 4; ++m ) {
        os << "dC_dxm[ " << m << " ] = " << _dC_dxm[ m ] << std::endl;;
    }
    os << "dC_dt = " << _dC_dt << std::endl;
    for ( m = 0; m < 4; ++m ) for ( n = 0; n < 4; ++n ) {
        os << "d2C_dxmdxn[ " << m << "," << n << " ] = "
            << _d2C_dxmdxn[ m ][ n ] << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimSimulator::ModPCGSolver
//

//------------------------------------------------------------------------------

SimSimulator::ModPCGSolver::ModPCGSolver()
 : _tolerance( 1e-2f )
{
}

//------------------------------------------------------------------------------

bool SimSimulator::ModPCGSolver::done() const
{
    return _done;
}

//------------------------------------------------------------------------------

const SimVector& SimSimulator::ModPCGSolver::result() const
{
    return _x;
}

//------------------------------------------------------------------------------

UInt32 SimSimulator::ModPCGSolver::getNbSteps() const
{
    return _nbSteps;
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::setTolerance( Float tolerance )
{
    _tolerance = tolerance;
}

//------------------------------------------------------------------------------

Float SimSimulator::ModPCGSolver::getTolerance() const
{
    return _tolerance;
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::preStep()
{
    setupPreconditioner();
    setupCG();
    _nbSteps = 0;
    _done = false;
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::setupPreconditioner()
{
    // FIXME: this assertion is failing... why?
    //DGFX_ASSERT( isSymmetric( _A ) );

    UInt32 N = _A.nbRows();
    if ( _P.nbRows() != _A.nbRows() ) {
        _P = SimMatrix( N, N );
        _Pinv = SimMatrix( N, N );

    }
    for ( UInt32 i = 0; i < N; ++i ) {
        const GeMatrix3& a = _A( i, i );
        GeMatrix3& p = _P( i, i );
        GeMatrix3& pinv = _Pinv( i, i );
        p = GeMatrix3::zero();
        pinv = GeMatrix3::zero();
        for ( UInt32 j = 0; j < 3; ++j ) {
            DGFX_ASSERT( !BaMath::isEqual( a( j, j ), 0, 10e-12f ) );
            // This is the inverse of the [BarWit98]'s recommendation...
            // but makes things work much better. See [AscBox03].
            p( j, j ) = a( j, j );
            pinv( j, j ) = 1 / a( j, j );
        }
    }
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::setupCG()
{
    _x = _z;
    if ( DO_ASCHER_BOXERMAN ) {
        filterCompInPlace( _x );
        _bhat = - ( _A * _x );
        _bhat += _b;
    }
    else {
        _bhat = _b;
    }
    filterInPlace( _bhat );
    _delta0 = ( _P * _bhat ).dot( _bhat );
    if ( DO_ASCHER_BOXERMAN ) {
        _x += filter( _y );
    }
    _r = filter( _b - _A * _x );
    _c = filter( _Pinv * _r );
    _deltaNew = _r.dot( _c );
}
    
//------------------------------------------------------------------------------

SimVector SimSimulator::ModPCGSolver::filter( const SimVector& a ) const
{
    DGFX_ASSERT( _S.size() == a.size() );
    SimVector result( a.size() );
    std::vector<GeMatrix3>::const_iterator Si = _S.begin();
    SimVector::const_iterator ai = a.begin();
    SimVector::iterator ri = result.begin();
    for( ; Si != _S.end(); ++Si, ++ai, ++ri ) {
        *ri = *Si * *ai;
    }
    return result;
}

//------------------------------------------------------------------------------

SimVector SimSimulator::ModPCGSolver::filterComp( const SimVector& a ) const
{
    DGFX_ASSERT( _S.size() == a.size() );
    SimVector result( a.size() );
    std::vector<GeMatrix3>::const_iterator Si = _S.begin();
    SimVector::const_iterator ai = a.begin();
    SimVector::iterator ri = result.begin();
    for( ; Si != _S.end(); ++Si, ++ai, ++ri ) {
        *ri = (GeMatrix3::identity() - *Si) * *ai;
    }
    return result;
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::filterInPlace( SimVector& a ) const
{
    DGFX_ASSERT( _S.size() == a.size() );
    std::vector<GeMatrix3>::const_iterator Si = _S.begin();
    SimVector::iterator ai = a.begin();
    for( ; Si != _S.end(); ++Si, ++ai ) {
        *ai = *Si * *ai;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::filterCompInPlace( SimVector& a ) const
{
    DGFX_ASSERT( _S.size() == a.size() );
    std::vector<GeMatrix3>::const_iterator Si = _S.begin();
    SimVector::iterator ai = a.begin();
    for( ; Si != _S.end(); ++Si, ++ai ) {
        *ai = (GeMatrix3::identity() - *Si) * *ai;
    }
}

//------------------------------------------------------------------------------

void SimSimulator::ModPCGSolver::step()
{
    if ( DEBUG_PCG ) {
        std::cout << "substep" << std::endl;
    }
    if ( _deltaNew < _tolerance * _tolerance * _delta0 ) {
        _done = true;
        return;
    }

    SimMatrix::multiply( _q, _A, _c );
    filterInPlace( _q );
    Float alpha = _deltaNew / _c.dot( _q );
    _x.plusEqualsScaled( alpha, _c );
    _r.plusEqualsScaled( -alpha, _q );

    SimMatrix::multiply( _s, _Pinv, _r );
    Float _deltaOld = _deltaNew;
    _deltaNew = _r.dot( _s );
    _c *= _deltaNew / _deltaOld;
    _c += _s;
    filterInPlace( _c );
    ++_nbSteps;
}

FREECLOTH_NAMESPACE_END
