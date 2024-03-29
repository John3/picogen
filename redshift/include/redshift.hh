//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef REDSHIFT_H_INCLUDED_20090223
#define REDSHIFT_H_INCLUDED_20090223

#include "setup.hh"

// Color types.
#include "contracts/color_contract.hh"
#include "basictypes/rgb.hh"
#include "traits/color_traits.hh"

// Special ray types.
#include "basictypes/incomingray.hh"
#include "basictypes/outgoingray.hh"

// TODO reorder

// coordinates/
#include "coordinates/uvcoordinates.hh"
#include "coordinates/imagecoordinates.hh"
#include "coordinates/lenscoordinates.hh"
#include "coordinates/texture2dcoordinates.hh"

// interaction/
#include "interaction/usercommandprocessor.hh"
#include "interaction/passivecommandprocessor.hh"
#include "interaction/progressreporter.hh"

// basictypes/
namespace redshift{class RenderTarget;}
#include "basictypes/rectangle.hh"
#include "basictypes/sample.hh"

// samplers/
#include "samplers/sampler.hh"

// cameras/
#include "cameras/camera.hh"

// basictypes/
#include "basictypes/differentialgeometry.hh"
#include "basictypes/material.hh"
#include "basictypes/intersection.hh"

#include "basictypes/background.hh"

// rendertargets/
#include "rendertargets/rendertargetlock.hh"
#include "rendertargets/rendertarget.hh"


// shapes/
#include "shapes/shape.hh"
#include "shapes/closedsphere.hh"

// primitive/
#include "primitives/primitive.hh"
#include "primitives/closedsphere.hh"
#include "primitives/heightmap.hh"

// Cameras.
#include "cameras/camera.hh"

// basictypes/
#include "basictypes/scene.hh"
#include "basictypes/heightmap.hh"


#endif // REDSHIFT_H_INCLUDED_20090223
