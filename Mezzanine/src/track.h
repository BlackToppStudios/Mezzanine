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

#include "vector3.h"
#include "exception.h"
#include "enumerations.h"
#include "interpolator.h"

#ifndef SWIG // STD headers are bad for Swig
    #include <cmath>
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a series of points as a line in 3D space.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB TrackOld
    {
    public:
        /// @brief Basic container type for point storage by this class.
        typedef std::vector<Vector3>               PointContainer;
        /// @brief Iterator type for points stored by this class.
        typedef PointContainer::iterator           PointIterator;
        /// @brief Const Iterator type for points stored by this class.
        typedef PointContainer::const_iterator     ConstPointIterator;
        /*/// @brief Basic container type for tangent storage by this class.
        typedef std::vector<Vector3>               TangentContainer;
        /// @brief Iterator type for tangent stored by this class.
        typedef TangentContainer::iterator         TangentIterator;
        /// @brief Const Iterator type for tangent stored by this class.
        typedef TangentContainer::const_iterator   ConstTangentIterator;//*/
    protected:
        /// @internal
        /// @brief Whether or not this Track forms a closed loop.
        bool Closed;
        /// @internal
        /// @brief Container storing all points via @ref Vector3 instances.
        PointContainer Points;
        /*/// @internal
        /// @brief Container storing all tangents via @ref Vector3 instances.
        TangentContainer Tangents;//*/
    public:
        /// @brief Class constructor.
        TrackOld();
        /// @brief Class destructor.
        virtual ~TrackOld();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Adds a point in 3D space to the end of this track.
        /// @param Point The location in 3D space to be added.
        /// @return Returns a reference to this.
        virtual void AddPoint(const Vector3& Point);
        /// @brief Adds a point in 3D space to the end of this track.
        /// @param X The location on the X axis to be added.
        /// @param Y The location on the Y axis to be added.
        /// @param Z The location on the Z axis to be added.
        /// @return Returns a reference to this.
        virtual void AddPoint(const Real X, const Real Y, const Real Z);
        /// @brief Changes the location of a point in this track.
        /// @param Index The index of the point to be updated.
        /// @param Point The location in 3D space to update to.
        /// @return Returns a reference to this.
        virtual void UpdatePoint(const UInt32 Index, const Vector3& Point);
        /// @brief Changes the location of a point in this track.
        /// @param Index The index of the point to be updated.
        /// @param X The location on the X axis to update to.
        /// @param Y The location on the Y axis to update to.
        /// @param Z The location on the Z axis to update to.
        /// @return Returns a reference to this.
        virtual void UpdatePoint(const UInt32 Index, const Real X, const Real Y, const Real Z);
        /// @brief Gets a point by index.
        /// @param Index The index of the point to be retrieved.
        /// @return Returns a const Vector3 reference to the location at the specified index.
        virtual const Vector3& GetPoint(const UInt32 Index) const;
        /// @brief Gets the number of points in this track.
        /// @return Returns the current number of points in this track.
        virtual UInt32 GetNumPoints() const;
        /// @brief Erases all points from this track.
        virtual void ClearAllPoints();

        /// @brief Sets this track to be a closed loop.
        /// @param Enable Whether or not to connect the last point in this track to the first point in this track.
        virtual void Close(bool Enable);
        /// @brief Gets whether or not this track is closed.
        /// @return Returns true if this track is currently closed, false otherwise.
        virtual bool IsClosed() const;
        /// @brief Reverses the order of points in this track.
        virtual void Reverse();
        /// @brief Creats a new track from a subset of points belonging to this track.
        /// @param First The index of the first point in this track to be included in the new track.
        /// @param Last The index of the last point in this track to be included in the new track.
        /// @return Returns a track containing all the points in the range specified.
        virtual TrackOld ExtractSubPath(const UInt32 First, const UInt32 Last);

        /// @brief Gets all the points belonging to this track.
        /// @return Returns a reference to the container storing all points in this track.
        virtual PointContainer& GetPoints();
        /// @brief Gets all the points belonging to this track.
        /// @return Returns a const reference to the container storing all points in this track.
        virtual const PointContainer& GetPoints() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Transform related methods

        /// @brief Translates each point in this track.
        /// @param Trans A Vector3 representing the amount of movement to be applied to the track.
        /// @return Returns a reference to this.
        virtual void Translate(const Vector3& Trans);
        /// @brief Translates each point in this track.
        /// @param X The amount of movement to apply on the X axis to each point in the track.
        /// @param Y The amount of movement to apply on the Y axis to each point in the track.
        /// @param Z The amount of movement to apply on the Z axis to each point in the track.
        /// @return Returns a reference to this.
        virtual void Translate(const Real X, const Real Y, const Real Z);
        /// @brief Scales each point in this track.
        /// @param Sc A Vector3 representing the amount of scaling to be applied to the track.
        /// @return Returns a reference to this.
        virtual void Scale(const Vector3& Sc);
        /// @brief Scales each point in this track.
        /// @param X The amount of scaling to apply on the X axis to each point on the track.
        /// @param Y The amount of scaling to apply on the Y axis to each point on the track.
        /// @param Z The amount of scaling to apply on the Z axis to each point on the track.
        /// @return Returns a reference to this.
        virtual void Scale(const Real X, const Real Y, const Real Z);

        ///////////////////////////////////////////////////////////////////////////////
        // Interpolation

        /// @brief Gets the world location of a certain point on the track.
        /// @param TrackPos The relative position on the track to get the 3D location of.  Range: 0.0 to 1.0.
        /// @return Returns the world location of the position on the track specified.
        virtual Vector3 GetLocationOnTrack(const Real TrackPos);
        /// @brief Gets the world location of a certain point on the track.
        /// @param Index
        /// @param TrackPos The relative position on the track between the specified point and the point after it to get the 3D location of.  Range: 0.0 to 1.0.
        /// @return Returns the world location of the position on the track specified.
        virtual Vector3 GetLocationOnTrack(const UInt32 Index, const Real TrackPos);
    };//TrackOld



    /// @brief A base type that provides container features for different tracks
    /// @details This uses vector underneath for its performance characteristics. If
    /// You are using a track with a small number of points
    template <typename InterpolatableType>
    class TrackBase
    {
        public:
            /// @brief The type of the Container storing the interpolatable data. This is a single point to change all the tracks
            typedef  std::vector<InterpolatableType> DataContainerType;

        protected:
            ///
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

    /// @brief
    template<typename InterpolatableType>
    class SmoothTrackIterator
    {
        private:
            /// @brief A pointer to the track under iteration.
            typename TrackBase<InterpolatableType>::DataContainerType* TrackUnderIteration;
        public:


    };

    /// @brief A track that uses linear interpolation
    template <typename InterpolatableType>
    class TrackLinear : public TrackBase<InterpolatableType>
    {       
        public:
            typedef typename TrackBase<InterpolatableType>::DataContainerType DataContainerType;
            typedef typename DataContainerType::iterator iterator;
            typedef typename DataContainerType::const_iterator const_iterator;
            typedef typename InterpolatableTraits<InterpolatableType>::LinearInterpolator Interpolator;

        public:
            Whole GetLineSegmentFor(Real Percentage) const
                { return (Percentage * (Real(TrackBase<InterpolatableType>::DataPoints.size()-1))); }

            Real GetPercentageThroughSegment(Real Percentage) const
            {
                Whole LineSegmentCount = TrackBase<InterpolatableType>::DataPoints.size() - 1;
                if(LineSegmentCount<1)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Cannot GetPercentageThroughSegment on a track without a single data segment. There must be two or more data points."); }
                if(1==LineSegmentCount)
                    { return Percentage; }
                return std::fmod(Percentage,1.0/Real(LineSegmentCount))*LineSegmentCount;
            }

            virtual InterpolatableType GetInterpolated(Real Percentage) const
            {
                Whole DataPointCount = TrackBase<InterpolatableType>::DataPoints.size();
                Whole Index = GetLineSegmentFor(Percentage); // Pick a Line Segment
                if(DataPointCount==Index) // If we are past the end give them the end, because this should only happen when percentage == 1.0
                    { return TrackBase<InterpolatableType>::DataPoints[Index]; }
                return Interpolator::Interpolate(TrackBase<InterpolatableType>::DataPoints[Index],  // The first point of the line segment
                                                 TrackBase<InterpolatableType>::DataPoints[Index+1],// The second point
                                                 GetPercentageThroughSegment(Percentage)); // The percentage we are through this line segment
            }

            virtual InterpolatableType GetInterpolatedAsLoop(Real Percentage) const
            {


            }

    };




}//Mezzanine

#endif

