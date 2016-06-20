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

#ifndef freecloth_sim_simSimulator_h
#define freecloth_sim_simSimulator_h

#ifndef freecloth_sim_package_h
#include <freecloth/simulator/package.h>
#endif

#ifndef freecloth_sim_simMatrix_h
#include <freecloth/simulator/simMatrix.h>
#endif

#ifndef freecloth_sim_simVector_h
#include <freecloth/simulator/simVector.h>
#endif

#ifndef freecloth_base_algorithm
#include <freecloth/base/algorithm>
#endif

#ifndef freecloth_base_list
#include <freecloth/base/list>
#endif

#ifndef freecloth_base_baTime_h
#include <freecloth/base/baTime.h>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_geom_geMesh_h
#include <freecloth/geom/geMesh.h>
#endif

#ifndef freecloth_geom_geMeshWingedEdge_h
#include <freecloth/geom/geMeshWingedEdge.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GeMatrix3;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class SimSimulator freecloth/simulator/simSimulator.h
 * \brief An implementation of Baraff & Witkin's cloth simulation algorithm.
 * 
 * Using the simulation is fairly straightforward. The client supplies an
 * initial mesh, which must have disc topology. The client also specifies the
 * density of the cloth, which indirectly defines the mass of each triangle of
 * the mesh. The client can also specify constraints on the cloth motion,
 * constraining both the position and the velocity of individual nodes of the
 * mesh.
 *
 * Time starts at zero, and is advanced by a fixed timestep by calls to step().
 * The client can retrieve the mesh after each timestep. The simulation can
 * be rewound to the beginning by calling rewind(). A SimStepStrategy class
 * is recommended for simulator stepping, using either the basic or adaptive
 * algorithm.
 *
 * There are many parameters for the simulation algorithm. See [BarWit98] or
 * the description in SimSimulator::Params for details.
 *
 * References:
 * - [BarWit98] D. Baraff and A. Witkin. Large Steps in Cloth Simulation.
 *    SIGGRAPH Conference Proceedings, 1998, 43-54.
 *    http://www-2.cs.cmu.edu/~baraff/papers/sig98.pdf
 * - [Pri03] D. Pritchard. Implementing Baraff & Witkin's Cloth Simulation.
 *    Unpublished, 2003.
 *    http://freecloth.enigmati.ca/docs/report-chaps/
 * - [Mac00] D. Macri. Real-Time Cloth. Game Developers Conference, 2000.
 *    http://www.gdconf.com/archives/2000/macri.doc
 * - [AscBox03] U. Ascher and E. Boxerman. On the modified conjugate gradient
 *    method in cloth simulation.
 *    http://www.cs.ubc.ca/spider/ascher/papers/ab.pdf
 */

// FIXME: the mesh should be allowed to have cylindrical or spherical
// topology, not just disc topology.
//
// FIXME: should really implement this as a Bridge pattern, to hide the
// implementation from clients better.
class SimSimulator : public RCBase
{
public:
    // ----- classes -----
    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class Params freecloth/simulator/simSimulator.h
     * \brief Parameters for cloth simulator, as described in [BarWit98].
     *
     * The stretch, shear, and bend parameters control the strength of the
     * associated internal forces. For example, a higher _k_stretch value
     * makes the internal stretch forces stronger, and hence makes the cloth
     * harder to stretch. The bend force can be controlled independently in
     * u and v. Generally, stretch forces should be very, very strong,
     * and bend forces should be very weak.
     *
     * The damping parameters control the strength of damping forces. A higher
     * damping value will make the cloth come to rest quicker, while a lower
     * value will allow it to oscillate back and forth before coming to rest.
     *
     * The b_u and b_v values allow the client to force stretch/compression
     * of the cloth. They're not very useful at present; they should really be
     * spatially varying to be useful.
     *
     * The g parameter is the constant for acceleration due to gravity,
     * by default 9.81 m/s^2.
     */
    class Params
    {
    public:
        // ----- member functions -----

        // MSVC bug requires this to be inline
        // Maya values: .5, .35, .01 (unknown scaling...)
        inline Params()
          : _k_stretch( 100.f ),
            _k_shear( 10.f ),
            _k_bend_u( 1e-5f ),
            _k_bend_v( 1e-5f ),
            _k_stretch_damp( 20.f ),
            _k_shear_damp( 2.f ),
            _k_bend_damp( 2e-6f ),
            _k_drag( .1f ),
            _b_u( 1.f ),
            _b_v( 1.f ),
            _g( 9.81f )
        {
        }

