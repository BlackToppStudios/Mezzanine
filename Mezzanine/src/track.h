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
            typedef std::vector<InterpolatableType> DataContainerType;

        protected:
            /// @brief The underlying container of Discrete datapoints
            DataContainerType DataPoints;

        public:
            /// @brief An iterator than can take an arbitrary amount of steps by interpolation.
            typedef SmoothTrackIterator<InterpolatableType> SmoothIteratorType;

            /// @brief Get the amount of stored DataPoints
            /// @note Name chosen to match standard containers
            /// @return How many data points exist on this track
            size_t size() const
                { return DataPoints.size(); }
            /// @brief Add another data point to the end of the track.
            /// @param AddedVale
            virtual void push_back(const InterpolatableType& AddedValue)
                { DataPoints.push_back(AddedValue); }
            /// @brief Remove all the points from the track
            void clear()
                { DataPoints.clear(); }

            virtual SmoothIteratorType begin(Integer Steps=100) const
            {
                return SmoothIteratorType(this, 0.0,
                                            (Steps?(PreciseReal(1.0)/PreciseReal(Steps)):0.0)
                                          );
            }

            virtual SmoothIteratorType end(Integer Steps=0) const
            {
                return SmoothIteratorType(this, 1.0,
                                            (Steps?(PreciseReal(1.0)/PreciseReal(Steps)):0.0)
                                          );
            }

            /// @brief Get a value between the beginning and the end
            /// @details in derived classes this will perform some simple(hopefully fast) calculation to get
            /// interpolated value between the beginning and the end fo the track. Depending on algorithm this
            /// may or may not respect the nodes.
            /// @param Percentage A value from 0 to 1 indicating when between the beginning and end the point should be.
            /// @return An InterpolatableType
            virtual InterpolatableType GetInterpolated(Real Percentage) const = 0;
    };

    template <typename InterpolatableType, typename InterpolatorType>
    class Track : public TrackBase<InterpolatableType>
    {
        public:
            /// @brief The type of the Container storing the interpolatable data. This is a single point to change all the tracks
            typedef std::vector<InterpolatableType> DataContainerType;

            /// @brief An iterator than can take an arbitrary amount of steps by interpolation.
            typedef SmoothTrackIterator<InterpolatableType> SmoothIteratorType;

            virtual InterpolatableType GetInterpolated(Real Percentage) const
            {
                return InterpolatorType::Interpolate(
                           TrackBase<InterpolatableType>::DataPoints.begin(),
                           TrackBase<InterpolatableType>::DataPoints.end(),
                           Percentage
                       );
            }



    };

    template <typename InterpolatableType, typename InterpolatorType>
    class TrackLooped : public Track<InterpolatableType, InterpolatorType>
    {
        public:
            /// @brief The type of the Container storing the interpolatable data. This is a single point to change all the tracks
            typedef std::vector<InterpolatableType> DataContainerType;

            /// @brief An iterator than can take an arbitrary amount of steps by interpolation.
            typedef SmoothTrackIterator<InterpolatableType> SmoothIteratorType;

            /// @brief Add another data point to the end of the track.
            /// @param
            virtual void push_back(const InterpolatableType& AddedValue)
            {
                *(TrackBase<InterpolatableType>::DataPoints.end()-1) = AddedValue;
                TrackBase<InterpolatableType>::DataPoints.push_back(*TrackBase<InterpolatableType>::DataPoints.begin());
            }
    };


}//Mezzanine

#endif

