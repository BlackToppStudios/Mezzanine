// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _track_h
#define _track_h

#include "datatypes.h"
#include "exception.h"
#include "enumerations.h"
#include "interpolator.h"
#include "trackiterator.h"

#ifndef SWIG // STD headers are bad for Swig
    #include <cmath>
    #include <iterator>
#endif

namespace Mezzanine
{
    /// @brief A base type that provides container features for different tracks
    /// @details Tracks are containers of a Discrete set of points, that are presented as
    /// a continuous range from 0 to 1. Interpolators are used to generate the data between
    /// the points as it is needed. For example, lets say there is an interpolator provided
    /// for integers and a track contains only two data points 0 and 100, requesting 0.5
    /// might return 50.
    /// @n @n
    /// This uses std::vector underneath for its performance characteristics.
    template <typename InterpolatorType>
    class Track
    {
        public:
            /// @brief The type this class and the interpolator it uses works with.
            typedef typename InterpolatorType::InterpolatableType InterpolatableType;

            /// @brief The type of the internal container storing the interpolatable data. This is a single point to change all the tracks.
            /// @details If an interpolator requires a special container this can be re-implemented
            /// to have the TrackBase use that instead. The type must implement at least:
            ///     - size()
            ///     - push_back(InterpolatableType)
            ///     - clear()
            ///     - A copy constructor
            ///     - A constructor that accepts an iterator range
            ///     - An iterator type
            typedef typename InterpolatorType::Storage DataContainerType;

            /// @brief An iterator than can take an arbitrary amount of steps by interpolation.
            typedef SmoothTrackIterator<InterpolatorType> SmoothIteratorType;

        protected:
            /// @brief The underlying container of Discrete datapoints.
            DataContainerType DataPoints;

            /// @brief Name of the track, primarily for serialization.
            String RawName;

        public:
            /// @brief Create a Track from a range of data points
            Track(typename DataContainerType::iterator Begin,
                  typename DataContainerType::iterator End)
                : DataPoints(Begin,End)
                {}

            Track(const DataContainerType& DataSet) : DataPoints(DataSet)
                {}

            Track()
                {}

            virtual ~Track()
                {}

            /// @brief Get the amount of stored DataPoints
            /// @note Name chosen to match standard containers
            /// @return How many data points exist on this track
            /// @note Name for compatibility with std templates
            size_t size() const
                { return DataPoints.size(); }
            /// @brief Get the amount of stored DataPoints
            /// @note Name chosen to match standard containers
            /// @return How many data points exist on this track
            /// @note Name for consistency with naming conventions and implemented in terms of size().
            size_t Size() const
                { return size(); }

            /// @brief Add another data point to the end of the track.
            /// @param AddedValue The data point to add to theend of the track.
            /// @note Name for compatibility with std templates.
            virtual void push_back(const InterpolatableType& AddedValue)
                { DataPoints.push_back(AddedValue); }
            /// @brief Add another data point to the end of the track.
            /// @param AddedValue The data point to add to theend of the track.
            /// @note Name for consistency with naming conventions and implemented in terms of push_back().
            virtual void Add(const InterpolatableType& AddedValue)
                { push_back(AddedValue); }

            /// @brief Remove all the points from the track
            void clear()
                { DataPoints.clear(); }
            /// @copydoc clear
            void Clear()
                { clear(); }

            /// @brief Get an Smooth iterator to the beginning of the track.
            /// @details A Smooth iterator will take a fixed number of steps across
            /// a data set, interpolating values not actually present.
            /// @param Steps How many steps to take, defaults to 100.
            virtual SmoothIteratorType begin(Integer Steps=100) const
            {
                return SmoothIteratorType(this, 0.0,
                                            (Steps?(PreciseReal(1.0)/PreciseReal(Steps)):0.0)
                                          );
            }
            /// @copydoc begin
            virtual SmoothIteratorType Begin(Integer Steps=100) const
                { return begin(Steps); }


            /// @brief Get an Smooth iterator to the end (not one past) of the track.
            /// @details A Smooth iterator will take a fixed number of steps across
            /// a data set, interpolating values not actually present.
            /// @param Steps How many steps to take if any, defaults to 0.
            virtual SmoothIteratorType end(Integer Steps=0) const
            {
                return SmoothIteratorType(this, 1.0,
                                            (Steps?(PreciseReal(1.0)/PreciseReal(Steps)):0.0)
                                          );
            }
            /// @copydoc end
            virtual SmoothIteratorType End(Integer Steps=0) const
                { return end(Steps); }

            virtual InterpolatableType GetInterpolated(Real Percentage) const
            {
                return InterpolatorType::Interpolate(
                           DataPoints.begin(),
                           DataPoints.end(),
                           Percentage
                       );
            }

            /// @brief Set the name for serialization.
            /// @param Name the Name for finding this track in serialized streams.
            void SetTrackName(String Name)
                { RawName = Name; }

            /// @brief Get the given name or generate a default name
            /// @return This will either return whatever was set with SetTrackName(String Name) or some value that likely unique.
            /// @warning Do not set one tracks name to another and these will remain under all but the most extreme situations.
            const String& GetTrackName() const
            {
                if(RawName.empty())
                    { return ToString(this); }
                return RawName;
            }

