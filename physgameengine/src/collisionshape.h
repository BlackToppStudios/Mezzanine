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
#ifndef _collisionshape_h
#define _collisionshape_h

#include "vector3.h"
#include "quaternion.h"

class btCollisionShape;
class btConvexShape;
class btConcaveShape;
class btCompoundShape;
class btBoxShape;
class btCapsuleShape;
class btConeShape;
class btConvexHullShape;
class btCylinderShape;
class btMultiSphereShape;
class btSphereShape;
class btGImpactMeshShape;
class btHeightfieldTerrainShape;
class btStaticPlaneShape;
class btSoftBodyCollisionShape;
class btBvhTriangleMeshShape;

namespace phys
{
    class CollisionShapeManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CollisionShape
    /// @headerfile collisionshape.h
    /// @brief This is the base class for all collision shapes.
    /// @details Currently there are a total of 13 collision shape classes inheriting from
    /// 3 other base classes.  Collision shapes are shape representations for Actors, AreaEffects,
    /// and other classes with bodies in the physics engine.  @n @n
    /// It's important to note that Collision shapes can be created and then re-used in as many World
    /// objects(at the same time) as you need, and it is encouraged to do this.
    ///////////////////////////////////////
    class PHYS_LIB CollisionShape
    {
        public:
            /// @enum ShapeType
            /// @brief This enum describes what kind of shape you are currently working with.
            enum ShapeType
            {
                ST_Box,
                ST_Capsule,
                ST_Compound,
                ST_Cone,
                ST_ConvexHull,
                ST_Cylinder,
                ST_MultiSphere,
                ST_Sphere,
                ST_GImpact,
                ST_Heightfield,
                ST_Plane,
                ST_ActorSoft,
                ST_TriMesh
            };
        protected:
            friend class CollisionShapeManager;
            btCollisionShape* ShapeBase;
            String Name;
        public:
            /// @brief Class Constructor.
            CollisionShape();
            /// @brief Class Destructor.
            virtual ~CollisionShape();
            /// @brief Gets the name of this shape.
            /// @return Returns a const reference string containing the name of this collision shape.
            virtual const String& GetName();
            /// @brief Sets the padding that will be applied when checking for collisions.
            /// @param Margin A real in world units representing how much padding is to be applied to this shape.
            virtual void SetMargin(const Real& Margin);
            /// @brief Gets the amount of padding currently being applied to the collision shape.
            /// @return Returns the amount of padding, in world units, is being applied to the collision shape.
            virtual Real GetMargin() const;
            /// @brief Scales the collision shape on each of it's axes.
            /// @param Scaling A vector3 representing how much scaling should be applied on each of the shapes 3 axes.
            virtual void SetScaling(const Vector3& Scaling);
            /// @brief Gets the current scaling being applied to the collision shape.
            /// @return Returns a vector3 representing the amount of scaling being applied to the shape.
            virtual Vector3 GetScaling() const;
            /// @brief Gets the type of Collision shape this is.
            /// @return Returns an enum value indicating the type of collision shape this is.
            virtual CollisionShape::ShapeType GetType() const = 0;
            /// @internal
            /// @brief Gets the internal shape pointer this collision shape is based on.
            /// @return Returns a pointer to the internal collision shape.
            virtual btCollisionShape* GetBulletShape() const;
    };//collisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class PrimitiveCollisionShape
    /// @headerfile collisionshape.h
    /// @brief This is the base class for all Primitive shapes.
    /// @details A primitive shape is any shape that has a simple rule for determining it's bounds.
    /// As a rule all Primitive shapes are Convex.  Primitive shapes are ideal for performace
    /// reasons and as such you should always try to appoximate objects with primitives, or a
    /// compound shape of primitives if at all possible.
    ///////////////////////////////////////
    class PHYS_LIB PrimitiveCollisionShape : public CollisionShape
    {
        protected:
            btConvexShape* PrimitiveShapeBase;
            /// @brief Sets the internal pointers on the base classes.
            void SetPointers(btConvexShape* Shape);
        public:
            /// @brief Class Constructor.
            PrimitiveCollisionShape();
            /// @brief Class Destructor.
            virtual ~PrimitiveCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const = 0;
    };//primitivecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class MeshCollisionShape
    /// @headerfile collisionshape.h
    /// @brief This is the base class for all Mesh shapes.
    /// @details A Mesh shape is any shape that uses a series of triangles to determine it's
    /// bounds.  As a rule all Mesh Shapes are considered Concave(even though the triangles don't
    /// need to form a concave shape, there is no way to assure the physics engine of that).  Mesh
    /// shapes are not very performant, and should only be used with static objects, such as terrain.
    ///////////////////////////////////////
    class PHYS_LIB MeshCollisionShape : public CollisionShape
    {
        protected:
            btConcaveShape* MeshShapeBase;
            /// @brief Sets the internal pointers on the base classes.
            void SetPointers(btConcaveShape* Shape);
        public:
            /// @brief Class Constructor.
            MeshCollisionShape();
            /// @brief Class Destructor.
            virtual ~MeshCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const = 0;
    };//meshcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class CompoundCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A collision shape composed of many other collision shapes placed and oriented in local space.
    /// @details This shape is the sum of all it's child shapes.  Unlike the MultiSphereCollisionShape and the
    /// ConvexHullCollisionShape, this shape does not form a convex hull of it's children.  When populating with
    /// primitives, Compound shapes offer the most flexibility with the best performace that you can get. @n @n
    /// When deleted a CompoundCollisionShape will clean up all of it's child shapes.
    ///////////////////////////////////////
    class PHYS_LIB CompoundCollisionShape : public CollisionShape
    {
        protected:
            btCompoundShape* CompoundShape;
            std::vector<CollisionShape*> ChildShapes;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            CompoundCollisionShape(const String& Name);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            CompoundCollisionShape(const String& Name, btCompoundShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~CompoundCollisionShape();
            /// @brief Adds a shape to this compound shape.
            /// @param Child The shape to be added to this shape.
            /// @param ChildLocation The location this child is to have in local space.
            /// @param ChildRotation The rotation to apply to this child in local space.
            virtual void AddChildShape(CollisionShape* Child, const Vector3& ChildLocation, const Quaternion& ChildRotation);
            /// @brief Adds a shape to this compound shape.
            /// @param Child The shape to be added to this shape.
            /// @param ChildLocation The location this child is to have in local space.
            virtual void AddChildShape(CollisionShape* Child, const Vector3& ChildLocation);
            /// @brief Gets the number of children belonging to this compound shape.
            /// @return Returns the number of children belonging to this shape.
            virtual Whole GetNumChildren();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//compoundcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class BoxCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A box physics shape.
    /// @details This is exactly what it sounds like.  A primitive shape of a box that is constructed
    /// by it's half extents.  Half extents can't be negative, but otherwise can be any value.
    ///////////////////////////////////////
    class PHYS_LIB BoxCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btBoxShape* BoxShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this shape.
            /// @param HalfExtents A vector3 representing half the shapes size on each axis.
            BoxCollisionShape(const String& Name, const Vector3& HalfExtents);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this shape.
            /// @param BulletShape The internal shape this shape is based on.
            BoxCollisionShape(const String& Name, btBoxShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~BoxCollisionShape();
            /// @brief Gets the half extents used to construct this box.
            /// @return Returns a vector3 containing the half extents of this box.
            virtual Vector3 GetHalfExtents() const;
            /// @brief Gets the half extents with padding.
            /// @return Returns a vector3 containing the half extents with the margin added to each axis.
            virtual Vector3 GetHalfExtentsWithMargin() const;
            /// @brief Checks to see if a point in local space is inside this shape.
            /// @return Returns a bool indicating whether or not the specified point is inside the shape.
            /// @param Location The specified point to perform the check.
            /// @param Tolorance The amount of leeway to give in the check.  If the distance from the
            /// specified point is equal or less then the Tolorance provided then this will return true.
            virtual bool IsInside(const Vector3& Location, const Real& Tolorance) const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//boxcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class CapsuleCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A capsule physics shape.
    /// @details This shape is commonly used for characters.  Keep in mind the total height of any
    /// capsule is height+2*radius, since the height provided only applies to the cylinder portion
    /// of the capsule.  Like Cones and Cylinders, Capsules can be aligned to any of the 3 linear
    /// axes(X, Y, or Z).
    ///////////////////////////////////////
    class PHYS_LIB CapsuleCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btCapsuleShape* CapsuleShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the capsule.
            /// @param Height The height of the cylindrical portion of the capsule.  Total height would be Height+2*radius.
            /// @param UpAxis Which axis the Capsule is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~CapsuleCollisionShape();
            /// @brief Gets the radius of the capsule.
            /// @return Returns a real representing the radius of the capsule.
            virtual Real GetRadius() const;
            /// @brief Gets the height of the capsule.
            /// @return Returns a real representing the length of the cylindrical portion of the capsule.
            virtual Real GetHeight() const;
            /// @brief Gets which axis this capsule is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//capsulecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ConeCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A cone physics shape.
    /// @details Keep in mind when building a cone shape, the provided radius is for the base of the cone,
    /// but the pivot point of the shape is still at it's center and not it's base.  Like Capsules and Cylinders,
    /// Cones can be aligned to any of the 3 linear axes(X, Y, or Z).
    ///////////////////////////////////////
    class PHYS_LIB ConeCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btConeShape* ConeShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the base of the Cone.
            /// @param Height The height of the Cone.
            /// @param UpAxis Which axis the Cone is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            ConeCollisionShape(const String& Name, btConeShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~ConeCollisionShape();
            /// @brief Gets the radius of the cone.
            /// @return Returns a real representing the radius at the base of the cone.
            virtual Real GetRadius() const;
            /// @brief Gets the height of the cone.
            /// @return Returns a real representing height of the cone.
            virtual Real GetHeight() const;
            /// @brief Gets which axis this cone is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//conecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ConvexHullCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A simple convex shape built from a low number of points in local space.
    /// @details A convex hull is commonly used to generate a simple approximation of another, and
    /// more complicated, shape.  Usually the number of points in a convex hull doesn't exceed 32.  @n @n
    /// When a convex hull is generated automatically from an algorithm, the best way to think of
    /// the resulting shape in relation to the original shape is if you were to wrap a rubber band
    /// around it on one plane.  Then do this for all planes and all surfaces.  The resulting shape looks
    /// and behaves similar to that.
    ///////////////////////////////////////
    class PHYS_LIB ConvexHullCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btConvexHullShape* ConvexHullShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Points A vector of vector3's containing all the points in local space to construct this shape from.
            ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~ConvexHullCollisionShape();
            /// @brief Adds a point to this shape.
            /// @param Point The location in local space to be added.
            virtual void AddPoint(const Vector3& Point);
            /// @brief Gets a stored point in this ConvexHull.
            /// @return Returns a Vector3 representing the location in local space of the specified point.
            /// @param Index The index of the point desired.
            virtual Vector3 GetPoint(const Whole& Index) const;
            /// @brief Gets the total number of points being stored in this shape.
            /// @return Returns a whole representing the current number of points in this shape.
            virtual Whole GetNumPoints() const;
            /// @brief Checks to see if a point in local space is inside this shape.
            /// @return Returns a bool indicating whether or not the specified point is inside the shape.
            /// @param Location The specified point to perform the check.
            /// @param Tolorance The amount of leeway to give in the check.  If the distance from the
            /// specified point is equal or less then the Tolorance provided then this will return true.
            virtual bool IsInside(const Vector3& Location, const Real& Tolorance) const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//convexhullcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class CylinderCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A cylinder physics shape.
    /// @details A primitive cylinder shape.  Like Capsules and Cones Cylinders can be
    /// aligned to any of the 3 linear axes(X, Y, or Z).
    ///////////////////////////////////////
    class PHYS_LIB CylinderCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btCylinderShape* CylinderShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the Cylinder.
            /// @param Height The height of the Cylinder.
            /// @param UpAxis Which axis the cylinder is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~CylinderCollisionShape();
            /// @brief Gets the half extents used to construct this cylinder.
            /// @return Returns a vector3 containing the half extents of this cylinder.
            virtual Vector3 GetHalfExtents() const;
            /// @brief Gets the half extents with padding.
            /// @return Returns a vector3 containing the half extents with the margin added to each axis.
            virtual Vector3 GetHalfExtentsWithMargin() const;
            /// @brief Gets the radius of the cylinder.
            /// @return Returns a real representing the radius at the base of the cylinder.
            virtual Real GetRadius() const;
            /// @brief Gets which axis this cylinder is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//cylindercollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class MultiSphereCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A physics shape comprised of multiple sphere's placed in local space.
    /// @details It's important to note that this shape isn't just a bunch of placed sphere's, but
    /// a convex hull comprised of all the provided sphere's.  You can make a capsule shape with
    /// two sphere's, one above the other(internally this is how it works).
    ///////////////////////////////////////
    class PHYS_LIB MultiSphereCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btMultiSphereShape* MultiSphereShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~MultiSphereCollisionShape();
            /// @brief Gets the location(in local space) of the sphere at the specified index.
            /// @return Returns a vector3 representing the location of the specified sphere.
            /// @param Index The index of the desired sphere.
            virtual Vector3 GetSphereLocation(const Whole& Index) const;
            /// @brief Gets the radius of the sphere at the specified index.
            /// @return Returns a Real representing the radius of the specified sphere.
            /// @param Index The index of the desired sphere.
            virtual Real GetSphereRadius(const Whole& Index) const;
            /// @brief Gets the number of spheres contained within this shape.
            /// @return Returns a Whole representing the number of spheres in this shape.
            virtual Whole GetNumSpheres() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//multispherecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class SphereCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A sphere physics shape.
    /// @details A simple sphere defined by it's radius.  'nuff said.
    ///////////////////////////////////////
    class PHYS_LIB SphereCollisionShape : public PrimitiveCollisionShape
    {
        protected:
            btSphereShape* SphereShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the sphere.
            SphereCollisionShape(const String& Name, const Real& Radius);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            SphereCollisionShape(const String& Name, btSphereShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~SphereCollisionShape();
            /// @brief Gets the radius of the sphere.
            /// @return Returns a real representing the radius of the sphere.
            virtual Real GetRadius() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//spherecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DynamicMeshCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A triangle mesh collsion shape based on a graphics mesh.
    /// @details As the name suggests, this is a TriMesh shape intended for dynamic objects.  This
    /// can be used with static objects, but using the StaticMeshCollisionShape class instead will
    /// yeild much better performance.  @n @n
    /// In general, this collision shape will provide "pixel-perfect" collisions, but this comes at
    /// a steep performance cost.  You can't have too many objects with this kind of shape before
    /// running into noticable performance drops.  This class should be avoided in real-time applications
    /// if at all possible.  Primitives provide much better performance.
    ///////////////////////////////////////
    class PHYS_LIB DynamicMeshCollisionShape : public MeshCollisionShape
    {
        protected:
            btGImpactMeshShape* GImpactShape;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            DynamicMeshCollisionShape(const String& Name, btGImpactMeshShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~DynamicMeshCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//gimpactcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class HeightfieldCollisionShape
    /// @headerfile collisionshape.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB HeightfieldCollisionShape : public MeshCollisionShape
    {
        protected:
            btHeightfieldTerrainShape* HeightfieldShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            HeightfieldCollisionShape(const String& Name);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            HeightfieldCollisionShape(const String& Name, btHeightfieldTerrainShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~HeightfieldCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//heightfieldcollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class PlaneCollisionShape
    /// @headerfile collisionshape.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB PlaneCollisionShape : public MeshCollisionShape
    {
        protected:
            btStaticPlaneShape* PlaneShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            PlaneCollisionShape(const String& Name);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~PlaneCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//planecollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorSoftCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A collision shape for soft actors.
    /// @details This collision shape is different from all the other collision shapes because it's
    /// meant to be used for only one type of object: ActorSoft's.  This shape doesn't use triangles
    /// in the traditional manner other mesh shapes do, but rather it uses the individual internal
    /// nodes that make the ActorSoft as points for building triangles to process collisions with.  As
    /// a result a SoftBodyCollisionShape should never be reused and should be kept unique to the Actor
    /// it applies to. @n @n
    /// Normally the ActorSoft will be responsible for the creation of it's own collision shape, so
    /// the user should never have to.  So there are no non-internal constructors provided for this class.
    ///////////////////////////////////////
    class PHYS_LIB ActorSoftCollisionShape : public MeshCollisionShape
    {
        protected:
            btSoftBodyCollisionShape* ActorSoftShape;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            ActorSoftCollisionShape(const String& Name, btSoftBodyCollisionShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~ActorSoftCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//softbodycollisionshape

    ///////////////////////////////////////////////////////////////////////////////
    /// @class StaticMeshCollisionShape
    /// @headerfile collisionshape.h
    /// @brief A triangle mesh collsion shape based on a graphics mesh.
    /// @details As the name suggests, this is a TriMesh shape intended for static objects.  If this
    /// shape is used in a dynamic object, it will simply not provide any collisions with other objects. @n @n
    /// This shape will provide "pixel-perfect" collisions like the DynamicMeshCollisionShape class, but
    /// is much more performant at the cost of only being usable on static objects.
    ///////////////////////////////////////
    class PHYS_LIB StaticMeshCollisionShape : public MeshCollisionShape
    {
        protected:
            btBvhTriangleMeshShape* BvhShape;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            StaticMeshCollisionShape(const String& Name, btBvhTriangleMeshShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~StaticMeshCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
    };//trimeshcollisionshape
}//phys

#endif
