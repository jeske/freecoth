//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_sim_simStepStrategyBasic_h
#define freecloth_sim_simStepStrategyBasic_h

#ifndef freecloth_simulator_package_h
#include <freecloth/simulator/package.h>
#endif

#ifndef freecloth_simulator_simStepStrategy_h
#include <freecloth/simulator/simStepStrategy.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class SimStepStrategyBasic freecloth/simulator/simStepStrategyBasic.h
 * \brief Basic stepping strategy.
 * \pattern Strategy
 *
 * The basic stepping strategy simply calls straight through to the
 * simulator's step/substep methods, and stops if a step fails.
 */
class SimStepStrategyBasic : public SimStepStrategy
{
public:

    // ----- types and enumerations -----
    typedef SimStepStrategy BaseClass;

    // ----- member functions -----
    explicit SimStepStrategyBasic( const RCShdPtr<Simulator>& simulator );

    virtual void rewind();
    virtual bool subStepsDone() const;
    virtual void preSubSteps();
    virtual void subStep();
    virtual void postSubSteps();
    virtual void cancelStep();
    virtual bool inStep() const;
    virtual bool stepSucceeded() const;

    bool stopped() const;

private:
    // ----- data members -----
    bool _stopped;
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

FREECLOTH_NAMESPACE_END

#endif
