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
#ifndef _rayquery_h
#define _rayquery_h

#include "ray.h"
#include "worldproxy.h"
#include "worldproxymanager.h"

namespace Mezzanine
{
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a single entry for a RayQuery result.
    ///////////////////////////////////////
    struct MEZZ_LIB RayQueryHit
    {
        /// @brief The hit location in world space.
        Vector3 HitLocation;
        /// @brief The hit normal in world space.
        Vector3 HitNormal;
        /// @brief The distance the hit was from the Ray origin.
        Real Distance;
        /// @brief The index of the child item of the hit Object.
        Integer SubObject;
        /// @brief A pointer to the object hit.
        WorldProxy* Object;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Struct constructor.
        RayQueryHit() :
            Distance(0.0),
            SubObject(-1),
            Object(NULL)
            {  }
        /// @brief Struct destructor.
        ~RayQueryHit() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Helpers

        /// @brief Gets whether or not this is a valid hit.
        /// @return Returns true if this has hit an actual object, false otherwise.
        Boole IsValid() const
            { return this->Object != NULL; }
        /// @brief Gets whether or not this hit is of a valid and static object.
        /// @return Returns true if this hit is valid and striking a static object.  Returns false if there is no hit or strikes a dynamic object.
        Boole IsStatic() const
            { return this->IsValid() && this->Object->IsStatic(); }
        /// @brief Gets whether or not this hit is of a valid and dynamic object.
        /// @return Returns true if this hit is valid and striking a dynamic object.  Returns false if there is no hit or strikes a static object.
        Boole IsDynamic() const
            { return this->IsValid() && !this->Object->IsStatic(); }

        /// @brief Gets the parent world object of the proxy hit by this ray query.
        /// @return Returns a WorldObject pointer to the parent of the it proxy.
        WorldObject* GetObjectParent() const
            { return this->Object->GetParentObject(); }
        /// @brief Gets the hit location in world space.
        /// @return Returns a Vector3 containing the world location of the ray hit.
        Vector3 GetLocalHitLocation() const
            { return this->Object->ConvertGlobalToLocalNoScale(this->HitLocation); }
        /// @brief Gets the hit normal in world space.
        /// @return Returns a Vector3 containing the world direction of the ray hit.
        Vector3 GetLocalHitNormal() const
            { return this->Object->GetOrientation() * this->HitNormal; }

        /// @brief Clears all the data contained in this result.
        RayQueryHit& Reset()
        {
            this->HitLocation.Zero();
            this->HitNormal.Zero();
            this->Distance = 0;
            this->SubObject = -1;
            this->Object = NULL;
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sorting Operators

        /// @brief Less-than Operator.
        /// @param Other The other RayQueryHit to be compared to.
        /// @return Returns true if this RayQueryHit should be sorted before the other RayQueryHit.
        Boole operator<(const RayQueryHit& Other)
            { return this->Distance < Other.Distance; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that this RayQueryHit should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( RayQueryHit::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("Distance").SetValue( this->Distance ) &&
                SelfRoot.AppendAttribute("ObjectID").SetValue( this->Object->GetProxyID() ) &&
                SelfRoot.AppendAttribute("ObjectCreatorType").SetValue( this->Object->GetCreator()->GetInterfaceType() ) &&
                SelfRoot.AppendAttribute("SubObjectIndex").SetValue( this->SubObject ) )
            {
                XML::Node LocationNode = SelfRoot.AppendChild("HitLocation");
                this->HitLocation.ProtoSerialize( LocationNode );
                XML::Node NormalNode = SelfRoot.AppendChild("HitNormal");
                this->HitNormal.ProtoSerialize( NormalNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",RayQueryHit::GetSerializableName(),true);
            }
        }
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world the Ray cast this result came from was performed in.
        void ProtoDeSerialize(const XML::Node& SelfRoot, World* TheWorld)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Deserializing RayQueryHit instances is not currently supported.");
            /// @todo Investigate ways to refactor how proxies are accessed to remove the need for a World pointer here.
            /*XML::Attribute CurrAttrib;

            if( SelfRoot.Name() == RayQueryHit::GetSerializableName() && !SelfRoot.Empty() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    UInt32 ObjectID = 0;
                    Whole CreatorType = 0;

                    CurrAttrib = SelfRoot.GetAttribute("Distance");
                    if( !CurrAttrib.Empty() )
                        this->Distance = CurrAttrib.AsReal();

                    CurrAttrib = SelfRoot.GetAttribute("ObjectID");
                    if( !CurrAttrib.Empty() )
                        ObjectID = CurrAttrib.AsUint();

                    CurrAttrib = SelfRoot.GetAttribute("ObjectCreatorType");
                    if( !CurrAttrib.Empty() )
                        CreatorType = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("SubObjectIndex");
                    if( !CurrAttrib.Empty() )
                        this->SubObject = CurrAttrib.AsInteger();

                    //this->Object = TheWorld->GetManager(CreatorType)->Get

                    XML::Node LocationNode = SelfRoot.GetChild("HitLocation").GetFirstChild();
                    if( !LocationNode.Empty() ) {
                        this->HitLocation.ProtoDeSerialize(LocationNode);
                    }

                    XML::Node NormalNode = SelfRoot.GetChild("HitNormal").GetFirstChild();
                    if( !NormalNode.Empty() ) {
                        this->HitNormal.ProtoDeSerialize(NormalNode);
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( RayQueryHit::GetSerializableName() ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,RayQueryHit::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }//*/
        }

        /// @brief Get the name of the the XML tag the RayQueryHit class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName()
            { return "RayQueryHit"; }
    };//RayQueryHit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for performing ray queries in a given subsystem.
    ///////////////////////////////////////
    class MEZZ_LIB RayQuery
    {
    public:
        /// @brief Convenience type for the container of results produced by this class.
        typedef std::vector<RayQueryHit>         ResultContainer;
        /// @brief Iterator type for results returned by this class.
        typedef ResultContainer::iterator        ResultIterator;
        /// @brief Const Iterator type for results returned by this class.
        typedef ResultContainer::const_iterator  ConstResultIterator;

