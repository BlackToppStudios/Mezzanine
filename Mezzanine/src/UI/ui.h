//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _ui_h
#define _ui_h

namespace Mezzanine
{
    /// @namespace Mezzanine::UI
    /// @brief This namespace is for all the classes belonging to the Graphical User Interface Subsystem.
    /// @details Our GUI subsystem is based on a heavily modified/forked version of Gorilla, and as such uses
    /// a similiar structure of classes and layout for it's config files(e.g. .mta files).
    namespace UI
    {

    }
}

#include "UI/uienumerations.h"
#include "UI/uimanager.h"

#include "UI/action.h"
#include "UI/actionhandler.h"
#include "UI/brutestrategy.h"
#include "UI/button.h"
#include "UI/character.h"
#include "UI/charactertraits.h"
#include "UI/checkbox.h"
#include "UI/defaultmarkupparser.h"
#include "UI/dropdownlist.h"
#include "UI/editbox.h"
#include "UI/font.h"
#include "UI/glyph.h"
#include "UI/gridcontainer.h"
#include "UI/horizontalcontainer.h"
#include "UI/horizontallayoutstrategy.h"
#include "UI/horizontalscrollbar.h"
#include "UI/hotkeyhandler.h"
#include "UI/imagelayer.h"
#include "UI/kerning.h"
#include "UI/layoutcontainer.h"
#include "UI/layoutstrategy.h"
#include "UI/linearcontainer.h"
#include "UI/linelist.h"
#include "UI/listbox.h"
#include "UI/markupparser.h"
#include "UI/menu.h"
#include "UI/menuwindow.h"
#include "UI/mousehoverstrategy.h"
#include "UI/multilinetextlayer.h"
#include "UI/nineboxstrategy.h"
#include "UI/pagedcontainer.h"
#include "UI/pageprovider.h"
#include "UI/positioninginfo.h"
#include "UI/quadrenderable.h"
#include "UI/quadrenderer.h"
#include "UI/radiobutton.h"
#include "UI/rect.h"
#include "UI/renderable.h"
#include "UI/renderlayer.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "UI/simplerenderer.h"
#include "UI/singlelinetextlayer.h"
#include "UI/sizinginfo.h"
#include "UI/spinner.h"
#include "UI/sprite.h"
#include "UI/stackedcontainer.h"
#include "UI/tabset.h"
#include "UI/textcursor.h"
#include "UI/textlayer.h"
#include "UI/textline.h"
#include "UI/textureatlas.h"
#include "UI/textureatlashandler.h"
#include "UI/unifieddim.h"
#include "UI/vertex.h"
#include "UI/verticalcontainer.h"
#include "UI/verticallayoutstrategy.h"
#include "UI/verticalscrollbar.h"
#include "UI/widget.h"
#include "UI/widgetfactory.h"
#include "UI/window.h"

#endif
