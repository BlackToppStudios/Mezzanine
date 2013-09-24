//c Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

The Mezzanine Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Mezzanine Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with The Mezzanine Engine. If not, see <http://www.gnu.org/licenses/>.
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
#ifndef _physicsproxy_cpp
#define _physicsproxy_cpp

#include "Physics/physicsproxy.h"
#include "Physics/collisionshape.h"
#include "Physics/physicsmanager.h"
#include "Physics/collisionshapemanager.h"

#include <btBulletDynamicsCommon.h>

namespace
{
    Mezzanine::Bool IsConvex(const Mezzanine::Physics::CollisionShape::ShapeType Type)
    {
        switch( Type )
        {
            // All the basic convex shapes
            case Mezzanine::Physics::CollisionShape::ST_Box:
            case Mezzanine::Physics::CollisionShape::ST_Capsule:
            case Mezzanine::Physics::CollisionShape::ST_Cone:
            case Mezzanine::Physics::CollisionShape::ST_ConvexHull:
            case Mezzanine::Physics::CollisionShape::ST_Cylinder:
            case Mezzanine::Physics::CollisionShape::ST_MultiSphere:
            case Mezzanine::Physics::CollisionShape::ST_Sphere:
            {
                return true;
                break;
            }
            default:
            {
                return false;
                break;
            }
        }
    }

    Mezzanine::Bool IsCompound(const Mezzanine::Physics::CollisionShape::ShapeType Type)
    {
        return Type == Mezzanine::Physics::CollisionShape::ST_Compound;
    }

    Mezzanine::Bool IsStaticTrimesh(const Mezzanine::Physics::CollisionShape::ShapeType Type)
    {
        return Type == Mezzanine::Physics::CollisionShape::ST_StaticTriMesh;
    }

