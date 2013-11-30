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
#ifndef _track_h
#define _track_h

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
    /// There are different interpolation methods. These are defined by the InterpolatableTraits
    /// for a given class. For example a track that does bezier interpolation might try to
    /// use InterpolatableTraits<Integer>::BezierInterpolator to do the math.
    /// @n @n
    /// This uses vector underneath for its performance characteristics.
    template <typename InterpolatableType>
    class TrackBase
    {
        public:
            /// @brief The type of the Container storing the interpolatable data. This is a single point to change all the tracks
            typedef  std::vector<InterpolatableType> DataContainerType;

        protected:
            /// @brief The underlying container of Discreate datapoints
            DataContainerType DataPoints;

        public:
            /// @brief Get the amount of stored DataPoints
            /// @note Name chosen to match standard containers
            /// @return How many data points exist on this track
            size_t size() const
                { return DataPoints.size(); }
            /// @brief Add another data point to the end of the track.
            /// @param AddedVale
            void push_back(const InterpolatableType& AddedValue)
                { DataPoints.push_back(AddedValue); }
            /// @brief Remove all the points from the track
            void clear()
                { DataPoints.clear(); }

            /// @brief Get a value between the beginning and the end
            /// @details in derived classes this will perform some simple(hopefully fast) calculation to get
            /// interpolated value between the beginning and the end fo the track. Depending on algorithm this
            /// may or may not respect the nodes.
            /// @param Percentage A value from 0 to 1 indicating when between the beginning and end the point should be.
            /// @return An InterpolatableType
            virtual InterpolatableType GetInterpolated(Real Percentage) const = 0;
            /// @brief As GetInterpolated but includes the first a second time at the end of the series.
            /// @param Percentage A value from 0 to 1 indicating when between the beginning and end the point should be.
            /// @return An InterpolatableType
            virtual InterpolatableType GetInterpolatedAsLoop(Real Percentage) const = 0;
    };

    /// @brief A track that uses linear interpolation for linear and looped tracks.
    /// @details This class is intended to provide an abstraction for any amount of
    /// datapoints and present a range from 0 to 1 for all tracks.
    template <typename InterpolatableType>
    class TrackLinear : public TrackBase<InterpolatableType>
    {
        public:
            /// @brief The type of the underlying container for InterpolatableType instances
            typedef typename TrackBase<InterpolatableType>::DataContainerType DataContainerType;
            /// @brief An Iterator for the raw points of the track
            typedef typename DataContainerType::iterator iterator;
            /// @brief An Iterator for the raw points of the track
            typedef typename TrackLinear::iterator Iterator;

            /// @brief An Iterator for the raw points of the track, guaranteed to not change the track
            typedef typename DataContainerType::const_iterator const_iterator;
            /// @brief An Iterator for the raw points of the track, guaranteed to not change the track
            typedef typename TrackLinear::const_iterator ConstIterator;
            /// @brief A type used for iterating over the range of interpolated values of the track, and guaranteed to not change the track.
            typedef SmoothTrackIterator<InterpolatableType> SmoothIterator;
            /// @brief The functor that does the actual interpolation of the points.
            typedef typename InterpolatableTraits<InterpolatableType>::LinearInterpolator Interpolator;

        public:
            /// @brief For a given percentage determine which two data points it is between.
            /// @param Percentage from 0.0(beginning) and 1.0(end) where is the point you want.
            /// @param Loop should we calculate as if there is another segment connection the last point to the first? Default to false.
            /// @return A Whole indicating which segment to use. 0 define as the segment connect index 0 and index 1. 1 is index 1 to 2, etc...
            Whole GetLineSegmentByPercent(Real Percentage, Bool Loop = false) const
                { return (Percentage * (Real(TrackBase<InterpolatableType>::DataPoints.size()-(Whole(!Loop)*1)))); }

            /// @brief How far betweent the two points of a segment is the provided Track Percentage.
            ///
            Real GetPercentageThroughSegment(Real Percentage, Bool Loop = false) const
            {
                Whole LineSegmentCount = TrackBase<InterpolatableType>::DataPoints.size()-1;
                if(LineSegmentCount<1)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Cannot GetPercentageThroughSegment on a track without a single data segment. There must be two or more data points."); }
                LineSegmentCount+=Whole(Loop);
                if(1==LineSegmentCount)
                    { return Percentage; }
                return std::fmod(PreciseReal(Percentage),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount;
            }

            InterpolatableType GetInterpolated(Real Percentage, Bool Loop) const
            {
                Whole DataPointCount = TrackBase<InterpolatableType>::DataPoints.size();
                Whole Index = GetLineSegmentByPercent(Percentage,Loop); // Pick a Line Segment
                Real LocalPercentage = GetPercentageThroughSegment(Percentage,Loop);
                if(Loop)
                {
                    if(DataPointCount<=Index)
                        { return TrackBase<InterpolatableType>::DataPoints[0]; }
                    if(DataPointCount-1<=Index) // If we are in the last segment connect and should connect to the begining
                    {
                        return Interpolator::Interpolate(TrackBase<InterpolatableType>::DataPoints[Index],
                                                         TrackBase<InterpolatableType>::DataPoints[0],
                                                         LocalPercentage);
                    }
                }else{
                    if(DataPointCount-1<=Index) // If we are past the end give them the end, because this should only happen when percentage == 1.0
                        { return TrackBase<InterpolatableType>::DataPoints[Index]; }

                }
                return Interpolator::Interpolate(TrackBase<InterpolatableType>::DataPoints[Index],  // The first point of the line segment
                                                 TrackBase<InterpolatableType>::DataPoints[Index+1],// The second point
                                                 LocalPercentage); // The percentage we are through this line segment
            }

            virtual InterpolatableType GetInterpolated(Real Percentage) const
                { return GetInterpolated(Percentage, false); }

            virtual InterpolatableType GetInterpolatedAsLoop(Real Percentage) const
                { return GetInterpolated(Percentage, true); }

    };




}//Mezzanine

#endif