            virtual void ProtoSerialize(XML::Node& CurrentRoot) const
            {
                Mezzanine::XML::Node TrackNode = CurrentRoot.AppendChild(SerializableName());

                if(TrackNode)
                {
                    Mezzanine::XML::Attribute VersionAttr = TrackNode.AppendAttribute("Version");
                    if( VersionAttr )
                    {
                        if( !VersionAttr.SetValue("1") )
                            { SerializeError("Create XML Version Attribute Values", SerializableName()); }
                    }else{
                        SerializeError("Create XML Version Attributes", SerializableName());
                    }


                    Mezzanine::XML::Attribute NameAttr = TrackNode.AppendAttribute("Name");
                    if( NameAttr )
                    {
                        if( !NameAttr.SetValue(GetTrackName()) )
                            { SerializeError("Create XML Name Values", SerializableName()); }
                    }else{
                        SerializeError("Create XML Name Attributes", SerializableName());
                    }

                    Mezzanine::XML::Node InterpolatorNode = TrackNode.AppendChild("Interpolator");
                    if(InterpolatorNode)
                    {
                        InterpolatorType::ProtoSerialize(InterpolatorNode);
                    }else{
                        SerializeError("Create XML Interpolator Node", SerializableName());
                    }

                    Mezzanine::XML::Node DataNode = TrackNode.AppendChild("DataPoints");
                    if(DataNode)
                    {
                        for(typename DataContainerType::const_iterator Iter = DataPoints.begin();
                            DataPoints.end()!=Iter;
                            ++Iter)
                            { Iter->ProtoSerialize(DataNode);}
                    }else{
                        SerializeError("Create XML DataPoints Node", SerializableName());
                    }

                }else{
                    SerializeError("Create XML Serialization Node", SerializableName());
                }
            }

            virtual void ProtoDeSerialize(const XML::Node& OneNode)
            {
                if ( String(OneNode.Name())==String(SerializableName()) )
                {
                    if(OneNode.GetAttribute("Version").AsInt() == 1)
                    {
                        Mezzanine::XML::Node InterpolatorNode = OneNode.GetChild("Interpolator").GetFirstChild();
                        if(InterpolatorNode.Name() == InterpolatorType::SerializableName())
                        {
                            InterpolatorType::ProtoDeSerialize(InterpolatorNode);
                        }else{
                            //DeSerializeError();
                            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Incompatible Interpolator Type Version for " + SerializableName() + ": Not " + InterpolatorType::SerializableName());
                        }
                    }else{
                        MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
                }

            }

            static String SerializableName()
                { return String("Track"); }


    };

    /// @brief A track that keeps an extra data point in the track to make sure it loops.
    /// @details when Add or push_back is called and there are 2 or more points the track,
    /// this seamlessly adds a copy of the first data point to the end of the track. When
    /// Iterating with a Smooth iterator this creates the impression of a loop.
    template <typename InterpolatorType>
    class TrackLooped : public Track<InterpolatorType>
    {
        protected:
            /// @brief The base most class of this type.
            typedef Track<InterpolatorType> BaseType;

        public:
            /// @brief The type this class and the interpolator it uses works with.
            typedef typename InterpolatorType::InterpolatableType InterpolatableType;

            /// @brief The type of the Container storing the interpolatable data. This is a single point to change all the tracks
            typedef std::vector<InterpolatableType> DataContainerType;

            /// @brief An iterator than can take an arbitrary amount of steps by interpolation.
            typedef SmoothTrackIterator<InterpolatableType> SmoothIteratorType;

            /// @brief
            TrackLooped(typename DataContainerType::iterator Begin,
                        typename DataContainerType::iterator End)
                : BaseType(Begin,End)
                {}

            TrackLooped(const DataContainerType& DataSet)
                : BaseType(DataSet)
                {}

            TrackLooped()
                {}

            ~TrackLooped()
                {}

            virtual void push_back(const InterpolatableType& AddedValue)
            {
                if(BaseType::DataPoints.size()>1)
                {
                    *(BaseType::DataPoints.end()-1) = AddedValue;
                    BaseType::DataPoints.push_back(*BaseType::DataPoints.begin());
                }else if(BaseType::DataPoints.size()==1){
                    BaseType::DataPoints.push_back(AddedValue);
                    BaseType::DataPoints.push_back(*BaseType::DataPoints.begin());
                }else{
                    BaseType::DataPoints.push_back(AddedValue);
                }
            }
    };


}//Mezzanine

#ifndef SWIG

/// @brief
/// @details
/// @param
/// @param
/// @return
template<typename T>
std::ostream& operator << (std::ostream& stream, const Mezzanine::Track<T>& Lint)
{
    Serialize(stream,Lint);
    return stream;
}

/// @brief
/// @details
/// @param
/// @param
/// @return
template<typename T>
std::istream& operator >> (std::istream& stream, Mezzanine::Track<T>& Lint)
    { return DeSerialize(stream, Lint); }


/// @brief
/// @details
/// @param
/// @param
/// @return
template<typename T>
std::ostream& operator << (std::ostream& stream, const Mezzanine::TrackLooped<T>& Lint)
{
    Serialize(stream,Lint);
    return stream;
}

/// @brief
/// @details
/// @param
/// @param
/// @return
template<typename T>
std::istream& operator >> (std::istream& stream, Mezzanine::TrackLooped<T>& Lint)
    { return DeSerialize(stream, Lint); }

#endif

#endif

