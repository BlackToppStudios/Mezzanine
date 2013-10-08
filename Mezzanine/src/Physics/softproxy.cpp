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
#ifndef _softproxy_cpp
#define _softproxy_cpp

#include "Physics/softproxy.h"
#include "Physics/physicsmanager.h"

#include "enumerations.h"
#include "stringtool.h"

//#include "Internal/meshtools.h.cpp"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace Mezzanine
{
    namespace Physics
    {
        /*void _Update()
        {
            //Ogre::VertexData* vertexData = entity->getMesh()->getSubMesh(0)->vertexData;

            //const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            //Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

            //unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
            //float* pReal = NULL;
            //for (size_t j = 0; j < vertexData->vertexCount; j++ )
            //{
            //    posElem->baseVertexPointerToElement(vertex, &pReal);
            //    *pReal++ = this->PhysicsSoftBody->m_nodes[j].m_x.x();
            //    *pReal++ = this->PhysicsSoftBody->m_nodes[j].m_x.y();
            //    *pReal++ = this->PhysicsSoftBody->m_nodes[j].m_x.z();
            //    vertex += vBuffer->getVertexSize();
            //}
            //vBuffer->unlock();

            //btVector3 position = this->PhysicsSoftBody->getWorldTransform().getOrigin();
            //btQuaternion rotation = this->PhysicsSoftBody->getWorldTransform().getRotation();
            btVector3 position = this->PhysicsSoftBody->m_clusters[0]->m_framexform.getOrigin();
            btQuaternion rotation = this->PhysicsSoftBody->m_clusters[0]->m_framexform.getRotation();
            this->GraphicsNode->setPosition(position.x(), position.y(), position.z());
            this->GraphicsNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
        }//*/


        SoftProxy::SoftProxy(const Real Mass, PhysicsManager* Creator) :
            CollidableProxy(Creator),
            PhysicsSoftBody(NULL)
        {
            MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Soft body support is still under construction and not complete.");
            this->CreateSoftObject(Mass);
        }

        SoftProxy::~SoftProxy()
        {
            if( this->IsInWorld() )
                this->RemoveFromWorld();

            delete this->PhysicsSoftBody;
        }

        void SoftProxy::CreateSoftObject(const Real Mass)
        {
            /*Internal::MeshInfo CurMesh;
            Internal::MeshTools::GetMeshVerticies(GraphicsObject,CurMesh);
            Internal::MeshTools::GetMeshIndicies(GraphicsObject,CurMesh);
            Internal::MeshTools::GetMeshNormals(GraphicsObject,CurMesh);
            Internal::MeshTools::GetMeshTextures(GraphicsObject,CurMesh);
            Internal::MeshTools::GetOtherMeshInfo(GraphicsObject,CurMesh);

            this->PhysicsSoftBody = btSoftBodyHelpers::CreateFromTriMesh(Entresol::GetSingletonPtr()->GetPhysicsManager()->_GetPhysicsWorldPointer()->getWorldInfo(), &CurMesh.Verticies[0].x, &CurMesh.Indicies[0], CurMesh.ICount/3);
            PhysicsObject=PhysicsSoftBody;
            PhysicsObject->setUserPointer( (WorldObject*)this );
            PhysicsShape = PhysicsSoftBody->getCollisionShape();
            PhysicsSoftBody->setTotalMass(mass, true);
            PhysicsSoftBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;
            PhysicsSoftBody->m_cfg.piterations = 5;
            PhysicsSoftBody->randomizeConstraints();
            PhysicsSoftBody->generateBendingConstraints(2);
            PhysicsSoftBody->generateClusters(20);

            CreateManualMesh(CurMesh);

            this->GraphicsObject = Entresol::GetSingletonPtr()->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(CurMesh.Name, CurMesh.Name + "M", CurMesh.Group);
            Ogre::Any OgreRef( (WorldObject*)this );
            GraphicsObject->setUserAny(OgreRef);

            this->PhysicsSoftBody->m_clusters[0]->m_collide = true;//*/

            // CreateManualMesh
            /*Ogre::ManualObject* ManualEntity = new Ogre::ManualObject(TheMesh.Name);
            ManualEntity->setDynamic(true);
            ManualEntity->estimateVertexCount(TheMesh.VCount);
            ManualEntity->estimateIndexCount(TheMesh.ICount);

            ManualEntity->begin(TheMesh.Material, TheMesh.RendOp, TheMesh.Group);
            for( int x=0 ; x < TheMesh.VCount ; x++ )
            {
                ManualEntity->position(TheMesh.Verticies[x]);
                ManualEntity->normal(TheMesh.Normals[x]);
                ManualEntity->textureCoord(TheMesh.Textures[x]);
            }
            for( int y=0 ; y < TheMesh.ICount ; y++ )
            {
                ManualEntity->index(TheMesh.Indicies[y]);
            }
            ManualEntity->end();
            ManualEntity->convertToMesh(TheMesh.Name + "M", TheMesh.Group);
            delete ManualEntity;//*/
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType SoftProxy::GetProxyType() const
        {
            return Mezzanine::PT_Physics_SoftProxy;
        }

        void SoftProxy::AddToWorld()
        {
            if( !this->IsInWorld() ) {
                this->Manager->_GetPhysicsWorldPointer()->addSoftBody( this->PhysicsSoftBody, this->CollisionGroup, this->CollisionMask );
            }
        }

        void SoftProxy::RemoveFromWorld()
        {
            if( this->IsInWorld() ) {
                this->Manager->_GetPhysicsWorldPointer()->removeSoftBody( this->PhysicsSoftBody );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Settings

        void SoftProxy::SetCollisionShape(CollisionShape* Shape)
        {
            // Do nothing.  Soft bodies have their own shapes specific to the nodes and positions of the individual soft body.
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void SoftProxy::SetLocation(const Vector3& Loc)
        {
            //this->PhysicsSoftBody->m_clusters[0]->m_framexform.setOrigin(Location.GetBulletVector3());
        }

        void SoftProxy::SetLocation(const Real X, const Real Y, const Real Z)
        {

        }

        Vector3 SoftProxy::GetLocation() const
        {
            return Vector3(0,0,0);
            //Vector3 temp(this->PhysicsSoftBody->m_clusters[0]->m_framexform.getOrigin());
            //return temp;
        }

        void SoftProxy::SetOrientation(const Quaternion& Ori)
        {
            //this->PhysicsSoftBody->m_clusters[0]->m_framexform.setRotation(Rotation.GetBulletQuaternion(true));
        }

        void SoftProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
        {

        }

        Quaternion SoftProxy::GetOrientation() const
        {
            return Quaternion(0,0,0,1);
        }

        void SoftProxy::SetScale(const Vector3& Sc)
        {

        }

        void SoftProxy::SetScale(const Real X, const Real Y, const Real Z)
        {

        }

        Vector3 SoftProxy::GetScale() const
        {
            return Vector3(1,1,1);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SoftProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
            if( !SelfRoot.AppendAttribute("InWorld").SetValue( this->IsInWorld() ? "true" : "false" ) ) {
                SerializeError("Create XML Attribute Values",SoftProxy::GetSerializableName(),true);
            }

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeShape(SelfRoot);
        }

        void SoftProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {

        }

        void SoftProxy::ProtoSeriailzeNodes(XML::Node& SelfRoot) const
        {

        }

        void SoftProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            Bool WasInWorld = false;
            XML::Attribute InWorldAttrib = SelfRoot.GetAttribute("InWorld");
            if( !InWorldAttrib.Empty() ) {
                WasInWorld = StringTools::ConvertToBool( InWorldAttrib.AsString() );
            }

            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeShape(SelfRoot);

            if( WasInWorld ) {
                this->AddToWorld();
            }
        }

        void SoftProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {

        }

        void SoftProxy::ProtoDeSeriailzeNodes(XML::Node& SelfRoot) const
        {

        }

        String SoftProxy::GetDerivedSerializableName() const
            { return SoftProxy::GetSerializableName(); }

        String SoftProxy::GetSerializableName()
            { return "SoftProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btSoftBody* SoftProxy::_GetPhysicsObject() const
            { return this->PhysicsSoftBody; }

        btCollisionObject* SoftProxy::_GetBasePhysicsObject() const
            { return this->PhysicsSoftBody; }
    }//Physics
}//Mezzanine

#endif
