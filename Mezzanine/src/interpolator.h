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
#ifndef _interpolator_h
#define _interpolator_h

/// @file
/// @brief Helper classes to assist in generating data points between two other data points.

#include "datatypes.h"
#include "exception.h"
#include "cubicspline.h"
#include "serialization.h"

#ifndef SWIG // STD headers are bad for Swig
    #include <cmath>
    #include <iterator>

    #include <iostream>
    #include <typeinfo>

    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    // Forward declaration
    template <typename InterpolatableType, typename InterpolatorType> class TrackStorage;

    /// @brief A simple functor for interpolating data points in a simple way.
    /// @details This interpolator provides certain guarantees.
    ///     - All data points will be valid interpolated values.
    ///     - There are "corners".
    ///     - Interpolation will be constant/O(1) time (and as fast as reasonable).
    ///     - This shape defined by interpolating a set of these will not leave a Convex Hull(or Axis Aligned Bounding Box) that could contain the data
    /// Corners can be thought of as any non-smooth change, and may not be intuitively in some interpolatable
    /// types.
    template <typename T>
    class MEZZ_LIB LinearInterpolator
    {
        public:
            /// @brief The type this will interpolate
            /// @note All Interpolators need to declare an InterpolatableType
            typedef T InterpolatableType;
            /// @brief The storage to use with thison tracks
            typedef std::vector<InterpolatableType> Storage;

            /// @brief Get a value at a given location between exactly two others.
            /// @param Begin The data point at one end of line segment
            /// @param End  The data point at the other end of line segment
            /// @param Location A value between 0.0 and 1.0 indicate what point on the line segment defined by Begin and End you want.
            /// @return A Value equal to Begin if 0.0 is passed, equal to End if 1.0 is passed equal to a point exactly in the middle if 0.5 is passed.
            static T InterpolateMath(T Begin, T End, Real Location)
                { return ((End-Begin)*Location)+Begin; }

            /// @brief Handles Interpolation of multiple points.
            /// @details This will treat each data point as if it were equidistant from its
            /// neighbors and find the datasegment the desired point resides in. Then it will
            /// return a data point partway through that data segment. For example if you have
            /// three Vector2's defining two data segments as follows:
            /// @n 0,0 - 1,1 - 2,0
            /// @n @n
            /// Requesting the following locations would return the following data points:
            /// @n 0.0 = 0,0
            /// @n 0.5 = 1,1
            /// @n 1.0 = 2,0
            /// @n
            /// @n 0.25 = 0.5,0.5
            /// @n 0.75 = 1.5,0.5
            /// @n @n Any floating point location between 0 and 1 could be requested following
            /// this pattern.
            /// @n @n Should the data segments not actually be equal in size that they will each
            /// still be treated as an equal length when calcuting size. For example if you
            /// have 2 data segments like the previous example, then .25 will be halfway through
            /// the first segment and .75 will be halfwy through the second. If you are
            /// interpolating a series of points through segment like these the larger data
            /// segment will be traversed faster. For example consider the following Real value
            /// values as data points:
            /// @n -5, 0, 100
            /// Requesting the following locations would return the following data points:
            /// @n 0.0 = -5
            /// @n 0.5 = 0
            /// @n 1.0 = 100
            /// @n
            /// @n 0.25 = -2.5
            /// @n 0.75 = 50
            /// @n
            /// @n 0.0 = -5
            /// @n 0.1 = -4
            /// @n 0.2 = -3
            /// @n 0.3 = -2
            /// @n 0.4 = -1
            /// @n 0.5 = 0
            /// @n 0.6 = 20
            /// @n 0.7 = 40
            /// @n 0.8 = 60
            /// @n 0.9 = 80
            /// @n 1.0 = 100
            /// @n @n Note how even though the location increase by only 0.1 each step
            /// the resulting interpolated data point move relative to the length of the
            /// segment.
            /// @param Begin An iterator at the beginning of a rande of data point
            /// @param End An iterator one past the end of the data range to interpolate.
            /// @param Location A value between 0.0 and 1.0 that represents
            /// @return A T at a location along the data segments defined by Begin and End.
            template<typename TIterator>
            static T GetInterpolatedFromMultiple(TIterator Begin, TIterator End, Real Location)
            {
                Whole DataPointCount = std::distance(Begin,End);
                Whole LineSegmentCount = DataPointCount-1;

                if(LineSegmentCount<1)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Cannot GetPercentageThroughSegment in GenericLinearInterpolator without a data segment. There must be two or more data points."); }
                if(1==LineSegmentCount)
                    { return InterpolateMath(*(Begin), *(Begin+1), Location); }

                Whole UsingLineSegment = Location * Real(LineSegmentCount); // Pick a Line Segment
                if(UsingLineSegment>=LineSegmentCount)                      // We are past or at the end of the line segments
                    { return *(End-1); }

                Real LocalPercentage = std::fmod(PreciseReal(Location), PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount;

                return InterpolateMath(*(Begin+UsingLineSegment),           // The first point of the line segment
                                       *(Begin+UsingLineSegment+1),
                                       LocalPercentage);                    // The percentage we are through this line segment
            }

            /// @brief This will interpolates data points with GetInterpolatedFromMultiple or InterpolateMath a required
            /// @details read about GetInterpolatedFromMultiple or InterpolateMath to see what kinds of results this
            /// can produce.
            /// @param Begin An iterator at the beginning of a range of data point
            /// @param End An iterator one past the end of the data range to interpolate.
            /// @param Location A value between 0.0 and 1.0 that represents
            /// @return A T at a location along the data segments defined by Begin and End.
            template<typename TIterator>
            static T Interpolate(TIterator Begin, TIterator End, Real Location)
            {
                if(Begin==End)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Requires at least 2 data points for linear interpolation, was provided 0."); }
                if(Begin+1==End)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Requires at least 2 data points for linear interpolation, was provided 1."); }

                if(Begin+2==End)
                    { return InterpolateMath(*Begin, *(Begin+1), Location); }

                return GetInterpolatedFromMultiple(Begin, End, Location);
            }

            void ProtoSerialize(XML::Node& CurrentRoot) const
            {
                Mezzanine::XML::Node LinearInterpolaterNode = CurrentRoot.AppendChild(SerializableName());

                if(LinearInterpolaterNode)
                {
                    Mezzanine::XML::Attribute VersionAttr = LinearInterpolaterNode.AppendAttribute("Version");
                    Mezzanine::XML::Attribute TypeAttr = LinearInterpolaterNode.AppendAttribute("InterpolatableType");
                    if( VersionAttr  )
                    {
                        if( VersionAttr.SetValue("1") && TypeAttr.SetValue(InterpolatableType::SerializableName()) )
                        {
                            return;
                        }else{
                            SerializeError("Create XML Attribute Values", SerializableName(),true);
                        }
                    }else{
                        SerializeError("Create XML Attributes", SerializableName(),true);
                    }
                }else{
                    SerializeError("Create XML Serialization Node", SerializableName(),true);
                }
            }

            void ProtoDeSerialize(const XML::Node& OneNode)
            {
                if ( String(OneNode.Name())==String(SerializableName()) )
                {
                    if(OneNode.GetAttribute("Version").AsInt() == 1)
                    {
                        if(OneNode.GetAttribute("InterpolatableType").AsString() == InterpolatableType::SerializableName())
                        {
                            return; // Class currently stores no data.
                        }else{
                            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Incompatible InterpolatableType Version for " + SerializableName() + ": Not " + InterpolatableType::SerializableName());
                        }
                    }else{
                        MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
                }

            }

            static String SerializableName()
                { return String("LinearInterpolator"); }
    };


    /// @brief A simple functor for interpolating data points in a simple way.
    /// @details This interpolator provides different guarantees different from the linear one:
    ///     - Data points, might not be valid interpolated values
    ///     - There are no "Corners".
    ///     - Execution could be as bad as O^N.
    ///     - This shape defined by interpolating a set of these will not leave a Convex Hull(or Axis Aligned Bounding Box) that could contain the data.
    ///     - Will be able to provide interpolated values for a small set of data points.
    /// There might be corners when connecting 2 different bezier curves if not careful, any
    /// bezier implementation taking a finite amount of points cannot help this.
    template <typename T>
    class MEZZ_LIB BezierInterpolator
    {
        public:
            /// @brief The type this will interpolate
            /// @note All Interpolators need to declare an InterpolatableType
            typedef T InterpolatableType;
            /// @brief The storage to use with thison tracks
            typedef std::vector<InterpolatableType> Storage;

            /// @brief Get a value at a given location between two others.
            /// @details This uses Linear interpolation recursively to produce a single curve
            /// following Bézier's curve algorithm. For example if interpolating the location 0.5
            /// on a set of 3 data points A,B,C and therefor 2 data segments AB and BC, you can imagine this as
            /// getting the point halfway down AB and the point halfway down down BC. Then this will
            /// get return the halfway between each of those points. This produces smooth curves but could
            /// perform slowly. For more details see the wikiedia pages on Bézier curves:
            /// http://en.wikipedia.org/wiki/Bézier_curve or http://en.wikipedia.org/wiki/B%C3%A9zier_curve
            /// @param Begin An Iterator to the begining of the range of the instances of the type to be Interpolated
            /// @param End The end (not one past the end) of the range that Begin started.
            /// @param A value between 0.0 and 1.0 indicate what point on the beziear spline defined by Begin and End you want.
            /// @return A Value equal or near to Begin if 0.0 is passed, equal to or near to End if 1.0 is passed equal to a point exactly in the middle if 0.5 is passed.
            /// @warning This is implemented as a recursive function with the only termination condition being the Edn iterator.
            template<typename TIterator>
            static T Interpolate(TIterator Begin, TIterator End, Real Location)
            {
                if(Begin==End || Begin+1==End)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Requires at least 1 data points for bezier interpolation."); }

                if(Begin+2==End)
                    { return LinearInterpolator<T>::Interpolate(Begin,End,Location); }

                std::vector<T> SubSection;
                for(TIterator Iter=Begin; Iter!=End-1; Iter++)
                    { SubSection.push_back(LinearInterpolator<T>::Interpolate(Iter,(Iter+2),Location)); }
                return Interpolate(SubSection.begin(),SubSection.end(),Location);
            }

            void ProtoSerialize(XML::Node& CurrentRoot) const
            {
                Mezzanine::XML::Node BezierInterpolaterNode = CurrentRoot.AppendChild(SerializableName());

                if(BezierInterpolaterNode)
                {
                    Mezzanine::XML::Attribute VersionAttr = BezierInterpolaterNode.AppendAttribute("Version");
                    Mezzanine::XML::Attribute TypeAttr = BezierInterpolaterNode.AppendAttribute("InterpolatableType");
                    if( VersionAttr  )
                    {
                        if( VersionAttr.SetValue("1") && TypeAttr.SetValue(InterpolatableType::SerializableName()) )
                        {
                            return;
                        }else{
                            SerializeError("Create XML Attribute Values", SerializableName(),true);
                        }
                    }else{
                        SerializeError("Create XML Attributes", SerializableName(),true);
                    }
                }else{
                    SerializeError("Create XML Serialization Node", SerializableName(),true);
                }
            }

            void ProtoDeSerialize(const XML::Node& OneNode)
            {
                if ( String(OneNode.Name())==String(SerializableName()) )
                {
                    if(OneNode.GetAttribute("Version").AsInt() == 1)
                    {
                        if(OneNode.GetAttribute("InterpolatableType").AsString() == InterpolatableType::SerializableName())
                        {
                            return; // Class currently stores no data.
                        }else{
                            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Incompatible InterpolatableType Version for " + SerializableName() + ": Not " + InterpolatableType::SerializableName());
                        }
                    }else{
                        MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
                }

            }

            static String SerializableName()
                { return String("BezierInterpolator"); }
    };

    /// @brief If something specifically needs the linear interpolator for T they should use this.
    /// @details This is with be a cubic spline where applicable, and will be
    /// more smooth that the others, and be at least as intuitive as the linear version:
    ///     - Data points, will be valid interpolated values.
    ///     - There are no "Corners".
    ///     - Execution time is O^N.
    ///     - This shape defined by interpolating a set of these *will* leave a Convex Hull(or Axis Aligned Bounding Box) that could contain the data.
    ///     - Will be able to interpolated arbitrary sets of data points.
    /// @warning This can interpolate cubic splines, but it cannot be used with the track at present.
    template <typename T>
    class MEZZ_LIB SlowSplineInterpolator
    {
        public:
            /// @brief The type this will interpolate
            /// @note All Interpolators need to declare an InterpolatableType
            typedef T InterpolatableType;
            /// @brief The storage for type for a cubic spline
            typedef CubicSpline<Real,InterpolatableType> Storage;

            /// @brief Calculates the desired location on a cubic spline
            /// @param Begin An iterator to the beginning of points to interpolate
            /// @param End An iterator to the end of a data series, not one passed it.
            /// @param Location A value between 0 and 1, that indicate how far along the curve the data point to retrieve is
            /// @return This will return an value along a curve that passes smoothly through all the points passed.
            template<typename TIterator>
            static T Interpolate(TIterator Begin, TIterator End, Real Location)
            {
                /*std::vector<T> Points;
                std::cout << "Unknown Type            : " << typeid(TIterator).name() << std::endl
                          << "Vector<T>               : " << typeid(std::vector<T>).name() << std::endl
                          << "Vector<T>::iterator     : " << typeid(typename std::vector<T>::iterator).name() << std::endl
                          << "Storage                 : " << typeid(Storage).name() << std::endl
                          << "Storage::iterator       : " << typeid(typename Storage::iterator).name() << std::endl
                          << "Storage::const_iterator : " << typeid(typename Storage::const_iterator).name() << std::endl
                          << "Is the unknown type a Storage::const_iterator? " << (typeid(typename Storage::const_iterator)==typeid(TIterator)) << std::endl
                             ;
                Points.insert(Points.end(),Begin,End);// */
                std::vector<T> Points(Begin, End);
                std::vector<Real> Spacing(Points.size());
                Real JumpSize = 1/PreciseReal(Points.size()-1);
                Real CurrentJump=0;
                for(std::vector<Real>::iterator Iter=Spacing.begin();
                    Iter!=Spacing.end();
                    Iter++)
                {
                    *Iter=CurrentJump;
                    CurrentJump+=JumpSize;
                }

                CubicSpline<Real,T> Spliney(Spacing,Points);
                return Spliney.interpolate(Location);
            }
    };


} // /namespace Mezzanine


#ifndef SWIG

/// @brief Used to Serialize an Mezzanine::LinearInterpolator to a human readable stream
/// @details The current XML format is extremely simple because there no data: 'c'.
/// @param Lint The Mezzanine::LinearInterpolator to be converted to characters.
/// @param stream The place to send the characters, that define the Mezzanine::LinearInterpolator.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
template<typename T>
std::ostream& operator << (std::ostream& stream, const Mezzanine::LinearInterpolator<T>& Lint)
{
    Serialize(stream,Lint);
    return stream;
}

/// @brief Used to de-serialize an Mezzanine::LinearInterpolator from a stream
/// @details This does nothing at the moment, but the instant state is required...
/// @param Lint The Mezzanine::LinearInterpolator that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::LinearInterpolator.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
template<typename T>
std::istream& operator >> (std::istream& stream, Mezzanine::LinearInterpolator<T>& Lint)
    { return DeSerialize(stream, Lint); }


/// @brief Used to Serialize an Mezzanine::BezierInterpolator to a human readable stream
/// @details The current XML format is extremely simple because there no data: 'c'.
/// @param Lint The Mezzanine::BezierInterpolator to be converted to characters.
/// @param stream The place to send the characters, that define the Mezzanine::BezierInterpolator.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
template<typename T>
std::ostream& operator << (std::ostream& stream, const Mezzanine::BezierInterpolator<T>& Lint)
{
    Serialize(stream,Lint);
    return stream;
}

/// @brief Used to de-serialize an Mezzanine::BezierInterpolator from a stream
/// @details This does nothing at the moment, but the instant state is required...
/// @param Lint The Mezzanine::BezierInterpolator that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::BezierInterpolator.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
template<typename T>
std::istream& operator >> (std::istream& stream, Mezzanine::BezierInterpolator<T>& Lint)
    { return DeSerialize(stream, Lint); }


#endif

#endif // Include guard