        // ----- data members -----
        // TODO: Maya has separate u/v control for this
        Float           _k_stretch;
        Float           _k_shear;
        Float           _k_bend_u, _k_bend_v;
        Float           _k_stretch_damp;
        Float           _k_shear_damp;
        Float           _k_bend_damp;
        //! Drag constant.
        //! Technically, this is 0.5 * C_D * rho, where rho is density of
        //! the medium and C_D is the drag coefficient. We combine this into
        //! a single constant for simplicity.
        Float           _k_drag;
        //! Stretch constant
        // FIXME: should be spatially varying, via texture.
        Float           _b_u,_b_v;
        //! Gravitational acceleration
        Float           _g;
    };

    // ----- types and enumerations -----

    //! The individual classes of forces.
    enum ForceType {
        F_STRETCH,
        F_SHEAR,
        F_BEND,
        F_GRAVITY,
        F_DRAG,

        NB_FORCES
    };

    // ----- member functions -----

    explicit SimSimulator( const GeMesh& initialMesh );

    const GeMesh& getMesh() const;
    const RCShdPtr<GeMesh>& getMeshPtr() const;

    //! Rewind simulation time to zero, and return the mesh to its initial
    //! state.
    void rewind();

    //! Advance the simulation by one timestep.
    //! step() is equivalent to calling
    //! - preSubSteps()
    //! - repeated subStep() calls
    //! - postSubSteps()
    void step();

    //@{
    //! Finer grained simulation control - allows the client more control over
    //! the computation. See step() for details.
    //! A step can be cancelled before subStepsDone() by calling cancelStep().
    bool subStepsDone() const;
    void preSubSteps();
    void subStep();
    void postSubSteps();
    void cancelStep();
    //@}

    bool inStep() const;
    //! Test if the last step succeeded. See getStretchLimit for details.
    bool stepSucceeded() const;

    //! Retrieve the current simulation time.
    BaTime::Instant getTime() const;

    //@{
    //! Mutator
    void setTimestep( BaTime::Duration );
    void setParams( const Params& );
    void setDensity( Float rho );
    void setPCGTolerance( Float );
    void setStretchLimit( Float );
    //@}

    //@{
    //! Accessor
    BaTime::Duration getTimestep() const;
    const Params& getParams() const;
    Float getDensity() const;
    //@}
    //! Accessor. The PCG tolerance determines the accuracy for solving the
    //! non-linear system in each timestep.
    Float getPCGTolerance() const;
    //! Accessor. The stretch limit determines the success/failure of each
    //! time step. If the Cu or Cv value for any triangle (excluding the
    //! alpha term) exceeds the stretch limit, the preceeding timestep is
    //! deemed to have failed.
    Float getStretchLimit() const;

    //! Remove all constraints on all vertices.
    void removeAllConstraints();
    //! Constrain movement of the vertex to the plane defined by the given
    //! normal.
    void setPosConstraintPlane( GeMesh::VertexId, const GeVector& );
    //! Constrain movement of the vertex to the given line.
    void setPosConstraintLine( GeMesh::VertexId, const GeVector& );
    //! Completely constrain the vertex, allowing no movement.
    void setPosConstraintFull( GeMesh::VertexId );

    //! Force the vertex to move with the specified velocity. This overrides
    //! any position constraints.
    void setVelConstraint( GeMesh::VertexId, const GeVector& );

    //@{
    //! Only exposed for debugging purposes. These return the values from the
    //! last successful step: i.e., one step out of date.
    GeVector getVelocity( GeMesh::VertexId vid ) const;
    GeVector getForce( GeMesh::VertexId vid ) const;
    Float getEnergy( ForceType ) const;
    Float getTriEnergy( ForceType, GeMesh::FaceId ) const;
    // The final two will only work in debug builds.
    GeVector getForce( ForceType type, GeMesh::VertexId vid ) const;
    GeVector getDampingForce( ForceType type, GeMesh::VertexId vid ) const;
    //@}

    //! Only exposed for debugging purposes.
    void setMesh( const GeMesh& );

private:

