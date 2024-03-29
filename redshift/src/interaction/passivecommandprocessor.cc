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

#include "../../include/setup.hh"
#include "../../include/interaction/usercommandprocessor.hh"
#include "../../include/interaction/passivecommandprocessor.hh"



namespace redshift { namespace interaction {
        
        

PassiveCommandProcessor::PassiveCommandProcessor()
: doQuit (false)
{
} 



void PassiveCommandProcessor::tick () {
}



bool PassiveCommandProcessor::userWantsToQuit () const {
        return doQuit;
}



void PassiveCommandProcessor::setQuitState (bool doQuit) {
        this->doQuit = doQuit;
}


                
PassiveCommandProcessor::~PassiveCommandProcessor () {
}

                

} }


