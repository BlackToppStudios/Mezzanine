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
#ifndef _serialization_h
#define _serialization_h

/// @file
/// @brief The interface for serialization

#include "datatypes.h"
#include "countedptr.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    /// @page Serialization
    /// @section SerializationMain
    /// Serialization is the process of converting a class instance into a serial sequence of bits. DeSerialization is taking those
    /// bits and reconstructing the original object without losing anything of value (in theory the only things not saved are cached
    /// values or values calulated as they are needed). These bits could be just about anything, because of its ubiquity we chose
    /// to serialize to xml (or something so similar as to be indistinguishable from standard xml). This allows a wide variety of tools
    /// to be used when working with and verifying these serialized classes. Additionally, transmitting and storing xml is easy to do,
    /// and can be done with a variety of other factors in mind. The xml text can be sent down any stream, put in any file, compressed,
    /// queried. You should see @ref XMLManual for information about the xml system itself.
    /// \n \n
    /// Topics:
    ///     - @ref serializationxml
    ///     - @ref serializationintegration
    ///         - @ref serializationmaking
    ///         - @ref serializationserializers
    ///         - @ref serializationlegacy
    ///     - @ref serializationoperators
    ///     - @ref serializationmisc
    /// @subsection serializationxml Serialization and XML
    /// The process of serializing doesn't just convert from class instance to text. Since our end goal is to convert live objects to
    /// XML it makes sense to closely integrate the Mezzanine::xml portion of the engine. If you plan on writing serialization
    /// and deserialization code you should read the following parts of the @ref XMLManual at a minimum:
    ///     - @ref XMLDOM
    ///     - @ref XMLAccessingBasics
    ///     - @ref XMLModifyingNodeData
    ///     - @ref XMLModifyingAttributeData
    ///
    /// The central object that will carry information during this process is the Mezzanine::XML::Node. The Mezzanine::XML::Node is an excellent
    /// tool for converting and storing data in a single unified heirarchy.
    /// \n \n
    /// C++ and most other object oriented languages heavily imply that class inheritance should be structured as hierarchies. Additionally
    /// Hierarchies are implied when complex class has other complex classes or datatypes as members. Both of these structures map
    /// cleanly onto the kind of hierarchies that a well formed xml documents provide.
    /// \n \n
    /// There are some relationships in video game objects that cross normal hierarchical boundaries. For example, A constraint
    /// references two actors, and defines a relationship between them. When serialized the constraint simply stores the name of
    /// the actor and looks it up in the actor manager during deserialization. This implies that the actors exist already, or that
    /// there is some mechanism to create the constraint and fill in the actor later. Several mechanisms were discussed to
    /// accomplish this, some include: two passes of processing where constraint would be done in the second pass, a work queue that
    /// would store objects that couldn't be deserialized yet, a prefetcher that would dig through the xml to find the required
    /// object.
    /// \n \n
    /// Those methods all likely could have been made to work. However, they are not optimal for a variety of reasons. All of them
    /// have a set of preconditions and require more computing resources and could potentially delay loading or transmission times.
    /// Some of them heavily imply that all of the items to deserialize must be stored in the same xml source. Some demand access to
    /// xml that may not have been transmitted yet.
    /// \n \n
    /// The simplest, most performant way to work around the issues that cross-hierarchical relationships presented was to ignore
    /// them. More specifically, throw an exception if an object reference during deserialization is not present. Then we ask that
    /// programmers who write code that must store, transmit and reconstruct class instances be aware of the following preconditions
    /// So can produce their own solutions:
    ///     - CollisionShapes must come before Actors and AreaEffects
    ///     - WorldNodes must come before Actors, Light and ParticleEffects. (this is still work in progress).
    ///         - Currently WorldNodes try to find the objects that are attached to them, and the attached tries to find the world node. (if one does not exist, this silently fails)
    ///     - Actors must come before constraints.
    ///     - Actors may have a WorldNode inside them, if this is the case, then the actor must come before Lights and ParticleEffects Attached to it.
    ///     - Actors must be done before SoundSets
    ///     - Sounds must be done before SoundSet (Still in progress)
    ///
    /// The easyiest way to meet these conditions and not consume an inordinate amount of computing resources, is to pay attention
    /// to the order that items are serialized in. If a program serializes the worldnodes, then the actors, then everything else
    /// it will have relatively little trouble making it work.
    /// @subsection serializationintegration Integrate Serialization into Your Code
    /// There several ways to interact with the current serialization system. One might have to create a class that can be
    /// serialized or deserialized. There may be situations where another system is emitting xml and it must be intergrated into
    /// an existing game. It may be desired to create a 'factory' that produces objects from and xml source or create a sink to put
    /// objects into so they can be serialized. Here we will discuss some of the ways that the serialization system can be extended
    /// and what kind of assumptions it makes, so that anyone can write software that interacts with it cleanly.
    /// @subsubsection serializationmaking Make a Serializable or a DeSerializable
    /// Creating a class that be serialized is easy. There is just one function that it must implement. If a class implements this,
    /// it is said to be Serializable:
    /// @code
    /// void SerializableClass::ProtoSerialize(XML::Node&) const;
    /// @endcode
    /// The member ProtoSerialize(XML::Node&) is expected to accept a Mezzanine::XML::Node and attach exactly one Node to it. This new Serialized
    /// node should contain all the data stored in the current state of the object being serialized. Storing data outside of this
    /// one node could cause undefined behavior.
    /// \n \n
    /// The exact layout of the data in the Serialized Node is not pre-determined. The creator of that function need only
    /// take into account any difficulties DeSerializing when creating this. Because of this concern it is advisable name the Serialized
    /// node something unique and appropriate and to include a 'Version' attribute on it. If the class
    /// changes, the DeSerialization function will only need to check the 'Version' attribute to know if and how it can handle it.
    /// \n \n
    /// Integrating with the DeSerialization code is pretty easy too. There are two functions you are expected to implement to
    /// create a DeSerializable:
    /// @code
    /// void DeSerializableClass::ProtoDeSerialize(const XML::Node&);
    /// static String DeSerializableClass::GetSerializableName();
    /// @endcode
    /// The GetSerializableName() is expected to simply return the name of the xml elements this class will DeSerialize. For example
    /// A Mezzanine::Vector3 returns "Vector3", and a Mezzanine::ActorRigid return "ActorRigid". If a class is both DeSerializable and
    /// serializable it makes sense to call this function when assigning the name to the Serialized Node it creates.
    /// \n \n
    /// ProtoDeSerialize(const XML::Node&), accepts a Mezzanine::XML::Node. The Node passed to it would correspond to the Serialized
    /// Node created by the ProtoSerialize(XML::Node&) function listed above. If xml is created by something then this is calling
    /// code is expecting this function to be the correct deserialization function. It is advisable but not required to verify the
    /// name of the xml node matches what is expected and that
    /// the 'Version' is something this code can handle. It is also advisable that every piece of data pulled out is verified the
    /// best it can be. If exceptions are thrown for every discrepency, then programmers using this will create xml and code that
    /// produce no discrepencies.
    /// \n \n
    /// The following template make use of only the 3 functions described above to Serialize or DeSerialize class instances:
    /// @code
    /// template <class T> std::ostream& Serialize(std::ostream& Stream, const T& Converted, const String& Indent = String("") );
    /// template <class T> std::istream& DeSerialize(std::istream& Stream, T& Converted);
    /// @endcode
    /// The functions make calls on the Mezzanine::xml system and expect a fairly basic set of conditions to be met before they are used.
    /// Serialize accepts an output stream and the class instance to be Serialized. It will create an XML::Document and populate it
    /// data from the class provided and then emit that into the stream. DeSerialize accepts an inputstream and the object to be
    /// populated. It expects the next xml element in the stream to be a serialized version of the passed object and will then
    /// overwrite as many of the values of the passed object as possible with the serialized values. For small items DeSerialize
    /// is fine, where possible it is better to have the XML::Document open the file or stream itself as to prevent the second
    /// pass through to find exactly one xml element.
    /// \n \n
    /// @subsubsection serializationserializers Working with Serializers and Deserializers
    /// In some cases, there are some pieces of information that cannot be supplied or
    /// entered by the class itself. This data must be provided by another class or upon creation of the class. This other class
    /// can implement the Serializer, DeSerializer, or both interfaces to make working with large amounts of serialization easier.
    /// \n \n
    /// For example EntityProxy can only accept a mesh upon construction. So overwriting an existing EntityProxy is impossible to do completely.
    /// It expected to be partially implemented, to the extent possible, in the class members. But if you have the need to create
    /// EntityProxys on the fly from data stored in files it makes sense to have a dedicated class or interface than can create these.
    /// Here is what goes into a Serializer:
    /// @code
    /// template <class Serializable> class Serializer
    /// {
    ///     virtual void Serializer::ProtoSerializeAll(XML::Node& CurrentRoot) const = 0;
    ///     virtual std::ostream& Serializer::SerializeAll(std::ostream& Stream) const;
    ///     virtual void Serializer::ProtoSerialize(const Serializable& Target, XML::Node& CurrentRoot) = 0;
    ///     virtual std::ostream& Serializer::Serialize(std::ostream& Stream, const Serializable& Target)
    /// };
    /// @endcode
    /// Serializer::ProtoSerialize() when implement should take the required steps to attach a Serialized Node to the
    /// Passed XML::Node that represent the Serialization target. It is expected to get the extra information that the target
    /// cannot provide from somewhere else. Ideally the the Serializer can be, or be associated with, a manager or container
    /// of some kind. There is not default implementation of this.
    /// \n \n
    /// Serializer::Serialize() Goes one step further than Serializer::ProtoSerialize() and also sends it down a stream. The
    /// default implements use Serializer::ProtoSerialize().
    /// \n \n
    /// Serializer::ProtoSerializeAll() performs a similar role to Serializer::ProtoSerialize(), but again, it goes one step
    /// further. Rather than accept a single Target to serialize it is expected that the Serializer go to the source of the
    /// Targets and serialize all of them that are available. All of the target should be contained in one Node attached to
    /// the Node the function accepts. This is not implemented by default, the logic is too specific to the items to be
    /// serialized.
    /// \n \n
    /// Serializer::SerializeAll() uses Serializer::ProtoSerializeAll() to send all of the available Targets in Serialized
    /// down a stream.
    /// \n \n
    /// The logic behind a DeSerializer is similar to a Serializer. The same types of methods, even similar implementations
    /// if the function is implemented. Like the ProtoDeSerialize() individual DeSerializables implement, the functions on
    /// a DeSerializer will be passed the nodes that would correspond to the those created by their counterparts on the
    /// Serializer. Here is the contents of a Deserializer:
    /// @code
    /// template <class DeSerializable> class DeSerializer
    /// {
    ///     virtual void DeSerializer::ProtoDeSerializeAll(const XML::Node& OneNode) = 0;
    ///     virtual std::istream& DeSerializer::DeSerializeAll(std::istream& Stream)
    ///     virtual DeSerializable* DeSerializer::ProtoDeSerialize(const XML::Node& OneNode) = 0;
    ///     virtual std::istream& DeSerializer::DeSerialize(std::istream& Stream)
    ///     virtual String ContainerName() const = 0;
    /// };
    /// @endcode
    /// The function ContainerName() should be used when creating and verifying the xml element that is parent to the items
    /// DeSerialized by ProtoDeSerializeAll(). The Default implmentation of DeSerializeAll() will use ContainerName to
    /// verify it has extracted the correct Node.
    /// \n \n
    /// There is no technical reason why a class cannot be both a serializer and a deserializer, or even multiple kinds of
    /// Serializers or DeSerializers. To keep things simple the Managers provided by the Mezzanine engine will store a pointer
    /// to the appropriate Serializer when one is required.
    /// @subsubsection serializationlegacy Integrating with External XML Providers
    /// Sometimes yu will be forced to work with a system that produces xml that is not structured in a similar way to this
    /// system. Sometimes it may be too costly or not possible to modify the code to integrate it. For these the following
    /// function exists:
    /// @code
    /// template <class T> void SloppyProtoSerialize(const T& Converted, XML::Node& CurrentRoot)
    /// @endcode
    /// This function will make a call on the the stream insertion operator of the class passed in. If one doesn't exist
    /// it is easy to add one in your code without chaning the original source. If one does exist than you should probably
    /// copy/paste the whole function and re-implement it calling the functions that emit the XML string or stream. If you
    /// want to implement a stream insertion operator, the function prototype should be similar to the stream insertion
    /// operator in the @ref serializationoperators section.
    /// @subsection serializationoperators Serialization Operators
    /// The stream insertion (<<) and stream extraction (>>) operators can be used for serializing and deserializing most items
    /// in the Mezzanine engine.
    ///
    /// Unfortunately due to conflict with the stream insertion operators provided with the iostreams library these couldn't be
    /// made into a template. That doesn't mean that they are difficult to implement. Here is a typical implemenation of stream
    /// insertion operators for XML serialization:
    /// @code
    /// std::ostream& operator << (std::ostream& stream, const Mezzanine::RigidDebris& DebrisToSerialize)
    /// {
    ///     Serialize(stream, DebrisToSerialize);
    ///     return stream;
    /// }
    ///
    /// std::istream& operator >> (std::istream& stream, Mezzanine::RigidDebris& x)
    ///     { return DeSerialize(stream, x); }
    ///
    /// void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::RigidDebris& x)
    ///     { x.ProtoDeSerialize(OneNode); }
    ///
    /// @endcode
    /// You will want to implement these functions with the appropriate type. The type Mezzanine::RigidDebris is used purely as example
    /// Though this is actual working code and was in the engine at one point, the current code is more sophiscticated
    /// \n \n
    /// The function operator<< simply calls Serialize and returns the stream, so it has all the pre and cost conditions of the Serialize
    /// function listed in the @ref serializationmaking section.
    /// \n \n
    /// The stream extraction operators are a little bit more interesting. The operator>>(istream,YourType), by virtue of calling Deserialize
    /// will wind up taking two passes over the XML. One looking for the ending tag that matches the first (it gets all the children of that tag too)
    /// and one performing the actual parsing. The operator>>(istream,YourType) will work only with completely parsed objects in memory. With the
    /// combination of these two all the heavy lifting of parsing is done up front, and the rest of the deserialization is just a bunch of pointer
    /// and string manipulation. Another possibility with your stream extraction operator, if you new that it had exactly one parent xml node ,you
    /// create without that first pass for improved performance.
    /// @subsection serializationmisc Other little Things
    /// To simplify and standardize errors thrown, the following functions exist:
    /// @code
    /// void SerializeError(const String& FailedTo, const String& ClassName, Boole SOrD = true);
    /// void DeSerializeError(const String& FailedTo, const String& ClassName, Boole SOrD = false);
    /// @endcode
    /// Both of these functions throw a Mezzanine::Exception with the descriptive text of "Could not {FailedTo} during {ClassName} [De]Serialization."
    /// If SOrD (Serialize Or Deserialize) is true the "De" is not printed.




    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A tool for serializing classes with specific issues serializing.
    /// @details Some classes have private members and it is impractical to change the class to expose this data. In this case a
    /// serializer could be made that to work around this limitation.
    /// \n \n
    /// This was designed with the idea that a manager could inherit from this or have a separate class that implements this as a member. There should also be
    /// no reason why something could not inherit from this and Mezzanine::DeSerializer. The type of this template is expected to match what this is serializing.
    ///////////////////////////////////////
    template <class Serializable>
    class MEZZ_LIB Serializer
    {
        public:
        /// @brief Get all of the data from the serializable class instance
        /// @details This is to be implemented in individual serializer with logic
        /// specific to the required tasks. It is expected to produce an XML::Node
        /// containing the entirety of the data required to reconstitute the serialized
        /// class. \n \n
        /// This is expected to gets it's knowledge about what to serialize some other than
        /// being passed as an argument. It could query a manager or be passed a series pointers
        /// that is needs to work with.
        /// \n \n
        /// This is not implemented by default.
        /// @param CurrentRoot The point in the XML hierarchy that all the items deserialized should be appended to.
        virtual void ProtoSerializeAll(XML::Node& CurrentRoot) const = 0;
        /// @brief Output the complete serialized data to a stream.
        /// @param Stream The std::ostream to send the data into.
        /// @details By default this is implemented in using ProtoSerializeAll().
        /// @return The populated ostream.
        virtual std::ostream& SerializeAll(std::ostream& Stream) const
        {
            Mezzanine::XML::Document Doc;
            Doc.Load("");
            ProtoSerializeAll(Doc);
            Doc.Print(Stream);
            return Stream;
        }

        /// @brief Get all the serialized data about one class instance in an XML::Node
        /// @param Target A reference to class instance to be deserialized.
        /// @details This is not implemented by default.
        /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
        virtual void ProtoSerialize(const Serializable& Target, XML::Node& CurrentRoot) = 0;
        /// @brief Output the specified member to a stream
        /// @param Target A reference to class instance to be deserialized.
        /// @param Stream The std::ostream to send the data into.
        /// @details The default implementation of this uses ProtoSerialize(const String&)
        /// @return The std::ostream that was passed in.
        virtual std::ostream& Serialize(std::ostream& Stream, const Serializable& Target)
        {
            Mezzanine::XML::Document Doc;
            Doc.Load("");
            ProtoSerialize(Target,Doc);
            Doc.Print(Stream);
            return Stream;
        }

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A tool for deserializing classes with specific issues deserializing them
    /// @details Some classes Must have certain values available at the time of construction. This make deserializing them by overwriting an existing
    /// class instance impractical. \n \n
    /// This is expected to work with classes that have implemented the required DeSerializable functions. Specifically This makes use of
    /// "static String GetSerializableName()", and it is expected that functions that must be implemented would call on "void ProtoDeSerialize(const XML::Node&)".
    /// The type of this template is expected to match what this is deserializing.
    /// \n \n
    /// This was designed with the idea that a manager could inherit from this or have a separate class that implements this as a member. There should also be
    /// no reason why something could not inherit from this and Mezzanine::Serializer.
    ///////////////////////////////////////
    template <class DeSerializable>
    class MEZZ_LIB DeSerializer
    {
        public:
        /// @brief Convert An XML Node into a complete series of live class instances
        /// @param OneNode A reference to the XML node to reconstitute into multiple Live classes.
        /// @details This is expected to put the deserialized items somewhere they can be accessed by the calling,
        /// but provides no facility for working them itself. \n \n
        /// Not implemented in default DeSerializer.
        virtual void ProtoDeSerializeAll(const XML::Node& OneNode)
        {
            // no checking occurs here, because this should be DeSerializeAll(istream&)
            XML::Node SingleItemNode = OneNode.GetFirstChild();
            while(SingleItemNode)
            {
                ProtoDeSerialize(SingleItemNode);
                SingleItemNode = SingleItemNode.GetNextSibling();
            }
        }
        /// @brief Get One node that has several of the appropriate kinds of nodes as children and deserialize all of them
        /// @param Stream The std::istream to get the data from.
        /// @details The default implementation of this uses ProtoDeSerializeAll(XML::Node&) to accept11
        /// The complete XML to serialise and assemble it in memory.
        /// @return This returns the input stream after the xml document has been extracted from it.
        virtual std::istream& DeSerializeAll(std::istream& Stream)
        {
            Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(Stream) );
            Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc(Mezzanine::XML::PreParseClassFromSingleTag(this->ContainerName(), OneTag) );
            ProtoDeSerializeAll(Doc->GetFirstChild());
            return Stream;
        }

        /// @brief Convert An XML Node into a complete live data structure
        /// @param OneNode A reference to the XML node to reconstitute into a live class instance.
        /// @details Not implemented in default serializer.
        /// @return A pointer to the freshly deserialized class instance.
        virtual DeSerializable* ProtoDeSerialize(const XML::Node& OneNode) = 0;
        /// @brief Get the serialized version of all the live data from the stream.
        /// @param Stream The std::istream to get the data from.
        /// @details The default implementation of this uses ProtoDeSerializeAll(XML::Node*) to accept
        /// The complete XML to serialise and assemble it in memory.
        /// @return This returns the input stream after the xml document has been extracted from it.
        virtual std::istream& DeSerialize(std::istream& Stream)
        {
            Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(Stream) );
            Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc(Mezzanine::XML::PreParseClassFromSingleTag(DeSerializable::GetSerializableName(), OneTag) );
            ProtoDeSerialize(Doc->GetFirstChild());
            return Stream;
        }

        /// @brief This will return the Name of the element that Contains multiple of the items to be DeSerialized
        /// @return A String that correctly indicates the name of an xml tag.
        virtual String ContainerName() const = 0;
    };

    /// @brief Convert any class that supports serialization or has a serializer to a string of chars in a stream
    /// @details Any Class will work with this template as long as it implements the method "XML::Node ProtoSerialize(XML::Document&) const"
    /// @param Stream The ostream to put the serializable into.
    /// @param Converted The item to be serialized, which must have a "XML::Node ProtoSerialize(XML::Node& CurrentRoot) const" method.
    /// @param Indent Defaults to nothing but can be set to "\t" to get normal
    /// @return A the stream that was passed and now contains the serialized object.
    template <class T>
    std::ostream& Serialize(std::ostream& Stream, const T& Converted, const String& Indent = String("") )
    {
        Mezzanine::XML::Document Doc;
        Doc.Load("");           // This sets the encoding to UTF8 ?!
        Converted.ProtoSerialize(Doc);
        Doc.Print(Stream, Indent.c_str());
        return Stream;
    }

    /// @brief Deserialize the next xml tag in the stream into a specific in memory class instance.
    /// @details "void ProtoDeSerialize(const XML::Node&)" and "static String GetSerializableName() const" must be implemented on
    /// the class instance that is passed in for this to work
    /// @param Stream The istream to extract the required data from
    /// @param Converted The Class member that is deserialized.
    /// @return This returns the istream that provided the serialized data.
    template <class T>
    std::istream& DeSerialize(std::istream& Stream, T& Converted)
    {
        Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(Stream) );
        Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", Converted.GetSerializableName(), OneTag) );

        Converted.ProtoDeSerialize(Doc->GetFirstChild());

        return Stream;
    }

    /// @internal
    /// @brief Used to interface with a previous version of the serialization code.
    /// @details The older serialization was implemented entirely in streaming operators. This uses those, however inneficient to get the XML::Node that
    /// the current serialization solution is centered around.
    /// @param Converted The class implementing older serialization code.
    /// @param CurrentRoot The place in the xml hiearchy to append the items to be sloppily ProtoSerialized.
    template <class T>
    void SloppyProtoSerialize(const T& Converted, XML::Node& CurrentRoot)
    {
        std::stringstream Depot;         //Make a place to store serialized XML
        XML::Document Staging;      //Make a place to convert from XML to an xml node
        Depot << Converted;         //Use old conversion tools to convert to serialized XML as if writing to a file
        Staging.Load(Depot);        //Load To the staging area as if loading XML form a file or whatever.

        CurrentRoot.AppendCopy(Staging.DocumentElement()); //Append our work as an XML::node to the desired place in the xml Hierarchy.
    }

    /// @brief Simply does some string concatenation, then throws an Exception
    /// @param FailedTo What failed to happed for example "create testnode" or "acquire a mutex"
    /// @param ClassName The name of the class throw the exception
    /// @param SOrD Defaults to true, and if true uses the word "Serialization", otherwise uses the word "DeSerialization"
    /// @throw A Mezzanine::Exception with the message "Could not {FailedTo} during {ClassName} [De]Serialization.""Could not {FailedTo} during {ClassName} [De]Serialization."
    void MEZZ_LIB SerializeError(const String& FailedTo, const String& ClassName, Boole SOrD = true);

    /// @brief Simply does some string concatenation, then throws an Exception
    /// @param FailedTo What failed to happed for example "create testnode" or "acquire a mutex"
    /// @param ClassName The name of the class throw the exception
    /// @param SOrD Defaults to false, and if true uses the word "Serialization", otherwise uses the word "DeSerialization"
    /// @throw A Mezzanine::Exception with the message "Could not {FailedTo} during {ClassName} [De]Serialization."
    /// @details This just calls SerializeError() with the third parameter false. This exists solely to make code
    /// A little more readable.
    void MEZZ_LIB DeSerializeError(const String& FailedTo, const String& ClassName, Boole SOrD = false);
}//Mezzanine

/*
/// @brief This will call convert an XML::Node into Text in a stream
/// @param Stream The std::ostream that the serializable will be stuffed into.
/// @param OneNode The xml to be converted
/// @return This returns Stream that is passed in, with the additional data of the serialized object.
//template <>
std::ostream& MEZZ_LIB operator<< <Mezzanine::XML::Node> (std::ostream& Stream, const Mezzanine::XML::Node& OneNode)
{
    OneNode.Print(Stream);
    return Stream;
}
*/

#endif
