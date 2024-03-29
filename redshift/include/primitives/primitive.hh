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

#ifndef PRIMITIVE_H_INCLUDED_20090301
#define PRIMITIVE_H_INCLUDED_20090301

namespace redshift { namespace primitive {
        class Primitive : public enable_shared_from_this<Primitive> {
        public:
                // If only the information is need whether the ray hits or not.
                virtual bool doesIntersect (RayDifferential const &ray) 
                                                                     const = 0;

                // Full fledged information including Normal, U/V, etc.
                virtual optional<Intersection> intersect(
                                        RayDifferential const &ray) const = 0;
                                        
                
                virtual bool doesIntersect (Sample const &sample) const {
                        return doesIntersect (sample.primaryRay);
                }
                
                virtual optional<Intersection> intersect(
                                        Sample const &sample) const {
                        return intersect (sample.primaryRay);
                }
                                        
                                        
                
                virtual ~Primitive () {}
        };
} }

#endif // PRIMITIVE_H_INCLUDED_20090301
