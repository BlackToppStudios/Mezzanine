//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _ui_h
#define _ui_h

namespace phys
{
    /// @namespace phys::UI
    /// @brief This namespace is for all the classes belonging to the Graphical User Interface Subsystem.
    /// @details Our GUI subsystem is based on a heavily modified/forked version of Gorilla, and as such uses
    /// a similiar structure of classes and even it's config files(e.g. .gorilla files).
    namespace UI
    {

    }
}

#include "uienumerations.h"

#include "uibasicrenderable.h"
#include "uibutton.h"
#include "uicaption.h"
#include "uicell.h"
#include "uicellgrid.h"
#include "uicheckbox.h"
#include "uidropdownlist.h"
#include "uilayer.h"
#include "uilinelist.h"
#include "uilistbox.h"
#include "uimarkuptext.h"
#include "uimenu.h"
#include "uimenuwindow.h"
#include "uipagedcellgrid.h"
#include "uiradiobutton.h"
#include "uirectangle.h"
#include "uirenderablecontainerwidget.h"
#include "uirenderablerect.h"
#include "uiresizinginfo.h"
#include "uiscreen.h"
#include "uiscrollbar.h"
#include "uiscrolledcellgrid.h"
#include "uispinner.h"
#include "uitabset.h"
#include "uitextbutton.h"
#include "uiviewportupdatetool.h"
#include "uiwidget.h"
#include "uiwindow.h"

#endif
