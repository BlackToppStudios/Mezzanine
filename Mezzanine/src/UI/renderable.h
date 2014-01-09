// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _uirenderable_h
#define _uirenderable_h

#include "uienumerations.h"
#include "UI/vertex.h"

namespace Mezzanine
{
    namespace UI
    {
        class UIManager;
        class Screen;
        class ScreenRenderData;
        class Widget;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Renderable
        /// @headerfile uirenderable.h
        /// @brief Basic class for all structures that get inserted into the rendering hierarchy.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Renderable
        {
        public:
            /// @enum RenderableType
            /// @brief A small enum to describe the type of renderable this is.
            enum RenderableType
            {
                RT_LineList,
                RT_Screen,
                RT_Widget
            };
        protected:
            friend class Widget;
            /// @internal
            /// @brief A pointer to the Screen that created this Renderable.
            Screen* ParentScreen;
            /// @internal
            /// @brief Stores whether this Renderable is to be rendered (also dependent on parent visibility).
            Boolean Visible;
            /// @internal
            /// @brief Stores whether this Renderables vertices need to be regenerated.
            Boolean Dirty;
            /// @internal
            /// @brief The unique name of this Renderable.
            String Name;

            /// @internal
            /// @brief Implementation method for serializing additional sets of data.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @internal
            /// @brief Implementation method for deseriailizing additional sets of data.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
        //public:
            /// @brief Blank constructor.
            /// @note This is primarily useful for (and used as) a basic constructor suitable for XML deserialization post-construction.
            /// @param Parent The parent screen that created this renderable.
            Renderable(Screen* Parent);
            /// @brief Class constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent screen that created this renderable.
            Renderable(const String& RendName, Screen* Parent);
            /// @brief Class destructor.
            virtual ~Renderable();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the name of this renderable.
            /// @return Returns a string containing the name of this renderable.
            const String& GetName() const;
            /// @brief Gets the type of renderable this is.
            /// @return Returns a RenderableType enum value coressponding the type of renderable this is.
            virtual RenderableType GetRenderableType() const = 0;
            /// @brief Gets the parent screen of this renderable.
            /// @return Returns a pointer to the screen this renderable belongs to.
            Screen* GetScreen() const;

            /// @brief Gets whether or not this renderable is a Widget.
            /// @return Returns true if this renderable is a Widget, false otherwise.
            Boolean IsWidget() const;
            /// @brief Gets whether or not this renderable is a Screen.
            /// @return Returns true if this renderable is a Screen, false otherwise.
            Boolean IsScreen() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility Methods

            /// @brief Sets the visibility of this renderable.
            /// @param CanSee Bool determining whether or not this renderable should be visible.
            virtual void SetVisible(Boolean CanSee) = 0;
            /// @brief Gets the visibility setting of this renderable.
            /// @return Returns a bool that is the current visibility setting of this renderable.
            virtual Boolean GetVisible() const = 0;
            /// @brief Gets whether or not this renderable is being drawn.
            /// @details This function will check the visibility of all parent objects to see if it is being
            /// drawn.  This will not tell you whether or not this renderable has it's own visibility setting
            /// enabled.  For that see: GetVisible().
            /// @return Returns a bool representing the visibility of this renderable.
            virtual Boolean IsVisible() const = 0;
            /// @brief Forces this renderable to be shown.
            virtual void Show() = 0;
            /// @brief Forces this renderable to hide.
            virtual void Hide() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that this renderable should be appended to.
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @brief Gets the most derived serializable name of this Renderable.
            /// @note When creating a new Renderable class verify this method has a valid return for it in order for serialization to work properly.
            /// @return Returns the name of the XML tag from the most derived class of "this".
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets whether or not this renderable is dirty.
            /// @return Returns true if this renderable is dirty, false otherwise.
            bool _IsDirty() const;
            /// @internal
            /// @brief Marks this renderable as dirty, and informs other renderables if needed.
            virtual void _MarkDirty() = 0;
            /// @internal
            /// @brief Appends the vertices of this renderable to another vector.
            /// @param RenderData The vector of vertex's to append to.
            virtual void _AppendRenderData(ScreenRenderData& RenderData) = 0;
        };//Renderable
    }//UI
}//Mezzanine

#endif
