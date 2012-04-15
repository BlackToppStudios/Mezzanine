//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef _particleemitter_h
#define _particleemitter_h

#include "datatypes.h"

namespace Ogre
{
    class ParticleEmitter;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ParticleEmitter
    /// @headerfile particleemitter.h
    /// @brief This class defines how particles of a given particle effect spawn.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ParticleEmitter
    {
        protected:
            friend class ParticleEffect;
            /// @internal
            /// @brief The internal emitter class this class get's it functionality from.
            Ogre::ParticleEmitter* Emitter;
            /// @internal
            /// @brief Internal Class constructor.
            ParticleEmitter(Ogre::ParticleEmitter* InternalEmitter);
        public:
            /// @brief Class destructor.
            virtual ~ParticleEmitter();

            /// @brief Sets a custom parameter of a particle emitter.
            /// @remarks The params available to change depends on the internal particle emitter used.
            /// You should check your particle script where possible to ensure particles you are
            /// changing are the type you expect.
            /// @param Name The name of the parameter to alter.
            /// @param Value The new value to set for the named parameter.
            void SetCustomParam(const String& Name, const String& Value);

            /// @brief Gets a custom parameter of a particle emitter.
            /// @remarks The params available to change depends on the internal particle emitter used.
            /// You should check your particle script where possible to ensure the particle has the
            /// param you are looking for.
            /// @param Name The name of the parameter to fetch.
            String GetCustomParam(const String& Name) const;
    };//ParticleEmitter
}//Mezzanine

#endif
