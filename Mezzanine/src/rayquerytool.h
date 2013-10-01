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
#ifndef _rayquerytool_h
#define _rayquerytool_h

#include "datatypes.h"
#include "crossplatformexport.h"
#include "vector3.h"

namespace Ogre
{
    class RaySceneQuery;
    class Vector3;
    class Quaternion;
    class Entity;
}

namespace Mezzanine
{
    class Plane;
    class Vector3WActor;
    class Ray;
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class RayQueryTool
    /// @headerfile rayquerytool.h
    /// @brief This provides a number of optional tools for working with a Mezzanine::World
    /// @details Currently this allows for more seamless mouse use, including 'picking'
    /// of objects with the mouse, and associated functionality.
    ///////////////////////////////////////
    class MEZZ_LIB RayQueryTool// : public InputQueryTool
    {
        private:
            /// @brief True if the last Query yielded a meaningful Result.
            bool ValidResult;
            /// @brief An Offset or location from something meaningful to the last query.
            Vector3 Offset;
            /// @brief If there is an Actor result to a query the result will be stored in this.
            /// @warning This is weak reference, Some other system could delete out from under us and we cannot delete this.
            ActorBase* IntersectedActor;

        public:

            /// @brief Create a RayQueryTool ready for querying
            RayQueryTool();

            ///////////////////////////////////////////////////////////////////////////////
            // Query Results
            ///////////////////////////////////////
            /// @brief Clears an previously stored return values.
            void ClearReturns();
            /// @brief Check to see if the last query found anything.
            /// @return True if something was found, false otherwise.
            bool LastQueryResultsValid() const;
            /// @brief Get an offset from the last query. Depending on the last query, this could be an Offset from a variety of things.
            /// @return A Vector3 if the last query worked and returns an Offset, A empty vector otherwise. Use LastQueryResultsValid() Prior to this.
            Vector3 LastQueryResultsOffset() const;
            /// @brief It is common to ray query for actors, if so the results can be retrieved with this.
            /// @return A pointer to an actor if the last query returns an actor and worked, a Null pointer otherwise. Use LastQueryResultsValid() Prior to this.
            /// @warning Does not confer ownership of actor, do not delete pointers returned by this unless you have taken special steps.
            ActorBase* LastQueryResultsActorPtr() const;

        protected:
            ///////////////////////////////////////////////////////////////////////////////
            // Query Helpers
            ///////////////////////////////////////


        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Ray Queries
            ///////////////////////////////////////
            /// @brief This will find the first Actor to intesect the Given ray.
            /// @details This use the graphics subsystem to cast a ray in the world. If the ray passes through any the triangles in an actor
            /// This will return that actor. This function runs in linear time relative to the amount of triangles in 3d meshes near the the
            /// ray being cast. This will start at ray.from and go to ray.to .
            /// @param ActorRay The Ray to search along.
            /// @param ObjectFlags A whole comprising all the valid objects to be checked in the scene.
            /// See WorldAndSceneObjectType in enumerations.h for a listing of objects to use as flags.
            /// @return This returns a pointer to an Vector3WActor, which contains the first actor along the ray and the point of intersection Relative to the actor
            static Vector3WActor GetFirstActorOnRayByPolygon(Ray ActorRay, Whole ObjectFlags);

            /// @brief Partially implemented. This should find the first Actor that is on or almost on the a given Ray.
            /// @details This casts a ray through the gameworld. The first actor with an Axis Aligned Bounding Box that intersects that ray is returned.
            /// Presently this is untested and does not return the Offset of the intersection. This should perform faster than WorldQueryTool::GetFirstActorOnRayByPolygon
            /// but not by a known amount.
            /// @param ActorRay The Ray to search along.
            /// @param ObjectFlags A whole comprising all the valid objects to be checked in the scene.
            /// See WorldAndSceneObjectType in enumerations.h for a listing of objects to use as flags.
            /// @return This returns a pointer to an actorbase, which is the first actor to have an Axis-Aligned Bounding Box along the ray.
            static Vector3WActor GetFirstActorOnRayByAABB(Ray ActorRay, Whole ObjectFlags);

            /// @brief This will find the actor under the mouse.
            /// @details This will use
            /// @param ObjectFlags A whole comprising all the valid objects to be checked in the scene.
            /// See WorldAndSceneObjectType in enumerations.h for a listing of objects to use as flags.
            /// @param RayLength The length of the ray to cast from te mouse to search for actors. This defaults to 1000.0.
            /// @param UsePolygon If true this will use GetFirstActorOnRayByPolygon, otherwise this will use GetFirstActorOnRayByAABB.
            /// @return This returns a Vector3WActor which has a pointer to the actor under the mouse, and a vector representing the distance of the mouse fromt the center of mass.
            static Vector3WActor GetActorUnderMouse(Whole ObjectFlags, Real RayLength=1000.0, bool UsePolygon=true);

            /// @brief Where does this Ray Meet this Plane?
            /// @details This does some fancy math to return the point where the ray and the plane intersent.
            /// This returns a NULL pointer in the event the ray and plane do not meet, or are completely coterminous.
            /// @param QueryRay This is the Ray that could intersent the plane
            /// @param QueryPlane This is the plane to be interesected.
            /// @return This returns a pointer to a vector that matches where the intersection of the plane and Ray, if possible
            static Vector3 RayPlaneIntersection(const Ray &QueryRay, const Plane &QueryPlane);

            /// @brief Get a Ray from the current viewport, following the mouse
            /// @details This calls on the graphics subsystem to get a ray from the location of the current camera
            /// @param Length how long of a ray do you want? Thsi defaults to 1000
            /// @return This returns a ray that matches originates at the camera and goes out in 3d space behind the mouse pointer.
            static Ray GetMouseRay(Real Length=1000);
    };
}



#endif
