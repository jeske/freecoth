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

#include <freecloth/simulator/simStepStrategyAdaptive.h>
#include <freecloth/simulator/simSimulator.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

namespace {
    using namespace freecloth;

    const Float TIMESTEP_CHANGE_FACTOR = 1.5f;
    const UInt32 NB_INTERNAL_STEPS_INC_DEFAULT = 2;
    const UInt32 MAX_NB_INTERNAL_STEPS_INC = 40;
    const Float INC_CHANGE_FACTOR = 1.5f;
}

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimStepStrategyAdaptive

//------------------------------------------------------------------------------

SimStepStrategyAdaptive::SimStepStrategyAdaptive(
    const RCShdPtr<Simulator>& simulator,
    UInt32 frameRate,
    BaTime::Duration maxTimestep
) : BaseClass( simulator ),
    _maxSubSteps( 1500 ),
    _size( 0 ),
    _maxTimestep( maxTimestep )
{
    _inStep = false;
    setFrameRate( frameRate );
    // Set _h to match _size
    changeStep( 0 );
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::rewind()
{
    _simulator->rewind();
    _inStep = false;
    _nbInternalStepsInc = NB_INTERNAL_STEPS_INC_DEFAULT;
    _nbInternalSteps = 0;
    _frame = 0;
    _size = 0;
    changeStep( 0 );
}

//------------------------------------------------------------------------------

bool SimStepStrategyAdaptive::subStepsDone() const
{
    DGFX_ASSERT( inStep() );
    return _doneSubSteps;
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::preSubSteps()
{
    DGFX_ASSERT( !inStep() );
    _stepEnd = BaTime::floatAsInstant(
        static_cast<Float>(_frame + 1) / _frameRate
    );
    _simulator->setTimestep( _h );
    _simulator->preSubSteps();
    _nbSubSteps = 0;
    _doneSubSteps = false;
    _fullStep = true;
    _inStep = true;
    _stepFailed = false;
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::subStep()
{
    DGFX_ASSERT( inStep() );
    if ( !_simulator->subStepsDone() ) {
        if ( _nbSubSteps > _maxSubSteps ) {
            _stepFailed = true;
            _simulator->cancelStep();
        }
        else {
            ++_nbSubSteps;
            _simulator->subStep();
        }
    }
    else {
        _simulator->postSubSteps();
        if ( _simulator->stepSucceeded() ) {
            if ( _fullStep ) {
                ++_nbInternalSteps;
                // FIXME: should we reduce the inc if a grow succeeds?
                if ( _nbInternalSteps == _nbInternalStepsInc ) {
                    // Time to try a larger step size
                    _nbInternalSteps = 0;
                    // Grow _h
                    changeStep( 1 );
                }

                // Now, check if we're done the frame.
                if ( _simulator->getTime() >= _stepEnd ) {
                    // Done internal steps.
                    _doneSubSteps = true;
                }
                else {
                    const BaTime::Duration fudge(
                        BaTime::floatAsDuration( 0.001f )
                    );
                    // If this is positive, the current timestep will take us
                    // past the end-of-frame.
                    BaTime::Duration diff(
                        _simulator->getTime() + _h - _stepEnd
                    );
                    if ( diff > fudge ) {
                        // This will take us way past the end-of-frame.
                        // Use a temporary smaller step size to compensate.
                        _fullStep = false;
                        _simulator->setTimestep( BaTime::getDuration(
                            _simulator->getTime(), _stepEnd
                        ) );
                    }
                    else if ( diff > -fudge ) {
                        // Our time step will take us right to end-of-frame,
                        // with a little numerical fudging. Treat this is a
                        // full step, but make sure we get *exactly* to the
                        // end of frame.
                        _simulator->setTimestep( BaTime::getDuration(
                            _simulator->getTime(), _stepEnd
                        ) );
                    }
                    else {
                        // Our timestep won't take us close to end-of-frame.
                        // Just go.
                        _simulator->setTimestep( _h );
                    }
                    // Start next internal step.
                    _simulator->preSubSteps();
                    _nbSubSteps = 0;
                }
            }
            else {
                // Completed a mini internal step to reach the end of frame.
                // This doesn't count towards our nbInternalSteps count.
                _doneSubSteps = true;
            }
        }
        else {
            _stepFailed = true;
        }
    }
    if ( _stepFailed ) {
        _stepFailed = false;

        // FIXME: deal with case where fullStep is false.
        // Internal step failed.
        if ( _nbInternalSteps == 0 ) {
            // This was the first try with this stepsize, so we probably
            // just increased the stepsize. Drop it back down, and increase
            // the wait time until the next stepsize increase attempt.
            changeStep( -1 );
            _nbInternalStepsInc = std::min(
                BaMath::roundUInt32(
                    _nbInternalStepsInc * INC_CHANGE_FACTOR
                ),
                MAX_NB_INTERNAL_STEPS_INC
            );
        }
        else {
            // We had had some success with this stepsize, but it failed
            // this time. Drop the stepsize, but try a bigger stepsize
            // again soon.
            changeStep( -1 );
            _nbInternalStepsInc = NB_INTERNAL_STEPS_INC_DEFAULT;
            _nbInternalSteps = 0;
        }
        _simulator->setTimestep( _h );
        _simulator->preSubSteps();
        _nbSubSteps = 0;
    }
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::changeStep( Int32 inc )
{
    static const Int32 minDenom = 2;
    static const Int32 maxDenom = 6;
    static const Int32 minPow = -3;
    static const Int32 maxSize = maxDenom - minDenom;
    _size = std::min( maxSize, _size + inc );
    if ( _size >= 0 ) {
        // In this regime, the timestep is a constant fraction of the frame
        // period. The size defines the denominator of the fraction.
        const Int32 denom = maxSize - _size + minDenom;
        _h = BaTime::floatAsDuration( 1.f/_frameRate ) / denom;
    }
    else {
        // In this regime, the timestep is a power-of-2 fraction of the
        // frame period. The size defines the exponent of the power of two.
        const Int32 exp = _size + 1 + minPow;
        DGFX_ASSERT( exp < minPow && exp <= 0 );
        _h = BaTime::floatAsDuration( 1.f/_frameRate ) / ( 1 << -exp );
    }
    _h = std::min( _h, _maxTimestep );
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::postSubSteps()
{
    DGFX_ASSERT( inStep() );
    _inStep = false;
    ++_frame;
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::cancelStep()
{
    DGFX_ASSERT( inStep() );
    // FIXME: this will still leave the simulator mid-frame.
    _simulator->cancelStep();
    _inStep = false;
}

//------------------------------------------------------------------------------

bool SimStepStrategyAdaptive::inStep() const
{
    return _inStep;
}

//------------------------------------------------------------------------------

bool SimStepStrategyAdaptive::stepSucceeded() const
{
    return true;
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::setFrameRate( UInt32 frameRate )
{
    DGFX_ASSERT( !inStep() );
    _frameRate = frameRate;
    BaTime::Duration framePeriod = BaTime::floatAsDuration( 1.f / frameRate );
    _h = std::min( _simulator->getTimestep(), framePeriod );
    _frame = BaMath::floorUInt32(
        BaTime::instantAsSeconds( _simulator->getTime() ) /
        BaTime::durationAsSeconds( framePeriod )
    );
    _nbInternalStepsInc = NB_INTERNAL_STEPS_INC_DEFAULT;
    _nbInternalSteps = 0;
}

//------------------------------------------------------------------------------

void SimStepStrategyAdaptive::setMaxTimestep( BaTime::Duration maxTimestep )
{
    DGFX_ASSERT( !inStep() );
    _maxTimestep = maxTimestep;
    if ( _maxTimestep > 0 ) {
        _h = std::min( _h, _maxTimestep );
    }
    // otherwise, timestep is unlimited
}

//------------------------------------------------------------------------------

UInt32 SimStepStrategyAdaptive::getFrameRate() const
{
    return _frameRate;
}

//------------------------------------------------------------------------------

BaTime::Duration SimStepStrategyAdaptive::getMaxTimestep() const
{
    return _maxTimestep;
}

FREECLOTH_NAMESPACE_END
