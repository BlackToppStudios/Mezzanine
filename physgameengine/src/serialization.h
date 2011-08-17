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
#ifndef _serialization_h
#define _serialization_h

#include "datatypes.h"
#include "xml.h"
#include "xmldoc.h"

#include <memory>

#ifdef PHYSXML
namespace phys
{

    /// @section Serialization
    /// The following functions are expected to be implemented on every serializable and deserializable:\n
    /// void ProtoSerialize(xml::Node& CurrentRoot) const; \n
    /// void ProtoDeSerialize(const xml::Node&); \n
    /// String SerializableName() const; \n
    /// Implementing these will allow them to work with the Serialize and Deserialize templates defined in
    /// serialization.h.

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A tool for serializing classes with specific issues serializing or deserializing them
    /// @details Some classes Must have certain values available at the time of construction. This make serializing them by overwriting an existing
    /// class instance impractical. Other classes have private members and it it impractical to change the class to expose this data. In this case a
    /// serializer could be made that is the friend of that class so it can do what is required.
    ///////////////////////////////////////
    class Serializer
    {
        #ifdef PHYSXML
        /// @brief Get the name of the clas to be serialized
        /// @return A string containing the name
        virtual String GetSerializedClassName() const = 0;

        /// @brief Get all of the data from the serializable class instance
        /// @details This is to be implemented in individual serializer with logic
        /// specific to the required tasks. It is expected to produced an xml::Node
        /// containing the entirety of the data required to reconstitute the serialized
        /// class.
        /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
        virtual void ProtoSerializeAll(xml::Node& CurrentRoot) const = 0;
        /// @brief Output to a stream the complete serialized data.
        /// @param Stream The std::ostream to send the data into.
        /// @details By default this is implemented in using ProtoSerializeAll().
        /// @return the modified ostream.
        virtual std::ostream& SerializeAll(std::ostream& Stream) const
        {
            phys::xml::Document Doc;
            Doc.Load("");
            ProtoSerializeAll(Doc);
            Doc.Print(Stream);
            return Stream;
        }

        /// @brief Convert An XML Node into a complete live data structure
        /// @param OneNode A pointer to the XML node to reconstitute into multiple Live classes.
        /// @details This is expected to put the deserialized items somewhere they can be accessed by the calling, but provides no facility for working them itself. Not implemented in default serializer.
        virtual void ProtoDeSerializeAll(xml::Node& OneNode) = 0;
        /// @brief Send the serialized version of all the live data into the stream.
        /// @param Stream The std::istream to get the data from.
        /// @details The default implementation of this uses ProtoDeSerializeAll(xml::Node*) to accept
        /// The complete XML to serialise and assemble it in memory.
        /// @return This returns the input stream after the xml document has been extracted from it.
        virtual std::istream& DeSerializeAll(std::istream& Stream)
        {
            phys::String OneTag( phys::xml::GetOneTag(Stream) );
            std::auto_ptr<phys::xml::Document> Doc(phys::xml::PreParseClassFromSingleTag(GetSerializedClassName(), OneTag) );
            ProtoDeSerializeAll(*Doc);
            return Stream;
        }

        /// @brief Get all the one data about one Submember in an xml::Node
        /// @param Name The name of the Serialize item to serialize
        /// @details This is not implemented by default.
        /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
        virtual void ProtoSerialize(const String& Name, xml::Node& CurrentRoot) = 0;
        /// @brief Output the specified member to a stream
        /// @param Name The name of the Serialize item to serialize
        /// @param Stream The std::ostream to send the data into.
        /// @details The default implementation of this uses ProtoSerialize(const String&)
        /// @return The std::ostream that was passed in.
        virtual std::ostream& Serialize(std::ostream& Stream, const String& Name)
        {
            phys::xml::Document Doc;
            Doc.Load("");
            ProtoSerialize(Name,Doc);
            Doc.Print(Stream);
            return Stream;
        }

        /// @brief Convert An XML Node into a complete live data structure
        /// @param OneNode A pointer to the XML node to reconstitute into a live class instance.
        /// @details This accepts a pointer to a node because not all nodes are copy constructable. Not implemented in default serializer.
        virtual void ProtoDeSerialize(xml::Node* OneNode, const String& Name) = 0;
        /// @brief Send the serialized version of all the live data into the stream.
        /// @param Stream The std::istream to get the data from.
        /// @details The default implementation of this uses ProtoDeSerializeAll(xml::Node*) to accept
        /// The complete XML to serialise and assemble it in memory.
        /// @return This returns the input stream after the xml document has been extracted from it.
        virtual std::istream& DeSerialize(std::istream& Stream, const String& Name) = 0;
        #endif
    };

