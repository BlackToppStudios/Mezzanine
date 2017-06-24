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
#ifndef _graphicsrenderablerayquery_cpp
#define _graphicsrenderablerayquery_cpp

#include "Graphics/renderablerayquery.h"
#include "Graphics/entityproxy.h"
#include "Graphics/billboardsetproxy.h"
#include "Graphics/particlesystemproxy.h"
#include "Graphics/scenemanager.h"
#include "Graphics/mesh.h"
#include "Graphics/meshinfo.h"
#include "Graphics/submesh.h"
#include "Graphics/vertextools.h"

#include "MathTools/intersections.h"

#include "entresol.h"
#include "world.h"
#include "managerbase.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboardParticleRenderer.h>

#include <limits>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // AABBQueryListener

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A custom AABB callback for Ogre ray scene queries that dumps the results we want into a usable container.
        ///////////////////////////////////////
        class MEZZ_LIB AABBQueryListener : public Ogre::RaySceneQueryListener
        {
        protected:
            /// @brief A container of the results to be returned.
            RayQuery::ResultContainer Results;
            /// @brief A copy of the ray being cast for this query.
            Ray CastRay;
            /// @brief A custom filter for potential ray query hits.
            RayQuery::FilterFunction FilterFunct;
        public:
            /// @brief Class constructor.
            /// @param BeingCast A copy of the ray being cast for this query.
            /// @param Filter The function that will filter hit results.
            AABBQueryListener(const Ray& BeingCast, const RayQuery::FilterFunction Filter);
            /// @brief Class destructor.
            virtual ~AABBQueryListener() = default;

            /// @brief Callback method for when a movable object is found on the ray.
            /// @param obj A pointer to the object hit.
            /// @param distance The distance from the ray origin the object was hit.
            /// @return Returns true to keep on processing the query, false to make it halt.
            virtual bool queryResult(Ogre::MovableObject* obj, Ogre::Real distance);
            /// @brief Callback method for when a world fragment is found on the ray.
            /// @param obj A pointer to the world fragment hit.
            /// @param distance The distance from the ray origin the object was hit.
            /// @return Returns true to keep on processing the query, false to make it halt.
            virtual bool queryResult(Ogre::SceneQuery::WorldFragment* fragment, Ogre::Real distance);

            /// @brief Sets the Ray being cast in the current query.
            /// @param BeingCast The Ray to be set for the current query.
            void SetRay(const Ray& BeingCast);
            /// @brief Gets the Ray being cast in the current query.
            /// @return Returns a const reference to the Ray being used by the query.
            const Ray& GetRay() const;
            /// @brief Sets the custom function that can filter ray query results.
            /// @param Filter The function that will filter hit results.
            void SetFilter(const RayQuery::FilterFunction Filter);
            /// @brief Gets the custom function that will filter ray query results.
            /// @return Returns the function object currently filtering results from this listener.
            RayQuery::FilterFunction GetFilter() const;
            /// @brief Gets the current results of the query.
            /// @return Returns the results of the query.
            const RayQuery::ResultContainer& GetResults() const;
            /// @brief Sorts the results by distance.
            void SortResults();
            /// @brief Resizes the results container
            /// @param Keep The number of results to shrink down to.
            void TruncateResults(const Whole Keep);
            /// @brief Clears the results in this listener.
            void ClearResults();
        };//AABBQueryListener

        ///////////////////////////////////////////////////////////////////////////////
        // ShapeQueryListener

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A custom Shape callback for Ogre ray scene queries that dumps the results we want into a usable container.
        ///////////////////////////////////////
        class MEZZ_LIB ShapeQueryListener : public AABBQueryListener
        {
        public:
            /// @brief Convenience type for ray tests performed in this listener.
            typedef MathTools::PlaneTestResult RayResult;
        protected:
            /// @brief Handles Entity specific logic for verifying a ray hit.
            /// @param obj A pointer to the object being verified.
            /// @return Returns true if the object was struck, false otherwise.
            Boole HandleEntity(Ogre::MovableObject* obj);
            /// @brief Handles BillboardSet specific logic for verifying a ray hit.
            /// @param obj A pointer to the object being verified.
            /// @return Returns true if the object was struck, false otherwise.
            Boole HandleBillboardSet(Ogre::MovableObject* obj);
            /// @brief Handles BillboardSet specific logic without checking for Mezzanine types.
            /// @param MezzObj A pointer to the Mezzanine proxy being worked with.  Used to record onto found results.
            /// @param obj A pointer to the object being verified.
            /// @return Returns true if the object was struck, false otherwise.
            Boole HandleBillboardSetNoCheck(RenderableProxy* MezzObj, Ogre::BillboardSet* obj);
            /// @brief Handles ParticleSystem specific logic for verifying a ray hit.
            /// @param obj A pointer to the object being verified.
            /// @return Returns true if the object was struck, false otherwise.
            Boole HandleParticleSystem(Ogre::MovableObject* obj);
        public:
            /// @brief Class constructor.
            /// @param BeingCast A copy of the ray being cast for this query.
            /// @param Filter The function that will filter hit results.
            ShapeQueryListener(const Ray& BeingCast, const RayQuery::FilterFunction Filter);
            /// @brief Class destructor.
            virtual ~ShapeQueryListener() = default;

            /// @copydoc AABBQueryListener::queryResult(Ogre::MovableObject* obj, Ogre::Real)
            virtual bool queryResult(Ogre::MovableObject* obj, Ogre::Real distance);
            /// @copydoc AABBQueryListener::queryResult(Ogre::SceneQuery::WorldFragment*, Ogre::Real)
            virtual bool queryResult(Ogre::SceneQuery::WorldFragment* fragment, Ogre::Real distance);
        };//ShapeQueryListener

        ///////////////////////////////////////////////////////////////////////////////
        // AABBQueryListener Methods

        AABBQueryListener::AABBQueryListener(const Ray& BeingCast, const RayQuery::FilterFunction Filter) :
            CastRay(BeingCast),
            FilterFunct(Filter)
            {  }

        bool AABBQueryListener::queryResult(Ogre::MovableObject* obj, Ogre::Real distance)
        {
            RenderableProxy* Prox = RenderableProxy::_Upcast(obj);
            Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
            if( FilterResult && Prox != nullptr ) {
                RayQueryHit NewHit;
                NewHit.Object = Prox;
                NewHit.Distance = distance;
                NewHit.HitLocation = this->CastRay.GetPointAtDistance(distance);
                NewHit.HitNormal = -(this->CastRay.Normal);
                NewHit.SubObject = -1;
                this->Results.push_back(NewHit);
            }
            return true;
        }

        bool AABBQueryListener::queryResult(Ogre::SceneQuery::WorldFragment* fragment, Ogre::Real distance)
        {
            /// @todo We don't use or interact with world fragments.
            /// If that changes then this should be implemented.
            return true;
        }

        void AABBQueryListener::SetRay(const Ray& BeingCast)
            { this->CastRay = BeingCast; }

        const Ray& AABBQueryListener::GetRay() const
            { return this->CastRay; }

        void AABBQueryListener::SetFilter(const RayQuery::FilterFunction Filter)
            { this->FilterFunct = Filter; }

        RayQuery::FilterFunction AABBQueryListener::GetFilter() const
            { return this->FilterFunct; }

        const RayQuery::ResultContainer& AABBQueryListener::GetResults() const
            { return this->Results; }

        void AABBQueryListener::SortResults()
            { std::sort(this->Results.begin(),this->Results.end()); }

        void AABBQueryListener::TruncateResults(const Whole Keep)
            { if( Keep < this->Results.size() ) this->Results.resize(Keep); }

        void AABBQueryListener::ClearResults()
            { this->Results.clear(); }

        ///////////////////////////////////////////////////////////////////////////////
        // ShapeQueryListener Methods

        ShapeQueryListener::ShapeQueryListener(const Ray& BeingCast, const RayQuery::FilterFunction Filter) :
            AABBQueryListener(BeingCast,Filter)
            {  }

        Boole ShapeQueryListener::HandleEntity(Ogre::MovableObject* obj)
        {
            RenderableProxy* Prox = RenderableProxy::_Upcast( obj );
            Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
            if( !( FilterResult && Prox != nullptr ) )
                return false;

            Real ResultDistance = std::numeric_limits<Real>::max();
            Integer ResultSubMesh = -1;
            RayResult Result(MathTools::PS_Neither,Vector3(0,0,0));

            Mesh* ObjMesh = static_cast<EntityProxy*>( Prox )->GetMesh();
            Whole NumSubMeshes = ObjMesh->GetNumSubMeshes();
            for( Whole CurrSubMesh = 0 ; CurrSubMesh < NumSubMeshes ; ++CurrSubMesh )
            {
                Vector3Vec Verts = ObjMesh->GetSubMesh(CurrSubMesh)->GetVertexPositions();
                IntVec Indices = ObjMesh->GetSubMesh(CurrSubMesh)->GetIndices();

                for( Whole IdxCount = 0 ; IdxCount < Indices.size() ; IdxCount += 3 )
                {
                    Vector3 PointA = Prox->ConvertLocalToGlobal( Verts[ Indices[IdxCount + 0] ] );
                    Vector3 PointB = Prox->ConvertLocalToGlobal( Verts[ Indices[IdxCount + 1] ] );
                    Vector3 PointC = Prox->ConvertLocalToGlobal( Verts[ Indices[IdxCount + 2] ] );
                    RayResult SingleResult = MathTools::Intersects(PointA,PointB,PointC,this->CastRay);

                    if( SingleResult.first != MathTools::PS_Neither ) {
                        Real SingleDistance = this->CastRay.Origin.Distance( SingleResult.second );
                        if( SingleDistance < ResultDistance ) {
                            Result = SingleResult;
                            ResultDistance = SingleDistance;
                            ResultSubMesh = CurrSubMesh;
                        }
                    }
                }
            }

            if( Result.first != MathTools::PS_Neither ) {
                RayQueryHit NewHit;
                NewHit.Object = Prox;
                NewHit.Distance = ResultDistance;
                NewHit.HitLocation = Result.second;
                NewHit.HitNormal = -(this->CastRay.Normal);
                NewHit.SubObject = ResultSubMesh;
                this->Results.push_back(NewHit);
                return true;
            }
            return false;
        }

        Boole ShapeQueryListener::HandleBillboardSet(Ogre::MovableObject* obj)
        {
            RenderableProxy* Prox = RenderableProxy::_Upcast( obj );
            Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
            if( FilterResult && Prox != nullptr ) {
                Ogre::BillboardSet* OgreCasted = static_cast<Ogre::BillboardSet*>(obj);
                return this->HandleBillboardSetNoCheck( Prox, OgreCasted );
            }
            return false;
        }

        Boole ShapeQueryListener::HandleBillboardSetNoCheck(RenderableProxy* MezzObj, Ogre::BillboardSet* obj)
        {
            Ogre::RenderOperation RenderOp;
            obj->getRenderOperation(RenderOp);

            Real ResultDistance = std::numeric_limits<Real>::max();
            Integer ResultBillboard = -1;
            RayResult Result(MathTools::PS_Neither,Vector3(0,0,0));

            Vector3 LocalRayOrigin = MezzObj->GetOrientation().GetInverse() * ( this->CastRay.Origin - MezzObj->GetLocation() );
            Vector3 LocalRayNormal = MezzObj->GetOrientation().GetInverse() * this->CastRay.Normal;
            Ray LocalRay(LocalRayOrigin,LocalRayNormal);

            if( RenderOp.operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST ) {
                Vector3Vec Verts = VertexTools::GetVertexPositions(RenderOp.vertexData);
                IntVec Indices = VertexTools::GetIndices(RenderOp.indexData);


                for( Whole IdxCount = 0 ; IdxCount < Indices.size() ; IdxCount += 3 )
                {
                    RayResult SingleResult = MathTools::Intersects( Verts[ Indices[IdxCount + 0] ],
                                                                    Verts[ Indices[IdxCount + 1] ],
                                                                    Verts[ Indices[IdxCount + 2] ],
                                                                    LocalRay);

                    if( SingleResult.first != MathTools::PS_Neither ) {
                        Real SingleDistance = LocalRayOrigin.Distance( SingleResult.second );
                        if( SingleDistance < ResultDistance ) {
                            Result = SingleResult;
                            ResultDistance = SingleDistance;
                            ResultBillboard = IdxCount / 6;
                            /// @todo The above line is a little sketch, it works because I dove into the source of Ogre
                            /// and verified how the indexes are generated.  However there is ZERO guarantee that it'll
                            /// stay that way with other systems or even updates to Ogre itself.
                        }
                    }
                }
            }else if( RenderOp.operationType == Ogre::RenderOperation::OT_POINT_LIST ) {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,
                               "Ray casting on point rendered billboards is not supported.");
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,
                               "Billboard is using an unsupported operation type for ray casting.");
            }

            if( Result.first != MathTools::PS_Neither ) {
                RayQueryHit NewHit;
                NewHit.Object = MezzObj;
                NewHit.Distance = ResultDistance;
                NewHit.HitLocation = Result.second;
                NewHit.HitNormal = -(this->CastRay.Normal);
                NewHit.SubObject = ResultBillboard;
                this->Results.push_back(NewHit);
                return true;
            }
            return false;
        }

        Boole ShapeQueryListener::HandleParticleSystem(Ogre::MovableObject* obj)
        {
            RenderableProxy* Prox = RenderableProxy::_Upcast( obj );
            Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
            if( FilterResult && Prox != nullptr ) {
                Ogre::ParticleSystem* OgreCasted = static_cast<Ogre::ParticleSystem*>(obj);
                Ogre::ParticleSystemRenderer* Renderer = OgreCasted->getRenderer();
                if( OgreCasted->getRenderer()->getType() == "billboard" ) {
                    typedef Ogre::BillboardParticleRenderer RendererType;
                    Ogre::BillboardSet* BillSet = static_cast<RendererType*>(Renderer)->getBillboardSet();
                    return this->HandleBillboardSetNoCheck( Prox, BillSet );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,
                                   "An unsupported Particle renderer type was found during shape raycast test.");
                }
            }
            return false;
        }

        bool ShapeQueryListener::queryResult(Ogre::MovableObject* obj, Ogre::Real distance)
        {
            if( obj->getMovableType() == Ogre::EntityFactory::FACTORY_TYPE_NAME ) {
                this->HandleEntity(obj);
            }else if( obj->getMovableType() == Ogre::BillboardSetFactory::FACTORY_TYPE_NAME ) {
                this->HandleBillboardSet(obj);
            }else if( obj->getMovableType() == Ogre::ParticleSystemFactory::FACTORY_TYPE_NAME ) {
                this->HandleParticleSystem(obj);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,
                               "An unsupported Ogre::MovableObject type was detected during shape raycast.");
            }
            // Always return true to continue the search.
            return true;
        }

        bool ShapeQueryListener::queryResult(Ogre::SceneQuery::WorldFragment* fragment, Ogre::Real distance)
        {
            /// @todo We don't use or interact with world fragments.
            /// If that changes then this should be implemented.
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderableRayQuery Methods

        RenderableRayQuery::RenderableRayQuery(SceneManager* ToQuery) :
            SceneMan(nullptr),
            QueryTool(nullptr),
            QueryFilter(std::numeric_limits<UInt32>::max())
            { this->UpdateQueryTool(ToQuery); }

        RenderableRayQuery::~RenderableRayQuery()
            { this->SceneMan->_GetGraphicsWorldPointer()->destroyQuery(this->QueryTool); }

        void RenderableRayQuery::UpdateQueryTool(SceneManager* NewQuery)
        {
            if( this->SceneMan != NewQuery ) {
                if( this->QueryTool != nullptr ) {
                    this->SceneMan->_GetGraphicsWorldPointer()->destroyQuery(this->QueryTool);
                    this->QueryTool = nullptr;
                }

                if( NewQuery != nullptr ) {
                    this->QueryTool = NewQuery->_GetGraphicsWorldPointer()->createRayQuery(Ogre::Ray(),Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
                    this->QueryTool->setQueryTypeMask(std::numeric_limits<UInt32>::max());
                    this->QueryTool->setSortByDistance(true);
                }
                this->SceneMan = NewQuery;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void RenderableRayQuery::SetWorld(World* ToQuery)
            { this->SetManager( static_cast<SceneManager*>( ToQuery->GetManager(ManagerBase::MT_SceneManager) ) ); }

        World* RenderableRayQuery::GetWorld() const
            { return this->SceneMan->GetWorld(); }

        void RenderableRayQuery::SetFilterFunction(const RayQuery::FilterFunction Filter)
            { this->FilterFunct = Filter; }

        RayQuery::FilterFunction RenderableRayQuery::GetFilterFunction() const
            { return this->FilterFunct; }

        void RenderableRayQuery::SetQueryFilter(const UInt32 Filter)
            { this->QueryFilter = Filter; }

        UInt32 RenderableRayQuery::GetQueryFilter() const
            { return this->QueryFilter; }

        void RenderableRayQuery::SetManager(SceneManager* Manager)
            { this->UpdateQueryTool(Manager); }

        SceneManager* RenderableRayQuery::GetManager() const
            { return this->SceneMan; }

        ///////////////////////////////////////////////////////////////////////////////
        // Fast Query

        RayQueryHit RenderableRayQuery::GetFirstAABBResult(const Ray& Cast) const
        {
            AABBQueryListener QueryListener(Cast,this->FilterFunct);
            this->QueryTool->setQueryMask(this->QueryFilter);
            this->QueryTool->setRay(Cast.GetOgreRay());
            this->QueryTool->execute(&QueryListener);

            QueryListener.SortResults();
            if( QueryListener.GetResults().size() > 0 ) {
                return QueryListener.GetResults().front();
            }else{
                RayQueryHit InvalidResult;
                return InvalidResult.Reset();
            }
        }

        RayQuery::ResultContainer RenderableRayQuery::GetAllAABBResults(const Ray& Cast, const Whole Limit) const
        {
            AABBQueryListener QueryListener(Cast,this->FilterFunct);
            this->QueryTool->setQueryMask(this->QueryFilter);
            this->QueryTool->setRay(Cast.GetOgreRay());
            this->QueryTool->setSortByDistance(true,Limit);
            this->QueryTool->execute(&QueryListener);
            QueryListener.SortResults();
            if( Limit > 0 ) {
                QueryListener.TruncateResults(Limit);
            }
            return QueryListener.GetResults();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Precise Query

        RayQueryHit RenderableRayQuery::GetFirstShapeResult(const Ray& Cast) const
        {
            ShapeQueryListener QueryListener(Cast,this->FilterFunct);
            this->QueryTool->setQueryMask(this->QueryFilter);
            this->QueryTool->setRay(Cast.GetOgreRay());
            this->QueryTool->execute(&QueryListener);

            QueryListener.SortResults();
            if( QueryListener.GetResults().size() > 0 ) {
                return QueryListener.GetResults().front();
            }else{
                RayQueryHit InvalidResult;
                return InvalidResult.Reset();
            }
        }

        RayQuery::ResultContainer RenderableRayQuery::GetAllShapeResults(const Ray& Cast, const Whole Limit) const
        {
            ShapeQueryListener QueryListener(Cast,this->FilterFunct);
            this->QueryTool->setQueryMask(this->QueryFilter);
            this->QueryTool->setRay(Cast.GetOgreRay());
            this->QueryTool->setSortByDistance(true,Limit);
            this->QueryTool->execute(&QueryListener);
            QueryListener.SortResults();
            if( Limit > 0 ) {
                QueryListener.TruncateResults(Limit);
            }
            return QueryListener.GetResults();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RenderableRayQuery::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( RenderableRayQuery::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("WorldName").SetValue( this->GetWorld()->GetName() ) &&
                SelfRoot.AppendAttribute("QueryFilter").SetValue( this->GetQueryFilter() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",RenderableRayQuery::GetSerializableName(),true);
            }
        }

        void RenderableRayQuery::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( SelfRoot.Name() == RenderableRayQuery::GetSerializableName() && !SelfRoot.Empty() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = SelfRoot.GetAttribute("WorldName");
                    if( !CurrAttrib.Empty() )
                        this->SetWorld( Entresol::GetSingletonPtr()->GetWorld( CurrAttrib.AsString() ) );

                    CurrAttrib = SelfRoot.GetAttribute("QueryFilter");
                    if( !CurrAttrib.Empty() )
                        this->SetQueryFilter( CurrAttrib.AsUint() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( RenderableRayQuery::GetSerializableName() ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,RenderableRayQuery::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String RenderableRayQuery::GetSerializableName()
            { return "RenderableRayQuery"; }
    }//Graphics
}//Mezzanine

#endif
