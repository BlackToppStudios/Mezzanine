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
#ifndef _collisionshape_cpp
#define _collisionshape_cpp

#include "collisionshape.h"
#include "collisionshapemanager.h"
#include "serialization.h"
#include "world.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletSoftBody/btSoftBodyInternals.h"

namespace phys
{
    /////////////////////////////////////////
    // CollisionShape Functions

    CollisionShape::CollisionShape()
        : ShapeBase(NULL)
    {
    }

    CollisionShape::~CollisionShape()
    {
    }

    const String& CollisionShape::GetName() const
    {
        return Name;
    }

    void CollisionShape::SetMargin(const Real& Margin)
    {
        ShapeBase->setMargin(Margin);
    }

    Real CollisionShape::GetMargin() const
    {
        return ShapeBase->getMargin();
    }

    void CollisionShape::SetScaling(const Vector3& Scaling)
    {
        ShapeBase->setLocalScaling(Scaling.GetBulletVector3());
    }

    Vector3 CollisionShape::GetScaling() const
    {
        return Vector3(ShapeBase->getLocalScaling());
    }

    btCollisionShape* CollisionShape::GetBulletShape() const
    {
        return ShapeBase;
    }

#ifdef PHYSXML
    void CollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Attribute NameAttr = CollisionNode.AppendAttribute("Name");
        if(!NameAttr)
            { SerializeError("Create Name Attribute", SerializableName()); }
        NameAttr.SetValue(this->GetName());

        xml::Attribute MarginAttr = CollisionNode.AppendAttribute("Margin");
        if(!MarginAttr)
            { SerializeError("Create Margin Attribute", SerializableName()); }
        MarginAttr.SetValue(this->GetMargin());

        xml::Node ScalingNode = CollisionNode.AppendChild("Scaling");
        if (!ScalingNode) { SerializeError("Create Name Attribute", SerializableName()); }
        this->GetScaling().ProtoSerialize(ScalingNode);
    }

    void CollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->CollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                if(OneNode.GetAttribute("Name"))
                    { this->Name = OneNode.GetAttribute("Name").AsString(); }
                this->SetMargin(OneNode.GetAttribute("Margin").AsReal());

                xml::Node ScalingNode = OneNode.GetChild("Scaling").GetFirstChild();
                if(!ScalingNode)
                    { DeSerializeError("locate Scaling node",SerializableName()); }
                this->SetScaling(Vector3(ScalingNode));
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CollisionShape::SerializableName()
        {   return String("CollisionShape"); }
#endif

    /////////////////////////////////////////
    // PrimitiveCollisionShape Functions

    PrimitiveCollisionShape::PrimitiveCollisionShape()
    {
    }

    PrimitiveCollisionShape::~PrimitiveCollisionShape()
    {
    }

    void PrimitiveCollisionShape::SetPointers(btConvexInternalShape* Shape)
        { ShapeBase = Shape; }

    btConvexInternalShape* PrimitiveCollisionShape::GetBulletConvexShape() const
        { return static_cast<btConvexInternalShape*>(ShapeBase); }

#ifdef PHYSXML
    void PrimitiveCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->PrimitiveCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->PrimitiveCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (!Version)
            { SerializeError("Create Version Attribute", SerializableName()); }
        Version.SetValue(1);

        xml::Node ImplicitShape = CollisionNode.AppendChild("ImplicitShape");
        if (!ImplicitShape)
            { SerializeError("Create ImplicitShape Node", SerializableName()); }
        Vector3(GetBulletConvexShape()->getImplicitShapeDimensions()).ProtoSerialize(ImplicitShape);

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void PrimitiveCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->PrimitiveCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);

                xml::Node ImplicitShape = OneNode.GetChild("ImplicitShape").GetFirstChild();
                if(!ImplicitShape)
                    { DeSerializeError("locate ImplicitShape node",SerializableName()); }
                GetBulletConvexShape()->setImplicitShapeDimensions(Vector3(ImplicitShape).GetBulletVector3());
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String PrimitiveCollisionShape::SerializableName()
        {   return String("PrimitiveCollisionShape"); }
#endif

    /////////////////////////////////////////
    // FieldCollisionShape Functions
    FieldCollisionShape::FieldCollisionShape()
    {
    }

    FieldCollisionShape::~FieldCollisionShape()
    {
    }

    void FieldCollisionShape::SetPointers(btConcaveShape* Shape)
        { ShapeBase = Shape; }

    btConcaveShape* FieldCollisionShape::GetBulletConcaveShape() const
        { return static_cast<btConcaveShape*>(ShapeBase); }

#ifdef PHYSXML
    void FieldCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->FieldCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->FieldCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void FieldCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->FieldCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String FieldCollisionShape::SerializableName()
        {   return String("FieldCollisionShape"); }
#endif

    /////////////////////////////////////////
    // MeshCollisionShape Functions

    MeshCollisionShape::MeshCollisionShape()
    {
    }

    MeshCollisionShape::~MeshCollisionShape()
    {
    }

    void MeshCollisionShape::SetPointers(btConcaveShape* Shape)
        { ShapeBase = Shape; }

    btConcaveShape* MeshCollisionShape::GetBulletConcaveShape() const
        { return static_cast<btConcaveShape*>(ShapeBase); }

#ifdef PHYSXML
    void MeshCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->MeshCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->MeshCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void MeshCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->MeshCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String MeshCollisionShape::SerializableName()
        {   return String("MeshCollisionShape"); }
