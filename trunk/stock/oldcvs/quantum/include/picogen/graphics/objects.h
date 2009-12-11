/***************************************************************************
 *            objects.h
 *
 *  Fri Oct 12 19:04:33 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _OBJECTS_H
#define _OBJECTS_H


namespace picogen {
    namespace graphics {

        namespace objects {

            namespace abstract {
                class IIntersectable {
                    private:
                        typedef ::picogen::geometrics::Ray Ray;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    public:
                        virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const = 0;
                        virtual ~IIntersectable() {};
                };

                class IAccelerationStructure : public IIntersectable {
                    public: virtual void invalidate() = 0;
                };

                class IScene : public IAccelerationStructure {
                    private:
                    public:
                        virtual void insert (const IIntersectable*i) = 0;
                        virtual void flush() = 0;
                        virtual void invalidate() = 0;
                        virtual ~IScene() {}
                };

                //> similar to IScene, but constrained to naked triangles
                class ITriScene : public IAccelerationStructure {
                    private:
                        typedef ::picogen::geometrics::Vector3d Vector3d;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    public:
                        virtual void setBRDF (const IBRDF *brdf) = 0;
                        virtual void insert (param_in (Vector3d,A), param_in (Vector3d,B), param_in (Vector3d,C)) = 0;
                        virtual void flush() = 0;
                        virtual void invalidate() = 0;
                        virtual ~ITriScene() {}
                };

                class ISky {
                    private:
                        typedef ::picogen::geometrics::Vector3d Vector3d;
                        typedef ::picogen::geometrics::Ray Ray;
                        typedef ::picogen::graphics::color::Color Color;
                        typedef ::picogen::real real;
                    public:
                        virtual ~ISky() {};
                        virtual void shade (param_out (Color,color), param_in (Ray,ray)) const = 0;
                        virtual void sunShade (param_out (Color,color), param_in (Ray,ray)) const = 0;
                        virtual void sunSample (param_out (Color,color), param_out (Ray,ray), param_out (real,p), param_in (Vector3d,position)) const = 0;
                        virtual void atmosphereShade (param_out (Color,color), param_in (Color,src_color), param_in (Ray,ray), real distance) const = 0;
                        virtual void getSunColor (param_out (Color,color)) const = 0;
                        virtual void getSunDirection (param_out (Vector3d,direction)) const = 0;
                        virtual real getSunArealFactor () const = 0;
                };
            } // namespace abstract
        } // namespace objects
    } // namespace graphics
}//namespace picogen
/*
#include "objects/Snow.h"
#include "objects/Rain.h"
//> above files moved to picogen/experimental as long as they are exactly that
*/

#include "objects/LinearList.h"
#include "objects/TriBIH.h"
#include "objects/Preetham.h"
#include "objects/SimpleHeightField.h"
#include "objects/VelocityInstance.h"
#include "objects/Sphere.h"
#include "objects/ZBNCubemap.h"
#include "objects/AABox.h"
#include "objects/ImplicitHeightField.h"
#include "objects/CloudAdapter.h"


#endif /* _OBJECTS_H */