        /// @brief Class constructor.
        RayQuery() = default;
        /// @brief Class destructor.
        virtual ~RayQuery() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        /// @brief Sets the World to be queried.
        /// @param ToQuery A pointer to the World the Ray cast will be performed in.
        virtual void SetWorld(World* ToQuery) = 0;
        /// @brief Gets the World being queried.
        /// @return Returns a pointer to the World the Ray cast is being performed in.
        virtual World* GetWorld() const = 0;
        /// @brief Sets the optional filter to only return hits on specific types of proxies.
        /// @remarks Overrides of this method will only care for specific proxy types that exist in it's own
        /// subsystem.  Using the wrong proxy types for a given subsystem will cause your ray query to return nothing
        /// on every cast.  You can use Mezzanine::PT_All_Proxies to cleanly reset it to find all results in any subsystem.
        /// @param Filter A bitmask containing the proxy types to treat as valid returns.
        virtual void SetProxyTypes(const UInt32 Filter) = 0;
        /// @brief Gets the optional filter used to only return hits on specific types of proxies.
        /// @return Returns a bitmask representing the valid types of proxies to be returned from a Ray cast.
        virtual UInt32 GetProxyTypes() const = 0;
        /// @brief Sets the optional filter to only return hits on specific types of internal objects.
        /// @remarks Overrides of this method will only care for specific flags, depending on the subsystem the Ray cast
        /// is being performed in.  Be sure to verify you are using the correct flags for the subsystem or you may find
        /// your Ray casts returning nothing constantly.
        /// @param Filter A bitmask containing the object types to treat as valid returns.
        virtual void SetQueryFilter(const UInt32 Filter) = 0;
        /// @brief Gets the optional filter to only return hits on specific types of internal objects.
        /// @return Returns a bitmask of the objects to hit.  The exact meaning of the bitmask depends on the subsystem the query is being performed in.
        virtual UInt32 GetQueryFilter() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Fast Query

        /// @brief Gets the first result of an AABB Ray cast.
        /// @remarks Be warned that AABBs are less accurate than shapes, and you may get a result that wouldn't occur if
        /// the test was conducted on the Shape rather than the AABB.  The hit location and hit normal contained in the
        /// result may be unusable depending on your use case.  This is just an approximation.
        /// @param Cast The Ray being cast in the World.
        /// @return Returns a RayQueryHit containing the result of the Ray cast.  Warning: Be sure to check the result is valid.
        virtual RayQueryHit GetFirstAABBResult(const Ray& Cast) const = 0;
        /// @brief Gets all results of an AABB Ray cast (with an optional limit).
        /// @remarks Be warned that AABBs are less accurate than shapes, and you may get a result that wouldn't occur if
        /// the test was conducted on the Shape rather than the AABB.  The hit location and hit normal contained in the
        /// result may be unusable depending on your use case.  This is just an approximation.
        /// @param Cast The Ray being cast in the World.
        /// @param Limit The number of results to limit the return to.  0 means unlimited.
        /// @return Returns a container of results from the Ray cast.  All results in the container are valid.
        virtual ResultContainer GetAllAABBResults(const Ray& Cast, const Whole Limit = 0) const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Precise Query

        /// @brief Gets the first result of an Shape Ray cast.
        /// @param Cast The Ray being cast in the World.
        /// @return Returns a RayQueryHit containing the result of the Ray cast.  Warning: Be sure to check the result is valid.
        virtual RayQueryHit GetFirstShapeResult(const Ray& Cast) const = 0;
        /// @brief Gets all results of an Shape Ray cast (with an optional limit).
        /// @param Cast The Ray being cast in the World.
        /// @param Limit The number of results to limit the return to.  0 means unlimited.
        /// @return Returns a container of results from the Ray cast.  All results in the container are valid.
        virtual ResultContainer GetAllShapeResults(const Ray& Cast, const Whole Limit = 0) const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that this RayQuery should be appended to.
        virtual void ProtoSerialize(XML::Node& ParentNode) const = 0;
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot) = 0;

        /// @brief Get the name of the the XML tag the RayQuery class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName()
            { return "RayQuery"; }
    };//RayQuery
}//Mezzanine

#endif
