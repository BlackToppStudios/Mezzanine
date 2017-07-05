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
#ifndef _mousepicker_h
#define _mousepicker_h

#include "ray.h"

namespace Mezzanine
{
    class World;
    class Entity;
    class MousePickDragger;
    class RayQuery;
    class RayQueryHit;
    namespace Input
    {
        class Mouse;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A helper class to facilitate the manipulation and movement of objects in the world directly using the mouse cursor.
    ///////////////////////////////////////
    class MEZZ_LIB MousePicker
    {
    public:
        /// @brief Convenience type for the delegate to use for custom filter of pickable bodies.
        using FilterDelegate = std::function< Boole(const RayQueryHit&) >;
    protected:
        /// @brief A pointer to the mouse being used for selection.
        Input::Mouse* Selector;
        /// @brief A pointer to the object that will perform the world query.
        RayQuery* Query;
        /// @brief A pointer to the dragger that will move the selected object.
        MousePickDragger* Dragger;
    public:
        /// @brief Blank constructor.
        /// @details The mouse, ray query, and dragger MUST be set before using this class.  This constructor
        /// exists to facilitate delayed construction when used as a non-pointer data member in other classes.
        MousePicker();
        /// @brief Full constructor.
        /// @details The MousePicker does NOT claim ownership over any pointer passed into it.
        /// @param Cursor A pointer to the system mouse.
        /// @param WorldQuery A pointer to the RayQuery to use.
        /// @param WorldDragger A pointer to the MouseDragger to use.
        MousePicker(Input::Mouse* Cursor, RayQuery* WorldQuery, MousePickDragger* WorldDragger);
        /// @brief Class destructor.
        ~MousePicker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization and Deinitialization

        /// @brief Assigns the components to be used by this picker and prepares it for use.
        /// @param Cursor A pointer to the system mouse.
        /// @param WorldQuery A pointer to the RayQuery to use.
        /// @param WorldDragger A pointer to the MouseDragger to use.
        void Initialize(Input::Mouse* Cursor, RayQuery* WorldQuery, MousePickDragger* WorldDragger);
        /// @brief Reverts the picker back to it's pre-init state by NULL'ing all the pointers inside.
        void Deinitialize();

        /// @brief Gets whether or not this Picker has valid pointers and can be used.
        /// @return Returns true if this picker is valid and initialized, false otherwise.
        Boole IsInitialized() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the mouse being used for selection.
        /// @return Returns a pointer to the mouse to use to generate the mouse ray in world space.
        Input::Mouse* GetSelector() const;
        /// @brief Gets the object that will perform the world query.
        /// @return Returns a pointer to the ray query that will find the object on the mouse ray.
        RayQuery* GetQuery() const;
        /// @brief Gets the dragger that will move the selected object.
        /// @return Returns a pointer to the dragger that will move the object found on the mouse ray.
        MousePickDragger* GetDragger() const;

        /// @brief Gets the Ray being cast from the camera position to the world.
        /// @return Returns a Ray being cast to the cursor from the camera in world space.
        Ray GetMouseRay() const;
        /// @brief Gets the world the mouse is currently in.
        /// @return Returns a pointer to the world being shown in the viewport the mouse cursor is over.
        World* GetMouseWorld() const;

        /// @brief Performs all the checks and updates to drag a target under the mouse.
        /// @note When implementing the delegate return true to allow picking to proceed, false to prevent it.  The
        /// delegate will be run on the results of the RayQuery.  If you want to filter sooner than that there you
        /// can pass a filter into the RayQuery itself.
        /// @param Filter A delegate to perform custom filtering of which bodies should and shouldn't be pickable.
        void Execute(const FilterDelegate& Filter);
    };//MousePicker
}//Mezzanine

#endif
