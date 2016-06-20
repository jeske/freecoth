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

#include <freecloth/simulator/simStepStrategyBasic.h>
#include <freecloth/simulator/simSimulator.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimStepStrategyBasic

//------------------------------------------------------------------------------

SimStepStrategyBasic::SimStepStrategyBasic(
    const RCShdPtr<Simulator>& simulator
) : BaseClass( simulator ),
    _stopped( false )
{
}

//------------------------------------------------------------------------------

bool SimStepStrategyBasic::subStepsDone() const
{
    if ( _stopped ) {
        return true;
    }
    return _simulator->subStepsDone();
}

//------------------------------------------------------------------------------

void SimStepStrategyBasic::rewind()
{
    _simulator->rewind();
    _stopped = false;
}

//------------------------------------------------------------------------------

void SimStepStrategyBasic::preSubSteps()
{
    if ( ! _stopped ) {
        _simulator->preSubSteps();
    }
}

//------------------------------------------------------------------------------

void SimStepStrategyBasic::subStep()
{
    if ( ! _stopped ) {
        _simulator->subStep();
    }
}

//------------------------------------------------------------------------------

void SimStepStrategyBasic::postSubSteps()
{
    if ( ! _stopped ) {
        _simulator->postSubSteps();
        if ( ! stepSucceeded() ) {
            _stopped = true;
        }
    }
}

//------------------------------------------------------------------------------

void SimStepStrategyBasic::cancelStep()
{
    _simulator->cancelStep();
}

//------------------------------------------------------------------------------

bool SimStepStrategyBasic::inStep() const
{
    return _simulator->inStep();
}

//------------------------------------------------------------------------------

bool SimStepStrategyBasic::stepSucceeded() const
{
    if ( ! _stopped ) {
        return _simulator->stepSucceeded();
    }
    return false;
}

//------------------------------------------------------------------------------

bool SimStepStrategyBasic::stopped() const
{
    return _stopped;
}

FREECLOTH_NAMESPACE_END
