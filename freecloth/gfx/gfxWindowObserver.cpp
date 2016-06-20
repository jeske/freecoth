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

#include <freecloth/gfx/gfxWindowObserver.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxWindowObserver

//------------------------------------------------------------------------------

GfxWindowObserver::~GfxWindowObserver()
{
}

//------------------------------------------------------------------------------

void GfxWindowObserver::idleReceived()
{
}

//------------------------------------------------------------------------------

void GfxWindowObserver::destroyed( GfxWindow& )
{
}

//------------------------------------------------------------------------------

void GfxWindowObserver::closeReceived( GfxWindow& )
{
}

//------------------------------------------------------------------------------

void GfxWindowObserver::displayReceived(
    GfxWindow&
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::keyPressed(
    GfxWindow&, GfxWindow::KeyID, GfxWindow::ModBitfield
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::keyDownReceived(
    GfxWindow&, GfxWindow::KeyID, GfxWindow::ModBitfield
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::keyUpReceived(
    GfxWindow&, GfxWindow::KeyID, GfxWindow::ModBitfield
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::mouseDownReceived(
    GfxWindow&, UInt32 x, UInt32 y, GfxWindow::ButtonID
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::mouseUpReceived(
    GfxWindow&, UInt32 x, UInt32 y, GfxWindow::ButtonID
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::mouseMoveReceived( GfxWindow&, UInt32, UInt32 )
{
}

//------------------------------------------------------------------------------

void GfxWindowObserver::windowResized(
    GfxWindow&, UInt32, UInt32, UInt32, UInt32
) {
}

//------------------------------------------------------------------------------

void GfxWindowObserver::uiReceived( GfxWindow&, UInt32 )
{
}
