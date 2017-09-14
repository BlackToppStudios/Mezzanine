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
#ifndef _entitycomponent_h
#define _entitycomponent_h

#include "enumerations.h"
#include "entitycomponentid.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    /// @addtogroup ECS
    /// @{

    class Entity;
    class EntityComponentManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for components comprising the functionality of an Entity.
    ///////////////////////////////////////
    class MEZZ_LIB EntityComponent
    {
    protected:
        /// @brief Pointer to the Entity this component belongs to.
        Entity* ParentEntity;
        /// @brief The ID of the component that unique among the pool it belongs to.
        EntityComponentID ComponentID;
    public:
        /// @brief Blank constructor.
        EntityComponent();
        /// @brief Normal/ID constructor.
        /// @param ID The unique ID assigned to the type of component an instance is.
        EntityComponent(const EntityComponentID& ID);
        /// @brief Class destructor.
        virtual ~EntityComponent() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the type of component this is.
        /// @return Returns an enum value for the type of component this is.
        virtual Mezzanine::ComponentType GetComponentType() const = 0;

        /// @brief Activates this components functionality.
        /// @remarks Not all components are alike.  Some components may not have functionality that can be enabled or disabled.  Other
        /// components may enable or disable only a part of their functionality.  Generally though, most will have behaviors that can
        /// be enabled or disabled.
        virtual void Activate() = 0;
        /// @brief Deactivates this components functionality.
        /// @remarks Not all components are alike.  Some components may not have functionality that can be enabled or disabled.  Other
        /// components may enable or disable only a part of their functionality.  Generally though, most will have behaviors that can
        /// be enabled or disabled.
        virtual void Deactivate() = 0;
        /// @brief Gets whether or not this component is activated.
        /// @return Returns true if this components functionality is enabled, false otherwise.
        virtual Boole IsActivated() const = 0;

        /// @brief Gets a pointer to the parent Entity controlling this component.
        /// @return Returns a pointer to the Entity controlling this component, or nullptr if this component isn't bound to a Entity.
        virtual Entity* GetParentEntity() const;
        /// @brief Gets the unique ID of this component.
        /// @remarks Component IDs are only unique to their family of proxies belonging to a specific subsystem.  Across subsystems IDs can be reused.
        /// @return Returns a UInt32 containing the unique ID for this component.
        virtual EntityComponentID GetComponentID() const;
        /// @brief Gets a pointer to this components creator.
        /// @return Returns a pointer to the EntityComponentManager that created this EntityComponent.
        virtual EntityComponentManager* GetCreator() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this instance should be appended to.
        virtual void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Convert the properties of this class to an XML::Node ready for serialization.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @brief Gets the most derived serializable name of this EntityComponent.
        /// @note When creating a new EntityComponent class verify this method has a valid return for it in order for serialization to work properly.
        /// @return Returns the name of the XML tag from the most derived class of "this".
        virtual String GetDerivedSerializableName() const;
        /// @brief Get the name of the the XML tag the component class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Binds this component to a Entity.
        /// @param NewParent A pointer to the Entity taking possession of this component.
        virtual void _Bind(Entity* NewParent);
    };//EntityComponent

    /// @}
}//Mezzanine

#endif