    // ----- types and enumerations -----

    // FIXME: implement custom matrix classes for these some time, if it
    // helps the performance much.
    typedef SimMatrix Matrix;
    typedef SimMatrix SymMatrix;
    typedef SimMatrix TridiagMatrix;

    // ----- classes -----

    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class FaceConsts freecloth/simulator/simSimulator.h
     *
     * Per-face constants, precomputed for efficiency.
     */
    class FaceConsts
    {
    public:
        // ----- member functions -----

        //! Calculate constants, given positions of vertices in unstretched
        //! mesh.
        void calc( const GePoint tp[3] );
        void debugOutput( std::ostream& os );
        
        // ----- data members -----
        
        //! \f$ \delta u_1 \f$ from [BarWit98]
        Float _du1;
        //! \f$ \delta u_2 \f$ from [BarWit98]
        Float _du2;
        //! \f$ \delta v_1 \f$ from [BarWit98]
        Float _dv1;
        //! \f$ \delta v_2 \f$ from [BarWit98]
        Float _dv2;
        //! \f$ \alpha \f$ from [Pri03]
        Float _alpha;

        //! Denominator from several [Pri03] equations
        Float _detInv;
    };

    //@{
    //! Internal class used for calculation of forces
    class CommonVars;
    class StretchVars;
    class ShearVars;
    class BendVars;
    //@}
    
    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class ModPCGSolver freecloth/simulator/simSimulator.h
     *
     * Preconditioned modified conjugate gradient solver. Solves a linear
     * system Ax=b subject to constraints defined by S and z, as defined by
     * [BarWit98] in section 5.3.
     *
     * We use the notation of [BarWit98], but implement the method
     * described by [AscBox03].
     */
    class ModPCGSolver
    {
    public:
        // ----- member functions -----

        ModPCGSolver();
        void preStep();
        void step();
        bool done() const;
        const SimVector& result() const;
        UInt32 getNbSteps() const;

        //! Specify the tolerance.
        void setTolerance( Float );
        //! Access the tolerance
        Float getTolerance() const;

        // ----- data members -----
        
        std::vector<GeMatrix3> _S;
        SimVector       _z;

        //! Final Ax=b system
        SymMatrix       _A;
        SimVector       _b;

        //! Result from last successful step, as per [AscBox03]
        SimVector       _y;

    private:
        // ----- member functions -----

        void setupPreconditioner();
        void setupCG();
        //! [BarWit98]'s filter method (also [AscBox03]'s S filter method)
        SimVector filter( const SimVector& ) const;
        //! [AscBox03]'s (I-S) filter method
        SimVector filterComp( const SimVector& ) const;

        void filterInPlace( SimVector& ) const;
        void filterCompInPlace( SimVector& ) const;

        // ----- data members -----
        
        SimMatrix       _P, _Pinv;
        bool            _done;
        UInt32          _nbSteps;
        Float           _tolerance;

        SimVector       _x, _bhat, _r, _c;
        Float           _delta0, _deltaNew;
        
        // Temporaries within step()
        SimVector       _q, _s;
    };


    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class StepData freecloth/simulator/simSimulator.h
     *
     * Simple utility class to hold important data. This data is updated
     * during each step, and can be queried by the user between steps.
     * It is given its own class to help with adaptive time stepping, where
     * we have to restore all of this data if a timestep fails.
     */
    class StepData
    {
    public:
        // ----- data members -----
        
        //! Current time
        Float           _time;
        SimVector       _f0;
        SimVector       _v0;
        SimVector       _lastDeltaV0;
        //@{
        //! For debugging
        SimVector       _f0i[ NB_FORCES ];
        SimVector       _d0i[ NB_FORCES ];
        //@}
        
        //! Total energy of cloth
        Float           _energy;
        //! Energy of cloth divided up into categories
        Float           _fenergy[ NB_FORCES ];
        //! Kinetic energy of cloth
        Float           _venergy;
        //! Energy of cloth divided up into categories and triangles
        std::vector<Float> _trienergy[ NB_FORCES ];
        std::vector<Float> _Cu,_Cv;
    };


    // ----- member functions -----

    //! Fill the mass matrix (_M) using the initial mesh's triangles areas
    //! and the density parameter, as per [BarWit98] section 2.2.
    void setupMass();

