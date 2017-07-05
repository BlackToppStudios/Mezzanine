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
#ifndef _mousepickdragger_h
#define _mousepickdragger_h

#include "transform.h"
#include "rayquery.h"
#include "plane.h"
#include "Physics/physicsenumerations.h"

namespace Mezzanine
{
    class World;
    class Entity;
    namespace Input
    {
        class Mouse;
    }
    namespace Physics
    {
        class CollidableProxy;
        class RigidProxy;
        class Generic6DofConstraint;
        class Point2PointConstraint;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Interface class for dragging an object in world space via the mouse.
    ///////////////////////////////////////
    class MEZZ_LIB MousePickDragger
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Types

        /// @brief Convenience type for the delegate to use for custom filter of draggable bodies.
        using FilterDelegate = std::function< Boole(const RayQueryHit&) >;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Class destructor.
        virtual ~MousePickDragger() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Determines a single target to be dragged from a container of candidates.
        /// @param Targets The container of candidates to choose from.
        /// @return Returns a pointer to the proxy that will be dragged.
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets) const = 0;
        /// @brief Determines a single target to be dragged from a container of candidates.
        /// @param Targets The container of candidates to choose from.
        /// @param Filter A delegate to perform custom filtering of which bodies should and shouldn't be draggable.
        /// @return Returns a pointer to the proxy that will be dragged.
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets, const FilterDelegate& Filter) const = 0;

        /// @brief Checks the parent Entity (if available) for more suitable proxies to be dragged.
        /// @param Target A pointer to the target to be dragged.
        /// @return Returns a pointer to a sibling EntityProxy from the parent Entity if a better match is found, or the Target parameter if not.
        virtual EntityProxy* GetBestProxy(EntityProxy* Target) const = 0;
        /// @brief Gets the object currently being dragged by this dragger.
        /// @return Returns a pointer to the object currently being dragged, or NULL if no object is being dragged.
        virtual EntityProxy* GetCurrentTarget() const = 0;
        /// @brief Gets the last object that was dragged by this dragger.
        /// @return Returns a pointer to the last EntityProxy moved by this dragger.
        virtual EntityProxy* GetLastTarget() const = 0;

        /// @brief Gets whether or not the dragger is currently dragging a target.
        /// @return Returns true if an object is currently being manipulated by this dragger, false otherwise.
        virtual Boole IsDragging() const = 0;

        /// @brief Initializes anything needed to begin dragging the object specified.
        /// @param Target The object to start dragging.
        /// @param Offset The local space location of where the object was "grabbed".
        /// @param MouseRay A ray cast from the mouse position on the screen to the world.
        /// @return Returns true if an object was grappled for dragging, false if the target failed to meet draggable criteria.
        virtual Boole StartDragging(EntityProxy* Target, const Vector3& Offset, const Ray& MouseRay) = 0;
        /// @brief Moves the object being dragged according to the mouse movement.
        /// @param MouseRay A ray cast from the mouse position on the screen to the world.
        /// @param Cursor A pointer to the mouse being used to drag the target.
        virtual void ContinueDragging(const Ray& MouseRay, Input::Mouse* Cursor) = 0;
        /// @brief Terminates the dragging and releases the target.
        virtual void StopDragging() = 0;
    };//MousePickDragger

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A MousePickDragger that will use a Physics Constraint to drag a target along an imaginary plane.
    /// @todo Update to allow dragging of SoftBodies.
    ///////////////////////////////////////
    class MEZZ_LIB PlaneDragger : public MousePickDragger
    {
    protected:
        /// @brief The imaginary plane to drag along.
        Plane DragPlane;
        /// @brief The activation state of the target to be restored after dragging completes.
        Physics::ActivationState SavedActivationState;
        /// @brief A pointer to the constraint doing our dragging.
        Physics::Generic6DofConstraint* Dragger;
        /// @brief A pointer to the CollidableProxy being dragged.
        Physics::CollidableProxy* LastTarget;

        /// @brief Creates a new dragger constraint.
        /// @param Target A pointer to the proxy/object being dragged.
        /// @param LocalTrans The point in local space where the dragger is attaching itself to the target.
        /// @return Returns a pointer to the created dragger.
        Physics::Generic6DofConstraint* CreateDragger(Physics::RigidProxy* Target, const Transform& LocalTrans);
    public:
        /// @brief Class constructor.
        /// @param ThePlane The plane to drag the object along.
        PlaneDragger(const Plane& ThePlane);
        /// @brief Class destructor.
        virtual ~PlaneDragger() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the plane to be used for determining the target location as the mouse moves.
        /// @param ThePlane The plane to drag the object along.
        void SetDragPlane(const Plane& ThePlane);
        /// @brief Gets the plane being used for determining the target location as the mouse moves.
        /// @return Returns a const reference to the Plane that is used for intersection tests with the mouse ray.
        const Plane& GetDragPlane() const;

        /// @brief Gets the constraint dragging the target.
        /// @return Returns a pointer to the physics constraint moving the target.
        Physics::Generic6DofConstraint* GetDraggingConstraint() const;

        /// @copydoc MousePickDragger::GetBestQueryHit(const RayQuery::ResultContainer&) const
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets) const;
        /// @copydoc MousePickDragger::GetBestQueryHit(const RayQuery::ResultContainer&) const
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets, const FilterDelegate& Filter) const;
        /// @copydoc MousePickDragger::GetBestProxy(EntityProxy*) const
        virtual EntityProxy* GetBestProxy(EntityProxy* Target) const;
        /// @copydoc MousePickDragger::GetCurrentTarget() const
        virtual EntityProxy* GetCurrentTarget() const;
        /// @copydoc MousePickDragger::GetLastTarget() const
        virtual EntityProxy* GetLastTarget() const;

        /// @copydoc MousePickDragger::IsDragging() const
        virtual Boole IsDragging() const;

        /// @copydoc MousePickDragger::StartDragging(EntityProxy*, const Vector3&, const Ray&)
        virtual Boole StartDragging(EntityProxy* Target, const Vector3& Offset, const Ray& MouseRay);
        /// @copydoc MousePickDragger::ContinueDragging(const Ray&, Input::Mouse*)
        virtual void ContinueDragging(const Ray& MouseRay, Input::Mouse* Cursor);
        /// @copydoc MousePickDragger::StopDragging()
        virtual void StopDragging();
    };//PlaneDragger

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A MousePickDragger that will keep the target at a fixed distance along the mouse ray.
    /// @todo Update to allow dragging of SoftBodies.
    ///////////////////////////////////////
    class MEZZ_LIB DistanceDragger : public MousePickDragger
    {
    protected:
        /// @brief The fixed distance to keep the drag target at on the mouse ray.
        Real DragDistance;
        /// @brief The amount to change the DragDistance when it is incremented or decremented.
        Real DragIncrement;
        /// @brief The activation state of the target to be restored after dragging completes.
        Physics::ActivationState SavedActivationState;
        /// @brief A pointer to the constraint doing our dragging.
        Physics::Generic6DofConstraint* Dragger;
        /// @brief A pointer to the CollidableProxy being dragged.
        Physics::CollidableProxy* LastTarget;

        /// @brief Creates a new dragger constraint.
        /// @param Target A pointer to the proxy/object being dragged.
        /// @param LocalTrans The point in local space where the dragger is attaching itself to the target.
        /// @return Returns a pointer to the created dragger.
        Physics::Generic6DofConstraint* CreateDragger(Physics::RigidProxy* Target, const Transform& LocalTrans);
    public:
        /// @brief Class constructor.
        /// @param DistIncrement The amount to change the DragDistance when it is incremented or decremented.
        DistanceDragger(const Real DistIncrement);
        /// @brief Class destructor.
        virtual ~DistanceDragger() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the amount to adjust the drag distance when incremented or decremented.
        /// @param DistIncrement The amount to change the DragDistance when it is incremented or decremented.
        void SetDragIncrement(const Real DistIncrement);
        /// @brief Gets the amount to adjust the drag distance when incremented or decremented.
        /// @return Returns a Real representing the amount to change the world unit distance when incrementing to decrementing.
        Real GetDragIncrement() const;

        /// @brief Increments the distance the dragged target will be kept from the camera.
        void IncrementDistance();
        /// @brief Decrements the distance the dragger target will be kept from the camera.
        void DecrementDistance();

        /// @brief Gets the constraint dragging the target.
        /// @return Returns a pointer to the physics constraint moving the target.
        Physics::Generic6DofConstraint* GetDraggingConstraint() const;

        /// @copydoc MousePickDragger::GetQueryHit(const RayQuery::ResultContainer&) const
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets) const;
        /// @copydoc MousePickDragger::GetBestQueryHit(const RayQuery::ResultContainer&) const
        virtual RayQueryHit GetBestQueryHit(const RayQuery::ResultContainer& Targets, const FilterDelegate& Filter) const;
        /// @copydoc MousePickDragger::GetBestProxy(EntityProxy*) const
        virtual EntityProxy* GetBestProxy(EntityProxy* Target) const;
        /// @copydoc MousePickDragger::GetCurrentTarget() const
        virtual EntityProxy* GetCurrentTarget() const;
        /// @copydoc MousePickDragger::GetLastTarget() const
        virtual EntityProxy* GetLastTarget() const;

        /// @copydoc MousePickDragger::IsDragging() const
        virtual Boole IsDragging() const;

        /// @copydoc MousePickDragger::StartDragging(EntityProxy*, const Vector3&, const Ray&)
        virtual Boole StartDragging(EntityProxy* Target, const Vector3& Offset, const Ray& MouseRay);
        /// @copydoc MousePickDragger::ContinueDragging(const Ray&, Input::Mouse*)
        virtual void ContinueDragging(const Ray& MouseRay, Input::Mouse* Cursor);
        /// @copydoc MousePickDragger::StopDragging()
        virtual void StopDragging();
    };//DistanceDragger
}//Mezzanine

#endif
