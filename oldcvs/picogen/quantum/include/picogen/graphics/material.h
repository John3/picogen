/***************************************************************************
 *            material.h
 *
 *  Fri Oct 12 18:58:29 2007
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


#ifndef _MATERIAL_H
#define _MATERIAL_H

namespace material{
	namespace abstract{
		class IBRDF{
			public:
				virtual ~IBRDF() {};
				// RandomSample assumes the in-/out.params to be in world
				// space  W , rather than in unit space U with the y-axis
				// being   (0|1|0).      Consequently,    if    necessary,
				// RandomSample,   if  necessary,   has   to  do  its own
				// W->U  transformation   on  the  in-parameters  and  it
				// must   guarantee  that   the   out-parameters  are  in
				// W-space
				//
				// returns false  if  the  in-ray is absorbed, and if so,
				// the out-params are undefined.
				virtual bool RandomSample(
					param_out(misc::prim::real,brdf),
					param_out(misc::prim::real,p),
					param_out(bool,specular),
					param_out(misc::geometrics::Ray,r_out),
					param_in(misc::geometrics::Ray,r_in),
					param_in(misc::geometrics::Vector3d,N)
				) const = 0;
		};

		class IShader{
			public:
				virtual ~IShader() {};
				virtual void Shade(
					image::color::Color &color,
					const misc::geometrics::Vector3d &normal,
					const misc::geometrics::Vector3d &position
				) const = 0;
		};
	};
};

#endif /* _MATERIAL_H */