// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsd3d9rendersyshelper_cpp
#define _graphicsd3d9rendersyshelper_cpp

#include "Graphics/d3d9rendersyshelper.h.cpp"

#include "exception.h"

#include <OgreRoot.h>
#include <OgreD3D9Plugin.h>

namespace
{
    /// @brief Pointer to the created rendersystem plugin for init/deinit.
    Ogre::D3D9Plugin* RSPlugin;
}

namespace Mezzanine
{
    namespace Graphics
    {
        void CreateD3D9RenderSystem()
        {
            if( RSPlugin == NULL ) {
                RSPlugin = new Ogre::D3D9Plugin();
                Ogre::Root::getSingletonPtr()->installPlugin( RSPlugin );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"The Direct3D9 render system has already been created.")
            }
        }

        void DestroyD3D9RenderSystem()
        {
            // Currently nothing.
            /// @todo If Ogre gets its act together this method could be implemented.
        }

        Graphics::RenderSystem GetD3D9RenderSystemType()
            { return Graphics::RS_DirectX9; }

        String GetD3D9RenderSystemName()
            { return "Direct3D9 Rendering Subsystem"; }

        String GetShortenedD3D9RenderSystemName()
            { return "Direct3D9"; }
    }//Graphics
}//Mezzanine

#endif