#endif

    /////////////////////////////////////////
    // CompoundCollisionShape Functions

    CompoundCollisionShape::CompoundCollisionShape(const String& Name)
    {
        this->Name = Name;
        ShapeBase = new btCompoundShape(false);
    }

    CompoundCollisionShape::CompoundCollisionShape(const String& Name, btCompoundShape* BulletShape)
    {
        this->Name = Name;
        ShapeBase = BulletShape;

        Whole NumChildren = GetBulletCompoundShape()->getNumChildShapes();
        for( Whole X = 0 ; X < NumChildren ; X++ )
        {
            btCollisionShape* CurrChild = GetBulletCompoundShape()->getChildShape(X);
            CollisionShape* CreatedShape = CreateShape(BulletSapeTypeToShapeType(CurrChild->getShapeType()), this->Name+"Child"+ToString(X), CurrChild);
            ChildShapes.push_back(CreatedShape);
            World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(CreatedShape);
        }
    }

    CompoundCollisionShape::~CompoundCollisionShape()
    {
        /*for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
        {
            //GetBulletCompoundShape()->removeChildShapeByIndex(X);
            delete ChildShapes[X];
        }
        ChildShapes.clear();*/
        delete GetBulletCompoundShape();
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation, const Quaternion& ChildRotation)
    {
        btTransform ChildTrans(ChildRotation.GetBulletQuaternion(),ChildLocation.GetBulletVector3());
        GetBulletCompoundShape()->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Child);
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation)
    {
        btTransform ChildTrans;
        ChildTrans.setIdentity();
        ChildTrans.setOrigin(ChildLocation.GetBulletVector3());
        GetBulletCompoundShape()->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Child);
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Transform& ChildLocation)
    {
        GetBulletCompoundShape()->addChildShape(ChildLocation.GetBulletTransform(),Child->GetBulletShape());
        ChildShapes.push_back(Child);
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Child);
    }

    Whole CompoundCollisionShape::GetNumChildren() const
    {
        return ChildShapes.size();
    }

    CollisionShape* CompoundCollisionShape::GetChild(Whole Index) const
    {
        return ChildShapes.at(Index);
    }

    void CompoundCollisionShape::RemoveChildShape(CollisionShape* Child)
    {
        for( std::vector<CollisionShape*>::iterator CSit = ChildShapes.begin() ; CSit != ChildShapes.end() ; CSit++ )
        {
            if(Child == (*CSit))
            {
                ChildShapes.erase(CSit);
                GetBulletCompoundShape()->removeChildShape(Child->GetBulletShape());
            }
        }
    }

    void CompoundCollisionShape::RemoveChildShape(const Whole& ChildIndex)
    {
        std::vector<CollisionShape*>::iterator CSit = ChildShapes.begin() + ChildIndex;
        ChildShapes.erase(CSit);
        GetBulletCompoundShape()->removeChildShapeByIndex(ChildIndex);
    }

    CollisionShape::ShapeType CompoundCollisionShape::GetType() const
    {
        return CollisionShape::ST_Compound;
    }

    btCompoundShape* CompoundCollisionShape::GetBulletCompoundShape() const
        { return static_cast<btCompoundShape*>(ShapeBase); }

