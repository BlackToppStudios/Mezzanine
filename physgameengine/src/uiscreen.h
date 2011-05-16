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
#ifndef _uiscreen_h
#define _uiscreen_h

#include "crossplatformexport.h"
#include "vector2.h"

#include <map>

namespace Gorilla
{
    class Screen;
    class Layer;
}

namespace phys
{
    class Viewport;
    class UIManager;
    namespace UI
    {
        class Button;
        class Widget;
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class UIScreen
        /// @headerfile uiscreen.h
        /// @brief This class is a helper class for creating UI's.  It is responsible for storing and keeping
        /// track of all the elements of a single UI screen.
        /// @details UI's can optionally be divided up into Screens, or "pages".  Each screen is batched together
        /// for rendering, so keeping the amount of screens to a minimum will improve performance.
        ///////////////////////////////////////
        class PHYS_LIB Screen
        {
            protected:
                Gorilla::Screen* GorillaScreen;
                UIManager* Manager;
                Viewport* GameViewport;
                String Name;
                String PrimaryAtlas;
                std::map<Whole,Layer*> Layers;
            public:
                /// @brief Internal constructor.
                /// @param name The name of this screen.
                /// @param Atlas The name of the primary atlas to be assigned to this screen.
                /// This can be overridden later, even by individual UI elements.
                /// @param WindowViewport The Viewport to which this screen belongs.
                Screen(const String& name, const String& Atlas, Viewport* WindowViewport);
                /// @brief Class destructor.
                virtual ~Screen();
                /// @brief Gets the name of this screen.
                /// @return Returns a string containing the name of this screen.
                virtual String& GetName();
                /// @brief Sets the screens visability.
                /// @param Visable A bool representing the visability of the screen.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the screens visability.
                /// @return Returns a bool representing the visability of the screen.
                virtual bool IsVisible();
                /// @brief Forces the screen to be shown.
                virtual void Show();
                /// @brief Forces the screen to hide.
                virtual void Hide();
                /// @brief Creates a layer in the GUI screen to place GUI objects in.
                /// @details A GUI layer is exactly that, a layer of GUI objects.  You can have multiple GUI
                /// layers per screen.  The Zorder of the layer determines it's visability if there are multiple
                /// layers.  If the Zorder of one layer is higher then another in the same space, then the Zorder
                /// with the higher Zorder will be rendered...giving it the appearance of being on top of the other
                /// GUI object or objects.
                /// @param Name The name to be given to the layer.
                /// @param Zorder The layers Zorder, as explained above.
                virtual Layer* CreateLayer(const String& Name, Whole Zorder);
                /// @brief Gets an already created layer by name.
                /// @return Returns a pointer to the layer of the specified name.
                virtual Layer* GetLayer(const String& Name);
                /// @brief Gets an already created layer by it's index.
                /// @return Returns a pointer to the layer at the specified index.
                virtual Layer* GetLayer(Whole Index);
                /// @brief Gets an already created layer by it's Zorder.
                /// @return Returns a pointer to the layer with the specified Zorder.
                virtual Layer* GetLayerbyZorder(Whole Zorder);
                /// @brief Gets the number of layers created and stored in this class.
                /// @return Returns the number of layers this class is storing.
                virtual Whole GetNumLayers();
                /// @brief Destroy's a previously created layer.
                /// @param ToBeDestroyed The layer to be destroyed.
                virtual void DestroyLayer(Layer* ToBeDestroyed);
                /// @brief Gets the current viewport dimensions.
                /// @return Returns a Vector2 representing the current viewport dimensions.
                virtual Vector2 GetViewportDimensions();
                /// @brief Gets the button the mouse is over if any.
                /// @details This function searches only the visable layers contained in this screen.
                /// @return Returns the button the mouse is over, or NULL if there are none.
                virtual Button* CheckButtonMouseIsOver();
                /// @brief Gets the widget the mouse is over if any.
                /// @details This function searches only the visable layers contained in this screen.
                /// @return Returns the widget the mouse is over, or NULL if there are none.
                virtual Widget* CheckWidgetMouseIsOver();
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set.
                virtual String GetPrimaryAtlas();
                /// @internal
                /// @brief Gets the internal screen this screen is based on.
                /// @return Returns a pointer to the Gorilla screen this screen is based on.
                virtual Gorilla::Screen* GetGorillaScreen();
        };//uiscreen
    }//ui
}//phys

#endif
