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
#ifndef _physicscompoundcollisionshape_cpp
#define _physicscompoundcollisionshape_cpp

#include "Physics/compoundcollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
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
                CollisionShape* CreatedShape = CreateShape(BulletSapeTypeToShapeType(CurrChild->getShapeType()), this->Name+"Child"+StringTools::ConvertToString(X), CurrChild);
                ChildShapes.push_back(CreatedShape);
                CollisionShapeManager::GetSingletonPtr()->StoreShape(CreatedShape);
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
            CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
        }

        void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation)
        {
            btTransform ChildTrans;
            ChildTrans.setIdentity();
            ChildTrans.setOrigin(ChildLocation.GetBulletVector3());
            GetBulletCompoundShape()->addChildShape(ChildTrans,Child->GetBulletShape());
            ChildShapes.push_back(Child);
            CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
        }

        void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Transform& ChildLocation)
        {
            GetBulletCompoundShape()->addChildShape(ChildLocation.GetBulletTransform(),Child->GetBulletShape());
            ChildShapes.push_back(Child);
            CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
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
                    return;
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

        void CompoundCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->CompoundCollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->CompoundCollisionShape::SerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", SerializableName()); }

            this->CollisionShape::ProtoSerialize(CollisionNode);

            XML::Node ChildShapesNode = CollisionNode.AppendChild("Shapes");
            if (!ChildShapesNode) { SerializeError("create ChildShapesNode",this->CompoundCollisionShape::SerializableName());}
            for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
            {
                //if() //the shape is in the manager
                //{
                    XML::Node OneChildShapeNode = ChildShapesNode.AppendChild("ChildShapeFromManager");
                    if(!OneChildShapeNode) { SerializeError("create ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName());}
                    XML::Attribute OneName = OneChildShapeNode.AppendAttribute("Name");
                    if(!OneName) { SerializeError("create Name Attribute on OneChildShapeNode",this->CompoundCollisionShape::SerializableName());}
                    OneName.SetValue(ChildShapes[X]->GetName());
                //}else{
                //    ChildShapes[X].ProtoSerialize(ChildShapesNode);
                //}
            }

        }

        void CompoundCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->CompoundCollisionShape::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild("CollisionShape");
                    if(!CollisionNode)
                        { DeSerializeError("locate CollisionShape node",SerializableName()); }
                    this->CollisionShape::ProtoDeSerialize(CollisionNode);

                    ChildShapes.clear(); // this will leak if any childshapes are not in the CollisionManager.
                    XML::Node ChildShapesNode = OneNode.GetChild("Shapes");
                    if(!ChildShapesNode) { DeSerializeError("Find Shapes Node",this->CompoundCollisionShape::SerializableName());}

                    XML::Node ChildNode = ChildShapesNode.GetFirstChild();
                    while(ChildNode)
                    {
                        if(String(ChildNode.Name())=="ChildShapeFromManager")
                        {
                            XML::Attribute OneName = ChildNode.GetAttribute("Name");
                            if(!OneName) { DeSerializeError("find Name Attribute on ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName()); }
                            CollisionShape* CurrentShape = CollisionShapeManager::GetSingletonPtr()->GetShape(OneName.AsString());
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
    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::CompoundCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::CompoundCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CompoundCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