    /// @brief Convert any class that supports serialization or has a serializer to a string of chars in a stream
    /// @details Any Class will work with this template as long as it implements the method "xml::Node ProtoSerialize(xml::Document&) const"
    /// @param Stream The ostream to put the serializable into.
    /// @param Converted The item to be serialized, which must have a "xml::Node ProtoSerialize(xml::Node& CurrentRoot) const" method.
    /// @param Indent Defaults to nothing but can be set to "\t" to get normal
    /// @return A the stream that was passed and now contains the serialized object.
    template <class T>
    std::ostream& Serialize(std::ostream& Stream, const T& Converted, const String& Indent = String("") )
    {
        phys::xml::Document Doc;
        Doc.Load("");           // This sets the encoding to UTF8 ?!
        Converted.ProtoSerialize(Doc);
        Doc.Print(Stream, Indent.c_str());
        return Stream;
    }

    /// @brief Deserialize the next xml tag in the stream into a specific in memory class instance.
    /// @details "void ProtoDeSerialize(const xml::Node&)" and "String SerializableName() const" must be implemented on the
    /// the class instance that is passed in for this to work
    /// @param Stream The istream to extract the required data from
    /// @param Converted The Class member that is deserialized.
    /// @return This returns the istream that provided the serialized data.
    template <class T>
    std::istream& DeSerialize(std::istream& Stream, T& Converted)
    {
        phys::String OneTag( phys::xml::GetOneTag(Stream) );
        std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", Converted.SerializableName(), OneTag) );

        Converted.ProtoDeSerialize(Doc->GetFirstChild());

        return Stream;
    }

    /// @internal
    /// @brief Used to interface with a previous version of the serialization code.
    /// @details The older serialization was implemented entirely in streaming operators. This uses those, however inneficient to get the xml::Node that
    /// the current serialization solution is centered around.
    /// @param Converted The class implementing older serialization code.
    /// @param CurrentRoot The place in the xml hiearchy to append the items to be sloppily ProtoSerialized.
    template <class T>
    void SloppyProtoSerialize(const T& Converted, xml::Node& CurrentRoot)
    {
        stringstream Depot;         //Make a place to store serialized XML
        xml::Document Staging;      //Make a place to convert from XML to an xml node
        Depot << Converted;         //Use old conversion tools to convert to serialized XML as if writing to a file
        Staging.Load(Depot);        //Load To the staging area as if loading XML form a file or whatever.

        CurrentRoot.AppendCopy(Staging.DocumentElement()); //Append our work as an xml::node to the desired place in the xml Hierarchy.
    }

    /// @brief Simply does some string concatenation, then throws an Exception
    /// @param FailedTo What failed to happed for example "create testnode" or "acquire a mutex"
    /// @param ClassName The name of the class throw the exception
    /// @param SOrD Defaults to true, and if true uses the word "Serialization", otherwise uses the word "DeSerialization"
    /// @throw A phys::Exception with the message "Could not {FailedTo} during {ClassName} [De]Serialization."
    void SerializeError(const String& FailedTo, const String& ClassName, bool SOrD = true);

    /// @brief Simply does some string concatenation, then throws an Exception
    /// @param FailedTo What failed to happed for example "create testnode" or "acquire a mutex"
    /// @param ClassName The name of the class throw the exception
    /// @param SOrD Defaults to false, and if true uses the word "Serialization", otherwise uses the word "DeSerialization"
    /// @throw A phys::Exception with the message "Could not {FailedTo} during {ClassName} [De]Serialization."
    /// @details This just calls SerializeError() with the third parameter false. This exists solely to make code
    /// A little more readable.
    void DeSerializeError(const String& FailedTo, const String& ClassName, bool SOrD = false);
}

/*
/// @brief This will call convert an xml::Node into Text in a stream
/// @param Stream The std::ostream that the serializable will be stuffed into.
/// @param OneNode The xml to be converted
/// @return This returns Stream that is passed in, with the additional data of the serialized object.
std::ostream& PHYS_LIB operator << (std::ostream& Stream, const phys::xml::Node& OneNode)
{
    OneNode.Print(Stream);
    return Stream;11
}
*/

#endif // \PHYSXML




#endif