    Mezzanine::Bool IsDynamicTrimesh(const Mezzanine::Physics::CollisionShape::ShapeType Type)
    {
        return Type == Mezzanine::Physics::CollisionShape::ST_DynamicTriMesh;
    }
}

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // ScalingShape Methods
/// @cond 0
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ScalingShape
        /// @brief This is a custom scaling shape that permits scaling specific to the object it is applied to.
        /// @details Scaling portion of a transform does not exist on RigidBodies in Bullet.  For an object to be
        /// scaled it has to be done on the collision shape.  However collision shapes can be shared and if they are
        /// it would scale all other objects it is shared with as well.  This shape is a simple scaling wrapper for
        /// collision shapes that can be created just for a single object that allows re-use and sharing of
        /// collision shapes when scaling them for different objects. @n @n
        /// Bullet does have another scaling shape built in for different types of shapes.  GImpact lacks one, but
        /// BVHTriangleMesh have an appropriate wrapper.  All the collision shapes under the Convex branch of the
        /// inheritance tree have another that only applies custom scaling uniformly on all axes.  This is
        /// unacceptable and one that allows independant scaling on each axis is needed.  That is where this class
        /// comes in.
        ///////////////////////////////////////
        ATTRIBUTE_ALIGNED16(class) MEZZ_LIB ScalingShape : public btConvexShape
        {
        protected:
            /// @brief The actual data in bullet this represents
            btConvexShape* ChildConvexShape;

            /// @brief The actual data in bullet this represents
            btVector3 ChildScaling;
        public:
            /// @cond false
            BT_DECLARE_ALIGNED_ALLOCATOR();
            /// @endcond

            /// @brief Class constructor.
            ScalingShape(btConvexShape* ChildShape, const btVector3& Scaling) :
                ChildConvexShape(ChildShape),
                ChildScaling(Scaling)
                { this->m_shapeType = UNIFORM_SCALING_SHAPE_PROXYTYPE; }
            /// @brief Class destructor.
            virtual ~ScalingShape()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            /// @brief Sets the child shape to be scaled by this wrapper.
            virtual void SetChildShape(btConvexShape* ChildShape)
            {
                this->ChildConvexShape = ChildShape;
            }
            /// @brief Gets the child shape being scaled by this wrapper.
            virtual btConvexShape* GetChildShape() const
            {
                return this->ChildConvexShape;
            }
            /// @brief Sets the amount of scaling to be applied to the child shape.
            virtual void SetChildScaling(const btVector3& Scaling)
            {
                this->ChildScaling = Scaling;
            }
            /// @brief Gets the amount of scaling being applied to the child shape.
            virtual btVector3 GetChildScaling() const
            {
                return this->ChildScaling;
            }
            /// @brief Gets the serialization name of this shape.
            virtual const char*	getName()const
            {
                return "UniformScalingShape";
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Transform Methods

            /// @brief No Idea.
            virtual btVector3 localGetSupportingVertexWithoutMargin(const btVector3& vec)const
            {
                btVector3 tmpVertex;
                tmpVertex = this->ChildConvexShape->localGetSupportingVertexWithoutMargin(vec);
                return tmpVertex * this->ChildScaling;
            }
            /// @brief No Idea.
            virtual btVector3 localGetSupportingVertex(const btVector3& vec)const
            {
                btVector3 tmpVertex;
                btScalar ChildMargin = this->ChildConvexShape->getMargin();
                tmpVertex = this->ChildConvexShape->localGetSupportingVertexWithoutMargin(vec);
                return (tmpVertex * this->ChildScaling) + btVector3(ChildMargin,ChildMargin,ChildMargin);
            }
            /// @brief No Idea.
            virtual void batchedUnitVectorGetSupportingVertexWithoutMargin(const btVector3* vectors,btVector3* supportVerticesOut,int numVectors) const
            {
                this->ChildConvexShape->batchedUnitVectorGetSupportingVertexWithoutMargin(vectors,supportVerticesOut,numVectors);
                for( int i = 0 ; i < numVectors ; i++ )
                {
                    supportVerticesOut[i] = supportVerticesOut[i] * this->ChildScaling;
                }
            }
            /// @brief Calculates the local inertia for this shape and it's child shape.
            virtual void calculateLocalInertia(btScalar mass,btVector3& inertia) const
            {
                btVector3 tmpInertia;
                this->ChildConvexShape->calculateLocalInertia(mass,tmpInertia);
                inertia = tmpInertia * this->ChildScaling;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from btCollisionShape

            /// @brief Gets the AABB of this shape.
            virtual void getAabb(const btTransform& trans,btVector3& aabbMin,btVector3& aabbMax) const
            {
                this->getAabbSlow(trans,aabbMin,aabbMax);
            }
            /// @brief Gets the AABB of this shape.
            virtual void getAabbSlow(const btTransform& t,btVector3& aabbMin,btVector3& aabbMax) const
            {
                btVector3 _directions[] =
                {
                    btVector3( 1.,  0.,  0.),
                    btVector3( 0.,  1.,  0.),
                    btVector3( 0.,  0.,  1.),
                    btVector3( -1., 0.,  0.),
                    btVector3( 0., -1.,  0.),
                    btVector3( 0.,  0., -1.)
                };

                btVector3 _supporting[] =
                {
                    btVector3( 0., 0., 0.),
                    btVector3( 0., 0., 0.),
                    btVector3( 0., 0., 0.),
                    btVector3( 0., 0., 0.),
                    btVector3( 0., 0., 0.),
                    btVector3( 0., 0., 0.)
                };

                for( int i = 0 ; i < 6 ; i++ )
                {
                    _directions[i] = _directions[i] * t.getBasis();
                }

                this->batchedUnitVectorGetSupportingVertexWithoutMargin(_directions, _supporting, 6);

                btVector3 aabbMin1(0,0,0),aabbMax1(0,0,0);

                for ( int i = 0; i < 3; ++i )
                {
                    aabbMax1[i] = t(_supporting[i])[i];
                    aabbMin1[i] = t(_supporting[i + 3])[i];
                }
                btVector3 marginVec(this->getMargin(),this->getMargin(),this->getMargin());
                aabbMin = aabbMin1-marginVec;
                aabbMax = aabbMax1+marginVec;
            }

            /// @brief Sets the scaling to be applied to the sharable/global child collision shape.
            virtual void setLocalScaling(const btVector3& scaling)
                { this->SetChildScaling(scaling); }
            /// @brief Gets the scaling being applied to the sharable/global child collision shape.
            virtual const btVector3& getLocalScaling() const
                { return this->ChildScaling; }
            /// @brief Sets the collision margin of the sharable/global child collision shape.
            virtual void setMargin(btScalar margin)
                { this->ChildConvexShape->setMargin(margin); }
            /// @brief Gets the collision margin of the sharable/global child collision shape.
            virtual btScalar getMargin() const
                { return this->ChildConvexShape->getMargin(); }
            /// @brief Gets the number of directions available for the first parameter in "getPreferredPenetrationDirection".
            virtual int	getNumPreferredPenetrationDirections() const
                { return this->ChildConvexShape->getNumPreferredPenetrationDirections(); }
            /// @brief Gets the direction objects should follow for penetration recovery at the specified index.
            virtual void getPreferredPenetrationDirection(int index, btVector3& penetrationVector) const
                { this->ChildConvexShape->getPreferredPenetrationDirection(index,penetrationVector); }
        };//ScalingShape
/// @endcond
        ///////////////////////////////////////////////////////////////////////////////
        // PhysicsProxy Methods

        PhysicsProxy::PhysicsProxy(PhysicsManager* Creator) :
            ProxyShape(NULL),
            ScalerShape(NULL),
            Manager(Creator),
            CollisionGroup(0),
            CollisionMask(0)
            {  }

        PhysicsProxy::~PhysicsProxy()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Bool PhysicsProxy::CanLocallyScale() const
            { return ( this->ScalerShape != NULL ); }

        AxisAlignedBox PhysicsProxy::GetAABB() const
            { return ( this->IsInWorld() ? AxisAlignedBox( Vector3( this->_GetBasePhysicsObject()->getBroadphaseHandle()->m_aabbMin ), Vector3( this->_GetBasePhysicsObject()->getBroadphaseHandle()->m_aabbMax ) ) : AxisAlignedBox() ); }

        Bool PhysicsProxy::IsInWorld() const
            { return ( this->_GetBasePhysicsObject()->getBroadphaseHandle() != NULL ); }

        WorldManager* PhysicsProxy::GetCreator() const
            { return this->Manager; }

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Settings

        void PhysicsProxy::SetCollisionGroupAndMask(const Int16 Group, const Int16 Mask)
        {
            this->CollisionGroup = Group;
            this->CollisionMask = Mask;
        }

        void PhysicsProxy::SetCollisionGroup(const Int16 Group)
        {
            this->CollisionGroup = Group;
        }

        void PhysicsProxy::SetCollisionMask(const Int16 Mask)
        {
            this->CollisionMask = Mask;
        }

        Int16 PhysicsProxy::GetCollisionGroup() const
        {
            return this->CollisionGroup;
        }

        Int16 PhysicsProxy::GetCollisionMask() const
        {
            return this->CollisionMask;
        }

        void PhysicsProxy::SetCollisionShape(CollisionShape* Shape)
        {
            if( this->ProxyShape != Shape )
            {
                if( Shape != NULL )
                {
                    switch( Shape->GetType() )
                    {
                        // All the basic convex shapes
                        case CollisionShape::ST_Box:
                        case CollisionShape::ST_Capsule:
                        case CollisionShape::ST_Cone:
                        case CollisionShape::ST_ConvexHull:
                        case CollisionShape::ST_Cylinder:
                        case CollisionShape::ST_MultiSphere:
                        case CollisionShape::ST_Sphere:
                        {
                            btConvexShape* ScaledShape = static_cast<btConvexShape*>( Shape->_GetInternalShape() );
                            if( this->ScalerShape == NULL ) {
                                this->ScalerShape = new ScalingShape( ScaledShape, this->BodyScale.GetBulletVector3() );
                            }else{
                                static_cast<Physics::ScalingShape*>( this->ScalerShape )->SetChildShape( ScaledShape );
                            }
                            this->_GetBasePhysicsObject()->setCollisionShape( this->ScalerShape );
                            break;
                        }
                        // The static bvh trimesh
                        case CollisionShape::ST_StaticTriMesh:
                        {
                            btBvhTriangleMeshShape* ScaledShape = static_cast<btBvhTriangleMeshShape*>( Shape->_GetInternalShape() );
                            if( this->ScalerShape == NULL ) {
                                this->ScalerShape = new btScaledBvhTriangleMeshShape( ScaledShape, this->BodyScale.GetBulletVector3() );
                            }else{
                                this->ScalerShape->setLocalScaling( this->BodyScale.GetBulletVector3() );
                            }
                            this->_GetBasePhysicsObject()->setCollisionShape( this->ScalerShape );
                            break;
                        }
                        // No idea what to do about compound shapes
                        case CollisionShape::ST_Compound:
                        // GImpact doesn't have anything to give it per object scaling
                        case CollisionShape::ST_DynamicTriMesh:
                        // These shapes are either specifically tailored to the object or just don't make sense to scale
                        case CollisionShape::ST_Heightfield:
                        case CollisionShape::ST_Plane:
                        case CollisionShape::ST_Soft:
                        default:
                        {
                            if( this->ScalerShape != NULL ) {
                                delete this->ScalerShape;
                                this->ScalerShape = NULL;
                            }
                            this->_GetBasePhysicsObject()->setCollisionShape( Shape->_GetInternalShape() );
                            break;
                        }
                    }
                    this->ProxyShape = Shape;
                }else{
                    if( this->ScalerShape != NULL ) {
                        delete this->ScalerShape;
                        this->ScalerShape = NULL;
                    }
                    this->ProxyShape = NULL;
                }

                // Gotta flicker to update the AABB appropriately
                if( this->IsInWorld() ) {
                    this->RemoveFromWorld();
                    this->AddToWorld();
                }
            }
        }

        CollisionShape* PhysicsProxy::GetCollisionShape() const
        {
            return this->ProxyShape;
        }

        void PhysicsProxy::SetCollisionResponse(bool Enable)
        {
            if( Enable != this->GetCollisionResponse() )
            {
                btCollisionObject* Base = this->_GetBasePhysicsObject();
                if( Enable ) {
                    Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
                }else{
                    Base->setCollisionFlags( Base->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE );
                }
            }
        }

        bool PhysicsProxy::GetCollisionResponse() const
        {
            return !(this->_GetBasePhysicsObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }

        void PhysicsProxy::SetCollisionFlags(const Whole Flags)
        {
            this->_GetBasePhysicsObject()->setCollisionFlags(Flags);
        }

        Whole PhysicsProxy::GetCollisionFlags() const
        {
            return this->_GetBasePhysicsObject()->getCollisionFlags();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Static or Kinematic Properties

        void PhysicsProxy::SetKinematic()
        {
            btCollisionObject* Base = this->_GetBasePhysicsObject();
            Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        }

        void PhysicsProxy::SetStatic()
        {
            btCollisionObject* Base = this->_GetBasePhysicsObject();
            Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT );
        }

        bool PhysicsProxy::IsKinematic() const
        {
            return this->_GetBasePhysicsObject()->isKinematicObject();
        }

        bool PhysicsProxy::IsStatic() const
        {
            return this->_GetBasePhysicsObject()->isStaticObject();
        }

        bool PhysicsProxy::IsStaticOrKinematic() const
        {
            return this->_GetBasePhysicsObject()->isStaticOrKinematicObject();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Physics Properties

        void PhysicsProxy::SetFriction(const Real Friction)
            { this->_GetBasePhysicsObject()->setFriction(Friction); }

        Real PhysicsProxy::GetFriction() const
            { return this->_GetBasePhysicsObject()->getFriction(); }

        void PhysicsProxy::SetRollingFriction(const Real Friction)
            { this->_GetBasePhysicsObject()->setRollingFriction(Friction); }

        Real PhysicsProxy::GetRollingFriction() const
            { return this->_GetBasePhysicsObject()->getRollingFriction(); }

        void PhysicsProxy::SetAnisotropicFriction(const Vector3& Friction, const Whole Mode)
            { this->_GetBasePhysicsObject()->setAnisotropicFriction(Friction.GetBulletVector3(),Mode); }

        Physics::AnisotropicFrictionFlags PhysicsProxy::GetAnisotropicFrictionMode() const
        {
            if( this->IsAnisotropicFrictionModeSet(Physics::AFF_AnisotropicFrictionDisabled) )
                return Physics::AFF_AnisotropicFrictionDisabled;
            if( this->IsAnisotropicFrictionModeSet(Physics::AFF_AnisotropicFriction) )
                return Physics::AFF_AnisotropicFriction;
            if( this->IsAnisotropicFrictionModeSet(Physics::AFF_AnisotropicRollingFriction) )
                return Physics::AFF_AnisotropicRollingFriction;

            return Physics::AFF_AnisotropicFrictionDisabled;
        }

        Bool PhysicsProxy::IsAnisotropicFrictionModeSet(const Whole Mode) const
            { return this->_GetBasePhysicsObject()->hasAnisotropicFriction(Mode); }

        Vector3 PhysicsProxy::GetAnisotropicFriction() const
            { return Vector3( this->_GetBasePhysicsObject()->getAnisotropicFriction() ); }

        void PhysicsProxy::SetRestitution(const Real& Restitution)
            { this->_GetBasePhysicsObject()->setRestitution(Restitution); }

        Real PhysicsProxy::GetRestitution() const
            { return this->_GetBasePhysicsObject()->getRestitution(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Activation State

        void PhysicsProxy::SetActivationState(const Physics::ActivationState State, bool Force)
        {
            if(Force) this->_GetBasePhysicsObject()->forceActivationState(State);
            else this->_GetBasePhysicsObject()->setActivationState(State);
        }

        Physics::ActivationState PhysicsProxy::GetActivationState() const
        {
            return static_cast<Physics::ActivationState>( this->_GetBasePhysicsObject()->getActivationState() );
        }

        bool PhysicsProxy::IsActive() const
        {
            return this->_GetBasePhysicsObject()->isActive();
        }

        void PhysicsProxy::SetDeactivationTime(const Real Time)
        {
            this->_GetBasePhysicsObject()->setDeactivationTime(Time);
        }

        Real PhysicsProxy::GetDeactivationTime() const
        {
            return this->_GetBasePhysicsObject()->getDeactivationTime();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void PhysicsProxy::SetLocation(const Vector3& Loc)
        {
            this->SetLocation(Loc.X,Loc.Y,Loc.Z);
        }

        void PhysicsProxy::SetLocation(const Real X, const Real Y, const Real Z)
        {
            btVector3 NewLoc(X,Y,Z);
            this->_GetBasePhysicsObject()->getWorldTransform().setOrigin(NewLoc);
            this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setOrigin(NewLoc);
        }

        Vector3 PhysicsProxy::GetLocation() const
        {
            return Vector3( this->_GetBasePhysicsObject()->getWorldTransform().getOrigin() );
        }

        void PhysicsProxy::SetOrientation(const Quaternion& Ori)
        {
            this->SetOrientation(Ori.X,Ori.Y,Ori.Z,Ori.W);
        }

        void PhysicsProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
        {
            btQuaternion NewRot(X,Y,Z,W);
            this->_GetBasePhysicsObject()->getWorldTransform().setRotation(NewRot);
            this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setRotation(NewRot);
        }

        Quaternion PhysicsProxy::GetOrientation() const
        {
            return Quaternion( this->_GetBasePhysicsObject()->getWorldTransform().getRotation() );
        }

        void PhysicsProxy::SetScale(const Vector3& Sc)
        {
            this->BodyScale = Sc;
            if( this->ScalerShape != NULL ) {
                this->ScalerShape->setLocalScaling(Sc.GetBulletVector3());
            }else if( this->ProxyShape != NULL ) {
                this->ProxyShape->SetScaling(Sc);
            }
        }

        void PhysicsProxy::SetScale(const Real X, const Real Y, const Real Z)
        {
            this->SetScale( Vector3(X,Y,Z) );
        }

        Vector3 PhysicsProxy::GetScale() const
        {
            return this->BodyScale;
        }

        void PhysicsProxy::Translate(const Vector3& Trans)
        {
            this->SetLocation( this->GetLocation() + Trans );
        }

        void PhysicsProxy::Translate(const Real X, const Real Y, const Real Z)
        {
            Vector3 Trans(X,Y,Z);
            this->SetLocation( this->GetLocation() + Trans );
        }

        void PhysicsProxy::Yaw(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Y());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Pitch(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_X());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Roll(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Z());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Rotate(const Vector3& Axis, const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Axis);
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Rotate(const Quaternion& Rotation)
        {
            Quaternion NewRot = this->GetOrientation() * Rotation;
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Scale(const Vector3& Scale)
        {
            Vector3 NewScale = this->GetScale() * Scale;
            this->SetScale(NewScale);
        }

        void PhysicsProxy::Scale(const Real X, const Real Y, const Real Z)
        {
            Vector3 NewScale(X,Y,Z);
            this->Scale(NewScale);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void PhysicsProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeShape(SelfRoot);
        }

        void PhysicsProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->WorldProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( PhysicsProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("CollisionGroup").SetValue( this->GetCollisionGroup() ) &&
                PropertiesNode.AppendAttribute("CollisionMask").SetValue( this->GetCollisionMask() ) &&
                PropertiesNode.AppendAttribute("CollisionFlags").SetValue( this->GetCollisionFlags() ) &&
                PropertiesNode.AppendAttribute("Friction").SetValue( this->GetFriction() ) &&
                PropertiesNode.AppendAttribute("RollingFriction").SetValue( this->GetRollingFriction() ) &&
                PropertiesNode.AppendAttribute("AnisotropicFrictionMode").SetValue( this->GetAnisotropicFrictionMode() ) &&
                PropertiesNode.AppendAttribute("Restitution").SetValue( this->GetRestitution() ) &&
                PropertiesNode.AppendAttribute("ActivationState").SetValue( static_cast<Whole>( this->GetActivationState() ) ) &&
                PropertiesNode.AppendAttribute("DeactivationTime").SetValue( this->GetDeactivationTime() ) &&
                PropertiesNode.AppendAttribute("ContactProcessingThreshold").SetValue( this->_GetContactProcessingThreshold() ) )
            {
                XML::Node AnisotropicFrictionNode = PropertiesNode.AppendChild("AnisotropicFriction");
                this->GetAnisotropicFriction().ProtoSerialize( AnisotropicFrictionNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",PhysicsProxy::GetSerializableName() + "Properties",true);
            }
        }

        void PhysicsProxy::ProtoSerializeShape(XML::Node& SelfRoot) const
        {
            XML::Node ShapeNode = SelfRoot.AppendChild( PhysicsProxy::GetSerializableName() + "Shape" );

            if( ShapeNode.AppendAttribute("Version").SetValue("1") &&
                ShapeNode.AppendAttribute("ProxyShape").SetValue( this->ProxyShape->GetName() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",PhysicsProxy::GetSerializableName() + "Shape",true);
            }
        }

        void PhysicsProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeShape(SelfRoot);
        }

        void PhysicsProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->WorldProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( PhysicsProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    Whole AFMode = 0;

                    CurrAttrib = PropertiesNode.GetAttribute("CollisionGroup");
                    if( !CurrAttrib.Empty() )
                        this->SetCollisionGroup( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("CollisionMask");
                    if( !CurrAttrib.Empty() )
                        this->SetCollisionMask( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("CollisionFlags");
                    if( !CurrAttrib.Empty() )
                        this->SetCollisionFlags( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("Friction");
                    if( !CurrAttrib.Empty() )
                        this->SetFriction( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("RollingFriction");
                    if( !CurrAttrib.Empty() )
                        this->SetRollingFriction( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("AnisotropicFrictionMode");
                    if( !CurrAttrib.Empty() )
                        AFMode = CurrAttrib.AsWhole();

                    CurrAttrib = PropertiesNode.GetAttribute("Restitution");
                    if( !CurrAttrib.Empty() )
                        this->SetRestitution( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("ActivationState");
                    if( !CurrAttrib.Empty() )
                        this->SetActivationState( static_cast<Physics::ActivationState>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("DeactivationTime");
                    if( !CurrAttrib.Empty() )
                        this->SetDeactivationTime( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("ContactProcessingThreshold");
                    if( !CurrAttrib.Empty() )
                        this->_SetContactProcessingThreshold( CurrAttrib.AsReal() );

                    // Get the properties that need their own nodes
                    XML::Node AnisotropicFrictionNode = PropertiesNode.GetChild("AnisotropicFriction").GetFirstChild();
                    if( !AnisotropicFrictionNode.Empty() ) {
                        Vector3 AF(AnisotropicFrictionNode);
                        this->SetAnisotropicFriction(AF,AFMode);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (PhysicsProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,PhysicsProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void PhysicsProxy::ProtoDeSerializeShape(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node ShapeNode = SelfRoot.GetChild( PhysicsProxy::GetSerializableName() + "Properties" );

            if( !ShapeNode.Empty() ) {
                if(ShapeNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = ShapeNode.GetAttribute("ProxyShape");
                    if( !CurrAttrib.Empty() ) {
                        CollisionShape* Shape = CollisionShapeManager::GetSingletonPtr()->GetShape( CurrAttrib.AsString() );
                        this->SetCollisionShape( Shape );
                    }
                }
            }
        }

        String PhysicsProxy::GetDerivedSerializableName() const
            { return PhysicsProxy::GetSerializableName(); }

        String PhysicsProxy::GetSerializableName()
            { return "PhysicsProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void PhysicsProxy::_SetContactProcessingThreshold(const Real Threshold)
            { this->_GetBasePhysicsObject()->setContactProcessingThreshold(Threshold); }

        Real PhysicsProxy::_GetContactProcessingThreshold() const
            { return this->_GetBasePhysicsObject()->getContactProcessingThreshold(); }
    }// Physics
}// Mezzanine

#endif