#ifdef PHYSXML
    void CompoundCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CompoundCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CompoundCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

        xml::Node ChildShapesNode = CollisionNode.AppendChild("Shapes");
        if (!ChildShapesNode) { SerializeError("create ChildShapesNode",this->CompoundCollisionShape::SerializableName());}
        for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
        {
            //if() //the shape is in the manager
            //{
                xml::Node OneChildShapeNode = ChildShapesNode.AppendChild("ChildShapeFromManager");
                if(!OneChildShapeNode) { SerializeError("create ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName());}
                xml::Attribute OneName = OneChildShapeNode.AppendAttribute("Name");
                if(!OneName) { SerializeError("create Name Attribute on OneChildShapeNode",this->CompoundCollisionShape::SerializableName());}
                OneName.SetValue(ChildShapes[X]->GetName());
            //}else{
            //    ChildShapes[X].ProtoSerialize(ChildShapesNode);
            //}
        }

    }

    void CompoundCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->CompoundCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);

                ChildShapes.clear(); // this will leak if any childshapes are not in the CollisionManager.
                xml::Node ChildShapesNode = OneNode.GetChild("Shapes");
                if(!ChildShapesNode) { DeSerializeError("Find Shapes Node",this->CompoundCollisionShape::SerializableName());}

                xml::Node ChildNode = ChildShapesNode.GetFirstChild();
                while(ChildNode)
                {
                    if(ChildNode.Name()=="ChildShapeFromManager")
                    {
                        xml::Attribute OneName = ChildNode.GetAttribute("Name");
                        if(!OneName) { DeSerializeError("find Name Attribute on ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName()); }
                        CollisionShape* CurrentShape = World::GetWorldPointer()->GetCollisionShapeManager()->GetShape(OneName.AsString());
                        if(!CurrentShape) { DeSerializeError("find correct shape in CollisionShape Manager",this->CompoundCollisionShape::SerializableName()); }
                        ChildShapes.push_back(CurrentShape);
                    }else{
                        CollisionShape* CurrentShape = CreateShape(ChildNode);
                        ChildShapes.push_back(CurrentShape);
                    }
                    ChildNode=ChildNode.GetNextSibling();
                }

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CompoundCollisionShape::SerializableName()
        {   return String("CompoundCollisionShape"); }
#endif

    /////////////////////////////////////////
    // BoxCollisionShape Functions

    BoxCollisionShape::BoxCollisionShape(const String& Name, const Vector3& HalfExtents)
    {
        this->Name = Name;
        SetPointers(new btBoxShape(HalfExtents.GetBulletVector3()));
    }

    BoxCollisionShape::BoxCollisionShape(const String& Name, btBoxShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

#ifdef PHYSXML
    BoxCollisionShape::BoxCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            this->Name=OneName.AsString();

            /*xml::Node HalfExtentsNode = OneNode.GetChild("HalfExtents").GetFirstChild();
            if (!HalfExtentsNode) { DeSerializeError("find HalfExtentsNode",BoxCollisionShape::SerializableName()); }
            SetPointers(new btBoxShape(Vector3(HalfExtentsNode).GetBulletVector3()));
            // */
            SetPointers(new btBoxShape(Vector3().GetBulletVector3()));

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /PHYSXML

    BoxCollisionShape::~BoxCollisionShape()
    {
        delete GetBulletBoxShape();
    }

    Vector3 BoxCollisionShape::GetCleanHalfExtents() const
    {
        return (this->GetHalfExtentsWithMargin()/this->GetScaling());
    }

    Vector3 BoxCollisionShape::GetHalfExtents() const
    {
        return Vector3(GetBulletBoxShape()->getHalfExtentsWithoutMargin());
    }

    Vector3 BoxCollisionShape::GetHalfExtentsWithMargin() const
    {
        return Vector3(GetBulletBoxShape()->getHalfExtentsWithMargin());
    }

    bool BoxCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
    {
        return GetBulletBoxShape()->isInside(Location.GetBulletVector3(),Tolerance);
    }

    CollisionShape::ShapeType BoxCollisionShape::GetType() const
    {
        return CollisionShape::ST_Box;
    }

    btBoxShape* BoxCollisionShape::GetBulletBoxShape() const
        { return static_cast<btBoxShape*>(ShapeBase); }
#ifdef PHYSXML
    void BoxCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->BoxCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->BoxCollisionShape::SerializableName());}

        /*
        xml::Node HalfExtentsNode = CollisionNode.AppendChild("HalfExtents");
        if (!HalfExtentsNode) { SerializeError("create HalfExtentsNode",this->BoxCollisionShape::SerializableName());}
        //this->GetHalfExtents().ProtoSerialize(HalfExtentsNode);
        this->GetCleanHalfExtents().ProtoSerialize(HalfExtentsNode);
        */

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void BoxCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->BoxCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String BoxCollisionShape::SerializableName()
        {   return String("BoxCollisionShape"); }
#endif // /PHYSXML
    /////////////////////////////////////////
    // CapsuleCollisionShape Functions

    void CapsuleCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
    {
        this->Name = Name;
        btCapsuleShape* CapsuleShape = 0;
        if(Axis_Y == UpAxis) CapsuleShape = new btCapsuleShape(Radius,Height);
        else if(Axis_X == UpAxis) CapsuleShape = new btCapsuleShapeX(Radius,Height);
        else if(Axis_Z == UpAxis) CapsuleShape = new btCapsuleShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow("Non-supported up Axis passed into CapsuleCollisionShape constructor.");
        SetPointers(CapsuleShape);
    }

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
        { this->Construct(Name,Radius,Height,UpAxis.IsStandardUnitAxis()); }

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        { this->Construct(Name,Radius,Height,UpAxis); }

#ifdef PHYSXML
    CapsuleCollisionShape::CapsuleCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            String Name_(OneName.AsString());

            xml::Attribute Axis = OneNode.GetAttribute("Axis");
            if (!Axis) { DeSerializeError("find Axis Attribute",CapsuleCollisionShape::SerializableName()); }
            /*
            xml::Attribute Radius = OneNode.GetAttribute("Radius");
            if (!Radius) { DeSerializeError("find Radius Attribute",CapsuleCollisionShape::SerializableName()); }
            xml::Attribute Height = OneNode.GetAttribute("Height");
            if (!Height) { DeSerializeError("find Height Attribute",CapsuleCollisionShape::SerializableName()); }
            //SetPointers(new CapsuleCollisionShape(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());  // make and deserialize the shape
            this->Construct(Name_,Radius.AsReal(),Height.AsReal(),(StandardAxis)Axis.AsInteger());
            */
            this->Construct(Name_,0,0,(StandardAxis)Axis.AsInteger());

            this->ProtoDeSerialize(OneNode);

        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /PHYSXML

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

    CapsuleCollisionShape::~CapsuleCollisionShape()
    {
        delete GetBulletCapsuleShape();
    }

    Real CapsuleCollisionShape::GetRadius() const
        { return GetBulletCapsuleShape()->getRadius(); }
    Real CapsuleCollisionShape::GetHeight() const
        { return (GetBulletCapsuleShape()->getHalfHeight() * 2.0); }

    Real CapsuleCollisionShape::GetCleanRadius() const
        { return (this->GetRadius()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
        //{ return GetRadiusWithMargin() / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
    Real CapsuleCollisionShape::GetCleanHeight() const
        //{ return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[this->GetUpStandardAxis()] - GetBulletCapsuleShape()->getMargin()); }
        { return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()) / GetScaling()[this->GetUpStandardAxis()]); }
    /* Real CapsuleCollisionShape::GetRadiusWithMargin() const
        { return this->GetRadius()+GetBulletCapsuleShape()->getMargin(); }*/

    Vector3 CapsuleCollisionShape::GetUpAxis() const
        { return Vector3::UnitOnAxis( (StandardAxis)GetBulletCapsuleShape()->getUpAxis() ); }

    StandardAxis CapsuleCollisionShape::GetUpStandardAxis() const
        { return (StandardAxis)GetBulletCapsuleShape()->getUpAxis(); }

    CollisionShape::ShapeType CapsuleCollisionShape::GetType() const
    {
        return CollisionShape::ST_Capsule;
    }

    btCapsuleShape* CapsuleCollisionShape::GetBulletCapsuleShape() const
        { return static_cast<btCapsuleShape*>(ShapeBase); }

#ifdef PHYSXML
    void CapsuleCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CapsuleCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CapsuleCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }
/*
        xml::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
        if (RadiusAttr)
            { RadiusAttr.SetValue(this->GetCleanRadius()); }
        else
            { SerializeError("Create RadiusAttr Attribute", SerializableName()); }

        xml::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
        if (HeightAttr)
            { HeightAttr.SetValue(this->GetCleanHeight()); }
        else
            { SerializeError("Create HeightAttr Attribute", SerializableName()); }
*/
        xml::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
        if (AxisAttr)
            { AxisAttr.SetValue(this->GetUpStandardAxis()); }
        else
            { SerializeError("Create AxisAttr Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void CapsuleCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->CapsuleCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CapsuleCollisionShape::SerializableName()
        {   return String("CapsuleCollisionShape"); }
#endif

    /////////////////////////////////////////
    // ConeCollisionShape Functions

    void ConeCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
    {
        this->Name = Name;
        btConeShape* ConeShape;
        if(Axis_Y == UpAxis) ConeShape = new btConeShape(Radius,Height);
        else if(Axis_X == UpAxis) ConeShape = new btConeShapeX(Radius,Height);
        else if(Axis_Z == UpAxis) ConeShape = new btConeShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up StandardAxis passed into ConeCollisionShape constructor."));
        SetPointers(ConeShape);
        this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

    Vector3 ConeCollisionShape::GetAxisMathBS() const
    {
        Vector3 Results;
        switch (GetUpStandardAxis())
        {
            case 0:
                    Results[0] = 1;
                    Results[1] = 0;
                    Results[2] = 2;
                break;
            case 1:
                    Results[0] = 0;
                    Results[1] = 1;
                    Results[2] = 2;
                break;
            case 2:
                    Results[0] = 0;
                    Results[1] = 2;
                    Results[2] = 1;
                break;
            default:
                World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up StandardAxis passed into ConeCollisionShape::GetAxisMathBS()."));
        }

        return Results;
    }

    ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
        { Construct(Name, Radius, Height, UpAxis.IsStandardUnitAxis()); }

    ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        { Construct(Name, Radius, Height, UpAxis); }

#ifdef PHYSXML
    ConeCollisionShape::ConeCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            String Name_(OneName.AsString());

            xml::Attribute Radius = OneNode.GetAttribute("Radius");                                                                             // Find Attributes
            if (!Radius) { DeSerializeError("find Radius Attribute",ConeCollisionShape::SerializableName()); }
            xml::Attribute Axis = OneNode.GetAttribute("Axis");
            if (!Axis) { DeSerializeError("find Axis Attribute",ConeCollisionShape::SerializableName()); }
            xml::Attribute Height = OneNode.GetAttribute("Height");
            if (!Height) { DeSerializeError("find Height Attribute",ConeCollisionShape::SerializableName()); }

            this->Construct(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());        // make and deserialize the shape
            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /PHYSXML

    ConeCollisionShape::ConeCollisionShape(const String& Name, btConeShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

    ConeCollisionShape::~ConeCollisionShape()
    {
        delete GetBulletConeShape();
    }

    Real ConeCollisionShape::GetRadius() const
    {
        return GetBulletConeShape()->getRadius();
    }

    Real ConeCollisionShape::GetHeight() const
    {
        return GetBulletConeShape()->getHeight();
    }

    Real ConeCollisionShape::GetCleanRadius() const
    {
        return (GetRadius()) / GetRadiusScaling();
    }

    Real ConeCollisionShape::GetCleanHeight() const
    {
        return GetHeight() / GetHeightScaling();
    }

    Real ConeCollisionShape::GetRadiusScaling() const
        { return (GetScaling()[GetAxisMathBS()[0]]+GetScaling()[GetAxisMathBS()[2]])/2.0; }

    Real ConeCollisionShape::GetHeightScaling() const
        { return GetScaling()[GetUpStandardAxis()]; }

    Vector3 ConeCollisionShape::GetUpAxis() const
        { return Vector3::UnitOnAxis( (StandardAxis)GetBulletConeShape()->getConeUpIndex() ); }

    StandardAxis ConeCollisionShape::GetUpStandardAxis() const
        { return (StandardAxis)GetBulletConeShape()->getConeUpIndex(); }


    CollisionShape::ShapeType ConeCollisionShape::GetType() const
    {
        return CollisionShape::ST_Cone;
    }

    btConeShape* ConeCollisionShape::GetBulletConeShape() const
        { return static_cast<btConeShape*>(ShapeBase); }

#ifdef PHYSXML
    void ConeCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->ConeCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->ConeCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
        if (RadiusAttr)
            { RadiusAttr.SetValue(this->GetCleanRadius()); }
        else
            { SerializeError("Create RadiusAttr Attribute", SerializableName()); }

        /*xml::Attribute DirtyRadius = CollisionNode.AppendAttribute("DirtyRadius");
        if (DirtyRadius)
            { DirtyRadius.SetValue(this->GetRadius()); }
        else
            { SerializeError("Create DirtyRadius Attribute", SerializableName()); }*/

        xml::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
        if (HeightAttr)
            { HeightAttr.SetValue(this->GetCleanHeight()); }
        else
            { SerializeError("Create HeightAttr Attribute", SerializableName()); }

        xml::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
        if (AxisAttr)
            { AxisAttr.SetValue(this->GetUpStandardAxis()); }
        else
            { SerializeError("Create AxisAttr Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void ConeCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->ConeCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ConeCollisionShape::SerializableName()
        {   return String("ConeCollisionShape"); }
#endif

    /////////////////////////////////////////
    // ConvexHullCollisionShape Functions

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points)
    {
        btScalar* BulletPoints = new btScalar[Points.size() * 3];
        for( Whole X = 0 ; X < Points.size() ; ++X )
        {
            BulletPoints[X*3]     = Points[X][0];
            BulletPoints[X*3+1]   = Points[X][1];
            BulletPoints[X*3+2]   = Points[X][2];
        }

        this->Name = Name;
        SetPointers(new btConvexHullShape(BulletPoints,Points.size(),3*sizeof(btScalar)));
        this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        delete[] BulletPoints;
    }

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

#ifdef PHYSXML
    ConvexHullCollisionShape::ConvexHullCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            this->Name = OneName.AsString();

            SetPointers(new btConvexHullShape());

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /PHYSXML

    ConvexHullCollisionShape::~ConvexHullCollisionShape()
    {
        delete GetBulletHullShape();
    }

    void ConvexHullCollisionShape::AddPoint(const Vector3& Point)
    {
        GetBulletHullShape()->addPoint(Point.GetBulletVector3());
    }

    Vector3 ConvexHullCollisionShape::GetUnscaledPoint(const Whole& Index) const
    {

        return Vector3 ( *(GetBulletHullShape()->getUnscaledPoints()+Index) );
    }

    Vector3 ConvexHullCollisionShape::GetPoint(const Whole& Index) const
    {
        return Vector3(GetBulletHullShape()->getScaledPoint(Index));
    }

    Whole ConvexHullCollisionShape::GetNumPoints() const
    {
        return GetBulletHullShape()->getNumPoints();
    }

    bool ConvexHullCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
    {
        return GetBulletHullShape()->isInside(Location.GetBulletVector3(),Tolerance);
    }

    CollisionShape::ShapeType ConvexHullCollisionShape::GetType() const
    {
        return CollisionShape::ST_ConvexHull;
    }

    btConvexHullShape* ConvexHullCollisionShape::GetBulletHullShape() const
        { return static_cast<btConvexHullShape*>(ShapeBase); }

#ifdef PHYSXML
    void ConvexHullCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->ConvexHullCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->ConvexHullCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Node PointsNode = CollisionNode.AppendChild("UnscaledPoints");
        if (!PointsNode) { SerializeError("create UnscaledPoints",this->ConvexHullCollisionShape::SerializableName());}

        for(Whole c=0; c<this->GetNumPoints(); ++c)
        {
            this->GetUnscaledPoint(c).ProtoSerialize(PointsNode);
        }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void ConvexHullCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->ConvexHullCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

                xml::Node UnscaledPoints = OneNode.GetChild("UnscaledPoints");
                if(!UnscaledPoints)
                    { DeSerializeError("locate UnscaledPoints node",SerializableName()); }

                xml::Node OnePoint = UnscaledPoints.GetFirstChild();
                while (OnePoint)
                {
                    this->AddPoint(Vector3(OnePoint));
                    OnePoint = OnePoint.GetNextSibling();
                }

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ConvexHullCollisionShape::SerializableName()
        {   return String("ConvexHullCollisionShape"); }
#endif


    /////////////////////////////////////////
    // CylinderCollisionShape Functions

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
    {
        this->Name = Name;
        if(Vector3::Unit_Y() == UpAxis) CylinderShape = new btCylinderShape(btVector3(Radius,Height,Radius));
        else if(Vector3::Unit_X() == UpAxis) CylinderShape = new btCylinderShapeX(btVector3(Height,Radius,Radius));
        else if(Vector3::Unit_Z() == UpAxis) CylinderShape = new btCylinderShapeZ(btVector3(Radius,Radius,Height));
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into CylinderCollisionShape constructor."));
        SetPointers(CylinderShape);
    }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape)
    {
        this->Name = Name;
        CylinderShape = BulletShape;
        SetPointers(CylinderShape);
    }

#ifdef PHYSXML
    CylinderCollisionShape::CylinderCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            this->Name = OneName.AsString();

            SetPointers(new btCylinderShape(btVector3(0,0,0)));

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /PHYSXML

    CylinderCollisionShape::~CylinderCollisionShape()
    {
        delete CylinderShape;
    }

    Vector3 CylinderCollisionShape::GetHalfExtents() const
    {
        Vector3 HalfExtents(CylinderShape->getHalfExtentsWithoutMargin());
        return HalfExtents;
    }

    Vector3 CylinderCollisionShape::GetHalfExtentsWithMargin() const
    {
        Vector3 HalfExtents(CylinderShape->getHalfExtentsWithMargin());
        return HalfExtents;
    }

    Real CylinderCollisionShape::GetRadius() const
    {
        return CylinderShape->getRadius();
    }

    Vector3 CylinderCollisionShape::GetUpAxis() const
    {
        switch(CylinderShape->getUpAxis())
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
        }
    }

    CollisionShape::ShapeType CylinderCollisionShape::GetType() const
    {
        return CollisionShape::ST_Cylinder;
    }

    btCylinderShape* CylinderCollisionShape::GetBulletCylinderShape() const
        { return static_cast<btCylinderShape*>(ShapeBase); }

#ifdef PHYSXML
    void CylinderCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CylinderCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CylinderCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }
/*
        xml::Node PointsNode = CollisionNode.AppendChild("UnscaledPoints");
        if (!PointsNode) { SerializeError("create UnscaledPoints",this->CylinderCollisionShape::SerializableName());}

        for(Whole c=0; c<this->GetNumPoints(); ++c)
        {
            this->GetUnscaledPoint(c).ProtoSerialize(PointsNode);
        }
        */

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void CylinderCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->CylinderCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

                /*xml::Node UnscaledPoints = OneNode.GetChild("UnscaledPoints");
                if(!UnscaledPoints)
                    { DeSerializeError("locate UnscaledPoints node",SerializableName()); }

                xml::Node OnePoint = UnscaledPoints.GetFirstChild();
                while (OnePoint)
                {
                    this->AddPoint(Vector3(OnePoint));
                    OnePoint = OnePoint.GetNextSibling();
                }*/

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CylinderCollisionShape::SerializableName()
        {   return String("CylinderCollisionShape"); }
#endif

    /////////////////////////////////////////
    // MultiSphereCollisionShape Functions

    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
    {
        if(Locations.size() != Radii.size())
            World::GetWorldPointer()->LogAndThrow(Exception("Vector size mismatch between Locations and Radii in MultiSphereCollisionShape constructor."));
        this->Name = Name;
        Whole Spheres = Locations.size();
        btVector3* BulletLocs = new btVector3[Spheres];
        btScalar* BulletRadii = new btScalar[Spheres];
        for( Whole X = 0 ; X < Spheres ; X++ )
        {
            BulletLocs[X] = Locations[X].GetBulletVector3();
            BulletRadii[X] = Radii[X];
        }
        MultiSphereShape = new btMultiSphereShape(BulletLocs,BulletRadii,Spheres);
        SetPointers(MultiSphereShape);
        delete BulletLocs;
        delete BulletRadii;
    }

    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape)
    {
        this->Name = Name;
        MultiSphereShape = BulletShape;
        SetPointers(MultiSphereShape);
    }

    MultiSphereCollisionShape::~MultiSphereCollisionShape()
    {
        delete MultiSphereShape;
    }

    Vector3 MultiSphereCollisionShape::GetSphereLocation(const Whole& Index) const
    {
        Vector3 Loc(MultiSphereShape->getSpherePosition(Index));
        return Loc;
    }

    Real MultiSphereCollisionShape::GetSphereRadius(const Whole& Index) const
    {
        return MultiSphereShape->getSphereRadius(Index);
    }

    Whole MultiSphereCollisionShape::GetNumSpheres() const
    {
        return MultiSphereShape->getSphereCount();
    }

    CollisionShape::ShapeType MultiSphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_MultiSphere;
    }

    /////////////////////////////////////////
    // SphereCollisionShape Functions

    SphereCollisionShape::SphereCollisionShape(const String& Name, const Real& Radius)
    {
        this->Name = Name;
        SphereShape = new btSphereShape(Radius);
        SetPointers(SphereShape);
    }

    SphereCollisionShape::SphereCollisionShape(const String& Name, btSphereShape* BulletShape)
    {
        this->Name = Name;
        SphereShape = BulletShape;
        SetPointers(SphereShape);
    }

    SphereCollisionShape::~SphereCollisionShape()
    {
        delete SphereShape;
    }

    Real SphereCollisionShape::GetRadius() const
    {
        return SphereShape->getRadius();
    }

    CollisionShape::ShapeType SphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_Sphere;
    }

    /////////////////////////////////////////
    // DynamicMeshCollisionShape Functions

    DynamicMeshCollisionShape::DynamicMeshCollisionShape(const String& Name, btGImpactMeshShape* BulletShape)
    {
        this->Name = Name;
        GImpactShape = BulletShape;
        SetPointers(GImpactShape);
    }

    DynamicMeshCollisionShape::~DynamicMeshCollisionShape()
    {
        delete GImpactShape;
    }

    CollisionShape::ShapeType DynamicMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_DynamicTriMesh;
    }

    /////////////////////////////////////////
    // HeightfieldCollisionShape Functions

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name)
    {

    }

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name, btHeightfieldTerrainShape* BulletShape)
    {
        this->Name = Name;
        HeightfieldShape = BulletShape;
        SetPointers(HeightfieldShape);
    }

    HeightfieldCollisionShape::~HeightfieldCollisionShape()
    {
        delete HeightfieldShape;
    }

    CollisionShape::ShapeType HeightfieldCollisionShape::GetType() const
    {
        return CollisionShape::ST_Heightfield;
    }

    /////////////////////////////////////////
    // PlaneCollisionShape Functions

    PlaneCollisionShape::PlaneCollisionShape(const String& Name)
    {

    }

    PlaneCollisionShape::PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape)
    {
        this->Name = Name;
        PlaneShape = BulletShape;
        SetPointers(PlaneShape);
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        delete PlaneShape;
    }

    CollisionShape::ShapeType PlaneCollisionShape::GetType() const
    {
        return CollisionShape::ST_Plane;
    }

    /////////////////////////////////////////
    // ActorSoftCollisionShape Functions

    ActorSoftCollisionShape::ActorSoftCollisionShape(const String& Name, btSoftBodyCollisionShape* BulletShape)
    {
        this->Name = Name;
        ActorSoftShape = BulletShape;
        SetPointers(ActorSoftShape);
    }

    ActorSoftCollisionShape::~ActorSoftCollisionShape()
    {
        delete ActorSoftShape;
    }

    CollisionShape::ShapeType ActorSoftCollisionShape::GetType() const
    {
        return CollisionShape::ST_ActorSoft;
    }

    /////////////////////////////////////////
    // StaticMeshCollisionShape Functions

    StaticMeshCollisionShape::StaticMeshCollisionShape(const String& Name, btBvhTriangleMeshShape* BulletShape)
    {
        /// @todo May need to create a btScaledBvhTriangleMeshShape for scaling this shape appropriately.
        this->Name = Name;
        BvhShape = BulletShape;
        SetPointers(BvhShape);
    }

    StaticMeshCollisionShape::~StaticMeshCollisionShape()
    {
        delete BvhShape;
    }

    CollisionShape::ShapeType StaticMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_StaticTriMesh;
    }

    /////////////////////////////////////////
    // Utility Functions
    CollisionShape* CreateShape(CollisionShape::ShapeType ShapeToCreate, const String& Name_, btCollisionShape* ShapeToModel)
    {
        if (ShapeToModel)
        {
            switch(ShapeToCreate)
            {
                case CollisionShape::ST_Box:
                    return new BoxCollisionShape(Name_,(btBoxShape*)ShapeToModel);
                case CollisionShape::ST_Capsule:
                    return new CapsuleCollisionShape(Name_,(btCapsuleShape*)ShapeToModel);
                case CollisionShape::ST_Cone:
                    return new ConeCollisionShape(Name_,(btConeShape*)ShapeToModel);
                case CollisionShape::ST_ConvexHull:
                    return new ConvexHullCollisionShape(Name_,(btConvexHullShape*)ShapeToModel);
                case CollisionShape::ST_Cylinder:
                    return new CylinderCollisionShape(Name_,(btCylinderShape*)ShapeToModel);
                case CollisionShape::ST_MultiSphere:
                    return new MultiSphereCollisionShape(Name_,(btMultiSphereShape*)ShapeToModel);
                case CollisionShape::ST_Sphere:
                    return new SphereCollisionShape(Name_,(btSphereShape*)ShapeToModel);
                case CollisionShape::ST_DynamicTriMesh:
                    return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                case CollisionShape::ST_Heightfield:
                    return new HeightfieldCollisionShape(Name_,(btHeightfieldTerrainShape*)ShapeToModel);
                case CollisionShape::ST_Plane:
                    return new PlaneCollisionShape(Name_,(btStaticPlaneShape*)ShapeToModel);
                case CollisionShape::ST_ActorSoft:
                    return new ActorSoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                case CollisionShape::ST_StaticTriMesh:
                    return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                case CollisionShape::ST_Compound: // holy recursive batman
                    return new CompoundCollisionShape(Name_,(btCompoundShape*)ShapeToModel);
                default:
                    World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance.");
            }
        }else{
            World::GetWorldPointer()->LogAndThrow("Attempting to convert an empty Bullet Collision Shape type into a CollisionShape instance.");
        }
    }

    CollisionShape* CreateShape(xml::Node OneNode)
    {
        CollisionShape::ShapeType ShapeToCreate = StringToShapeType(OneNode.Name());

        String Name_("Defunct");

        switch(ShapeToCreate)
        {
            case CollisionShape::ST_Box:
                return new BoxCollisionShape(OneNode);
            case CollisionShape::ST_Capsule:
                return new CapsuleCollisionShape(OneNode);
            case CollisionShape::ST_Cone:
                return new ConeCollisionShape(OneNode);
            case CollisionShape::ST_ConvexHull:
                return new ConvexHullCollisionShape(OneNode);
            case CollisionShape::ST_Cylinder:
                return new CylinderCollisionShape(OneNode);
            case CollisionShape::ST_MultiSphere:
            {
                std::vector<Vector3> Points;
                std::vector<Real> Radii;
                Points.push_back(Vector3(0,0,0)); Points.push_back(Vector3(1,0,0));
                Radii.push_back(1); Radii.push_back(1);
                //return new MultiSphereCollisionShape(Name_,(btMultiSphereShape*)ShapeToModel);
            }
            case CollisionShape::ST_Sphere:
                return new SphereCollisionShape(Name_,1);
            case CollisionShape::ST_DynamicTriMesh:     /// @todo Complete CreateShape function once DynamicMeshCollisionShape can be deserialized
                //return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a DynamicMeshCollisionShape.");
            case CollisionShape::ST_Heightfield:
                return new HeightfieldCollisionShape(Name_);
            case CollisionShape::ST_Plane:
                return new PlaneCollisionShape(Name_);
            case CollisionShape::ST_ActorSoft:          /// @todo Complete CreateShape function once ActorSoftCollisionShape can be deserialized
                //return new ActorSoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a ActorSoftCollisionShape.");
            case CollisionShape::ST_StaticTriMesh:      /// @todo Complete CreateShape function once StaticMeshCollisionShape can be deserialized
                //return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a ActorSoftCollisionShape.");
            case CollisionShape::ST_Compound: // holy recursive batman
                return new CompoundCollisionShape(Name_);
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance.");
        }
    }

    CollisionShape::ShapeType BulletSapeTypeToShapeType(int BulletShapeType)
    {
        switch(BulletShapeType)
        {
            case BOX_SHAPE_PROXYTYPE:           return CollisionShape::ST_Box;
            case CAPSULE_SHAPE_PROXYTYPE:       return CollisionShape::ST_Capsule;
            case CONE_SHAPE_PROXYTYPE:          return CollisionShape::ST_Cone;
            case CONVEX_HULL_SHAPE_PROXYTYPE:   return CollisionShape::ST_ConvexHull;
            case CYLINDER_SHAPE_PROXYTYPE:      return CollisionShape::ST_Cylinder;
            case MULTI_SPHERE_SHAPE_PROXYTYPE:  return CollisionShape::ST_MultiSphere;
            case SPHERE_SHAPE_PROXYTYPE:        return CollisionShape::ST_Sphere;
            case GIMPACT_SHAPE_PROXYTYPE:       return CollisionShape::ST_DynamicTriMesh;
            case TERRAIN_SHAPE_PROXYTYPE:       return CollisionShape::ST_Heightfield;
            case STATIC_PLANE_PROXYTYPE:        return CollisionShape::ST_Plane;
            case SOFTBODY_SHAPE_PROXYTYPE:      return CollisionShape::ST_ActorSoft;
            case TRIANGLE_MESH_SHAPE_PROXYTYPE: return CollisionShape::ST_StaticTriMesh;
            case COMPOUND_SHAPE_PROXYTYPE:      return CollisionShape::ST_Compound;
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Bullet Collision Shape type into a phys::CollisionShapeShapeType.");
        }
    }

    String ShapeTypeToString(CollisionShape::ShapeType ShapeToConvert)
    {
        switch(ShapeToConvert)
        {
            case CollisionShape::ST_Box:            return String("BoxCollisionShape");
            case CollisionShape::ST_Capsule:        return String("CapsuleCollisionShape");
            case CollisionShape::ST_Cone:           return String("ConeCollisionShape");
            case CollisionShape::ST_ConvexHull:     return String("ConvexHullCollisionShape");
            case CollisionShape::ST_Cylinder:       return String("CylinderCollisionShape");
            case CollisionShape::ST_MultiSphere:    return String("MultiSphereCollisionShape");
            case CollisionShape::ST_Sphere:         return String("SphereCollisionShape");
            case CollisionShape::ST_DynamicTriMesh: return String("DynamicMeshCollisionShape");
            case CollisionShape::ST_Heightfield:    return String("HeightfieldCollisionShapeString(");
            case CollisionShape::ST_Plane:          return String("PlaneCollisionShape");
            case CollisionShape::ST_ActorSoft:      return String("ActorSoftCollisionShape");
            case CollisionShape::ST_StaticTriMesh:  return String("StaticMeshCollisionShape");
            case CollisionShape::ST_Compound:       return String("CompoundCollisionShape");
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a String.");
        }
    }

    CollisionShape::ShapeType StringToShapeType(const String& TypeName)
    {
        if(TypeName.size()<5)
            { World::GetWorldPointer()->LogAndThrow("Attempting to convert a CollisionShape::ShapeType String into a CollisionShape::ShapeType which is too short to be valid."); }
        switch(TypeName.at(3))
        {
            case 'C':
                if (String("BoxCollisionShape")==TypeName)
                    { return CollisionShape::ST_Box; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'C'."); }
            case 's':
                if (String("CapsuleCollisionShape")==TypeName)
                    { return CollisionShape::ST_Capsule; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 's'."); }
            case 'e':
                if (String("SphereCollisionShape")==TypeName)
                    { return CollisionShape::ST_Sphere; }
                else if (String("ConeCollisionShape")==TypeName)
                    { return CollisionShape::ST_Cone; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'e'."); }
            case 'v':
                if (String("ConvexHullCollisionShape")==TypeName)
                    { return CollisionShape::ST_ConvexHull; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'v'."); }
            case 'i':
                if (String("CylinderCollisionShape")==TypeName)
                    { return CollisionShape::ST_Cylinder; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'i'."); }
            case 't':
                if (String("StaticMeshCollisionShape")==TypeName)
                    { return CollisionShape::ST_StaticTriMesh; }
                else if (String("MultiSphereCollisionShape")==TypeName)
                    { return CollisionShape::ST_MultiSphere; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 't'."); }
            case 'a':
                if (String("DynamicMeshCollisionShape")==TypeName)
                    { return CollisionShape::ST_DynamicTriMesh; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'a'."); }
            case 'g':
                if (String("HeightfieldCollisionShapeString")==TypeName)
                    { return CollisionShape::ST_Heightfield; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'g'."); }
            case 'n':
                if (String("PlaneCollisionShape")==TypeName)
                    { return CollisionShape::ST_Plane; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'n'."); }
            case 'o':
                if (String("ActorSoftCollisionShape")==TypeName)
                    { return CollisionShape::ST_ActorSoft; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'o'."); }
            case 'p':
                if (String("CompoundCollisionShape")==TypeName)
                    { return CollisionShape::ST_Compound; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'p'."); }
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType.");
        }
    }

    ///////////////////////////////////////
    // CollisionShapeDeSerializer
    CollisionShape* CollisionShapeDeSerializer::PerformDeSerialization(std::istream& Stream)
    {
        xml::Document* Doc = new xml::Document();
        String OneTag( phys::xml::GetOneTag(Stream) );
        if(!Doc->Load(OneTag.c_str()))
        {
            delete Doc;
            World::GetWorldPointer()->LogAndThrow(StringCat("Could not Deserialize XML Stream which should contain a Collision Shape, XML looked Like: ", OneTag) );
        }

        CollisionShape* Results = ProtoDeSerialize(Doc->GetFirstChild());
        delete Doc;
        return Results;
    }

    CollisionShape* CollisionShapeDeSerializer::ProtoDeSerialize(const xml::Node& OneNode)
        { return CreateShape(OneNode); }

    std::istream& CollisionShapeDeSerializer::DeSerialize(std::istream& Stream)
    {
        PerformDeSerialization(Stream);
        return Stream;
    }

    CollisionShape* CollisionShapeDeSerializer::DeSerializeAndRetrieve(std::istream& Stream)
        { return PerformDeSerialization(Stream); }

    String CollisionShapeDeSerializer::ContainerName() const
        { return String("Shapes"); }
}

#ifdef PHYSXML
/*
CollisionShape
PrimitiveCollisionShape
FieldCollisionShape
MeshCollisionShape
BoxCollisionShape
CapsuleCollisionShape
CompoundCollisionShape
ConeCollisionShape
ConvexHullCollisionShape
CylinderCollisionShape
MultiSphereCollisionShape
SphereCollisionShape
DynamicMeshCollisionShape
HeightfieldCollisionShape
PlaneCollisionShape
ActorSoftCollisionShape
StaticMeshCollisionShape
 */
    std::ostream& operator << (std::ostream& stream, const phys::CollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::CollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::CollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::PrimitiveCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::PrimitiveCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::PrimitiveCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::FieldCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::FieldCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::FieldCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::MeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::MeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::MeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::BoxCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::BoxCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::BoxCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::CapsuleCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::CapsuleCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::CapsuleCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::CompoundCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::CompoundCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::CompoundCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::ConeCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::ConeCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::ConeCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::ConvexHullCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::ConvexHullCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::ConvexHullCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::CylinderCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::CylinderCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::CylinderCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::MultiSphereCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::MultiSphereCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::MultiSphereCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::SphereCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::SphereCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::SphereCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }
     std::ostream& operator << (std::ostream& stream, const phys::DynamicMeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::DynamicMeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::DynamicMeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::HeightfieldCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::HeightfieldCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::HeightfieldCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::PlaneCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::PlaneCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::PlaneCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::ActorSoftCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::ActorSoftCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::ActorSoftCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const phys::StaticMeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, phys::StaticMeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::StaticMeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

#endif  // \physxml


#endif
