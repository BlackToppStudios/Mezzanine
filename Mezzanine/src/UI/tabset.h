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
#ifndef _uitabset_h
#define _uitabset_h

#include "UI/stackedcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a widget that stores sets of renderables but only displays one at a time.
        /// @details This widget is useful for tabbed option displays.
        ///////////////////////////////////////
        class MEZZ_LIB TabSet : public StackedContainer
        {
        public:
            /// @brief String containing the type name for this class: "TabSet".
            static const String TypeName;
        protected:
            friend class TabSetFactory;
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            TabSet(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            TabSet(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            TabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created TabSet will belong to.
            TabSet(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~TabSet();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            ///////////////////////////////////////////////////////////////////////////////
            // TabSet Properties

            ///////////////////////////////////////////////////////////////////////////////
            // TabSet Configuration

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

        };//TabSet

        /*class Button;
        typedef class EnclosedRenderableContainerWidget RenderableCollection;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableSetData
        /// @headerfile uitabset.h
        /// @brief This is a helper class for the storage of Renderable Set's in a Tabset.
        ///////////////////////////////////////
        struct MEZZ_LIB RenderableSetData
        {
            /// @brief The name of the Accessor/Set Pair.
            String Name;
            /// @brief The text button allowing access to viewing the Set.
            Button* Accessor;
            /// @brief The collection of renderables to display.
            RenderableCollection* Collection;

            /// @brief No initialization constructor.
            RenderableSetData() : Accessor(NULL), Collection(NULL) {};
            /// @brief Full initialization constructor.
            RenderableSetData(ConstString& name, Button* access, RenderableCollection* set) : Name(name), Accessor(access), Collection(set) {};
        };//RenderableSetData
        ///////////////////////////////////////////////////////////////////////////////
        /// @class TabSet
        /// @headerfile uitabset.h
        /// @brief This is a widget that stores sets of renderables but only displays one at a time.
        /// @details This widget is useful for tabbed option displays.
        ///////////////////////////////////////
        class MEZZ_LIB TabSet : public Widget
        {
            protected:
                friend class RenderableFactory;
                Whole SetsAdded;
                std::vector<RenderableSetData*> Sets;
                Rect TemplateSetRect;
                RenderableSetData* VisibleSet;
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
                /// @internal
                /// @brief Internal function for setting the location(position) of this widget.
                virtual void SetLocation(const Vector2& Position);
                /// @internal
                /// @brief Internal function for setting the area(size) of this widget.
                virtual void SetArea(const Vector2& Size);
            //public:
                /// @brief Class constructor.
                /// @param name The Name for the Widget.
                /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by this Widget.
                /// @param parent The parent screen that created this widget.
                TabSet(const String& name, const Rect& SetRect, Screen* parent);
                /// @brief Class destructor.
                virtual ~TabSet();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Creating and working with Renderable Sets
                ///////////////////////////////////////
                /// @brief Creates a new Set in this widget.
                /// @details The names generated for the accessor and set will be [Name]+"Access" and [Name]+"Set".
                /// @return Returns a RenderableSetData struct containing the newly created accessor and set.
                /// @param Name The Name for the new Set.
                /// @param AccessorRect The Rect representing the position and size of all the Renderable Sets generated by this Widget.
                /// @param GlyphHeight The height(in relative units) desired for the text that will be displayed in the Accessor button.
                /// @param Text The text to set in the TextButton.
                virtual RenderableSetData* CreateRenderableSet(const String& Name, const Rect& AccessorRect, const Real& GlyphHeight, const String& Text);
                /// @brief Gets a RenderableSetData by Index.
                /// @return Returns a pointer to the RenderableSetData at the requested Index.
                /// @param Index The index of the RenderableSetData to retrieve.
                virtual RenderableSetData* GetRenderableSetData(const Whole& Index);
                /// @brief Gets an RenderableSetData by Name.
                /// @return Returns a pointer to the RenderableSetData of the requested Name.
                /// @param SetDataName The name of the RenderableSetData to retrieve.
                virtual RenderableSetData* GetRenderableSetData(const String& SetDataName);
                /// @brief Gets a RenderableCollection by Index.
                /// @return Returns a pointer to the RenderableSet at the requested Index.
                /// @param Index The index of the RenderableSet to retrieve.
                virtual RenderableCollection* GetRenderableCollection(const Whole& Index);
                /// @brief Gets an RenderableCollection by Name.
                /// @return Returns a pointer to the RenderableSet of the requested Name.
                /// @param SetDataName The name of the RenderableSet to retrieve the collection from.
                virtual RenderableCollection* GetRenderableCollection(const String& SetDataName);
                /// @brief Gets a RenderableCollection by Accessor button.
                /// @return Returns a pointer to the RenderableCollection associated with the provided Accessor.
                /// @param Accessor The text button that provides access to the desired RenderableCollection.
                virtual RenderableCollection* GetRenderableCollection(Button* Accessor);
                /// @brief Gets an Accessor by Index.
                /// @return Returns a pointer to the Accessor at the requested Index.
                /// @param Index The index of the Accessor to retrieve.
                virtual Button* GetAccessor(const Whole& Index);
                /// @brief Gets an Accessor by Name.
                /// @return Returns a pointer to the Accessor of the requested Name.
                /// @param SetDataName The name of the RenderableSet to retrieve the accessor from.
                virtual Button* GetAccessor(const String& SetDataName);
                /// @brief Gets an Accessor by RenderableCollection.
                /// @return Returns a pointer to the Accessor associated with the provided RenderableCollection.
                /// @param Collection The Collection of renderables being provided access by the desired button.
                virtual Button* GetAccessor(RenderableCollection* Collection);
                /// @brief Gets the number of Sets stored in this TabSet.
                /// @return Returns a Whole representing the number of RenderableSets being stored in this Widget.
                virtual Whole GetNumRenderableSets();
                /// @brief Destroys an existing RenderableSetData.
                /// @param ToBeDestroyed The RenderableSetData that will be destroyed.
                virtual void DestroyRenderableSet(RenderableSetData* ToBeDestroyed);
                /// @brief Destroys all RenderableSetData's currently stored in this widget.
                virtual void DestroyAllRenderableSets();
        };//TabSet//*/
    }//UI
}//Mezzanine

#endif
