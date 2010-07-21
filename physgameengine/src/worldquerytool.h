//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _worldquerytool_h
#define _worldquerytool_h

#include "world.h"
#include "actorbase.h"
#include "vector3wactor.h"
#include "metacode.h"
#include "ray.h"
#include "plane.h"

#include <bitset>

namespace Ogre
{
    class RaySceneQuery;
    class Vector3;
    class Quaternion;
    class Entity;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldQueryTool
    /// @headerfile worldquerytool.h
    /// @brief This provides a number of optional tools for working with a phys::World
    /// @details Currently this allows for more seamless mouse use, including 'picking'
    /// of objects with the mouse, and associated functionality.
    ///////////////////////////////////////
    class WorldQueryTool
    {
        private:
            /// @internal
            /// @brief This is gameworld we will be querying
            World* GameWorld;

            /// @internal
            /// @brief This holds the Mouse X coordinate as of the last time Gather Events was called
            Whole MouseXCache;

            /// @internal
            /// @brief This holds the Mouse Y coordinate as of the last time Gather Events was called
            Whole MouseYCache;

            /// @internal
            /// @brief This is the mouse button limit of this class.
            const static unsigned short int MouseButtonLimit = 16;

            /// @internal
            /// @brief A place to store which mouse buttons are pushed.
            std::bitset<MouseButtonLimit> MouseButtonCache;

            /// @internal
            /// @brief A place to store which keys are pressed or not.
            std::bitset<MetaCode::KEY_LAST> KeyboardButtonCache;

            /// @internal
            /// @brief An Ogre construct used to find information about the locations of graphical object
            Ogre::RaySceneQuery* RayQuery;


            /// @internal
            /// @brief used to get information about 3d graphical objects at specific locations
            void GetMeshInformation(Ogre::Entity *entity,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);


        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Basic Stuff
            ///////////////////////////////////////
            /// @brief Basic Constructor.
            /// @details This creates a WorldQueryTool Ready to run queries on the the world you pass it.
            /// @param GameWorld_ This is a pointer to the phys::World to be queried
            WorldQueryTool(World* GameWorld_);

            /// @brief Destructor
            /// @details Deletes everything in the world query tool.
            ~WorldQueryTool();

            ///////////////////////////////////////////////////////////////////////////////
            // Simple User Input Queries
            ///////////////////////////////////////
            /// @brief This gets the X coordinate of the mouse
            /// @details This gets the X location of this mouse. This runs in constant time.
            /// @return This returns a Whole number which represents the X coordinate of the mouse.
            Whole GetMouseX();

            /// @brief This gets the Y coordinate of the mouse
            /// @details This gets the Y location of this mouse. This runs in constant time.
            /// @return This returns a Whole number which represents the Y coordinate of the mouse.
            Whole GetMouseY();

            /// @brief Returns whether a specific Mouse button is pushed
            /// @details This runs in constant time and returns a true is the requested mouse button is pressed. Buttons that are being pressed
            /// are considered pressed, and buttons that are being lifted are considered unpressed. This only supports the first 16 mouse buttons
            /// at this point (numbered 0 to 15)
            /// @param MouseButton This is the mouse button that is being checked
            /// @exception "Unsupported mouse button access through WorldQueryTool" This is thrown whenever a mouse button is requested that is beyond the limit that is supported. Currently this limit is 16
            /// @return This returns a bool which is set to true if the requested button is pressed or held down, and false otherwise.
            bool IsMouseButtonPushed(short unsigned int MouseButton);

            /// @brief Returns whether a specific KEyboard button is pushed
            /// @details This runs in constant time and returns a true is the requested mouse button is pressed. Buttons that are being pressed
            /// are considered pressed, and buttons that are being lifted are considered unpressed.
            /// @param KeyboardButton This is the button that is being checked.
            // This functionality has not yet been implemented @exception "Unsupported Keyboard button" This is thrown whenever a button is requested that is beyond the limit that is supported. See
            /// @return This returns a bool which is set to true if the requested button is pressed or held down, and false otherwise.
            bool IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton);

            ///////////////////////////////////////////////////////////////////////////////
            // World Ray Queries
            ///////////////////////////////////////
            /// @brief This will find the first Actor to intesect the Given ray.
            /// @details This use the graphics subsystem to cast a ray in the world. If the ray passes through any the triangles in an actor
            /// This will return that actor. This function runs in linear time relative to the amount of triangles in 3d meshes near the the
            /// ray being cast. This will start at ray.from and go to ray.to .
            /// @param ActorRay The Ray to search along.
            /// @return This returns a pointer to an Vector3WActor, which contains the first actor along the ray and the point of intersection Relative to the actor
            Vector3WActor* GetFirstActorOnRayByPolygon(Ray ActorRay);

            /// @brief Partially implemented. This should find the first Actor that is on or almost on the a given Ray.
            /// @details This casts a ray through the gameworld. The first actor with an Axis Aligned Bounding Box that intersects that ray is returned.
            /// Presently this is untested and does not return the Offset of the intersection. This should perform faster than WorldQueryTool::GetFirstActorOnRayByPolygon
            /// but not by a known amount.
            /// @param ActorRay The Ray to search along.
            /// @return This returns a pointer to an actorbase, which is the first actor to have an Axis-Aligned Bounding Box along the ray.
            Vector3WActor* GetFirstActorOnRayByAABB(Ray ActorRay);

            /// @brief This will find the actor under the mouse.
            /// @details This will use
            /// @param UsePolygon If true this will use GetFirstActorOnRayByPolygon, otherwise this will use GetFirstActorOnRayByAABB .
            /// @param RayLength The length of the ray to cast from te mouse to search for actors. This defaults to 1000.0
            /// @return This returns a Vector3WActor which has a pointer to the actor under the mouse, and a vector representing the distance of the mouse fromt the center of mass.
            Vector3WActor* GetActorUnderMouse(Real RayLength=1000.0, bool UsePolygon=true);

            /// @brief Where does this Ray Meet this Plane?
            /// @details This does some fancy math to return the point where the ray and the plane intersent.
            /// This returns a NULL pointer in the event the ray and plane do not meet, or are completely coterminous.
            /// @param QueryRay This is the Ray that could intersent the plane
            /// @param QueryPlane This is the plane to be interesected.
            /// @return This returns a pointer to a vector that matches where the intersection of the plane and Ray, if possible
            Vector3* RayPlaneIntersection(const Ray &QueryRay, const Plane &QueryPlane);

            /// @brief Get a Ray from the current viewport, following the mouse
            /// @details This calls on the graphics subsystem to get a ray from the location of the current camera
            /// @param Length how long of a ray do you want? Thsi defaults to 1000
            /// @return This returns a ray that matches originates at the camera and goes out in 3d space behind the mouse pointer.
            Ray* GetMouseRay(Real Length=1000);

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Stuff
            ///////////////////////////////////////
            /// @brief This gathers any user-input/event data that might be queryed
            /// @details This should be called periodcally (ideally in the post user input callback) to allow this
            /// to gather data from the phys::World 's event manager. When called this will drop prior event data
            /// and any relevant queries will come from this new data. At the caller's discretion this method
            /// can properly delete any events pulled from the event manager. \n \n
            /// This runs in linear time relative to the events in the event manager. This will usually be a trivial amount
            /// if this is run each iteration and excess events are removed (either by this method or some other form of event cleanup)
            /// @param ClearEventsFromEventMgr If set to true, This method will properly remove any events it pulls from the event manager.
            void GatherEvents(bool ClearEventsFromEventMgr = false);
    };
}



#endif
