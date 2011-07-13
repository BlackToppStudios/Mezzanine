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
#ifndef _serializable_h
#define _serializable_h

#include "datatypes.h"
#include "xml.h"
#include "xmldoc.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Serializable
    /// @headerfile serializable.h
    /// @brief Aids in the serializing of multiple classes to streams
    /// @details If PHYSXML is not defined this class declares nothing. It is expected for classes implementing
    /// This to wrap inherited functions in
    ///////////////////////////////////////
    class PHYS_LIB Serializable
    {
        #ifdef PHYSXML
        public:
            /// Used to Identify where the serialized XML contents should go
            enum SerializableType {
                Attribute = 1,          ///< Is a SerializableAttribute
                Node = 2               ///< Is a SerializableNode
            };

            /// @brief This should return the name of the node/attribute/NameValue Pair
            /// @details For Attributes this will be what comes before the '=' and for Nodes the name is the part of the xml tag as follows '<name foo="bar">foobar</name>'
            /// @return This returns a String
            virtual String GetName() const = 0;

            /// @brief This should return the innards/pcdata of an xml node, value of and attribute, The value of the name value pair.
            /// @details For Attributes this will be what comes after the '=' and for Nodes the value is the part of the xml tag as follows '<fubar foo="bar">Value</fubar>'
            /// @return This returns a String
            virtual String GetValue() const = 0;

            /// @brief This will do the work when calling the << operator
            /// @param Stream The ostream to stuff data into
            /// @return This will return a stream containing the serialized object
            virtual std::ostream& Serialize(std::ostream& Stream) const = 0;

            /// @brief Help identify type of serializable
            /// @return a SerializableType matching the type of the derived type.
            virtual SerializableType GetType() const = 0;

            /// @brief Virtual Deconstructor
            virtual ~Serializable()
                { }
        #endif
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Used to serialize Simple name Value pairs
    /// @details If PHYSXML is not defined this class declares nothing. This class implements all of serializable.
    ///////////////////////////////////////
     class PHYS_LIB SerializableAttribute : public Serializable
    {
        #ifdef PHYSXML
        protected:
            /// @brief Holds the Stuff before the equal sign
            String Name;

            ///@brief Holds the stuff
            String Value;

        public:
            /// @brief Initializing Constructor
            template<class Datum> SerializableAttribute(String _Name, Datum _Value) : Name(_Name), Value(ToString(_Value))
                { }

            /// @brief This returns the name of This Attribute
            /// @return This returns a String
            virtual String GetName() const
                { return Name; }

            /// @brief This returns the Value of this attribute as a string
            /// @return This returns a String Containing the Value
            virtual String GetValue() const
                { return Value; }

            /// @brief When calling the << operator, this will create [Name="Value"]
            /// @return This will return a stream containing the serialized name Value Pair.
            virtual std::ostream& Serialize(std::ostream& Stream) const
            {
                Stream << xml::EscapeXML(Name) << "=\"" << xml::EscapeXML(Value) <<"\"";
                return Stream;
            }

            /// @brief When calling the << operator, this will create an XML Attribute to help serialization move along a little quicker than with string manipulation
            /// @return This will return an XML Attribute that will hold the requested data.
            virtual xml::Attribute Serialize() const
            {
                xml::Attribute temp;
                temp.SetName(Name.c_str());
                temp.SetName(Value.c_str());
                return temp;
            }

            /// @brief Get the type of this
            /// @return Always returns Serializable::Attribute
            virtual SerializableType GetType() const
                { return Serializable::Attribute; }

            /// @brief Virtual Deconstructor
            virtual ~SerializableAttribute()
                { };
        #endif
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Used to serialize Complex items or items with subvalues
    /// @details If PHYSXML is not defined this class declares nothing. This class implements some of serializable.
    /// It is is expected that a derived class will implment GetName(), DeSerialize()
    /// \n GetName() Should return the name of the class being Serialized.
    /// \n
    ///////////////////////////////////////
    class PHYS_LIB SerializableNode : public Serializable
    {
        #ifdef PHYSXML
        protected:
            /// @brief This holds the data to serialize during the process of serializing
            mutable vector<Serializable*> ToSerialize;

        public:
            /// @brief Gather Serializables from derived classes
            /// @details This must be implemented by derived classes. This Should fill ToSerialize with a list of all the serializables that need to be made to properly preserve the state of the Serializable.
            virtual void GatherRequirements() const = 0;

            /// @brief When calling the << operator this is what, indirectly, does the work, this will create an XML Node to help serialization move along a little quicker than with string manipulation
            /// @return This will return an XML Node that will hold the Data.
            virtual xml::Node Serialize() const;

            /// @brief This will do the work when calling the << operator directly.
            /// @details This will Call GatherRequirements and expects it to be populated with the required data.
            /// @param Stream The ostream to stuff data into.
            /// @return This will return a stream containing the serialized object.
            virtual std::ostream& Serialize(std::ostream& Stream)
            {
                GatherRequirements();
                Stream << Serialize();
                return Stream;
            }

            /// @brief This will get the value as per the following
            /// @details This will Call GatherRequirements and expects it to be populated with the required data.
            /// @return This will return a String containing all the the serialized children of the Node
            virtual String GetValue() const;

            /// @brief This will do the work when calling the >> operator.
            /// @details When Streaming from an XML Doc to this Serializable this is called.
            /// @param OneNode The xml::Node that is being Serialized.
            virtual void DeSerialize(const xml::Node& OneNode) = 0;

            /// @brief Get the type of this
            /// @return Always returns Serializable::Node
            virtual SerializableType GetType() const
                { return Serializable::Node; }

            /// @brief Virtual Deconstructor
            virtual ~SerializableNode()
                { }

        #endif
    };

}

#ifdef PHYSXML
/// @brief This will call Serializable::Serialize() and put the contents into the Stream.
/// @param Stream The std::ostream that the serializable will be stuffed into.
/// @param Outy This is the object that will be serialized.
/// @return This returns Stream that is passed in, with the additional data of the serialized object
std::ostream& PHYS_LIB operator << (std::ostream& Stream, const phys::Serializable& Outy);

/// @brief Deserial XML in a stream to a proper Class
/// @param Iny to item to overwrite with the serialized data
/// @param Stream The istream to get XML from
/// @warning The XML in the stream needs to start with the same class you are serializing too.
std::istream& PHYS_LIB operator >> (std::istream& Stream, phys::SerializableNode& Iny);

/// @brief Simply calls Serializable Deserialize on The Item to deserialize
/// @param Iny The item to Serialize in
/// @param OneNode Parsed XML Data ready to go from the XML Subsystem.
void PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::SerializableNode& Iny);

#endif // \PHYSXML




#endif