    //! Code executed at the end of postSubSteps(). This does the step setup
    //! (temporary clearing) and stretch/shear calculation. In the normal
    //! scheme of things, this would be the start of preSubSteps(), but we
    //! need to do stretch calculation to test and see if the timestep
    //! succeeded or not, and roll back if necessary.
    void postSubStepsFinale();
    //! Precompute values that don't change over time.
    void calcFaceConsts();
    //! Calculate the stretch and shear conditions.
    void calcStretchShear(
        const GeMesh::FaceWrapper& face
    );
    //! Calculate the stretch condition and its derivatives.
    void calcStretch(
        const GeMesh::FaceWrapper& face,
        const FaceConsts& fc,
        const CommonVars& cv,
        const GeVector v0[ 3 ]
    );
    //! Calculate the shear condition and its derivatives.
    void calcShear(
        const GeMesh::FaceWrapper& face,
        const FaceConsts& fc,
        const CommonVars& cv,
        const GeVector v0[ 3 ]
    );
    //! Calculate the bend condition and its derivatives.
    void calcBend(
        const GeMeshWingedEdge::HalfEdgeWrapper& edge
    );
    //! Verify variables common to stretch/shear conditions.
    void verifyCommon();
    //! Verify the stretch condition and its derivatives.
    void verifyStretch(
        const FaceConsts& fc,
        const CommonVars& cv,
        const GePoint x[3],
        const GeVector v0[ 3 ]
    );
    //! Verify the shear condition and its derivatives.
    void verifyShear(
        const FaceConsts& fc,
        const CommonVars& cv,
        const GePoint x[3],
        const GeVector v0[ 3 ]
    );
    //! Verify the bend condition and its derivatives.
    void verifyBend(
        const GeMeshWingedEdge::HalfEdgeWrapper& edge
    );
    
    // ----- data members -----
    
    // FIXME: initial mesh should be RCShdPtr<const GeMesh>
    //! Duration: class lifetime.
    //@{
    RCShdPtr<GeMesh> _initialMesh;
    RCShdPtr<GeMeshWingedEdge> _initialMeshWingedEdge;
    //@} 

    //! Duration: updated after each step.
    RCShdPtr<GeMesh> _mesh;
    //! Duration: updated after each step.
    StepData        _sd;
    
    //! Cloth parameters. Duration: user-defined, per-step.
    Params          _params;
    //! Density of cloth ( kg / m^2 ). Duration: user-defined, per-step.
    Float           _rho;
    //! Timestep, measured in seconds. Duration: user-defined, per-step.
    Float           _h;
    //! Velocity constraints. Duration: user-defined, per-step.
    SimVector       _z0;
    //! Maximum stretch allowed in a successful step. Duration: user-defined,
    //! per-step.
    Float _stretchLimit;

    //! True if a step is in progress.
    bool            _inStep;
    //! Step success flag. Duration: updated after each step.
    bool            _stepSuccessFlag;


    //! Mass per particle. Duration: class lifetime.
    TridiagMatrix   _M;
    //! Sum of diagonals of _M. Duration: class lifetime.
    Float           _totalMass;

    //! Force gradient. Duration: temporary used during step calculation.
    SymMatrix       _df_dx;
    SymMatrix       _df_dv;

    //! Face constants. Used for optimisation of stretch/shear calculation.
    //! Duration: class lifetime.
    std::vector<FaceConsts> _faceConsts;

    //! Unit face normals. Used for optimisation of bend calculation.
    //! Duration: temporary used during preStep().
    std::vector<GeVector> _faceUnitNormals;
    //! Face normals' inverse magnitudes. Used for optimisation of bend
    //! calculation. Duration: temporary used during preStep().
    std::vector<Float>    _faceNormalIMs;

    //! Saved data, restored if a timestep fails
    std::vector<GePoint>  _savedVertices;
    //! Saved data, restored if a timestep fails. Effectively, this contains
    //! the step data for the last successful step.
    StepData              _savedStepData;

    //! If set, the postSubStepsFinale() routine will be called at the start
    //! of preSubSteps() instead of at the end of postSubSteps().
    bool                 _doFinaleInPre;

    //! Duration: temporary used during step calculation.
    ModPCGSolver    _modPCG;
};

FREECLOTH_NAMESPACE_END

#endif
