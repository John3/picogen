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

#ifndef HEIGHTMAP_CODEGEN_HH_INCLUDED_20090423
#define HEIGHTMAP_CODEGEN_HH_INCLUDED_20090423

namespace heightmap_codegen {
        class NamespaceMaker ;
}


#include "../heightmap-layers.hh"

namespace heightmap_codegen {
        
        
        
        class NamespaceMaker {
        public:
                NamespaceMaker () ;                
                QString getNamespace () const ;                
                void generateNewNamespace () ;                
        private:
                int num;                
        };
        
        
        
}

#endif // HEIGHTMAP_CODEGEN_HH_INCLUDED_20090423