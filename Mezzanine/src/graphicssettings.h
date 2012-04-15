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
#ifndef _graphicssettings_h
#define _graphicssettings_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class GraphicsSettings
    /// @headerfile graphicsmanager.h
    /// @brief This stores all the possible configuration options the graphics manager supports.
    /// @details The graphics manager stores one of these for all of it's configuration options,
    /// additionally one can be created and passed into the manager to set all the configuration
    /// options at once.
    ///////////////////////////////////////
    struct MEZZ_LIB GraphicsSettings
    {
        /// @brief This stores the Height of the renderwindow.
        Whole RenderHeight;
        /// @brief This stores the Width of the renderwindow.
        Whole RenderWidth;
        /// @brief This stores the device refresh rate in Hz.
        Whole RefreshRate;
        /// @brief This is the desired state of whether the window is fullscreen or not.
        bool Fullscreen;
        /// @brief This is the desired state of whether to enable VSync or not.
        bool VSync;
        /// @brief Struct Constructor
        GraphicsSettings() : RenderHeight(0),RenderWidth(0),RefreshRate(0),Fullscreen(false),VSync(false) {}
        GraphicsSettings& operator= (const GraphicsSettings &GS)
        {
            this->RenderHeight = GS.RenderHeight;
            this->RenderWidth = GS.RenderWidth;
            this->RefreshRate = GS.RefreshRate;
            this->Fullscreen = GS.Fullscreen;
            this->VSync = GS.VSync;
            return *this;
        }
    };//graphicssettings
}//Mezzanine

#endif
